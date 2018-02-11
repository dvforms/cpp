#include "FormGenerator.h"
#include "FormComponent.h"  // for json, to_json
#include "FormSection.h"
#include <json.h>
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
  schema["$schema"] = "http://dvforms.org/v1#";
  schema["sections"] = buildSections();
  schema["properties"] = {};
  schema["expressions"] = {};
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

void FormGenerator::parseJSON( const json &j ) {
  *this = *j.as<FormGenerator>();
}

void dv::forms::from_json( const json &j, FormGenerator &form, const dv::json::JSONPath &path ) {
  auto sections = j.sub( "sections" );
  if ( sections ) {
    for ( const auto &section : sections->objectIterator() ) {
      auto sec = form.addSection( section.first );
      dv::json::JSONSerialiser<FormSection>::from_json( *section.second, *sec, path / "sections" / section.first );
    }
  }
}
