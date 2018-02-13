#include "FormInput.h"
#include <json.h> // for JSON
#include <memory>

using namespace dv::forms;
using namespace dv::json;
FormInput::FormInput() = default;

FormInput::~FormInput() = default;

json FormInput::generateSchema() const {
  json rt;

  rt["type"] = getType();
  rt["label"] = label;
  if ( !help.empty() ) { rt["help"] = help; }

  return rt;
}

void FormInput::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  auto value = j.sub( "label" );
  if ( value ) { dv::json::JSONSerialiser<FormInput>::from_json( *value, label, path / "label" ); }
  value = j.sub( "help" );
  if ( value ) { dv::json::JSONSerialiser<FormInput>::from_json( *value, help, path / "help" ); }
}
