#pragma once
#ifndef DVFORMSCPP_FORMSECTION_H
#define DVFORMSCPP_FORMSECTION_H

#include "FieldContainer.h"
#include "FormComponent.h"
#include "FormFwd.h"
#include <list>
#include <memory>
#include <string> // for string
#include <utility>

namespace dv {
  namespace json {
    class JSONPath;
  }
  namespace forms {
    class FormSection : public FieldContainer {
    public:
      explicit FormSection();
      ~FormSection() override;
      json generateSchema() const override;
      const std::string &getLabel() const;
      void setLabel( const std::string &nLabel );
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;

    protected:
      std::string label;
    };
  }
}

#endif // DVFORMSCPP_FORMSECTION_H
