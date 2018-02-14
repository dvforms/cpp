#include "FieldContainer.h"
#include "FormFieldTypeDeterminer.h"
#include "FormExpression.h"
#include "FormExpressionAnd.h"
#include "FormExpressionWrapper.h"
#include "FormExpressionEquals.h"
#include "FormExpressionOr.h"
#include "FormExpressionContext.h"
#include "FormExpressionNot.h"
#include "FormExpressionEmpty.h"
#include "FormExpressionField.h"
#include "FormExpressionValue.h"
#include "FormExpressionGreaterThan.h"
#include <json.h>                     // for JSONContext, JSON, JSONPath, JSONErrorCollector, operator+, JSON::ObjectIterator, JSONSerialiser, Type, Type::O...
#include <stddef.h>                   // for size_t
#include <utility>                    // for pair
#include "FormGenerator.h"            // for FormGenerator

using namespace dv::forms;
using dv::json::JSONContext;

void FieldContainer::addComponent( const std::string &name, const FormComponentPtr &field ) {
  if ( field ) { fields.emplace( name, field ); }
}

void FieldContainer::addExpression( const std::string &name, const FormExpressionPtr &expression ) {
  expressions.emplace( name, getForm()->create<FormExpressionWrapper>( name, expression ) );
}

void FieldContainer::fromJSON( const json &j, const dv::json::JSONPath &path ) {
  auto c = JSONContext::current()->get<FormExpressionContext>()
                                 ->enter(
                                   std::static_pointer_cast<FormExpressionContainer>( std::dynamic_pointer_cast<FieldContainer>( shared_from_this() ) ) );
  auto value = j.sub( "expressions" );
  if ( value ) {
    auto form = getForm();
    for ( const auto &item : value->objectIterator() ) {
      addExpression( item.first, expressionFromJSON( *item.second, *form, path / item.first ) );
    }
  }
  value = j.sub( "fields" );
  if ( value ) {
    auto form = getForm();
    for ( const auto &item : value->objectIterator() ) {
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

FormExpressionPtr FieldContainer::expressionFromJSON( const json &j, FormGenerator &form, const dv::json::JSONPath &path ) {
  dv::forms::FormExpressionPtr rt;
  if ( j.type() != dv::json::Type::OBJECT ) {
    dv::json::JSONContext::current()->get<dv::json::JSONErrorCollector>()->error( path, "Expected to be an object found " + j.type() );
  } else {
    const auto &item = j.objectIterator().begin();
    if ( item != j.objectIterator().end() ) {
      if ( item->first == "and" ) {
        rt = form.create<FormExpressionAnd>();
      } else if ( item->first == "equals" ) {
        rt = form.create<FormExpressionEquals>();
      } else if ( item->first == "or" ) {
        rt = form.create<FormExpressionOr>();
      } else if ( item->first == "not" ) {
        rt = form.create<FormExpressionNot>();
      } else if ( item->first == "empty" ) {
        rt = form.create<FormExpressionEmpty>();
      } else if ( item->first == "field" ) {
        rt = form.create<FormExpressionField>();
      } else if ( item->first == "value" ) {
        rt = form.create<FormExpressionValue>();
      } else if ( item->first == ">" ) {
        rt = form.create<FormExpressionGreaterThan>();
      } else {
        dv::json::JSONContext::current()->get<dv::json::JSONErrorCollector>()->error( path / item->first, "Unknown expression type " );
      }
      if ( rt ) {
        dv::json::JSONSerialiser<FieldContainer>::from_json( j, rt, path / item->first );
      }
    }
  }
  return rt;
}

FormExpressionWrapperPtr FieldContainer::getExpression( const std::string &name ) const {
  FormExpressionWrapperPtr rt;
  const auto &item = expressions.find( name );
  if ( item != expressions.end() ) {
    rt = item->second;
  }
  return rt;
}
