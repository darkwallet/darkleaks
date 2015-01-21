#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <bitcoin/bitcoin.hpp>
#include "aes256.h"
using namespace bc;
namespace fs = boost::filesystem;

typedef std::vector<ec_point> ec_point_list;

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        std::cerr << "Usage: dl_prove DOCUMENT CHUNKS BLOCK_HASH REVEAL"
            << std::endl;
        return -1;
    }
    const fs::path doc_path = argv[1];
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
    const data_chunk hash = decode_hex(argv[3]);
    if (hash.empty() || hash.size() != hash_size)
    {
        std::cerr << "dl_prove: not a valid BLOCK_HASH." << std::endl;
        return -1;
    }
    std::ifstream infile(doc_path.native(), std::ifstream::binary);
    infile.seekg(0, std::ifstream::end);
    size_t file_size = infile.tellg();
    infile.seekg(0, std::ifstream::beg);
    size_t chunk_size = file_size / chunks;
    // AES works on blocks of 16 bytes. Round up to nearest multiple.
    chunk_size += 16 - (chunk_size % 16);
    BITCOIN_ASSERT(chunk_size % 16 == 0);
    //std::cout << "Creating chunks of "
    //    << chunk_size << " bytes each." << std::endl;
    ec_point_list all_pubkeys;
    while (infile)
    {
        data_chunk buffer(chunk_size);
        // Copy chunk to public chunk file.
        char* data = reinterpret_cast<char*>(buffer.data());
        infile.read(data, chunk_size);
        // Create a seed.
        BITCOIN_ASSERT(ec_secret_size == hash_size);
        ec_secret secret = bitcoin_hash(buffer);
        ec_point pubkey = secret_to_public_key(secret);
        // Once we spend funds, we reveal the decryption pubkey.
        all_pubkeys.push_back(pubkey);
    }
    // Beginning bytes of block hash are zero, so use end bytes.
    std::seed_seq seq(hash.rbegin(), hash.rend());
    index_list random_values(reveal);
    seq.generate(random_values.begin(), random_values.end());
    for (size_t value: random_values)
    {
        size_t index = value % all_pubkeys.size();
        std::cout << (index + 1) << " " << all_pubkeys[index] << std::endl;
    }
    return 0;
}

