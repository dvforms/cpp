#include "FormExpressionContext.h"
#include "FormExpressionContainer.h"
#include <json.h>

using namespace dv::forms;

FormExpressionContext::ContextExitHelper::~ContextExitHelper() {
  dv::json::JSONContext::current()->get<FormExpressionContext>()->pop();
}

FormExpressionContext::ContextExitHelper FormExpressionContext::enter( const FormExpressionContainerPtr &container ) {
  containers.push_back( container );
  return FormExpressionContext::ContextExitHelper();
}

void FormExpressionContext::pop() {
  containers.pop_back();
}

FormExpressionWrapperPtr FormExpressionContext::getExpression( const std::string &name ) {
  FormExpressionWrapperPtr rt;

  for ( const auto &item : containers ) {
    rt = item->getExpression( name );
    if ( rt ) { break; }
  }

  return rt;
}
