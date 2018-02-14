#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONEMPTY_H
#define DVFORMSCPP_FORMEXPRESSIONEMPTY_H

#include "FormFwd.h"
#include "FormExpressionNested.h"

namespace dv {
  namespace forms {
    class FormExpressionEmpty : public FormExpressionNested {
    protected:
      const char *getType() const override;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONEMPTY_H
