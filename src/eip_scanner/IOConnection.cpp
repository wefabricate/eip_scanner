//
// Created by Aleksey Timin on 11/18/19.
//

#include "eip_scanner/IOConnection.h"
#include "eip_scanner/eip/CommonPacketItemFactory.h"
#include "eip_scanner/eip/CommonPacket.h"
#include "eip_scanner/cip/connectionManager/NetworkConnectionParams.h"
#include "eip_scanner/utils/Buffer.h"
#include "eip_scanner/utils/Logger.h"
#include "eip_scanner/tracing/eip_tracepoint.h"

namespace eip_scanner {
	using utils::Logger;
	using utils::LogLevel;
	using utils::Buffer;
	using eip::CommonPacket;
	using eip::CommonPacketItemFactory;
	using cip::connectionManager::NetworkConnectionParams;

	IOConnection::IOConnection()
			: _o2tNetworkConnectionId{0}
			, _t2oNetworkConnectionId{0}
			, _o2tAPI{0}
			, _t2oAPI{0}
			, _o2tDataSize{0}
			, _t2oDataSize{0}
			, _o2tFixedSize(true)
			, _t2oFixedSize(true)
			, _o2tTimer{0}
			, _t2o_timer{0}
			, _connectionTimeoutMultiplier{0}
			, _connectionTimeoutCount{0}
			, _o2tSequenceNumber{0}
			, _t2oSequenceNumber{0}
			, _serialNumber{0}
			, _transportTypeTrigger{0}
			, _o2tRealTimeFormat{0}
			, _t2oRealTimeFormat{0}
			, _sequenceValueCount{0}
			, _connectionPath(0)
			, _originatorVendorId{0}
			, _originatorSerialNumber{0}
			, _outputData()
			, _receiveDataHandle([](auto, auto, auto) {})
			, _closeHandle([]() {})
			, _sendDataHandle([](auto) {})
			, _isOpen(false)
			, _lastHandleTime(std::chrono::steady_clock::now()) {
	}

	IOConnection::~IOConnection() = default;

	void IOConnection::setDataToSend(const std::vector<uint8_t> &data) {
		_outputData = data;
	}

	void IOConnection::setReceiveDataListener(ReceiveDataHandle handle) {
		_receiveDataHandle = std::move(handle);
	}

	void IOConnection::setSendDataListener(SendDataHandle handle) {
        	_sendDataHandle = std::move(handle);
	}

	void IOConnection::setCloseListener(CloseHandle handle) {
		_closeHandle = std::move(handle);
	}

	void IOConnection::notifyReceiveData(const std::vector<uint8_t> &data) {
		Buffer buffer(data);
		cip::CipUdint runtimeHeader = 0;
		cip::CipUint sequenceValueCount = 0;

		if (_t2oRealTimeFormat) {
			buffer >> runtimeHeader;
		}

		if ((_transportTypeTrigger & NetworkConnectionParams::CLASS1) > 0
			|| (_transportTypeTrigger & NetworkConnectionParams::CLASS3) > 0) {
			buffer >> sequenceValueCount;
			lttng_ust_tracepoint(eip_scanner, transmission_event, "Receive_data", sequenceValueCount, _t2oNetworkConnectionId);
		}

		std::vector<uint8_t> ioData(data.begin() + buffer.pos(), data.end());
		if (_t2oFixedSize && ioData.size() != _t2oDataSize) {
			Logger(LogLevel::WARNING) << "Connection T2O_ID=" << _t2oNetworkConnectionId
				<< " has fixed size " << _t2oDataSize << " bytes but " << ioData.size()
				<< " bytes were received. Ignore this data.";
		} else {
			_connectionTimeoutCount = 0;
			lttng_ust_tracepoint(eip_scanner, span_start, "process_receive", _t2oNetworkConnectionId);
			_receiveDataHandle(runtimeHeader, sequenceValueCount,
							   ioData);
			lttng_ust_tracepoint(eip_scanner, span_stop, _t2oNetworkConnectionId);
		}
	}

	bool IOConnection::notifyTick() {
		auto now = std::chrono::steady_clock::now();
		auto sinceLastHandle =
			std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastHandleTime);

		auto periodInMicroS = sinceLastHandle.count() * 1000;
		_connectionTimeoutCount += periodInMicroS;
		if (!_isOpen) {
			return false;
		}
		if (_connectionTimeoutCount > _connectionTimeoutMultiplier * _t2oAPI) {
			Logger(LogLevel::WARNING) << "Connection T2O_ID=" << _t2oNetworkConnectionId << " is closed by timeout";
			_closeHandle();
			return false;
		}

		_lastHandleTime = now;

		_o2tTimer += periodInMicroS;
		if (_o2tTimer >= _o2tAPI) {
			_o2tSequenceNumber++;
			CommonPacket commonPacket;
			CommonPacketItemFactory factory;
			commonPacket << factory.createSequenceAddressItem(_o2tNetworkConnectionId, _o2tSequenceNumber);

			Buffer buffer;
			if ((_transportTypeTrigger & NetworkConnectionParams::CLASS1) > 0
				|| (_transportTypeTrigger & NetworkConnectionParams::CLASS3) > 0) {
				buffer << ++_sequenceValueCount;
			}

			if (_o2tRealTimeFormat) {
				cip::CipUdint header = 1; //TODO: Always RUN
				buffer << header;
			}

			_o2tTimer = 0;
			lttng_ust_tracepoint(eip_scanner, span_start, "process_send", _o2tNetworkConnectionId);
			_sendDataHandle(_outputData);
			lttng_ust_tracepoint(eip_scanner, span_stop, _o2tNetworkConnectionId);
			if (_o2tFixedSize && _outputData.size() != _o2tDataSize)  {
				Logger(LogLevel::WARNING) << "Connection O2T_ID=" << _o2tNetworkConnectionId
										  << " has fixed size " << _o2tDataSize << " bytes but " << _outputData.size()
										  << " bytes are to send. Don't send this data.";
			} else {
				buffer << _outputData;
				commonPacket << factory.createConnectedDataItem(buffer.data());

				lttng_ust_tracepoint(eip_scanner, transmission_event, "Send_data", _o2tSequenceNumber, _o2tNetworkConnectionId);
				_socket->Send(commonPacket.pack());
			}
		}

		return true;
	}
}
