#include "FormGenerator.h"
#include "FormComponent.h"       // for json, to_json
#include "FormSection.h"         //
#include "FormExpressionWrapper.h" //
#include "FormExpressionContext.h"//
#include "FieldContainer.h"         // for FieldContainer
#include <stdexcept>                // for runtime_error
#include <UnorderedIndexedMap.h> //
#include <json.h>                //
#include <sstream>               //
#include <utility>               // for pair

using namespace dv::forms;
using namespace dv::json;

const std::string dv::forms::currentSchemaVersion = "http://dvforms.org/v1#";

FormGenerator::FormGenerator() = default;

FormGenerator::~FormGenerator() = default;

std::string FormGenerator::getSchema() const {
  std::ostringstream str;
  auto schema = generateSchema();
  str << schema;
  return str.str();
}

json FormGenerator::generateSchema() const {
  json schema;
  schema["$schema"] = currentSchemaVersion;
  if ( !sections.empty() ) { schema["sections"] = buildSections(); }
  //  schema["properties"] = {};
  if ( !expressions.empty() ) {
    schema["expressions"] = buildExpressions();
  }
  return schema;
}

json FormGenerator::buildSections() const {
  json rt;
  auto i = 1;
  for ( const auto &item : sections ) {
    auto &j = rt[item.first] = item.second;
    j["order"] = i++;
  }
  return rt;
}

json FormGenerator::buildExpressions() const {
  json rt;
  for ( const auto &item : expressions ) {
    rt[item.first] = item.second->getSubExpression();
  }
  return rt;
}

FormSectionPtr &FormGenerator::addSection( const std::string &name ) {
  auto section = create<FormSection>();
  auto item = sections.emplace( name, section );
  return item->second;
}

void FormGenerator::parseJSON( const json &j ) {
  JSONContext context;
  auto c = context.enter();
  auto errors = std::make_shared<JSONErrorCollectorImpl>();
  context.attach<JSONErrorCollector>( errors );
  auto other = j.as<FormGenerator>();
  if ( errors->empty() ) {
    *this = std::move( *other );
  } else {
    std::ostringstream msg;
    bool first = true;
    for ( const auto &item : errors->getMessages() ) {
      if ( !first ) {
        msg << std::endl;
      } else {
        first = false;
      }
      msg << item;
    }
    throw std::runtime_error( msg.str() );
  }
}

FormGenerator &FormGenerator::operator=( const FormGenerator &other ) {
  sections = other.sections;
  return *this;
}

void dv::forms::from_json( const json &j, FormGenerator &form, const dv::json::JSONPath &path ) {
  JSONContext::current()->attach( form );
  auto c = JSONContext::current()->get<FormExpressionContext>()->enter( form.shared_from_this() );
  auto value = j.sub( "expressions" );
  if ( value ) {
    for ( const auto &expression : value->objectIterator() ) {
      auto exp = FieldContainer::expressionFromJSON( *expression.second, form, path / expression.first );
      form.expressions[expression.first] = form.create<FormExpressionWrapper>( expression.first, exp );
    }
  }

  value = j.sub( "sections" );
  if ( value ) {
    for ( const auto &section : value->objectIterator() ) {
      auto sec = form.addSection( section.first );
      dv::json::JSONSerialiser<FormSection>::from_json( *section.second, *sec, path / "sections" / section.first );
    }
  }
}

FormExpressionWrapperPtr FormGenerator::getExpression( const std::string &name ) const {
  FormExpressionWrapperPtr rt;

  const auto &item = expressions.find( name );
  if ( item != expressions.end() ) {
    rt = item->second;
  }

  return rt;
}
