#include "FormInputText.h"

using namespace dv::forms;

FormInputText::FormInputText() = default;

FormInputText::~FormInputText() = default;

FieldType FormInputText::getType() const { return FieldType::TEXT; }
