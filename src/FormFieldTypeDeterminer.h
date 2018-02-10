#pragma once
#ifndef DVFORMSCPP_FORMFIELDTYPEDETERMINER_H
#define DVFORMSCPP_FORMFIELDTYPEDETERMINER_H

#include "FormComponent.h"

namespace dv {
  namespace forms {
    class FormFieldTypeDeterminer {
     public:
      static FormComponentPtr
      fromJSON( const json &j, FormGeneratorPtr &form, const dv::json::JSONErrorCollectorPtr &collector, const dv::json::JSONPath &path );
      static FormComponentPtr
      fromJSON( const dv::json::JSONPtr &j, FormGeneratorPtr &form, const dv::json::JSONErrorCollectorPtr &collector, const dv::json::JSONPath &path );
    };
  }
}

#endif //DVFORMSCPP_FORMFIELDTYPEDETERMINER_H
