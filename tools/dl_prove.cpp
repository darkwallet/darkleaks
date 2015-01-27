#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <darkleaks.hpp>
#include "utility.hpp"

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        std::cerr << "Usage: dl_prove DOCUMENT CHUNKS BLOCK_HASH REVEAL"
            << std::endl;
        return -1;
    }
    const std::string document_filename = argv[1];
    const std::string chunks_str = argv[2];
    const std::string reveal_str = argv[4];
    size_t chunks = 0, reveal = 0;
    try
    {
        chunks = boost::lexical_cast<size_t>(chunks_str);
        reveal = boost::lexical_cast<size_t>(reveal_str);
    }
    catch (const boost::bad_lexical_cast&)
    {
        std::cerr << "dl_start: bad CHUNKS or REVEAL provided." << std::endl;
        return -1;
    }
    const std::string hex = argv[3];
    std::string hash;
    if (!hex_to_hash(hex, hash))
    {
        std::cerr << "dl_prove: not a valid BLOCK_HASH." << std::endl;
        return -1;
    }
    auto result = darkleaks::prove(
        document_filename, chunks, hash, reveal);
    for (const auto row: result)
    {
        std::cout << (row.index + 1) << " "
            << hexify(row.pubkey) << std::endl;
    }
    return 0;
}

