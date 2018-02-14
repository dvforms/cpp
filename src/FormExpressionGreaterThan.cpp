#include "FormExpressionGreaterThan.h"

using namespace dv::forms;

FormExpressionGreaterThan::FormExpressionGreaterThan() = default;
FormExpressionGreaterThan::~FormExpressionGreaterThan() = default;

const char *FormExpressionGreaterThan::getType() const {
  return ">";
}
