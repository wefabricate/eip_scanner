//
// Created by Aleksey Timin on 12/11/19.
//

#include <gtest/gtest.h>
#include "Mocks.h"
#include "EIPScanner/vendor/ra/powerFlex525/DPIFaultManager.h"

using namespace eipScanner;
using namespace eipScanner::vendor::ra::powerFlex525;

class TestDPIFaultManager : public ::testing::Test {
public:
	const std::vector<uint8_t> FULL_INFORMATION_DATA = {
			0x01,0x0,		// fault code
			0x02,			// DSI port
			0x03,			// DSI device
			'E', 'R', 'R', 'O', 'R', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // fault text
			0x01, 0x02, 0x03, 0x4, 0x5, 0x6, 0x7,0x8,	// timer value
			0x3, 0x0		// flags
	};

	void SetUp() override {
		_messageRouter = std::make_shared<TMockMessageRouter>();
		_nullSession = nullptr;

	}

	TMockMessageRouter::SPtr _messageRouter;
	SessionInfoIf::SPtr  _nullSession;
};

// tripped is not reimplemented
TEST_F(TestDPIFaultManager, DISABLED_ShouldGenerateTrippedEvent) {
	DPIFaultManager manager;

	cip::CipUsint  tripped = -1;
	manager.setTrippedDeviceListener([&tripped](bool value) {
		tripped = value;
	});

	cip::MessageRouterResponse response;
	response.setData({1});

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x97, 0, 4))).WillOnce(::testing::Return(response));

	manager.handleFaultParameters(_nullSession, _messageRouter);
	EXPECT_EQ(1, tripped);
}

// tripped is not reimplemented
TEST_F(TestDPIFaultManager, DISABLED_ShouldNotGenerateTrippedEventIfItIsNotChanged) {
	DPIFaultManager manager;

	cip::CipUsint  tripped = -1;
	manager.setTrippedDeviceListener([&tripped](bool value) {
		tripped = value;
	});

	cip::MessageRouterResponse response;
	response.setData({0});

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 4))).WillOnce(::testing::Return(response));

	manager.handleFaultParameters(_nullSession, _messageRouter);
	//EXPECT_EQ(0, tripped);

	tripped = 0xff;
	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 4))).WillOnce(::testing::Return(response));

	manager.handleFaultParameters(_nullSession, _messageRouter);
	//EXPECT_EQ(0xff, tripped);
}


// tripped is not reimplemented
TEST_F(TestDPIFaultManager, DISABLED_ShouldThrowExceptionIfFailedToReadFaultTrip) {
	DPIFaultManager manager;

	cip::MessageRouterResponse response;
	response.setGeneralStatusCode(cip::GeneralStatusCodes::ATTRIBUTE_NOT_SUPPORTED);

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x0F, 0, 4))).WillOnce(::testing::Return(response));

	EXPECT_THROW(manager.handleFaultParameters(_nullSession, _messageRouter),std::runtime_error);
}


TEST_F(TestDPIFaultManager, ShouldWriteCommand) {
	DPIFaultManager manager;
	cip::MessageRouterResponse response;

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::SET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 3),
											 std::vector<uint8_t >{2})).WillOnce(::testing::Return(response));

	manager.writeCommand(DPIFaultManagerCommands::CLEAR_FAULT_QUEUE, _nullSession, _messageRouter);
}

TEST_F(TestDPIFaultManager, ShouldGenerateEventForEachNewFaultAndWhenCleanTheQueue) {
	DPIFaultManager manager;

	std::vector<DPIFaultParameter> faultParameters;
	manager.setNewFaultListener([&faultParameters](const DPIFaultParameter& object) {
		faultParameters.push_back(object);
	});

	cip::MessageRouterResponse response;
	response.setData({4,0});

	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 7, 1)
	)).WillOnce(::testing::Return(response));

	response.setData({5,0});
	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 8, 1)
	)).WillOnce(::testing::Return(response));

	response.setData({0,0});
	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 9, 1)
	)).WillOnce(::testing::Return(response));

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::SET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 3),
											 std::vector<uint8_t >{2}))
											 .WillOnce(::testing::Return(response));

	manager.handleFaultParameters(_nullSession, _messageRouter);
	EXPECT_EQ(2, faultParameters.size());
	EXPECT_EQ(4, faultParameters[0].getFullInformation().faultDetails.faultCode);
	EXPECT_EQ(5, faultParameters[1].getFullInformation().faultDetails.faultCode);
}

TEST_F(TestDPIFaultManager, ShouldStopReadingFaultsIfHasFaultCodeZero) {
	DPIFaultManager manager;

	std::vector<DPIFaultParameter> faultParameters;
	manager.setNewFaultListener([&faultParameters](const DPIFaultParameter& object) {
		faultParameters.push_back(object);
	});

	cip::MessageRouterResponse response;
	response.setData({0, 0});

	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 7, 1)
	)).WillOnce(::testing::Return(response));

	manager.handleFaultParameters(_nullSession, _messageRouter);
	EXPECT_TRUE(faultParameters.empty());
}

TEST_F(TestDPIFaultManager, ShouldGenerateEventForEachNewFaultAndNotCleanFaults) {
	DPIFaultManager manager(false, false, false);

	std::vector<DPIFaultParameter> faultParameters;
	manager.setNewFaultListener([&faultParameters](const DPIFaultParameter& object) {
		faultParameters.push_back(object);
	});

	cip::MessageRouterResponse response;
	response.setData({4,0});

	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 7, 1)
	)).WillOnce(::testing::Return(response));

	response.setData({5,0});
	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 8, 1)
	)).WillOnce(::testing::Return(response));

	response.setData({0,0});
	EXPECT_CALL(*_messageRouter, sendRequest(
			_nullSession,
			cip::ServiceCodes::GET_ATTRIBUTE_SINGLE,
			cip::EPath(0x0F, 9, 1)
	)).WillOnce(::testing::Return(response));

	EXPECT_CALL(*_messageRouter, sendRequest(_nullSession, cip::ServiceCodes::SET_ATTRIBUTE_SINGLE,
											 cip::EPath(0x97, 0, 3),
											 std::vector<uint8_t >{2})).Times(0);

	manager.handleFaultParameters(_nullSession, _messageRouter);
	EXPECT_EQ(2, faultParameters.size());
}