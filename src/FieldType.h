#pragma once
#ifndef DVFORMSCPP_FIELDTYPE_H
#define DVFORMSCPP_FIELDTYPE_H

#include "json.h"

namespace dv {
  namespace forms {
    using json = dv::json::JSON;
    enum class FieldType {
      TEXT,
      NUMBER,
      EMAIL,
      CURRENCY,
      STATIC,
      ENUM
    };

    std::ostream &operator<<( std::ostream &os, FieldType type );
    std::ostream &operator>>( std::ostream &os, FieldType &type );
    void to_json( json &json, FieldType type );
  }
}

#endif //DVFORMSCPP_FIELDTYPE_H
