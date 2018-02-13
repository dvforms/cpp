#pragma once
#ifndef DVFORMSCPP_FORMINPUTSTATIC_H
#define DVFORMSCPP_FORMINPUTSTATIC_H

#include "FormFwd.h" // IWYU pragma: keep
#include "FormInputSimple.h"

namespace dv {
  namespace forms {
    class FormInputStatic : public FormInputSimple {
    public:
      FormInputStatic();
      ~FormInputStatic() override;
      FieldType getType() const override;
    };
  }
}

#endif // DVFORMSCPP_FORMINPUTSTATIC_H
