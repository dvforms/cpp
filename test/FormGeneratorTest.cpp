#include <gtest/gtest.h>
#include <gmock/gmock.h>    // IWYU pragma: keep
#include <FormGenerator.h>
#include <FormSection.h>
#include <FormInputText.h>  // IWYU pragma: keep
#include "JSONPath.h"       // IWYU pragma: keep
#include "json.h"           // for JSON, JSONDiffListenerImpl, operator<<
#include "JSONParser.h"
#include <fstream>
#include <memory>           // for __shared_ptr_access, shared_ptr, make_shared

using namespace testing;
using namespace dv::forms;
using namespace dv::json;

class FormGeneratorTest : public ::testing::Test {
  void SetUp() override {
    RecordProperty( "COVERS", "FormGenerator.cpp:FormGenerator.h" );
  }
};

TEST_F( FormGeneratorTest, Test ) {
  auto form = std::make_shared<FormGenerator>();
  auto section = form->addSection( "Main" );
  form->addSection( "ABC" );
  section->add<FormInputText>( "test" );
  JSON expected;
  expected["sections"]["ABC"]["order"] = 2;
  expected["sections"]["ABC"]["fields"] = nullptr;
  expected["sections"]["Main"]["order"] = 1;
  expected["sections"]["Main"]["fields"]["test"]["order"] = 1;
  expected["sections"]["Main"]["fields"]["test"]["required"] = false;
  expected["sections"]["Main"]["fields"]["test"]["type"] = "text";
  expected["expressions"] = nullptr;
  expected["properties"] = nullptr;
  auto schema = form->generateSchema();

  JSONDiffListenerImpl listener;
  expected.compare( schema, listener );
  EXPECT_EQ( expected, schema ) << listener;
}

TEST_F( FormGeneratorTest, ParseTest ) {
  auto form = std::make_shared<FormGenerator>();

  json j;
  JSONParser p;
  std::ifstream in;
  in.open( SOURCE_DIR "/example.json" );
  ASSERT_TRUE( in.is_open() );
  p.parseInto( j, in );
  ASSERT_NE( j, nullptr );
  form->parseJSON( j );

//  FAIL() << j << std::endl << "---------------" << std::endl << form->generateSchema();
}
