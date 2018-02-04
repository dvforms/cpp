#pragma once
#ifndef DVFORMS_DVFORMGENERATOR_H
#define DVFORMS_DVFORMGENERATOR_H

#include <string>
#include <unordered_map>
#include "json.h"

namespace dv {
  namespace forms {
    using json = dv::json::JSON;
    class FormSection;
    typedef std::shared_ptr<FormSection> FormSectionPtr;
    class FormGenerator : public std::enable_shared_from_this<FormGenerator> {
    public:
      FormGenerator();
      virtual ~FormGenerator();
      virtual std::string getSchema() const;
      json generateSchema() const;
      virtual FormSectionPtr &addSection( const std::string &name );

      template<typename T, typename... Args>
      std::shared_ptr<T> create( Args... );
    protected:
      json buildSections() const;
      std::unordered_map<std::string, FormSectionPtr> sections;
    };

    template<typename T, typename... Args>
    std::shared_ptr<T> FormGenerator::create( Args... args ) {
      auto rt = std::make_shared<T>( std::forward<Args...>( args )... );
      rt->setForm( shared_from_this() );
      return rt;
    }
  }
}

#endif //DVFORMS_FORMGENERATOR_H
