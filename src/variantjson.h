#pragma once
#ifndef DVFORMSCPP_VARIANTJSON_H
#define DVFORMSCPP_VARIANTJSON_H

#include "json.h"
#include <boost/variant.hpp>

namespace boost {
  using json = dv::json::JSON;
  using namespace dv::json;
  namespace to_json_detail {
    class to_json_visitor : public boost::static_visitor<> {
    public:
      explicit to_json_visitor( json &njson ) : json( &njson ) {}

      template<class T> void operator()( const std::shared_ptr<T> &value ) {
        if ( value ) {
          *json = *value;
        }
      }

      template<class T> void operator()( const std::weak_ptr<T> &value ) {
        ( *this )( value.lock() );
      }

      template<class T> void operator()( T &value ) {
        *json = value;
      }

    protected:
      json *json;
    };
  }

  template<typename... Args> inline void to_json( json &json, const boost::variant<Args...> &value ) {
    to_json_detail::to_json_visitor visitor( json );
    value.apply_visitor( visitor );
  }
}

#endif //DVFORMSCPP_VARIANTJSON_H
