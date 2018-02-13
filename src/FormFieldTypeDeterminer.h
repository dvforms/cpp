#pragma once
#ifndef DVFORMSCPP_FORMFIELDTYPEDETERMINER_H
#define DVFORMSCPP_FORMFIELDTYPEDETERMINER_H

#include "FormFwd.h"
#include <jsonfwd.h>

namespace dv {
  namespace json {
    class JSONPath;
  }
  namespace forms {
    class FormFieldTypeDeterminer {
    public:
      static FormComponentPtr fromJSON( const json &j, FormGeneratorPtr &form, const dv::json::JSONPath &path );
      static FormComponentPtr fromJSON( const dv::json::JSONPtr &j, FormGeneratorPtr &form, const dv::json::JSONPath &path );
    };
  }
}

#endif // DVFORMSCPP_FORMFIELDTYPEDETERMINER_H
