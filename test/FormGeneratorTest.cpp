#include <gmock/gmock.h> // IWYU pragma: keep
#include <gtest/gtest.h> //
#include "Forms.h"       //
#include <fstream>       //
#include <json.h>        // for JSON, JSONDiffListenerImpl, operator<<
#include <memory>        // for __shared_ptr_access, shared_ptr, make_shared

using namespace testing;
using namespace dv::forms;
using namespace dv::json;

class FormGeneratorTest : public ::testing::Test {
  void SetUp() override { RecordProperty( "COVERS", "FormGenerator.cpp:FormGenerator.h" ); }
};

TEST_F( FormGeneratorTest, Test ) {
  auto form = std::make_shared<FormGenerator>();
  auto section = form->addSection( "Main" );
  form->addSection( "ABC" );
  auto field = section->addField<FormInputText>( "test" );
  field->setRequired( true );
  JSON expected;
  expected["$schema"] = "http://dvforms.org/v1#";
  expected["sections"]["ABC"]["order"] = 2;
  expected["sections"]["ABC"]["fields"] = {};
  expected["sections"]["Main"]["order"] = 1;
  expected["sections"]["Main"]["fields"]["test"]["order"] = 1;
  expected["sections"]["Main"]["fields"]["test"]["required"] = true;
  expected["sections"]["Main"]["fields"]["test"]["type"] = "text";
  expected["sections"]["Main"]["fields"]["test"]["label"] = "";
  //  expected["expressions"] = {};
  //  expected["properties"] = {};
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
  try {
    form->parseJSON( j );
  } catch ( std::exception &e ) {
    FAIL() << e.what();
  }

  auto schema = form->generateSchema();
  JSONDiffListenerImpl listener;
  j.compare( schema, listener );
  EXPECT_EQ( j, schema ) << listener;
}
