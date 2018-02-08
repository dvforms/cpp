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
      template<typename X, typename Y>
        struct variant_has_type {
         private:
          template<typename T, typename V>
            struct has_type;

          template<typename T, typename... Ts>
            struct has_type<T, boost::variant<T, Ts...> > {
              static const bool value = true;
            };

          template<typename T, typename U, typename... Ts>
            struct has_type<T, boost::variant<U, Ts...> > : has_type<T, boost::variant<Ts..., void> > {};

          template<typename T, typename... Ts>
            struct has_type<T, boost::variant<void, Ts...> > {
              static const bool value = false;
            };
         public:
          static const bool value = has_type<X, Y>::value;
        };
      template<typename X, typename Y> struct variant_has_type<const X, Y> : public variant_has_type<X, Y> {};
      template<typename X, typename Y> struct variant_has_type<X &, Y> : public variant_has_type<X, Y> {};

      template<typename X, typename Y>
        struct variant_is_convertible {
         private:
          template<typename T, typename V>
            struct is_convertible;

          template<typename T, typename V, typename... Ts>
            struct is_convertible<T, boost::variant<V, Ts...>> {
              static const bool value = std::is_convertible<T, V>::value || is_convertible<T, boost::variant<Ts..., void>>::value;
            };

          template<typename T, typename... Ts>
            struct is_convertible<T, boost::variant<void, Ts...>> {
              static const bool value = false;
            };
         public:
          static const bool value = is_convertible<X, Y>::value;
        };
      template<typename X, typename Y> struct variant_is_convertible<const X, Y> : public variant_is_convertible<X, Y> {};

      static_assert( variant_is_convertible<const char[], JSONTypes::valueType>::value, "Should be convertible" );

      static_assert( variant_has_type<std::string, JSONTypes::valueType>::value, "should have" );
      static_assert( variant_has_type<const std::string, JSONTypes::valueType>::value, "should have" );
      static_assert( variant_has_type<std::string &, JSONTypes::valueType>::value, "should have" );
      static_assert( variant_has_type<const std::string &, JSONTypes::valueType>::value, "should have" );
      static_assert( !variant_has_type<float, JSONTypes::valueType>::value, "shouldn't have" );

      static_assert( std::is_assignable<JSONTypes::stringType, const char[1]>::value, "bla" );

      struct to_json_function {
       private:
        template<typename JsonType, typename T, typename std::enable_if<variant_has_type<T, JSONTypes::valueType>::value, int>::type = 0>
        void call( JsonType &j, T &&val, PriorityTag<10> ) const noexcept {
          j.value = val;
        }

        template<typename JsonType, typename T>
        auto call( JsonType &j, T &&value, PriorityTag<9> ) const noexcept -> decltype( j.value = JSONTypes::stringType( value ), void() ) {
          j.value = JSONTypes::stringType( value );
        }

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

//        template<typename JsonType, typename T>
//        void call( JsonType &, T &&, PriorityTag<0> ) const noexcept {
//          static_assert( sizeof( JsonType ) == 0, "could not find to_json() method in T's namespace" );
//        }

       public:
        template<typename JsonType, typename T>
        void operator()( JsonType &j, T &&val ) const noexcept( noexcept( std::declval<to_json_function>().call( j, std::forward<T>( val ),
                                                                                                                 PriorityTag<10> {} ) ) ) {
          return call( j, std::forward<T>( val ), PriorityTag<10> {} );
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

      static_assert( std::is_floating_point<const double>::value, "bla" );

      template<typename A, typename B> struct are_comparable {
        typedef typename std::remove_reference<typename std::remove_const<A>::type>::type X;
        typedef typename std::remove_reference<typename std::remove_const<B>::type>::type Y;
        static const bool floatValue =
          ( std::is_floating_point<X>::value && std::is_integral<Y>::value ) ||
          ( std::is_floating_point<Y>::value && std::is_integral<X>::value ) ||
          ( std::is_floating_point<X>::value && std::is_floating_point<Y>::value );
        static const bool value = std::is_convertible<X, Y>::value && !floatValue;
      };

      struct json_compare_function {
       private:
        template<typename O> struct visitor : public boost::static_visitor<bool> {
          visitor( O o, const JSON &json ) : oo( o ), j( json ) {}

          template<typename JsonType, typename Other=O> bool call( JsonType &, const std::nullptr_t &, Other &&, PriorityTag<10> ) const {
            return std::is_same<typename std::remove_const<typename std::remove_reference<Other>::type>::type, std::nullptr_t>::value;
          }

          template<typename JsonType, typename Current, typename Other=O, typename std::enable_if<are_comparable<Current, Other>::floatValue, int>::type = 0>
          bool call( JsonType &, Current &&value, Other &&other, PriorityTag<8> ) const {
            return abs( other - value ) <= std::numeric_limits<JSONTypes::doubleType>::epsilon();
          }

          template<typename JsonType, typename Current, typename Other=O, typename std::enable_if<are_comparable<Other, Current>::value, int>::type = 0>
          auto call( JsonType &, Current &&value, Other &&other, PriorityTag<7> ) const -> decltype( value == other ) {
            return value == other;
          }

          template<typename JsonType, typename Current, typename Other=O,
            typename std::enable_if<variant_is_convertible<Other, JSONTypes::valueType>::value, int>::type = 0>
          bool call( JsonType &, Current &&, Other &&, PriorityTag<6> ) const {
            return false;
          }

          template<typename JsonType, typename Current, typename Other=O>
          bool call( const JsonType &, Current &, Other &&other, PriorityTag<5> ) const noexcept( noexcept( json_compare( std::declval<JsonType>(),
                                                                                                                          std::declval<Other>() ) ) ) {
            return json_compare( j, other );
          }

//        template<typename JsonType, typename T>
//        bool call( const JsonType &, T &, PriorityTag<0> ) const noexcept {
//          static_assert( sizeof( JsonType ) == 0, "could not find json_compare() method in T's namespace" );
//          return false;
//        }

          O oo;
          const JSON &j;

          template<typename X>
          bool operator()( X &&val ) const {
            return call( j, std::forward<X>( val ), oo, PriorityTag<10> {} );
          }
        };

       public:
        template<typename JsonType, typename T>
        bool operator()( JsonType &j, T &&val ) const noexcept {
          visitor<T> v( std::forward<T>( val ), j );
          return j.value.apply_visitor( v );
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
