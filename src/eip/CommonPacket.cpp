//
// Created by Aleksey Timin on 11/16/19.
//
#include <stdexcept>
#include "src/eip/CommonPacket.h"
#include "src/utils/Buffer.h"
#include "src/cip/Types.h"

namespace eipScanner {
namespace eip {
	using utils::Buffer;

	CommonPacket::CommonPacket()
		: _items(0) {
	}

	CommonPacket::~CommonPacket() = default;

	CommonPacket &CommonPacket::operator<<(const CommonPacketItem &item) {
		_items.push_back(item);
		return *this;
	}

	std::vector<uint8_t> CommonPacket::pack() const {
		Buffer buffer;

		buffer << static_cast<cip::CipUint>(_items.size());
		for (auto& item :_items) {
			buffer << item.pack();
		}

		return buffer.data();
	}

	void CommonPacket::expand(const std::vector<uint8_t> &data) {
		_items.clear();

		Buffer buffer(data);
		cip::CipUint count;
		buffer >> count;
		for (int i = 0; i < count && !buffer.empty(); ++i) {
			cip::CipUint typeId;
			cip::CipUint length;
			buffer >> typeId >> length;

			std::vector<uint8_t> itemData(length);
			buffer >> itemData;

			if (!buffer.isValid()) {
				throw std::runtime_error("Wrong Common Packet format");
			}

			_items.push_back(CommonPacketItem(static_cast<CommonPacketItemIds>(typeId), itemData));
		}
	}

	const CommonPacketItem::Vec &CommonPacket::getItems() {
		return _items;
	}
}
}