#include "FormInputText.h"
#include <json.h>
#include <memory>

using namespace dv::forms;

FormInputText::FormInputText() = default;

FormInputText::~FormInputText() = default;

FieldType FormInputText::getType() const { return FieldType::TEXT; }

json FormInputText::generateSchema() const {
  auto rt = FormInputSimple::generateSchema();
  if ( !suggestions.empty() ) {
    json &a = rt["suggestions"];
    a = dv::json::Type::ARRAY;
    for ( const auto &it : suggestions ) { a.emplaceBack( it ); }
  }
  return rt;
}

void FormInputText::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  FormInputSimple::fromJSON( j, path );
  auto val = j.sub( "suggestions" );
  if ( val ) {
    for ( const auto &item : val->arrayIterator() ) {
      std::string rt;
      dv::json::JSONSerialiser<FormInputText>::from_json( *item, rt, path / "suggestions" );
      suggestions.emplace_back( rt );
    }
  }
}
