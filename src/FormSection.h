#pragma once
#ifndef DVFORMSCPP_FORMSECTION_H
#define DVFORMSCPP_FORMSECTION_H

#include "FieldContainer.h"
#include "FormFwd.h"
#include <string> // for string

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
