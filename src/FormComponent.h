#pragma once
#ifndef DVFORMSCPP_FORMCOMPONENT_H
#define DVFORMSCPP_FORMCOMPONENT_H
#include <memory>
#include "json.h"
#include <type_traits>  // for enable_if, is_base_of

namespace dv {
  namespace forms {
    using json = dv::json::JSON;
    class FormGenerator;
    typedef std::shared_ptr<FormGenerator> FormGeneratorPtr;
    class FormComponent;
    typedef std::shared_ptr<FormComponent> FormComponentPtr;
    class FormComponent : public std::enable_shared_from_this<FormComponent> {
    public:
      FormComponent();
      virtual ~FormComponent();
      virtual json generateSchema() const = 0;
      FormGeneratorPtr getForm() const;
    protected:
      void setForm( const FormGeneratorPtr &nForm );
    private:
      std::weak_ptr<FormGenerator> form;
      friend class FormGenerator;
    };

    template<class T, typename std::enable_if<std::is_base_of<FormComponent, T>::value, int>::type = 0>
    inline void to_json( json &j, const T &value ) {
      j = value.generateSchema();
    }

    template<class T, typename std::enable_if<std::is_base_of<FormComponent, T>::value, int>::type = 0>
    inline void to_json( json &j, const std::shared_ptr<T> &value ) {
      if ( value ) {
        to_json( j, *value );
      } else {
        j = nullptr;
      }
    }
  }
}

#endif //DVFORMSCPP_FORMCOMPONENT_H
