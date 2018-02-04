#include "FormGenerator.h"
#include "FormSection.h"
#include <sstream>
#include <utility>          // for pair
#include "FormComponent.h"  // for json, to_json

using namespace dv::forms;
using namespace dv::json;

FormGenerator::FormGenerator() = default;

FormGenerator::~FormGenerator() = default;

std::string FormGenerator::getSchema() const {
  std::ostringstream str;
  auto schema = generateSchema();
  str << schema;
  return str.str();
}

json FormGenerator::generateSchema() const {
  json schema;
  schema["sections"] = buildSections();
  schema["properties"] = nullptr;
  schema["expressions"] = nullptr;
  return schema;
}

json FormGenerator::buildSections() const {
  json rt;
  for ( const auto &item : sections ) {
    rt[item.first] = item.second;
  }
  return rt;
}

FormSectionPtr &FormGenerator::addSection( const std::string &name ) {
  auto section = create<FormSection>( sections.size() + 1 );
  auto item = sections.emplace( name, section );
  return item.first->second;
}
