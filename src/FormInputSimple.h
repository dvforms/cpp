#pragma once
#ifndef DVFORMSCPP_FORMINPUTSIMPLE_H
#define DVFORMSCPP_FORMINPUTSIMPLE_H

#include "FormFwd.h" // IWYU pragma: keep
#include "FormInput.h"
#include <boost/variant.hpp>
#include <memory> // for shared_ptr, weak_ptr

namespace dv {
  namespace json {
    class JSONPath;
  }
  namespace forms {
    class FormInputSimple : public FormInput {
    public:
      FormInputSimple();
      ~FormInputSimple() override;
      json generateSchema() const override;
      typedef boost::variant<bool, FormExpressionWrapperWeakPtr> requiredType;
      typedef FormExpressionWrapperWeakPtr validType;
      typedef FormExpressionWrapperWeakPtr visibleType;
      const requiredType &getRequired() const;
      void setRequired( const requiredType &nRequired );
      const validType &getValid() const;
      void setValid( const validType &nValid );
      const visibleType &getVisible() const;
      void setVisible( const visibleType &nVisible );
      const std::string &getPlaceholder() const;
      void setPlaceholder( const std::string &placeholder );
      size_t getSize() const;
      void setSize( size_t size );
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;

    protected:
      requiredType required;
      validType valid;
      visibleType visible;
      std::string placeholder;
      size_t size{ 0 };

      FormExpressionWrapperPtr jsonToExpression( const json &j, const dv::json::JSONPath &path );
    };
  }
}

#endif // DVFORMSCPP_FORMINPUTSIMPLE_H
