#pragma once
#ifndef DVFORMS_DVFORMGENERATOR_H
#define DVFORMS_DVFORMGENERATOR_H

#include "FormFwd.h"
#include <UnorderedIndexedMap.h>
#include <algorithm> // for forward
#include <functional>
#include <map>
#include <memory> // for shared_ptr, enable_shared_from_this, make_shared
#include <string>

namespace dv {
  namespace forms {
    class FormGenerator : public std::enable_shared_from_this<FormGenerator> {
    public:
      FormGenerator();
      FormGenerator( const FormGenerator & ) = default;
      FormGenerator( FormGenerator && ) = default;
      virtual ~FormGenerator();
      virtual std::string getSchema() const;
      json generateSchema() const;
      void parseJSON( const json &j );
      virtual FormSectionPtr &addSection( const std::string &name );

      FormGenerator &operator=( const FormGenerator & );
      FormGenerator &operator=( FormGenerator && ) noexcept = default;

      template<typename T, typename... Args> const std::shared_ptr<T> create( Args... );

    protected:
      json buildSections() const;
      dv::json::UnorderedIndexedMap<std::string, FormSectionPtr> sections;
      std::map<std::string, FormExpressionPtr> expressions;
    };

    template<typename T, typename... Args> const std::shared_ptr<T> FormGenerator::create( Args... args ) {
      auto rt = std::make_shared<T>( std::forward<Args...>( args )... );
      rt->setForm( shared_from_this() );
      return rt;
    }

    void from_json( const json &j, FormGenerator &form, const dv::json::JSONPath &path );
  }
}

#endif // DVFORMS_FORMGENERATOR_H
