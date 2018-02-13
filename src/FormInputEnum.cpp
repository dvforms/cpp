#include "FormInputEnum.h"
#include <boost/variant/get.hpp> // for get
#include <json.h>                // for JSON, JSONContext, Type, JSONErrorCollector, JSONSerialiser, Type::ARRAY, Type::OBJECT
#include <memory>                // for shared_ptr, __shared_ptr_access, make_shared, allocator
#include <utility>               // for pair

using namespace dv::forms;

FormInputEnum::FormInputEnum() = default;

FormInputEnum::~FormInputEnum() = default;

FieldType FormInputEnum::getType() const { return FieldType::ENUM; }

json FormInputEnum::generateSchema() const {
  json j = FormInputSimple::generateSchema();

  bool assoc = false;
  for ( const auto &item : options ) {
    if ( item.first.type() != typeid( std::size_t ) ) {
      assoc = true;
      break;
    }
  }

  auto &opt = j["options"];
  if ( assoc ) {
    for ( const auto &item : options ) { opt[toKey( item.first )] = item.second; }
  } else {
    for ( const auto &item : options ) { opt.emplaceBack( std::make_shared<json>( item.second ) ); }
  }

  return j;
}

void dv::forms::from_json( const json &j, FormInputEnum &input, const dv::json::JSONPath &path ) {
  dv::json::JSONSerialiser<FormInputEnum>::from_json( j, *static_cast<FormInputSimple *>( &input ), path );

  auto opts = j.sub( "options" );
  if ( opts ) {
    if ( opts->type() == dv::json::Type::OBJECT ) {
      for ( const auto &item : opts->objectIterator() ) { input.options.emplace( item.first, item.second->as<std::string>() ); }
    } else if ( opts->type() == dv::json::Type::ARRAY ) {
      size_t i = 0;
      for ( const auto &item : opts->arrayIterator() ) { input.options.emplace( i++, item->as<std::string>() ); }
    } else {
      dv::json::JSONContext::current()->get<dv::json::JSONErrorCollector>()->error( path, "Unexpected type" );
    }
  }
}

std::string FormInputEnum::toKey( const FormInputEnum::keyType &key ) const { return boost::get<std::string>( key ); }
