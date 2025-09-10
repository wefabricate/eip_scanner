//
// Created by Aleksey Timin on 11/16/19.
//

#include <gtest/gtest.h>
#include "eip_scanner/utils/Logger.h"

int main(int argc, char **argv) {
	eip_scanner::utils::Logger::setLogLevel(eip_scanner::utils::LogLevel::DEBUG);

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}