#pragma once
#ifndef DVFORMSCPP_JSONVALUEFWD_H
#define DVFORMSCPP_JSONVALUEFWD_H

#include <stddef.h>           // for size_t
#include <boost/variant.hpp>  // for variant
#include <cstdint>            // for int64_t
#include <iosfwd>             // for ostream, nullptr_t
#include <memory>             // for shared_ptr
#include <string>             // for string
#include <unordered_map>      // for unordered_map
#include <vector>             // for vector

namespace dv {
  namespace json {
    enum class Type {
      OBJECT,
      ARRAY,
      BOOL,
      INT,
      DOUBLE,
      STRING,
      NULLVALUE
    };

    std::ostream &operator<<( std::ostream &os, Type type );

    class JSON;
    class JSONPath;// IWYU pragma: keep
    class JSONDiffListener;// IWYU pragma: keep
    class JSONDiffListenerImpl;
    typedef std::shared_ptr<JSON> JSONPtr;

    struct JSONTypes {
      typedef std::nullptr_t nullType;
      typedef int64_t intType;
      typedef bool boolType;
      typedef double doubleType;
      typedef std::string stringType;
      typedef std::unordered_map<stringType, JSONPtr> objectType;
      typedef std::vector<JSONPtr> arrayType;
      typedef stringType keyType;
      typedef boost::variant<nullType, intType, boolType, doubleType, stringType, objectType, arrayType> valueType;
      typedef size_t indexType;
    };

    std::ostream &operator<<( std::ostream &os, const JSON &json );
    std::ostream &operator<<( std::ostream &os, const JSONDiffListenerImpl &listener );

    template<typename T>
    inline bool operator==( T &&v, const JSON &j ) {
      return j == v;
    }

    template<typename T>
    inline bool operator!=( T &&v, const JSON &j ) {
      return j != v;
    }
  }
}

#endif //DVFORMSCPP_JSONVALUEFWD_H
