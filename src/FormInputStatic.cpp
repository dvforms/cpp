#include "FormFwd.h"
#include "FormInputStatic.h"

using namespace dv::forms;

FormInputStatic::FormInputStatic() = default;
FormInputStatic::~FormInputStatic() = default;

FieldType FormInputStatic::getType() const {
  return FieldType::STATIC;
}
