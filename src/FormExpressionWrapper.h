#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONWRAPPER_H
#define DVFORMSCPP_FORMEXPRESSIONWRAPPER_H

#include "FormFwd.h"
#include "FormExpression.h"
#include <string>            // for string
#include <jsonfwd.h>

namespace dv {
  namespace forms {
    class FormExpressionWrapper : public FormExpression {
    public:
      FormExpressionWrapper( const std::string &nName, const FormExpressionPtr &nSubExpression );
      virtual ~FormExpressionWrapper();
      json generateSchema() const override;
      const std::string &getName() const;
      const FormExpressionPtr &getSubExpression() const;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;
    protected:
      std::string name;
      FormExpressionPtr subExpression;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONWRAPPER_H
