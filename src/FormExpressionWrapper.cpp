#include "FormExpressionWrapper.h"
#include "FormExpression.h"

using namespace dv::forms;

FormExpressionWrapper::FormExpressionWrapper( const std::string &nName, const FormExpressionPtr &nSubExpression ) : name( nName ),
                                                                                                                    subExpression( nSubExpression ) {}

json FormExpressionWrapper::generateSchema() const {
  return json( name );
}

const std::string &FormExpressionWrapper::getName() const {
  return name;
}

const FormExpressionPtr &FormExpressionWrapper::getSubExpression() const {
  return subExpression;
}

void FormExpressionWrapper::fromJSON( const json &j, const dv::json::JSONPath &path ) {
}

FormExpressionWrapper::~FormExpressionWrapper() = default;
