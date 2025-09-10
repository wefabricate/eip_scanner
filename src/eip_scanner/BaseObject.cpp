//
// Created by Aleksey Timin on 12/9/19.
//

#include "BaseObject.h"

namespace eip_scanner {

	BaseObject::BaseObject(cip::CipUint classId, cip::CipUint instanceId)
		: _classId(classId)
		, _instanceId(instanceId) {
	}

	cip::CipUint BaseObject::getClassId() const {
		return _classId;
	}

	cip::CipUint BaseObject::getInstanceId() const {
		return _instanceId;
	}
}