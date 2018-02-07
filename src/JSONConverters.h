#pragma once
#ifndef DVFORMSCPP_JSONCONVERTERS_H
#define DVFORMSCPP_JSONCONVERTERS_H

#include <boost/variant.hpp>  // for static_visitor, variant
#include <map>                // for map
#include <memory>             // for shared_ptr, __shared_ptr_access, enable_shared_from_this, make_shared, weak_ptr
#include <type_traits>        // for enable_if, is_integral, is_base_of, is_floating_point
#include <unordered_map>      // for unordered_map
#include <vector>             // for vector
#include "JSONSerialiser.h"   // for variant_has_type, JSONSerialiser
#include "JSONValue.h"        // for JSON
#include "jsonfwd.h"          // for JSONTypes::intType, JSONTypes::stringType, JSONTypes::valueType, Type, JSONTypes::doubleType, Type::DOUBLE, Type::INT

namespace dv {
  namespace json {
    template<typename T>
    inline void to_json( JSON &json, const std::map<JSON::stringType, T> &v ) {
      json = nullptr;
      for ( const auto &it : v ) {
        json[it.first] = it.second;
      }
    }

    template<typename T>
    inline void to_json( JSON &json, const std::unordered_map<JSON::stringType, T> &v ) {
      json = nullptr;
      for ( const auto &it : v ) {
        json[it.first] = it.second;
      }
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    inline void to_json( JSON &json, T v ) {
      json = static_cast<JSON::intType>(v);
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    inline void from_json( const JSON &json, T &v ) {
      v = static_cast<T>(json.as<JSON::intType>());
    }

    namespace detail {
      class to_json_visitor : public boost::static_visitor<> {
       public:
        explicit to_json_visitor( JSON &njson ) : json( &njson ) {}

        template<class T> void operator()( const std::shared_ptr<T> &value ) {
          if ( value ) {
            ( *this )( *value );
          } else {
            *json = nullptr;
          }
        }

        template<class T> void operator()( const std::weak_ptr<T> &value ) {
          ( *this )( value.lock() );
        }

        template<typename T, typename std::enable_if<variant_has_type<T, JSON::valueType>::value, int>::type = 0>
        void operator()( T &value ) {
          *json = value;
        }

        template<class T, typename std::enable_if<!variant_has_type<T, JSON::valueType>::value, int>::type = 0>
        void operator()( T &value ) {
          JSONSerialiser<T>::to_json( *json, value );
        }

       protected:
        JSON *json;
      };
    }

    template<typename... Args> inline void to_json( JSON &json, const boost::variant<Args...> &value ) {
      detail::to_json_visitor visitor( json );
      value.apply_visitor( visitor );
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    bool json_compare( const JSON &j, T value ) {
      bool rt = false;

      if ( j.type() == Type::INT ) {
        rt = static_cast<JSON::intType>(j) == value;
      }

      return rt;
    }

    template<typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    bool json_compare( const JSON &j, T value ) {
      bool rt = false;

      if ( j.type() == Type::DOUBLE ) {
        rt = static_cast<JSON::doubleType>(j) == value;
      }

      return rt;
    }

    template<typename T>
    bool json_compare( const JSON &j, const std::vector<T> &value ) {
      bool rt = true;

      if ( value.size() != j.size() ) {
        rt = false;
      } else {
        auto values = value.begin();
        for ( const auto &item : j.arrayIterator() ) {
          if ( *item != *values ) {
            rt = false;
            break;
          }
          ++values;
        }
      }

      return rt;
    }

    template<typename T, typename std::enable_if<std::is_base_of<std::enable_shared_from_this<T>, T>::value, int>::type = 0>
    std::shared_ptr<T> json_construct( T *, JSON * ) {
      return std::make_shared<T>();
    }
  }
}

#endif //DVFORMSCPP_JSONCONVERTERS_H
