#include "FormSection.h"
#include <cstddef> // for size_t
#include <json.h>  // for JSON
#include <memory>   // for shared_ptr, __shared_ptr_access, __shared_ptr_access<>::element_type

using namespace dv::forms;
using std::size_t;

FormSection::FormSection() = default;

FormSection::~FormSection() = default;

json FormSection::generateSchema() const {
  json rt = FieldContainer::generateSchema();
  if ( !label.empty() ) { rt["label"] = label; }
  return rt;
}

const std::string &FormSection::getLabel() const { return label; }

void FormSection::setLabel( const std::string &nLabel ) { label = nLabel; }

void FormSection::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  FieldContainer::fromJSON( j, path );
  auto value = j.sub( "label" );
  if ( value ) { dv::json::JSONSerialiser<std::string>::from_json( *value, label, path / "label" ); }
}
