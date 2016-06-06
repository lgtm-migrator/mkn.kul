/**
Copyright (c) 2016, Philip Deegan.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
    * Neither the name of Philip Deegan nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "kul/os.hpp"
#include "kul/cli.hpp"
#include "kul/log.hpp"
#include "kul/proc.hpp"

TEST(StringTest, ParseCommandLineArguments) {
    std::vector<std::string> v;
    kul::cli::asArgs("/path/to \"words in quotes\" words\\ not\\ in\\ quotes end", v);
	EXPECT_EQ(4, v.size());
	EXPECT_EQ("/path/to", v[0]);
	EXPECT_EQ("words in quotes", v[1]);
	EXPECT_EQ("words not in quotes", v[2]);
	EXPECT_EQ("end", v[3]);
}

TEST(StringTest, SplitByChar) {
    std::vector<std::string> v;
    kul::String::SPLIT("split - by - char - dash", '-', v);
	EXPECT_EQ(4, v.size());
	EXPECT_EQ("split ", v[0]);
	EXPECT_EQ(" by ", v[1]);
	EXPECT_EQ(" char ", v[2]);
	EXPECT_EQ(" dash", v[3]);
}

TEST(StringTest, SplitByString) {
    std::vector<std::string> v;
    kul::String::SPLIT("split - by - char - dash", "-", v);
	EXPECT_EQ(4, v.size());
	EXPECT_EQ("split ", v[0]);
	EXPECT_EQ(" by ", v[1]);
	EXPECT_EQ(" char ", v[2]);
	EXPECT_EQ(" dash", v[3]);
}

TEST(StringTest, SplitByEscapedChar) {
    std::vector<std::string> v;
    kul::String::ESC_SPLIT("split \\- by - char - dash", '-', v);
	EXPECT_EQ(3, v.size());
	EXPECT_EQ("split \\- by ", v[0]);
	EXPECT_EQ(" char ", v[1]);
	EXPECT_EQ(" dash", v[2]);
}

TEST(OperatingSystemTests, HasRAMUsageSupport) {
 	ASSERT_TRUE(kul::this_proc::physicalMemory());
 	ASSERT_TRUE(kul::this_proc::virtualMemory());
}

TEST(OperatingSystemTests, HasFullFileTimeStampSupport) {
  	kul::File f("mkn.yaml");
  	ASSERT_TRUE(f.is());
  	kul::fs::TimeStamps fts = f.timeStamps();
 	ASSERT_TRUE(fts.accessed());
  	ASSERT_TRUE(fts.created());
  	ASSERT_TRUE(fts.modified()); 	 	
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
