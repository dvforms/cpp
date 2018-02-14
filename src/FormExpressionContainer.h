#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONCONTAINER_H
#define DVFORMSCPP_FORMEXPRESSIONCONTAINER_H

#include "FormFwd.h"
#include "FormExpression.h"

namespace dv {
  namespace forms {
    class FormExpressionContainer {
    public:
      virtual FormExpressionWrapperPtr getExpression( const std::string &name ) const = 0;
    };
  }
}

#endif //DVFORMSCPP_FORMEXPRESSIONCONTAINER_H
