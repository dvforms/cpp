#pragma once
#ifndef DVFORMSCPP_FORMSECTION_H
#define DVFORMSCPP_FORMSECTION_H

#include "FormComponent.h"
#include "FormGenerator.h"
#include <memory>
#include <unordered_map>
#include <string>              // for string

namespace dv {
  namespace forms {
    class FormSection;
    typedef std::shared_ptr<FormSection> FormSectionPtr;
    class FormSection : public FormComponent {
    public:
      explicit FormSection();
      ~FormSection() override;
      json generateSchema() const override;

      template<class T> std::shared_ptr<T> add( const std::string &name );
      const std::string &getLabel() const;
      void setLabel( const std::string &label );

     protected:
      std::unordered_map<std::string, FormComponentPtr> fields;
      std::string label;
      virtual void addComponent( const std::string &name, const FormComponentPtr & );
      friend void from_json( const json &j, FormSection &section, const dv::json::JSONPath &path );
    };

    template<class T>
    std::shared_ptr<T> FormSection::add( const std::string &name ) {
      auto f = getForm();
      std::shared_ptr<T> rt;
      if ( f ) {
        rt = f->create<T>();
      } else {
        rt = std::make_shared<T>();
      }
      addComponent( name, rt );
      return rt;
    }

    void from_json( const json &j, FormSection &section, const dv::json::JSONPath &path );
  }
}

#endif //DVFORMSCPP_FORMSECTION_H
