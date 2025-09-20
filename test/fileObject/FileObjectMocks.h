//
// Created by Aleksey Timin on 11/24/19.
//

#ifndef EIPSCANNER_TEST_FILEOBJECT_MOCKS_H
#define EIPSCANNER_TEST_FILEOBJECT_MOCKS_H

#include "../Mocks.h"
#include "eip_scanner/MessageRouter.h"
#include "eip_scanner/FileObject.h"
#include "eip_scanner/fileObject/FileObjectState.h"
#include "eip_scanner/utils/Buffer.h"

namespace eip_scanner {
namespace fileObject {
	using utils::Buffer;
	using eip_scanner::fileObject::FileObjectAttributesCodes;
	using eip_scanner::fileObject::FILE_OBJECT_CLASS_ID;


	static void mockGetFileObjectState(TMockMessageRouter::SPtr messageRouter,
			SessionInfoIf::SPtr si, cip::CipUint objectId, FileObjectStateCodes stateCode)  {
		cip::MessageRouterResponse response;
		Buffer buffer;
		buffer << static_cast<cip::CipUsint>(stateCode);

		response.setData(buffer.data());
		response.setGeneralStatusCode(cip::SUCCESS);

		EXPECT_CALL(*messageRouter, sendRequest(si, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
												 cip::EPath(FILE_OBJECT_CLASS_ID, objectId,
															static_cast<cip::CipUint>(FileObjectAttributesCodes::STATE)),
												 std::vector<uint8_t>())
		).WillOnce(::testing::Return(response));
	}

}
}
#endif //EIPSCANNER_TEST_FILEOBJECT_MOCKS_H
