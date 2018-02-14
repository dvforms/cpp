#include "FormExpressionNested.h"
#include "FormExpression.h"
#include "FieldContainer.h"

using namespace dv::forms;

json FormExpressionNested::generateSchema() const {
  json rt;
  if ( nested ) {
    rt[getType()] = nested->generateSchema();
  } else {
    rt[getType()] = nullptr;
  }
  return rt;
}

void FormExpressionNested::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  auto val = j.sub( getType() );
  if ( val ) {
    nested = FieldContainer::expressionFromJSON( *val, *getForm(), path / getType() );
  }
}
