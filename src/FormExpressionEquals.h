#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONEQUALS_H
#define DVFORMSCPP_FORMEXPRESSIONEQUALS_H

#include "FormFwd.h"
#include "FormExpressionBoolean.h"

namespace dv {
  namespace forms {
    class FormExpressionEquals : public FormExpressionBoolean {
    public:
    protected:
      const char *getType() const override;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONEQUALS_H
