//
// Created by Aleksey Timin on 12/9/19.
//

#ifndef EIPSCANNER_MOCKS_H
#define EIPSCANNER_MOCKS_H

#include <gmock/gmock.h>
#include "eip_scanner/MessageRouter.h"

class TMockMessageRouter : public eip_scanner::MessageRouter {
public:
	using SPtr = std::shared_ptr<TMockMessageRouter>;

	MOCK_CONST_METHOD3(sendRequest, eip_scanner::cip::MessageRouterResponse(
			eip_scanner::SessionInfoIf::SPtr si,
			eip_scanner::cip::CipUsint service,
			const eip_scanner::cip::EPath &path));

	MOCK_CONST_METHOD4(sendRequest, eip_scanner::cip::MessageRouterResponse(
			eip_scanner::SessionInfoIf::SPtr si,
			eip_scanner::cip::CipUsint service,
			const eip_scanner::cip::EPath &path,
			const std::vector<uint8_t> &data));

	MOCK_CONST_METHOD5(sendRequest, eip_scanner::cip::MessageRouterResponse(
			eip_scanner::SessionInfoIf::SPtr si,
			eip_scanner::cip::CipUsint service,
			const eip_scanner::cip::EPath &path,
			const std::vector<uint8_t> &data,
			const eip_scanner::eip::CommonPacketItem::Vec&));
};

class TMockSessionInfo : public eip_scanner::SessionInfoIf {
public:
	using SPtr = std::shared_ptr<TMockSessionInfo>;
	MOCK_CONST_METHOD1(sendAndReceive, eip_scanner::eip::EncapsPacket(const eip_scanner::eip::EncapsPacket& packet));
	MOCK_CONST_METHOD0(getSessionHandle, eip_scanner::cip::CipUdint());
	MOCK_CONST_METHOD0(getRemoteEndPoint, eip_scanner::sockets::EndPoint());
};

class TMockSocket : public eip_scanner::sockets::BaseSocket {
public:
	using SPtr = std::shared_ptr<TMockSocket>;
	TMockSocket() : eip_scanner::sockets::BaseSocket("", 0) {}
	MOCK_CONST_METHOD1(Send, void(const std::vector<uint8_t>& data));
	MOCK_CONST_METHOD1(Receive, std::vector<uint8_t>(size_t size));
};

#endif //EIPSCANNER_MOCKS_H
