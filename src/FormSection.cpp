#include "FormSection.h"
#include "json.h"   // for JSON
#include <utility>  // for pair

using namespace dv::forms;

FormSection::FormSection() = default;

FormSection::~FormSection() = default;

json FormSection::generateSchema() const {
  json rt;

  std::unordered_map<std::string, json> fieldsJSON;

  for ( const auto &item: fields ) {
    auto j = item.second->generateSchema();
    j["order"] = fields.size() - fieldsJSON.size();
    fieldsJSON.emplace( item.first, j );
  }

  rt["fields"] = fieldsJSON;
  return rt;
}

void FormSection::addComponent( const std::string &name, const FormComponentPtr &field ) {
  fields.emplace( name, field );
}
