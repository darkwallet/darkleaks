#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <bitcoin/bitcoin.hpp>
#include "aes256.h"
using namespace bc;
namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: dl_secrets DOCUMENT CHUNKS" << std::endl;
        std::cerr << "Good default for CHUNKS is 100" << std::endl;
        return -1;
    }
    const fs::path document_full_path = argv[1];
    const fs::path doc_path = document_full_path.parent_path();
    const fs::path doc_filename = document_full_path.filename();
    const std::string chunks_str = argv[2];
    size_t chunks = 0;
    try
    {
        chunks = boost::lexical_cast<size_t>(chunks_str);
    }
    catch (const boost::bad_lexical_cast&)
    {
        std::cerr << "dl_secrets: bad CHUNKS provided." << std::endl;
        return -1;
    }
    const fs::path public_chunks_path =
        doc_path / (doc_filename.native() + "_public_chunks");
#if 0
    if (!fs::create_directory(public_chunks_path))
    {
        std::cerr << "dl_secrets: error creating path '"
            << public_chunks_path.native() << "'" << std::endl;
        return -1;
    }
#endif
    std::ifstream infile(document_full_path.native(), std::ifstream::binary);
    infile.seekg(0, std::ifstream::end);
    size_t file_size = infile.tellg();
    infile.seekg(0, std::ifstream::beg);
    size_t chunk_size = file_size / chunks;
    // AES works on blocks of 16 bytes. Round up to nearest multiple.
    chunk_size += 16 - (chunk_size % 16);
    BITCOIN_ASSERT(chunk_size % 16 == 0);
    //std::cout << "Creating chunks of "
    //    << chunk_size << " bytes each." << std::endl;
    // Write the bidding address and chunks
    size_t i = 0;
    hash_list hashes;
    while (infile)
    {
        data_chunk buffer(chunk_size);
        // Copy chunk to public chunk file.
        char* data = reinterpret_cast<char*>(buffer.data());
        infile.read(data, chunk_size);
        ++i;
        // Create a seed.
        BITCOIN_ASSERT(ec_secret_size == hash_size);
        ec_secret secret = bitcoin_hash(buffer);
        std::cout << secret_to_wif(secret) << std::endl;
    }
    return 0;
}

