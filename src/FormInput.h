#pragma once
#ifndef DVFORMSCPP_FORMINPUT_H
#define DVFORMSCPP_FORMINPUT_H

#include "FormFwd.h"
#include "FormComponent.h"
#include <string>           // for string

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
      friend void from_json( const json &j, FormInput &input, const dv::json::JSONPath &path );
    };

    void from_json( const json &j, FormInput &input, const dv::json::JSONPath &path );
  }
}

#endif //DVFORMSCPP_FORMINPUT_H
