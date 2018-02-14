#pragma once
#ifndef DVFORMSCPP_FORMEXPRESSIONCONTEXT_H
#define DVFORMSCPP_FORMEXPRESSIONCONTEXT_H

#include "FormFwd.h"  // for FormExpressionContainerPtr, DVFORMS_WARN_UNUSED, FormExpressionWrapperPtr
#include <memory>     // for enable_shared_from_this
#include <string>     // for string
#include <vector>     // for vector

namespace dv {
  namespace forms {
    class FormExpressionContext : public std::enable_shared_from_this<FormExpressionContext> {
    public:
      typedef FormExpressionContext defaultContextType;

      struct ContextExitHelper {
        ContextExitHelper( const ContextExitHelper & ) = delete;
        ContextExitHelper() = default;
        ContextExitHelper( ContextExitHelper && ) = default;
        ~ContextExitHelper();
        ContextExitHelper &operator=( const ContextExitHelper & ) = delete;
        ContextExitHelper &operator=( ContextExitHelper && ) = default;
      };

      ContextExitHelper enter( const FormExpressionContainerPtr &container ) DVFORMS_WARN_UNUSED;
      FormExpressionWrapperPtr getExpression( const std::string &name );

    protected:
      std::vector<FormExpressionContainerPtr> containers;

      void pop();
    };
  }
}

#endif // DVFORMSCPP_FORMEXPRESSIONCONTEXT_H
