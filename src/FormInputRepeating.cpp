#include "FormInputRepeating.h"

using namespace dv::forms;

FieldType FormInputRepeating::getType() const { return FieldType::REPEATING; }

json FormInputRepeating::generateSchema() const {
  auto rt = FormInput::generateSchema();
  rt["fields"] = FieldContainer::generateSchema()["fields"];
  if ( inlineValue ) { rt["inline"] = true; }
  if ( minRows ) { rt["minRows"] = minRows; }
  if ( maxRows != Unlimited ) { rt["maxRows"] = maxRows; }
  return rt;
}

void FormInputRepeating::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  FormInput::fromJSON( j, path );
  FieldContainer::fromJSON( j, path );

  auto value = j.sub( "minRows" );
  if ( value ) { dv::json::JSONSerialiser<FormInputRepeating>::from_json( *value, minRows, path / "minRows" ); }
  value = j.sub( "maxRows" );
  if ( value ) {
    if ( value->type() == dv::json::Type::STRING && *value == "unlimited" ) {
      maxRows = FormInputRepeating::Unlimited;
    } else {
      dv::json::JSONSerialiser<FormInputRepeating>::from_json( *value, maxRows, path / "maxRows" );
    }
  }
  value = j.sub( "inline" );
  if ( value ) { dv::json::JSONSerialiser<FormInputRepeating>::from_json( *value, inlineValue, path / "inline" ); }
}
