#pragma once
#ifndef DVFORMSCPP_FORMINPUTSIMPLE_H
#define DVFORMSCPP_FORMINPUTSIMPLE_H

#include "FormInput.h"
#include <boost/variant.hpp>
#include <memory>             // for shared_ptr, weak_ptr
#include "FieldType.h"        // for json

namespace dv {
  namespace forms {
    class FormExpression;

    class FormInputSimple : public FormInput {
    public:
      FormInputSimple();
      ~FormInputSimple() override;
      json generateSchema() const override;
      typedef boost::variant<bool, std::weak_ptr<FormExpression>> requiredType;
      typedef std::shared_ptr<FormExpression> validType;
      typedef std::shared_ptr<FormExpression> visibleType;
      const requiredType &getRequired() const;
      void setRequired( const requiredType &nRequired );
      const validType &getValid() const;
      void setValid( const validType &nValid );
      const visibleType &getVisible() const;
      void setVisible( const visibleType &nVisible );
    protected:
      requiredType required;
      validType valid;
      visibleType visible;
      friend void from_json( const json &j, FormInputSimple &input, const dv::json::JSONErrorCollectorPtr &collector, const dv::json::JSONPath &path );
    };

    void from_json( const json &j, FormInputSimple &input, const dv::json::JSONErrorCollectorPtr &collector, const dv::json::JSONPath &path );
  }
}

#endif //DVFORMSCPP_FORMINPUTSIMPLE_H
