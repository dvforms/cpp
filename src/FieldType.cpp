#include "FormFwd.h"
#include <json.h>  // IWYU pragma: keep

void dv::forms::to_json( json &json, dv::forms::FieldType type, const dv::json::JSONPath &/*path*/ ) {
  switch ( type ) {
    case FieldType::TEXT:
      json = "text";
      break;
    case FieldType::NUMBER:
      json = "number";
      break;
    case FieldType::EMAIL:
      json = "email";
      break;
    case FieldType::CURRENCY:
      json = "currency";
      break;
    case FieldType::STATIC:
      json = "static";
      break;
    case FieldType::ENUM:
      json = "enum";
      break;
    case FieldType::MULTIPLE:
      json = "multiple";
      break;
    case FieldType::REPEATING:
      json = "repeating";
      break;
  }
}
