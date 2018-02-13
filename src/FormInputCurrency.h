#pragma once
#ifndef DVFORMSCPP_FORMINPUTCURRENCY_H
#define DVFORMSCPP_FORMINPUTCURRENCY_H

#include "FormFwd.h" // IWYU pragma: keep
#include "FormInputNumber.h"

namespace dv {
  namespace forms {
    class FormInputCurrency : public FormInputNumber {
    public:
      FormInputCurrency();
      ~FormInputCurrency() override;
      FieldType getType() const override;
    };
  }
}

#endif // DVFORMSCPP_FORMINPUTCURRENCY_H
