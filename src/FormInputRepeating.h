#pragma once
#ifndef DVFORMSCPP_FORMINPUTREPEATING_H
#define DVFORMSCPP_FORMINPUTREPEATING_H

#include "FormFwd.h"
#include "FormInput.h"

namespace dv {
  namespace forms {
    class FormInputRepeating : public FormInput {
     public:
      FieldType getType() const override;
    };
  }
}

#endif //DVFORMSCPP_FORMINPUTREPEATING_H
