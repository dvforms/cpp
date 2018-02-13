#include "FormSection.h"
#include "FormFieldTypeDeterminer.h"
#include <cstddef>                    // for size_t
#include <json.h>                     // for JSON
#include <utility>                    // for pair

using namespace dv::forms;
using std::size_t;

FormSection::FormSection() = default;

FormSection::~FormSection() = default;

json FormSection::generateSchema() const {
  json rt;

  size_t i = 1;
  auto &f = rt["fields"];
  f = {};
  for ( const auto &item: fields ) {
    auto j = item.second->generateSchema();
    j["order"] = i++;
    f[item.first] = j;
  }

  if ( !label.empty() ) {
    rt["label"] = label;
  }
  return rt;
}

void FormSection::addComponent( const std::string &name, const FormComponentPtr &field ) {
  if ( field ) {
    fields.emplace( fields.end(), name, field );
  }
}

const std::string &FormSection::getLabel() const {
  return label;
}

void FormSection::setLabel( const std::string &nLabel ) {
  label = nLabel;
}

void dv::forms::from_json( const json &j, FormSection &section, const dv::json::JSONPath &path ) {
  auto fields = j.sub( "fields" );
  if ( fields ) {
    auto form = section.getForm();
    for ( const auto &item : fields->objectIterator() ) {
      section.addComponent( item.first, FormFieldTypeDeterminer::fromJSON( item.second, form, path / "fields" / item.first ) );
    }
  } else {
    dv::json::JSONContext::current()->get<dv::json::JSONErrorCollector>()->error( path, "No fields" );
  }

  auto label = j.sub( "label" );
  if ( label ) {
    dv::json::JSONSerialiser<std::string>::from_json( *label, section.label, path / "label" );
  }
}
