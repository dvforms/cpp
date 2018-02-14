#pragma once
#ifndef DVFORMSCPP_FORMINPUTMULTIPLE_H
#define DVFORMSCPP_FORMINPUTMULTIPLE_H

#include "FieldContainer.h"
#include "FormFwd.h"
#include "FormInput.h"

namespace dv {
  namespace forms {
    class FormInputMultiple : public FormInput, public FieldContainer {
    public:
      FieldType getType() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;
      json generateSchema() const override;

    public:
    };
  }
}

#endif // DVFORMSCPP_FORMINPUTMULTIPLE_H
