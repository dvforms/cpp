#pragma once
#ifndef DVFORMSCPP_FORMINPUT_H
#define DVFORMSCPP_FORMINPUT_H

#include "FormComponent.h"
#include "FieldType.h"

namespace dv {
  namespace forms {
    class FormInput : public FormComponent {
    public:
      FormInput();
      ~FormInput() override;
      json generateSchema() const override;
      virtual FieldType getType() const = 0;

    protected:
      std::string label;
      std::string help;
    };
  }
}

#endif //DVFORMSCPP_FORMINPUT_H
