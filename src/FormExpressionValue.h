#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONVALUE_H
#define DVFORMSCPP_FORMEXPRESSIONVALUE_H

#include "FormFwd.h"
#include "FormExpression.h"

namespace dv {
  namespace forms {
    class FormExpressionValue : public FormExpression {
    public:
      FormExpressionValue() = default;
      FormExpressionValue( const json &value );

      template<typename T> FormExpressionValue( T &&nValue ) {
        value = nValue;
      }

      json generateSchema() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;

    protected:
      json value;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONVALUE_H
