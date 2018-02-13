#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSION_H
#define DVFORMSCPP_FORMEXPRESSION_H

#include "FormComponent.h"
#include "FormFwd.h" // IWYU pragma: keep

namespace dv {
  namespace forms {
    class FormExpression : public FormComponent {
    public:
      FormExpression();
      ~FormExpression() override;
    };
  }
}
#endif // DVFORMSCPP_FORMEXPRESSION_H
