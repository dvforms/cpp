#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONFIELD_H
#define DVFORMSCPP_FORMEXPRESSIONFIELD_H

#include "FormFwd.h"
#include "FormExpression.h"

namespace dv {
  namespace forms {
    class FormExpressionField : public FormExpression {
    public:
      json generateSchema() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;

    protected:
      std::string field;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONFIELD_H
