#include <boost/python.hpp>
#include <darkleaks.hpp>

namespace python = boost::python;

const char* greet()
{
    return "hello";
}

BOOST_PYTHON_MODULE(_darkleaks)
{
    using namespace boost::python;
    def("greet", greet);
}

