#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <DVFormGenerator.h>
#include <FormInputText.h>

using namespace testing;
using namespace dv::forms;

class DVFormGeneratorTest : public ::testing::Test {
  void SetUp() override {
    RecordProperty( "COVERS", "DVFormGenerator.cpp:DVFormGenerator.h" );
  }
};

TEST_F( DVFormGeneratorTest, Test ) {
  auto form = std::make_shared<DVFormGenerator>();
  form->addComponent( std::make_shared<FormInputText>() );
}
