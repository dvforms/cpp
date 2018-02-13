#include "FormFieldTypeDeterminer.h"
#include "FormFwd.h"
#include "FormGenerator.h" // for FormGeneratorPtr, FormGenerator
#include "FormInput.h"
#include "FormInputCurrency.h"
#include "FormInputEnum.h"
#include "FormInputMultiple.h"
#include "FormInputRepeating.h"
#include "FormInputSimple.h" // for from_json
#include "FormInputStatic.h"
#include "FormInputText.h" // for FormInputText
#include <json.h>          // for JSONContext, JSONErrorCollector, JSONPath, JSON, JSONSerialiser
#include <memory>          // for shared_ptr, __shared_ptr_access
#include <sstream>         // for operator<<, ostringstream, basic_ostream
#include <string>          // for operator==, basic_string, operator+, char_traits, string

using namespace dv::forms;

FormComponentPtr FormFieldTypeDeterminer::fromJSON( const json &j, FormGeneratorPtr &form, const dv::json::JSONPath &path ) {
  FormComponentPtr rt;
  const auto type = j.sub( "type" );
  if ( !type ) {
    std::ostringstream msg;
    msg << "No type field!: " << j;
    dv::json::JSONContext::current()->get<dv::json::JSONErrorCollector>()->error( path / "type", msg.str() );
  } else {
    auto x = type->as<std::string>();

    if ( x == "repeating" ) {
      rt = dv::json::JSONSerialiser<FormInput>::from_json( j, form->create<FormInputRepeating>(), path );
    } else if ( x == "multiple" ) {
      rt = dv::json::JSONSerialiser<FormInput>::from_json( j, form->create<FormInputMultiple>(), path );
    } else if ( x == "currency" ) {
      rt = dv::json::JSONSerialiser<FormInput>::from_json( j, form->create<FormInputCurrency>(), path );
    } else if ( x == "enum" ) {
      rt = dv::json::JSONSerialiser<FormInput>::from_json( j, form->create<FormInputEnum>(), path );
    } else if ( x == "text" ) {
      rt = dv::json::JSONSerialiser<FormInput>::from_json( j, form->create<FormInputText>(), path );
    } else if ( x == "static" ) {
      rt = dv::json::JSONSerialiser<FormInput>::from_json( j, form->create<FormInputStatic>(), path );
    } else {
      dv::json::JSONContext::current()->get<dv::json::JSONErrorCollector>()->error( path / "type", "Unknown field type " + x );
    }
  }

  return rt;
}

FormComponentPtr FormFieldTypeDeterminer::fromJSON( const dv::json::JSONPtr &j, FormGeneratorPtr &form, const dv::json::JSONPath &path ) {
  return fromJSON( *j, form, path );
}
