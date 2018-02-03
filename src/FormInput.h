#pragma once
#ifndef DVFORMSCPP_FORMINPUT_H
#define DVFORMSCPP_FORMINPUT_H

#include "FormComponent.h"

namespace dv {
  namespace forms {
    class FormInput : public FormComponent {
    public:
      FormInput();
      virtual ~FormInput();
    };
  }
}

#endif //DVFORMSCPP_FORMINPUT_H
