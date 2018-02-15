#include "FormInputSimple.h"
#include "FormComponent.h"  // for to_json
#include "FormExpression.h" // IWYU pragma: keep
#include "FormExpressionContext.h"
#include "FormExpressionWrapper.h"// IWYU pragma: keep
#include <json.h>
#include <memory>                   // for shared_ptr, allocator, __shared_ptr_access, __shared_ptr_access<>::element_type, weak_ptr

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

  if ( required != false ) {
    if ( required.type() == typeid( bool ) ) {
      rt["required"] = boost::get<bool>( required );
    } else {
      rt["required"] = boost::get<FormExpressionWrapperWeakPtr>( required ).lock();
    }
  }
  if ( !valid.expired() ) { rt["valid"] = valid.lock(); }
  if ( !visible.expired() ) { rt["visible"] = visible.lock(); }
  if ( !placeholder.empty() ) { rt["placeholder"] = placeholder; }
  if ( size != 0 ) { rt["size"] = size; }

  return rt;
}

FormInputSimple::~FormInputSimple() = default;

const FormInputSimple::requiredType &FormInputSimple::getRequired() const { return required; }

void FormInputSimple::setRequired( const FormInputSimple::requiredType &nRequired ) { required = nRequired; }

const FormInputSimple::validType &FormInputSimple::getValid() const { return valid; }

void FormInputSimple::setValid( const FormInputSimple::validType &nValid ) { valid = nValid; }

const FormInputSimple::visibleType &FormInputSimple::getVisible() const { return visible; }

void FormInputSimple::setVisible( const FormInputSimple::visibleType &nVisible ) { visible = nVisible; }

const std::string &FormInputSimple::getPlaceholder() const { return placeholder; }

void FormInputSimple::setPlaceholder( const std::string &nPlaceholder ) { placeholder = nPlaceholder; }

size_t FormInputSimple::getSize() const { return size; }

void FormInputSimple::setSize( size_t nSize ) { size = nSize; }

void FormInputSimple::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  FormInput::fromJSON( j, path );
  auto val = j.sub( "required" );
  if ( val ) {
    if ( val->is<bool>() ) {
      required = val->as<bool>();
    } else {
      std::string expressionName;
      dv::json::JSONSerialiser<FormInputSimple>::from_json( *val, expressionName, path / "required" );
      auto exp = JSONContext::current()->get<FormExpressionContext>()->getExpression( expressionName );
      if ( !exp ) {
        JSONContext::current()->get<JSONErrorCollector>()->error( path / "required", "Expression " + expressionName + " not found" );
      } else {
        required = exp;
      }
    }
  }
  val = j.sub( "placeholder" );
  if ( val ) { dv::json::JSONSerialiser<FormInputSimple>::from_json( *val, placeholder, path / "placeholder" ); }

  val = j.sub( "size" );
  if ( val ) { dv::json::JSONSerialiser<FormInputSimple>::from_json( *val, size, path / "size" ); }

  val = j.sub( "valid" );
  if ( val ) {
    valid = jsonToExpression( *val, path / "valid" );
  }
  val = j.sub( "visible" );
  if ( val ) {
    visible = jsonToExpression( *val, path / "visible" );
  }
}

FormExpressionWrapperPtr FormInputSimple::jsonToExpression( const json &j, const dv::json::JSONPath &path ) {
  FormExpressionWrapperPtr rt;
  std::string expressionName;
  dv::json::JSONSerialiser<FormInputSimple>::from_json( j, expressionName, path );
  rt = JSONContext::current()->get<FormExpressionContext>()->getExpression( expressionName );
  if ( !rt ) {
    JSONContext::current()->get<JSONErrorCollector>()->error( path, "Expression " + expressionName + " not found" );
  }
  return rt;
}
