#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONOR_H
#define DVFORMSCPP_FORMEXPRESSIONOR_H

#include "FormFwd.h"
#include "FormExpressionBoolean.h"

namespace dv {
  namespace forms {
    class FormExpressionOr : public FormExpressionBoolean {
    public:
    protected:
      const char *getType() const override DVFORMS_PURE;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONOR_H
