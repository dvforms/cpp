#include "FormInputNumber.h"

using namespace dv::forms;

FormInputNumber::FormInputNumber() = default;

FormInputNumber::~FormInputNumber() = default;

FieldType FormInputNumber::getType() const {
  return FieldType::NUMBER;
}
