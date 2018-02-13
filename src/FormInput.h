#pragma once
#ifndef DVFORMSCPP_FORMINPUT_H
#define DVFORMSCPP_FORMINPUT_H

#include "FormComponent.h"
#include "FormFwd.h"
#include <string> // for string

namespace dv {
  namespace forms {
    class FormInput : public virtual FormComponent {
    public:
      FormInput();
      ~FormInput() override;
      json generateSchema() const override;
      virtual FieldType getType() const = 0;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;

    protected:
      std::string label;
      std::string help;
    };
  }
}

#endif // DVFORMSCPP_FORMINPUT_H
