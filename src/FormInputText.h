#pragma once
#ifndef DVFORMSCPP_FORMINPUTTEXT_H
#define DVFORMSCPP_FORMINPUTTEXT_H

#include "FormFwd.h" // IWYU pragma: keep
#include "FormInputSimple.h"

namespace dv {
  namespace forms {
    class FormInputText : public FormInputSimple {
    public:
      FormInputText();
      ~FormInputText() override;
      FieldType getType() const override;
      json generateSchema() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;
    protected:
      std::vector<std::string> suggestions;
    };
  }
}

#endif // DVFORMSCPP_FORMINPUTTEXT_H
