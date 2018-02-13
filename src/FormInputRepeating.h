#pragma once
#ifndef DVFORMSCPP_FORMINPUTREPEATING_H
#define DVFORMSCPP_FORMINPUTREPEATING_H

#include "FieldContainer.h"
#include "FormFwd.h"
#include "FormInput.h"

namespace dv {
  namespace forms {
    class FormInputRepeating : public FormInput, public FieldContainer {
    public:
      FieldType getType() const override;
      json generateSchema() const override;
      static const size_t Unlimited = std::numeric_limits<size_t>::max();
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;
    protected:
      bool inlineValue{ false };
      size_t minRows{ 0 };
      size_t maxRows{ Unlimited };
    };
  }
}

#endif // DVFORMSCPP_FORMINPUTREPEATING_H
