#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <json.h>
#include <JSONPath.h>

using namespace testing;
using namespace dv::json;

class JSONTest : public ::testing::Test {
  void SetUp() override {
    RecordProperty( "COVERS", "JSON.cpp:JSON.h" );
  }
};

TEST_F( JSONTest, Test ) {
  JSON json1, json2;

  JSONDiffListenerImpl listener;

  EXPECT_TRUE( json1.compare( json2, listener ) ) << listener;
  listener.clear();

  json1["abc"] = nullptr;
  json2["xyz"] = nullptr;
  EXPECT_FALSE( json1.compare( json2, listener ) );
  EXPECT_EQ( "$.abc: Missing from right\n$.xyz: Missing from left\n", listener.toString() );
  listener.clear();

  json1 = 1;
  json2 = 1;
  EXPECT_TRUE( json1.compare( json2, listener ) ) << listener;
  listener.clear();

  json1 = 1;
  json2 = 1.0;
  EXPECT_FALSE( json1.compare( json2, listener ) );
  EXPECT_EQ( "$: Type mismatch int != double\n", listener.toString() );
  listener.clear();

  json1 = 1;
  json2 = 2;
  EXPECT_FALSE( json1.compare( json2, listener ) );
  EXPECT_EQ( "$: 1 != 2\n", listener.toString() );
  listener.clear();
}
