#include <darkleaks.hpp>

#include <fstream>
#include <boost/filesystem.hpp>
#include <bitcoin/bitcoin.hpp>
#include "utility.hpp"
#include "aes256.h"

namespace darkleaks {

using namespace bc;
namespace fs = boost::filesystem;

string_list secrets(
    const std::string document_filename,
    const size_t chunks)
{
    std::ifstream infile(document_filename, std::ifstream::binary);
    infile.seekg(0, std::ifstream::end);
    size_t file_size = infile.tellg();
    infile.seekg(0, std::ifstream::beg);
    size_t chunk_size = file_size / chunks;
    // AES works on blocks of 16 bytes. Round up to nearest multiple.
    chunk_size += 16 - (chunk_size % 16);
    BITCOIN_ASSERT(chunk_size % 16 == 0);
    //std::cout << "Creating chunks of "
    //    << chunk_size << " bytes each." << std::endl;
    string_list result;
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
        result.push_back(secret_to_wif(secret));
    }
    return result;
}

} // namespace darkleaks

