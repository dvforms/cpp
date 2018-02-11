#pragma once
#ifndef DVFORMS_DVFORMGENERATOR_H
#define DVFORMS_DVFORMGENERATOR_H

#include "FormComponent.h"// IWYU pragma: keep
#include <json.h>
#include <string>
#include <unordered_map>
#include <algorithm>      // for forward
#include <memory>         // for shared_ptr, enable_shared_from_this, make_shared

namespace dv {
  namespace forms {
    using json = dv::json::JSON;
    class FormSection;
    typedef std::shared_ptr<FormSection> FormSectionPtr;
    class FormGenerator;
    typedef std::shared_ptr<FormGenerator> FormGeneratorPtr;
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

      FormGenerator &operator=( const FormGenerator & ) = default;
      FormGenerator &operator=( FormGenerator && ) = default;

      template<typename T, typename... Args>
      const std::shared_ptr<T> create( Args... );
    protected:
      json buildSections() const;
      std::unordered_map<std::string, FormSectionPtr> sections;
    };

    template<typename T, typename... Args>
    const std::shared_ptr<T> FormGenerator::create( Args... args ) {
      auto rt = std::make_shared<T>( std::forward<Args...>( args )... );
      rt->setForm( shared_from_this() );
      return rt;
    }

    void from_json( const json &j, FormGenerator &form, const dv::json::JSONPath &path );
  }
}

#endif //DVFORMS_FORMGENERATOR_H
