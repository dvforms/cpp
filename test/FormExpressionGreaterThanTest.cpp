#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Forms.h>
#include "BooleanExpressionMatcher.h"

using namespace testing;
using namespace dv::forms;
using namespace dv::json;

class FormExpressionGreaterThanTest : public ::testing::Test {
  void SetUp() override {
    RecordProperty( "COVERS", "FormExpressionGreaterThan.cpp:FormExpressionGreaterThan.h" );
  }
};

TEST_F( FormExpressionGreaterThanTest, TestToJson ) {
  auto form = std::make_shared<FormGenerator>();

  auto exp = form->create<FormExpressionGreaterThan>();
  exp->add<FormExpressionValue>( "hi all" );

  json expected;
  expected[">"][0]["value"] = "hi all";

  auto schema = exp->generateSchema();
  JSONDiffListenerImpl listener;
  expected.compare( schema, listener );
  EXPECT_EQ( expected, schema ) << listener;
}

TEST_F( FormExpressionGreaterThanTest, TestFromJson ) {
  auto form = std::make_shared<FormGenerator>();

  json input;
  input[">"][0]["value"] = "hi all";

  auto exp = FieldContainer::expressionFromJSON( input, *form, JSONPath() );

  ASSERT_THAT( exp.get(), WhenDynamicCastTo<FormExpressionGreaterThan *>(
    booleanExpressionMatcher<FormExpressionGreaterThan *>().subExpressionsMatch( SizeIs( 1 ) ).build() )
  ) << exp->generateSchema();
}
