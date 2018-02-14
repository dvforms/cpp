#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONNESTED_H
#define DVFORMSCPP_FORMEXPRESSIONNESTED_H

#include "FormFwd.h"
#include "FormExpression.h"

namespace dv {
  namespace forms {
    class FormExpressionNested : public FormExpression {
    public:
      json generateSchema() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;

    protected:
      FormExpressionPtr nested;

      virtual const char *getType() const = 0;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONNESTED_H
