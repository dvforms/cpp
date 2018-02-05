#include "FormGenerator.h"
#include "FormComponent.h"  // for json, to_json
#include "FormSection.h"
#include <sstream>
#include <utility>          // for pair

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
  auto i = sections.size();
  for ( const auto &item : sections ) {
    auto &j = rt[item.first] = item.second;
    j["order"] = i--;
  }
  return rt;
}

FormSectionPtr &FormGenerator::addSection( const std::string &name ) {
  auto section = create<FormSection>();
  auto item = sections.emplace( name, section );
  return item.first->second;
}
