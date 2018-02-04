#include "FormInputSimple.h"
#include "variantjson.h"
#include "FormExpression.h"

using namespace dv::forms;
using namespace dv::json;

FormInputSimple::FormInputSimple() : FormInput() {
}

json FormInputSimple::generateSchema() const {
  auto rt = FormInput::generateSchema();

  if ( !required.empty() ) {
    rt["required"] = required;
  }
  if ( valid ) {
    rt["valid"] = valid;
  }
  if ( visible ) {
    rt["visible"] = visible;
  }

  return rt;
}

FormInputSimple::~FormInputSimple() = default;

const FormInputSimple::requiredType &FormInputSimple::getRequired() const {
  return required;
}

void FormInputSimple::setRequired( const FormInputSimple::requiredType &nRequired ) {
  required = nRequired;
}

const FormInputSimple::validType &FormInputSimple::getValid() const {
  return valid;
}

void FormInputSimple::setValid( const FormInputSimple::validType &nValid ) {
  valid = nValid;
}

const FormInputSimple::visibleType &FormInputSimple::getVisible() const {
  return visible;
}

void FormInputSimple::setVisible( const FormInputSimple::visibleType &nVisible ) {
  visible = nVisible;
}

