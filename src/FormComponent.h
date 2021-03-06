#pragma once
#ifndef DVFORMSCPP_FORMCOMPONENT_H
#define DVFORMSCPP_FORMCOMPONENT_H

#include "FormFwd.h"
#include <json.h> // IWYU pragma: keep
#include <memory>
#include <type_traits> // for enable_if, is_base_of

namespace dv {
  namespace forms {
    class FormComponent : public std::enable_shared_from_this<FormComponent> {
    public:
      FormComponent();
      virtual ~FormComponent();
      virtual json generateSchema() const = 0;
      FormGeneratorPtr getForm() const;
      virtual void fromJSON( const json &j, const dv::json::JSONPath &path ) = 0;

    protected:
      void setForm( const FormGeneratorPtr &nForm );

    private:
      FormGeneratorWeakPtr form;
      friend class FormGenerator;
    };

    template<class T, typename std::enable_if<std::is_base_of<FormComponent, T>::value, int>::type = 0>
    inline void to_json( json &j, const T &value, const dv::json::JSONPath & /*path*/ ) {
      j = value.generateSchema();
    }

    template<class T, typename std::enable_if<std::is_base_of<FormComponent, T>::value, int>::type = 0>
    inline void to_json( json &j, const std::shared_ptr<T> &value, const dv::json::JSONPath &path ) {
      if ( value ) {
        to_json( j, *value, path );
      } else {
        j = nullptr;
      }
    }

    template<typename T, typename std::enable_if<std::is_base_of<FormComponent, T>::value, int>::type = 0> std::shared_ptr<T> json_construct( T *, json * ) {
      return std::make_shared<T>();
    }

    template<typename T, typename std::enable_if<std::is_base_of<FormComponent, T>::value, int>::type = 0>
    void from_json( const json &j, T &component, const dv::json::JSONPath &path ) {
      component.fromJSON( j, path );
    }
  }
}

#endif // DVFORMSCPP_FORMCOMPONENT_H
