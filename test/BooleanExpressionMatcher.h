#pragma once
#ifndef DVFORMSCPP_BOOLEANEXPRESSIONMATCHER_H
#define DVFORMSCPP_BOOLEANEXPRESSIONMATCHER_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <FormExpressionBoolean.h>

namespace testing {

  template<typename T=dv::forms::FormExpressionBoolean>
    class BooleanExpressionMatcher : public testing::MatcherInterface<T> {
    public:
      typedef BooleanExpressionMatcher<T> self;
      static_assert( std::is_base_of<dv::forms::FormExpressionBoolean, typename std::remove_pointer<T>::type>::value,
                     "Must be a subtype of FormExpressionBoolean" );

      bool MatchAndExplain( T expression, testing::MatchResultListener *listener ) const override {
        bool rt = true;
        if ( subExpressionMatcher ) {
          *listener << "Sub Expressions ";
          rt = subExpressionMatcher->MatchAndExplain( expression->getSubExpressions(), listener );
        }
        return rt;
      }

      self &subExpressionsMatch( const Matcher <std::vector<dv::forms::FormExpressionPtr>> &matcher ) {
        subExpressionMatcher = &matcher;
        return *this;
      }

      void DescribeTo( ::std::ostream *os ) const override {
        if ( subExpressionMatcher ) {
          *os << "Sub Expressions ";
          subExpressionMatcher->DescribeTo( os );
        }
      }

      void DescribeNegationTo( ::std::ostream *os ) const override {
        if ( subExpressionMatcher ) {
          *os << "Sub Expressions ";
          subExpressionMatcher->DescribeNegationTo( os );
        }
      }

      Matcher <T> build() {
        return MakePolymorphicMatcher( *this );
      }

    private:
      const Matcher <std::vector<dv::forms::FormExpressionPtr>> *subExpressionMatcher{ nullptr };
//      const Matcher<typename T::value_type> value;
    };

  template<typename T=dv::forms::FormExpressionBoolean>
  inline BooleanExpressionMatcher<T> booleanExpressionMatcher() {
    return /*MakePolymorphicMatcher*/( BooleanExpressionMatcher<T>() );
  }
}

#endif //DVFORMSCPP_BOOLEANEXPRESSIONMATCHER_H
