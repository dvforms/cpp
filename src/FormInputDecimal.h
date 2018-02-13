#pragma once
#ifndef DVFORMSCPP_FORMINPUTDECIMAL_H
#define DVFORMSCPP_FORMINPUTDECIMAL_H
#include "FormFwd.h"
#include "FormInputSimple.h"

namespace dv {
  namespace forms {
    class FormInputDecimal : public FormInputSimple {
    public:
      FieldType getType() const override;
    };
  }
}

#endif //DVFORMSCPP_FORMINPUTDECIMAL_H
