#pragma once
#ifndef DVFORMSCPP_JSONPATH_H
#define DVFORMSCPP_JSONPATH_H

#include <boost/variant.hpp>
#include <string>
#include "json.h"
#include <vector>

namespace dv {
  namespace json {
    class JSONPath {
    public:
      JSONPath operator/( const JSON::stringType &name ) const;
      JSONPath operator/=( const JSON::stringType &name );
      JSONPath operator[]( size_t index ) const;
      bool operator<( const JSONPath &other ) const;
      bool operator>( const JSONPath &other ) const;
      bool operator<=( const JSONPath &other ) const;
      bool operator>=( const JSONPath &other ) const;
      bool operator==( const JSONPath &other ) const;
      int compare( const JSONPath &other ) const;

      std::string toString() const;
    protected:
      typedef boost::variant<JSON::stringType, size_t> componentType;
      std::vector<componentType> path;
    };

    inline std::ostream &operator<<( std::ostream &os, const JSONPath &path ) {
      os << path.toString();
      return os;
    }
  }
}

#endif //DVFORMSCPP_JSONPATH_H
