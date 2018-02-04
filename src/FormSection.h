#pragma once
#ifndef DVFORMSCPP_FORMSECTION_H
#define DVFORMSCPP_FORMSECTION_H

#include "FormComponent.h"
#include "FormGenerator.h"
#include <memory>
#include <unordered_map>
#include <string>              // for string
#include <cstdint>          // for int32_t

namespace dv {
  namespace forms {
    class FormSection;
    typedef std::shared_ptr<FormSection> FormSectionPtr;
    class FormSection : public FormComponent {
    public:
      explicit FormSection( int32_t nOrder );
      ~FormSection() override;
      json generateSchema() const override;

      template<class T> std::shared_ptr<T> add( const std::string &name );

    protected:
      int32_t order;
      std::unordered_map<std::string, FormComponentPtr> fields;
      virtual void addComponent( const std::string &name, const FormComponentPtr & );
    };

    template<class T>
    std::shared_ptr<T> FormSection::add( const std::string &name ) {
      auto rt = getForm()->create<T>();
      addComponent( name, rt );
      return rt;
    }
  }
}

#endif //DVFORMSCPP_FORMSECTION_H
