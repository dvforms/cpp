#pragma once
#ifndef DVFORMSCPP_FORMCOMPONENTCONTAINER_H
#define DVFORMSCPP_FORMCOMPONENTCONTAINER_H
#include "FormComponent.h"
#include <list>

namespace dv {
  namespace forms {
    class FormComponentContainer : public FormComponent {
    public:
      FormComponentContainer();
      virtual void addComponent( const FormComponentPtr &component );
      virtual ~FormComponentContainer();

    protected:
      std::list<FormComponentPtr> components;
    };
  }
}

#endif //DVFORMSCPP_FORMCOMPONENTCONTAINER_H
