#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONGREATERTHAN_H
#define DVFORMSCPP_FORMEXPRESSIONGREATERTHAN_H

#include "FormFwd.h"
#include "FormExpressionBoolean.h"

namespace dv {
  namespace forms {
    class FormExpressionGreaterThan : public FormExpressionBoolean {
    public:
      FormExpressionGreaterThan();
      virtual ~FormExpressionGreaterThan();
    protected:
      const char *getType() const override;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONGREATERTHAN_H
