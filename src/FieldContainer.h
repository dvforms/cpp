#pragma once
#ifndef DVFORMSCPP_FIELDCONTAINER_H
#define DVFORMSCPP_FIELDCONTAINER_H

#include "FormComponent.h"
#include "FormExpressionContainer.h"
#include "FormFwd.h"
#include "FormGenerator.h"
#include <UnorderedIndexedMap.h>
#include <map>
#include <string>

namespace dv {
  namespace forms {
    class FieldContainer : public FormExpressionContainer, public virtual FormComponent {
    public:
      template<class T> std::shared_ptr<T> addField( const std::string &name );
      template<class T> std::shared_ptr<T> addExpression( const std::string &name );
      json generateSchema() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;
      static FormExpressionPtr expressionFromJSON( const json &j, FormGenerator &form, const dv::json::JSONPath &path );
      FormExpressionWrapperPtr getExpression( const std::string &name ) const override;

    protected:
      dv::json::UnorderedIndexedMap<std::string, FormComponentPtr> fields;
      std::map<std::string, FormExpressionWrapperPtr> expressions;
      virtual void addComponent( const std::string &name, const FormComponentPtr & );
      virtual void addExpression( const std::string &name, const FormExpressionPtr & );
    };

    template<class T> std::shared_ptr<T> FieldContainer::addField( const std::string &name ) {
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

    template<class T> std::shared_ptr<T> FieldContainer::addExpression( const std::string &name ) {
      auto f = getForm();
      std::shared_ptr<T> rt;
      rt = f->create<T>();
      addExpression( name, rt );
      return rt;
    }
  }
}

#endif // DVFORMSCPP_FIELDCONTAINER_H
