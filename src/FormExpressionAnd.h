#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONAND_H
#define DVFORMSCPP_FORMEXPRESSIONAND_H

#include "FormFwd.h"
#include "FormExpressionBoolean.h"

namespace dv {
  namespace forms {
    class FormExpressionAnd : public FormExpressionBoolean {
    public:
    protected:
      const char *getType() const override;
    };
  }
}

#endif // DVFORMSCPP_FORMEXPRESSIONAND_H
