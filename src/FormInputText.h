#pragma once
#ifndef DVFORMSCPP_FORMINPUTTEXT_H
#define DVFORMSCPP_FORMINPUTTEXT_H

#include "FormInputSimple.h"
#include "FieldType.h"        // for FieldType

namespace dv {
  namespace forms {
    class FormInputText : public FormInputSimple {
    public:
      FormInputText();
      ~FormInputText() override;
      FieldType getType() const override;
    };
  }
}

#endif //DVFORMSCPP_FORMINPUTTEXT_H
