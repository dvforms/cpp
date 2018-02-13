#include "FormInput.h"
#include <json.h>  // for JSON
#include <memory>

using namespace dv::forms;
using namespace dv::json;
FormInput::FormInput() = default;

FormInput::~FormInput() = default;

json FormInput::generateSchema() const {
  json rt;

  rt["type"] = getType();
  if ( !label.empty() ) {
    rt["label"] = label;
  }
  if ( !help.empty() ) {
    rt["help"] = help;
  }

  return rt;
}

void dv::forms::from_json( const json &j, FormInput &input, const dv::json::JSONPath &path ) {
  auto label = j.sub( "label" );
  if ( label ) {
    dv::json::JSONSerialiser<FormInput>::from_json( *label, input.label, path / "label" );
  }
  auto help = j.sub( "help" );
  if ( help ) {
    dv::json::JSONSerialiser<FormInput>::from_json( *help, input.help, path / "help" );
  }
}
