#include "FormInputSimple.h"
#include "FormComponent.h"  // for to_json
#include "FormExpression.h" // IWYU pragma: keep
#include "FormGenerator.h"
#include "json.h"

using namespace dv::forms;
using namespace dv::json;

FormInputSimple::FormInputSimple() = default;

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

void dv::forms::from_json( const json &j, FormInputSimple &input, const dv::json::JSONPath &/*path*/ ) {
  auto val = j.sub( "required" );
  if ( val ) {
    if ( val->is<bool>() ) {
      input.required = val->as<bool>();
    } else {
//      input.required = input.getForm()->getExpression( val.as<std::string>() );
    }
  }
}
