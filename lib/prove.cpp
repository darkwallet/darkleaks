#include <darkleaks.hpp>

#include <fstream>
#include <set>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <bitcoin/bitcoin.hpp>
#include "utility.hpp"
#include "aes256.h"

namespace darkleaks {

using namespace bc;
namespace fs = boost::filesystem;

typedef std::vector<ec_point> ec_point_list;
typedef std::set<size_t> index_set;

size_t cycle(size_t value)
{
    if (value == 0)
        value = std::numeric_limits<size_t>::max();
    --value;
    return value;
}

prove_result prove(
    const std::string document_filename,
    const size_t chunks,
    const std::string block_hash,
    size_t reveal)
{
    prove_result result;
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
    // Avoid infinite loop deadlock.
    if (reveal > all_pubkeys.size())
        reveal = all_pubkeys.size();
    // Beginning bytes of block hash are zero, so use end bytes.
    std::seed_seq seq(block_hash.rbegin(), block_hash.rend());
    index_list random_values(reveal);
    seq.generate(random_values.begin(), random_values.end());
    // Use this to keep track of indexes we've added
    // to avoid duplicates.
    index_set ensure_unique;
    for (size_t value: random_values)
    {
        size_t index = value % all_pubkeys.size();
        // Ensure uniqueness of indexes.
        while (ensure_unique.find(index) != ensure_unique.end())
        {
            value = cycle(value);
            index = value % all_pubkeys.size();
        }
        ensure_unique.insert(index);
        // Now we have the index.
        ec_point pubkey = all_pubkeys[index];
        prove_result_row row{index, data_to_string(pubkey)};
        // Add row to results.
        result.push_back(row);
    }
    return result;
}

} // namespace darkleaks

