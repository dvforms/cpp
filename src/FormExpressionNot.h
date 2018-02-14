#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONNOT_H
#define DVFORMSCPP_FORMEXPRESSIONNOT_H

#include "FormFwd.h"
#include "FormExpressionNested.h"

namespace dv {
  namespace forms {
    class FormExpressionNot : public FormExpressionNested {
    protected:
      const char *getType() const override;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONNOT_H
