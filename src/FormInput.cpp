#include "FormInput.h"

using namespace dv::forms;
using namespace dv::json;
FormInput::FormInput() = default;

FormInput::~FormInput() = default;

json FormInput::generateSchema() const {
  json rt;

  rt["type"] = getType();
  if ( !label.empty() ) {
    rt["label"] = label;
  }
  if ( !help.empty() ) {
    rt["help"] = help;
  }

  return rt;
}
