#pragma once
#ifndef DVFORMSCPP_FORMINPUTSIMPLE_H
#define DVFORMSCPP_FORMINPUTSIMPLE_H

#include "FormInput.h"
#include <boost/variant.hpp>

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
    };
  }
}

#endif //DVFORMSCPP_FORMINPUTSIMPLE_H
