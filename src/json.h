#pragma once
#ifndef DVFORMSCPP_JSON_H
#define DVFORMSCPP_JSON_H

#include <boost/variant.hpp>
#include <unordered_map>
#include <vector>
#include <map>

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
    class JSONPath;

    template<typename T>
      struct has_to_json {
      private:
        template<typename X>
        static constexpr auto check( X * ) -> typename std::is_same<decltype( to_json( std::declval<JSON &>(), std::declval<const X &>() ) ), void>::type;
        template<typename> static constexpr std::false_type check( ... );
        typedef decltype( check<T>( 0 ) ) type;
      public:
        static constexpr bool value = type::value;
      };

    class JSONDiffListener {
    public:
      virtual ~JSONDiffListener() = default;
      virtual bool isInterested() = 0;
      virtual void recordDifference( const JSONPath &path, const std::string &diff ) = 0;
    };

    class JSONDiffNullListener : public JSONDiffListener {
    public:
      ~JSONDiffNullListener() override = default;

      bool isInterested() override { return false; }

      void recordDifference( const JSONPath &/*path*/, const std::string &/*compare*/ ) override {}
    };

    class JSONDiffListenerImpl : public JSONDiffListener {
    public:
      ~JSONDiffListenerImpl() override = default;
      bool isInterested() override;
      void recordDifference( const JSONPath &path, const std::string &diff ) override;
      void clear();
      std::string toString() const;
    protected:
      std::map<JSONPath, std::string> differences;
      friend std::ostream &operator<<( std::ostream &os, const JSONDiffListenerImpl &listener );
    };

    std::ostream &operator<<( std::ostream &os, const JSONDiffListenerImpl &listener );

    typedef std::shared_ptr<JSON> JSONPtr;

    class JSON : public std::enable_shared_from_this<JSON> {
    public:
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
      JSON() = default;
      JSON( const JSON &other ) = default;
      JSON( JSON &&other ) = default;
      ~JSON() = default;

      template<typename T, typename std::enable_if<has_to_json<T>::value, int>::type = 0>
      inline JSON &operator=( const T &v );

      JSON &operator=( nullType );
      JSON &operator=( boolType v );
      JSON &operator=( const stringType &v );
      JSON &operator=( const char *v );
      JSON &operator=( intType );
      JSON &operator=( doubleType );
      JSON &operator=( const JSON &other ) = default;
      JSON &operator=( JSON &&other ) = default;

      template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
      inline JSON &operator=( T v ) {
        *this = static_cast<intType>(v);
        return *this;
      }

      JSON &operator[]( const keyType &key ) __attribute__((pure));
      JSON &operator[]( indexType index ) __attribute__((pure));
      bool operator==( const JSON &other ) const;
      Type type() const;

      bool compare( const JSON &other, JSONDiffListener &listener ) const;

      void dump( std::ostream &os, unsigned int indent = 0 ) const;

    protected:
      valueType value;
      void dump( std::ostream &os, unsigned int indent, unsigned int level ) const;
      void writeEscaped( std::ostream &os, const stringType &v ) const;
      bool compare( const JSON &other, JSONDiffListener &listener, const JSONPath &path ) const;
    };

    template<typename T, typename std::enable_if<has_to_json<T>::value, int>::type>
    JSON &JSON::operator=( const T &v ) {
      to_json( *this, v );
      return *this;
    }

    template<typename T>
    inline void to_json( JSON &json, const std::map<JSON::stringType, T> &v ) {
      json = nullptr;
      for ( const auto &it : v ) {
        json[it.first] = it.second;
      }
    }

    std::ostream &operator<<( std::ostream &os, const JSON &json );
  }
}

#endif //DVFORMSCPP_JSON_H
