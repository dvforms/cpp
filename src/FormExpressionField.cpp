#include "FormExpressionField.h"

using namespace dv::forms;

json FormExpressionField::generateSchema() const {
  json rt;
  rt["field"] = field;
  return rt;
}

void FormExpressionField::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  auto val = j.sub( "field" );
  if ( val ) {
    dv::json::JSONSerialiser<FormExpressionField>::from_json( *val, field, path / "field" );
  }
}
