#pragma once
#ifndef DVFORMSCPP_FORMINPUTNUMBER_H
#define DVFORMSCPP_FORMINPUTNUMBER_H

#include "FormFwd.h" // IWYU pragma: keep
#include "FormInputSimple.h"

namespace dv {
  namespace forms {
    class FormInputNumber : public FormInputSimple {
    public:
      FormInputNumber();
      ~FormInputNumber() override;
      FieldType getType() const override;
    };
  }
}

#endif // DVFORMSCPP_FORMINPUTNUMBER_H
