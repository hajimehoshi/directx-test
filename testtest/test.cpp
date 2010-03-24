#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Foo
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE(foo) {
  BOOST_CHECK(1);
}
