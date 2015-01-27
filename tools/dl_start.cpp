#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <darkleaks.hpp>
#include "utility.hpp"

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: dl_start DOCUMENT CHUNKS" << std::endl;
        std::cerr << "Good default for CHUNKS is 100" << std::endl;
        return -1;
    }
    const std::string document_filename = argv[1];
    const std::string chunks_str = argv[2];
    size_t chunks = 0;
    try
    {
        chunks = boost::lexical_cast<size_t>(chunks_str);
    }
    catch (const boost::bad_lexical_cast&)
    {
        std::cerr << "dl_start: bad CHUNKS provided." << std::endl;
        return -1;
    }
    const fs::path document_full_path = document_filename;
    const fs::path doc_path = document_full_path.parent_path();
    const fs::path doc_filename = document_full_path.filename();
    const fs::path public_chunks_path =
        doc_path / (doc_filename.native() + "_public_chunks");
    if (!fs::create_directory(public_chunks_path))
    {
        std::cerr << "dl_start: error creating path '"
            << public_chunks_path.native() << "'" << std::endl;
        return -1;
    }
    const std::string chunks_path = public_chunks_path.native();
    std::cout << "Created directory: " << chunks_path << "/" << std::endl;
    // Now create the chunks!
    size_t created_chunks = darkleaks::start(
        document_filename, chunks_path, chunks);
    // Finished.
    std::cout << created_chunks << " chunks created." << std::endl;
    std::cout << "Choose a future block height and "
        "a number of chunks to release." << std::endl;
    std::cout << "Announce them to the world." << std::endl;
    return 0;
}

