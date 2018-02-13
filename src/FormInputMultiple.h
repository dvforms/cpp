#pragma once
#ifndef DVFORMSCPP_FORMINPUTMULTIPLE_H
#define DVFORMSCPP_FORMINPUTMULTIPLE_H

#include "FormFwd.h"
#include "FormInput.h"

namespace dv {
  namespace forms {
    class FormInputMultiple : public FormInput {
     public:
      FieldType getType() const override;
     public:
    };
  }
}

#endif //DVFORMSCPP_FORMINPUTMULTIPLE_H
