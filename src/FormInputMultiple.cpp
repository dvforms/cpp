#include "FormInputMultiple.h"

using namespace dv::forms;

FieldType FormInputMultiple::getType() const { return FieldType::MULTIPLE; }

void FormInputMultiple::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  FormInput::fromJSON( j, path );
  FieldContainer::fromJSON( j, path );
}

json FormInputMultiple::generateSchema() const {
  auto rt = FormInput::generateSchema();

  rt["fields"] = FieldContainer::generateSchema()["fields"];

  return rt;
}
