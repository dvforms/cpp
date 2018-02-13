#include "FormInputCurrency.h"

using namespace dv::forms;

FormInputCurrency::FormInputCurrency() = default;

FormInputCurrency::~FormInputCurrency() = default;

FieldType FormInputCurrency::getType() const {
  return FieldType::CURRENCY;
}
