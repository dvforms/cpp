#include "FormInputSimple.h"
#include "FormComponent.h"  // for to_json
#include "FormExpression.h" // IWYU pragma: keep
#include "../../../../../opt/local/libexec/llvm-5.0/include/c++/v1/string"
#include <json.h>

using namespace dv::forms;
using namespace dv::json;

FormInputSimple::FormInputSimple() = default;
namespace dv {
  namespace forms {
    template<typename T> bool operator==( const std::weak_ptr<T> &a, const std::weak_ptr<T> &b ) { return a.lock() == b.lock(); }
  }
}

bool operator!=( const FormInputSimple::requiredType &require, bool value ) { return require != FormInputSimple::requiredType( value ); }

json FormInputSimple::generateSchema() const {
  auto rt = FormInput::generateSchema();

  if ( required != false ) { rt["required"] = required; }
  if ( valid ) { rt["valid"] = valid; }
  if ( visible ) { rt["visible"] = visible; }
  if ( !placeholder.empty() ) { rt["placeholder"] = placeholder; }

  return rt;
}

FormInputSimple::~FormInputSimple() = default;

const FormInputSimple::requiredType &FormInputSimple::getRequired() const { return required; }

void FormInputSimple::setRequired( const FormInputSimple::requiredType &nRequired ) { required = nRequired; }

const FormInputSimple::validType &FormInputSimple::getValid() const { return valid; }

void FormInputSimple::setValid( const FormInputSimple::validType &nValid ) { valid = nValid; }

const FormInputSimple::visibleType &FormInputSimple::getVisible() const { return visible; }

void FormInputSimple::setVisible( const FormInputSimple::visibleType &nVisible ) { visible = nVisible; }

const std::string &FormInputSimple::getPlaceholder() const {
  return placeholder;
}

void FormInputSimple::setPlaceholder( const std::string &nPlaceholder ) {
  placeholder = nPlaceholder;
}

void FormInputSimple::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  FormInput::fromJSON( j, path );
  auto val = j.sub( "required" );
  if ( val ) {
    if ( val->is<bool>() ) {
      required = val->as<bool>();
    } else {
      //      input.required = input.getForm()->getExpression( val.as<std::string>() );
    }
  }
  val = j.sub( "placeholder" );
  if ( val ) {
    dv::json::JSONSerialiser<FormInputSimple>::from_json( *val, placeholder, path / "placeholder" );
  }
}
