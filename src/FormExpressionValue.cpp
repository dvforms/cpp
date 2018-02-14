#include "FormExpressionValue.h"

using namespace dv::forms;

json FormExpressionValue::generateSchema() const {
  json rt;
  rt["value"] = value;
  return rt;
}

void FormExpressionValue::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  auto val = j.sub( "value" );
  if ( val ) {
    value = *val;
  }
}

FormExpressionValue::FormExpressionValue( const json &nValue ) {
  value = nValue;
}
