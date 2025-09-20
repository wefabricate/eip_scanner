//
// Created by Aleksey Timin on 11/23/19.
//

#ifndef EIPSCANNER_FILEOBJECT_FILEOBJECTSTATE_H
#define EIPSCANNER_FILEOBJECT_FILEOBJECTSTATE_H

#include <memory>
#include "eip_scanner/SessionInfo.h"
#include "eip_scanner/MessageRouter.h"
#include "eip_scanner/FileObject.h"
#include "eip_scanner/cip/GeneralStatusCodes.h"
#include "eip_scanner/utils/Logger.h"

namespace eip_scanner {
namespace fileObject {
	const cip::CipUsint FILE_OBJECT_CLASS_ID = 0x37;
	const cip::CipUsint MAX_TRANSFER_SIZE = 255;

	enum class FileObjectAttributesCodes : cip::CipUint {
		STATE = 1,
	};

	enum class FileObjectServiceCodes : cip::CipUint {
		INITIATE_UPLOAD = 0x4B,
		UPLOAD_TRANSFER = 0x4F
	};

	enum class TransferPacketTypeCodes : cip::CipUsint {
		FIRST = 0,
		MIDDLE = 1,
		LAST = 2,
		ABORT = 3,
		FIRST_AND_LAST = 4,
	};

	class FileObjectState {
	public:
		using UPtr = std::unique_ptr<FileObjectState>;

		FileObjectState(FileObjectStateCodes state, FileObject &owner, cip::CipUint objectId, MessageRouter::SPtr messageRouter);
		virtual ~FileObjectState();

		virtual void initiateUpload(SessionInfoIf::SPtr si, EndUploadHandler handle);
		virtual bool transfer(SessionInfoIf::SPtr si);

		FileObjectStateCodes getStateCode() const;
		void SyncState(SessionInfoIf::SPtr si);

	protected:
		void logWithStateName(utils::LogLevel logLevel, const std::string &message) const;
		std::string getStateName() const;

		template <class State, class ... Types>
		void setState(Types ... args) {
			_owner._state = std::make_unique<State>(_owner, _objectId, _messageRouter, args...);
		}

		MessageRouter::SPtr _messageRouter;
		cip::CipUint _objectId;
		FileObjectStateCodes _stateCode;
		FileObject &_owner;
	};
}
}

#endif  // EIPSCANNER_FILEOBJECT_FILEOBJECTSTATE_H
