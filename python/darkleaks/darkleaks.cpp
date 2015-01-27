#include <boost/python.hpp>
#include <darkleaks.hpp>

namespace python = boost::python;

python::object prove(
    const std::string document_filename,
    const size_t chunks,
    const std::string block_hash,
    const size_t reveal)
{
    python::list converted;
    auto result = darkleaks::prove(
        document_filename, chunks, block_hash, reveal);
    for (auto row: result)
    {
        converted.append(python::make_tuple(
            row.index, row.pubkey));
    }
    return converted;
}

python::object secrets(
    const std::string document_filename,
    const size_t chunks)
{
    python::list converted;
    auto result = darkleaks::secrets(document_filename, chunks);
    for (auto secret: result)
    {
        converted.append(secret);
    }
    return converted;
}

BOOST_PYTHON_MODULE(_darkleaks)
{
    using namespace boost::python;
    def("start", darkleaks::start);
    def("prove", prove);
    def("unlock", darkleaks::unlock);
    def("secrets", secrets);
}

