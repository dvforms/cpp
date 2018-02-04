#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSION_H
#define DVFORMSCPP_FORMEXPRESSION_H
#include <memory>
#include "FormComponent.h"

namespace dv {
  namespace forms {
    class FormExpression;
    typedef std::shared_ptr<FormExpression> FormExpressionPtr;
    class FormExpression : public FormComponent {
    public:
      FormExpression();
      ~FormExpression() override;
    };
  }
}
#endif //DVFORMSCPP_FORMEXPRESSION_H
