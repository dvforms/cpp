#include "FieldContainer.h"
#include "FormFieldTypeDeterminer.h"

using namespace dv::forms;

void FieldContainer::addComponent( const std::string &name, const FormComponentPtr &field ) {
  if ( field ) { fields.emplace( name, field ); }
}

void FieldContainer::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  auto fields = j.sub( "fields" );
  if ( fields ) {
    auto form = getForm();
    for ( const auto &item : fields->objectIterator() ) {
      addComponent( item.first, FormFieldTypeDeterminer::fromJSON( item.second, form, path / "fields" / item.first ) );
    }
  } else {
    dv::json::JSONContext::current()->get<dv::json::JSONErrorCollector>()->error( path, "No fields" );
  }
}

json FieldContainer::generateSchema() const {
  json rt;
  size_t i = 1;
  auto &f = rt["fields"];
  f = {};
  for ( const auto &item : fields ) {
    auto j = item.second->generateSchema();
    j["order"] = i++;
    f[item.first] = j;
  }
  return rt;
}
