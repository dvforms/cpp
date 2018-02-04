#include "FormSection.h"
#include <map>      // for map
#include <utility>  // for pair
#include "json.h"   // for JSON

using namespace dv::forms;

FormSection::FormSection( int32_t nOrder ) : FormComponent(), order( nOrder ) {
}

FormSection::~FormSection() = default;

json FormSection::generateSchema() const {
  json rt;

  std::map<std::string, json> fieldsJSON;

  int i = 1;
  for ( const auto &item: fields ) {
    auto j = item.second->generateSchema();
    j["order"] = i;
    fieldsJSON.emplace( item.first, j );
  }

  rt["fields"] = fieldsJSON;
  rt["order"] = order;
  return rt;
}

void FormSection::addComponent( const std::string &name, const FormComponentPtr &field ) {
  fields.emplace( name, field );
}
