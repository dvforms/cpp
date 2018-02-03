#pragma once
#ifndef DVFORMSCPP_FORMCOMPONENT_H
#define DVFORMSCPP_FORMCOMPONENT_H
#include <memory>

namespace dv {
  namespace forms {
    class FormComponent;
    typedef std::shared_ptr<FormComponent> FormComponentPtr;
    class FormComponent : public std::enable_shared_from_this<FormComponent> {
    };
  }
}

#endif //DVFORMSCPP_FORMCOMPONENT_H
