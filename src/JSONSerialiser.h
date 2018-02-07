#pragma once
#ifndef DVFORMSCPP_JSONSERIALISER_H
#define DVFORMSCPP_JSONSERIALISER_H

#include <assert.h>           // for assert
#include <algorithm>          // for forward
#include <boost/variant.hpp>  // for variant
#include <memory>             // for shared_ptr, weak_ptr
#include <stdexcept>          // for runtime_error
#include <type_traits>        // for declval, enable_if, remove_cv
#include "jsonfwd.h"          // for JSONTypes, JSONTypes::valueType

namespace dv {
  namespace json {
    class JSON;

    template<unsigned N> struct PriorityTag : PriorityTag<N - 1> {};
    template<> struct PriorityTag<0> {};

    namespace detail {
      template<typename T, typename V>
        struct variant_has_type;

      template<typename T, typename... Ts>
        struct variant_has_type<T, boost::variant<T, Ts...> > {
          static const bool value = true;
        };

      template<typename T, typename U, typename... Ts>
        struct variant_has_type<T, boost::variant<U, Ts...> > : variant_has_type<T, boost::variant<Ts..., void> > {
        };

      template<typename T, typename U, typename... Ts>
        struct variant_has_type<const T, boost::variant<U, Ts...> > : variant_has_type<T, boost::variant<Ts..., void> > {
        };

      template<typename T, typename... Ts>
        struct variant_has_type<T, boost::variant<void, Ts...> > {
          static const bool value = false;
        };

      struct to_json_function {
       private:
        template<typename JsonType, typename T>
        auto call( JsonType &j, std::shared_ptr<T> &&val, PriorityTag<2> ) const noexcept( noexcept( to_json( j, std::forward<std::shared_ptr<T >>( val ) ) ) )
        -> decltype( to_json( j, std::forward<std::shared_ptr<T >>( val ) ), void() ) {
          return to_json( j, std::forward<std::shared_ptr<T >>( val ) );
        }

        template<typename JsonType, typename T>
        auto call( JsonType &j, std::weak_ptr<T> &&val, PriorityTag<2> ) const noexcept( noexcept( to_json( j, std::forward<std::weak_ptr<T >>( val ) ) ) )
        -> decltype( to_json( j, std::forward<std::weak_ptr<T >>( val ) ), void() ) {
          return to_json( j, std::forward<std::weak_ptr<T >>( val ) );
        }

        template<typename JsonType, typename T>
        auto call( JsonType &j, T &&val, PriorityTag<1> ) const noexcept( noexcept( to_json( j, std::forward<T>( val ) ) ) )
        -> decltype( to_json( j, std::forward<T>( val ) ), void() ) {
          return to_json( j, std::forward<T>( val ) );
        }

        template<typename JsonType, typename T>
        void call( JsonType &, T &&, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( JsonType ) == 0, "could not find to_json() method in T's namespace" );
        }

       public:
        template<typename JsonType, typename T>
        void operator()( JsonType &j, T &&val ) const noexcept( noexcept( std::declval<to_json_function>().call( j, std::forward<T>( val ),
                                                                                                                 PriorityTag<1> {} ) ) ) {
          return call( j, std::forward<T>( val ), PriorityTag<2> {} );
        }
      };

      struct from_json_function {
       private:
        template<typename JsonType, typename T>
        auto call( const JsonType &j, std::shared_ptr<T> &val, PriorityTag<3> ) const noexcept( noexcept( from_json( j, *val ) ) )
        -> decltype( from_json( j, *val ), void() ) {
          assert( val );
          return from_json( j, *val );
        }

        template<typename JsonType, typename T>
        auto call( const JsonType &j, T &val, PriorityTag<2> ) const noexcept( noexcept( from_json( j, val ) ) )
        -> decltype( from_json( j, val ), void() ) {
          return from_json( j, val );
        }

        template<typename JsonType, typename T>
        T &call( const JsonType &/*j*/, T &/*val*/, PriorityTag<1>,
                 typename std::enable_if<variant_has_type<typename std::remove_cv<T>::type, JSONTypes::valueType>::value, int>::type = 0 ) const {
          throw std::runtime_error( "Invalid conversion" );
        }

        template<typename JsonType, typename T>
        void call( const JsonType &, T &, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( JsonType ) == 0, "could not find from_json() method in T's namespace" );
        }

       public:
        template<typename JsonType, typename T>
        auto operator()( const JsonType &j, T &val ) const noexcept( noexcept( std::declval<from_json_function>().call( j, val, PriorityTag<3> {} ) ) )
        -> decltype( std::declval<from_json_function>().call( j, val, PriorityTag<3> {} ) ) {
          return call( j, val, PriorityTag<3> {} );
        }
      };

      struct json_compare_function {
       private:
        template<typename JsonType, typename T>
        bool call( const JsonType &j, T &val, PriorityTag<1> ) const noexcept( noexcept( json_compare( j, val ) ) ) {
          return json_compare( j, val );
        }

        template<typename JsonType, typename T>
        bool call( const JsonType &, T &, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( JsonType ) == 0, "could not find json_compare() method in T's namespace" );
          return false;
        }

       public:
        template<typename JsonType, typename T>
        bool
        operator()( const JsonType &j, T &val ) const noexcept( noexcept( std::declval<json_compare_function>().call( j, val, PriorityTag<1> {} ) ) ) {
          return call( j, val, PriorityTag<1> {} );
        }
      };

      struct json_construct_function {
       private:
        template<typename JsonType, typename T>
        auto call( JsonType *j, T *val, PriorityTag<1> ) const noexcept( noexcept( json_construct( val, j ) ) )
        -> decltype( json_construct( val, j ) ) {
          return json_construct( val, j );
        }

        template<typename JsonType, typename T>
        T call( const JsonType *, T *, PriorityTag<0> ) const noexcept {
          return T();
        }

       public:
        template<typename JsonType, typename T>
        auto
        operator()( JsonType *j, T *val ) const noexcept( noexcept( std::declval<json_construct_function>().call( j, val, PriorityTag<1> {} ) ) )
        -> decltype( std::declval<json_construct_function>().call( j, val, PriorityTag<1> {} ) ) {
          return call( j, val, PriorityTag<1> {} );
        }
      };

      template<typename T>
        struct static_const {
          static constexpr T value{};
        };

      template<typename T>
        constexpr T static_const<T>::value;
    }

    namespace {
      constexpr const auto &to_json = detail::static_const<detail::to_json_function>::value;
      constexpr const auto &from_json = detail::static_const<detail::from_json_function>::value;
      constexpr const auto &json_compare = detail::static_const<detail::json_compare_function>::value;
      constexpr const auto &json_construct = detail::static_const<detail::json_construct_function>::value;
    }

    template<typename T = void>
      struct JSONSerialiser {
        template<typename JsonType, typename ValueType>
        static void from_json( JsonType &&j, ValueType &val ) noexcept( noexcept( ::dv::json::from_json( std::forward<JsonType>( j ), val ) ) ) {
          ::dv::json::from_json( std::forward<JsonType>( j ), val );
        }

        template<typename JsonType, typename ValueType>
        static void to_json( JsonType &j, ValueType &&val ) noexcept( noexcept( ::dv::json::to_json( j, std::forward<ValueType>( val ) ) ) ) {
          ::dv::json::to_json( j, std::forward<ValueType>( val ) );
        }

        template<typename JsonType, typename ValueType>
        static bool compare( JsonType &j, ValueType &&val ) noexcept( noexcept( ::dv::json::json_compare( j, std::forward<ValueType>( val ) ) ) ) {
          return ::dv::json::json_compare( j, std::forward<ValueType>( val ) );
        }
      };

    template<typename T = void>
      struct JSONConstructor {
        typedef decltype( ::dv::json::json_construct( std::declval<JSON *>(), std::declval<T *>() ) ) constructType;

        static constructType construct( T *val ) {
          return ::dv::json::json_construct( static_cast<JSON *>(nullptr), val );
        }
      };
  }
}
#endif //DVFORMSCPP_JSONSERIALISER_H
