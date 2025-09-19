
//
// Created by Aleksey Timin on 12/4/19.
//

#include <gtest/gtest.h>
#include "eip_scanner/cip/CipString.h"
#include "eip_scanner/utils/Buffer.h"

using eip_scanner::cip::CipBaseString;
using eip_scanner::utils::Buffer;

TEST(TestCipString, ShuoldConvertToStdString) {
	CipBaseString<uint8_t> cipString("Hello!");

	EXPECT_EQ("Hello!", cipString.toStdString());
}

TEST(TestCipString, ShuoldDecodeAndEncodeByBuffer) {
	CipBaseString<uint8_t> srcString("Hello!");
	CipBaseString<uint8_t> dstString;

	Buffer buffer;
	buffer << srcString >> dstString;


	EXPECT_EQ(dstString.toStdString(), srcString.toStdString());
}