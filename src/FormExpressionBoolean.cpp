#include "FormExpressionBoolean.h"
#include "FieldContainer.h"

using namespace dv::forms;

json FormExpressionBoolean::generateSchema() const {
  json rt;
  rt = {};
  auto &a = rt[getType()];
  for ( const auto &item : subExpressions ) {
    auto j = item->generateSchema();
    a.emplaceBack( j );
  }
  return rt;
}

void FormExpressionBoolean::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  auto val = j.sub( getType() );
  if ( val ) {
    size_t index = 0;
    for ( const auto &item : val->arrayIterator() ) {
      auto exp = FieldContainer::expressionFromJSON( *item, *getForm(), ( path / getType() )[index] );
      subExpressions.push_back( exp );
      index++;
    }
  }
}

const std::vector<FormExpressionPtr> &FormExpressionBoolean::getSubExpressions() const {
  return subExpressions;
}
