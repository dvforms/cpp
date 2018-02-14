#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONBOOLEAN_H
#define DVFORMSCPP_FORMEXPRESSIONBOOLEAN_H

#include "FormFwd.h"
#include "FormExpression.h"
#include "FormGenerator.h"

namespace dv {
  namespace forms {
    class FormExpressionBoolean : public FormExpression {
    public:
      json generateSchema() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;
      template<typename T, typename... Args> std::shared_ptr<T> add( Args &&... args );
      const std::vector<FormExpressionPtr> &getSubExpressions() const;

    protected:
      virtual const char *getType() const = 0;
      std::vector<FormExpressionPtr> subExpressions;
    };

    template<typename T, typename... Args>
    std::shared_ptr<T> FormExpressionBoolean::add( Args &&... args ) {
      auto exp = getForm()->create<T>( std::forward<Args>( args )... );
      subExpressions.emplace_back( exp );
      return exp;
    }
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONBOOLEAN_H
