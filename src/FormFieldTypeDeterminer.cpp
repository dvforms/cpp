#include "FormFieldTypeDeterminer.h"
#include <sstream>
#include "JSONErrorCollector.h"
#include "FormGenerator.h"
#include "FormInputText.h"

using namespace dv::forms;

FormComponentPtr
FormFieldTypeDeterminer::fromJSON( const json &j, FormGeneratorPtr &form, const dv::json::JSONErrorCollectorPtr &collector, const dv::json::JSONPath &path ) {
  FormComponentPtr rt;
  const auto type = j.sub( "type" );
  if ( !type ) {
    std::ostringstream msg;
    msg << "No type field!: " << j;
    collector->error( path / "type", msg.str() );
  } else {
    auto x = type->as<std::string>();

    if ( x == "repeating" ) {
    } else if ( x == "multiple" ) {
    } else if ( x == "currency" ) {
    } else if ( x == "enum" ) {
    } else if ( x == "text" ) {
      rt = dv::json::JSONSerialiser<FormInput>::from_json( j, form->create<FormInputText>(), collector, path );
    } else if ( x == "static" ) {
    } else {
      collector->error( path / "type", "Unknown field type " + x );
    }
  }

  return rt;
}

FormComponentPtr FormFieldTypeDeterminer::fromJSON( const dv::json::JSONPtr &j, FormGeneratorPtr &form, const dv::json::JSONErrorCollectorPtr &collector,
                                                    const dv::json::JSONPath &path ) {
  return fromJSON( *j, form, collector, path );
}
