#pragma once
#ifndef DVFORMSCPP_FIELDCONTAINER_H
#define DVFORMSCPP_FIELDCONTAINER_H

#include "FormComponent.h"
#include "FormFwd.h"
#include "FormGenerator.h"
#include <UnorderedIndexedMap.h>
#include <string>

namespace dv {
  namespace forms {
    class FieldContainer : public virtual FormComponent {
    public:
      template<class T> std::shared_ptr<T> add( const std::string &name );
      json generateSchema() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;
    protected:
      dv::json::UnorderedIndexedMap<std::string, FormComponentPtr> fields;
      virtual void addComponent( const std::string &name, const FormComponentPtr & );
    };

    template<class T> std::shared_ptr<T> FieldContainer::add( const std::string &name ) {
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
  }
}

#endif // DVFORMSCPP_FIELDCONTAINER_H
