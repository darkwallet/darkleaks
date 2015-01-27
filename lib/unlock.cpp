#include <darkleaks.hpp>

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <bitcoin/bitcoin.hpp>
#include "utility.hpp"
#include "aes256.h"

namespace darkleaks {

using namespace bc;
namespace fs = boost::filesystem;

typedef std::vector<ec_point> ec_point_list;

payment_address bidding_address(const ec_point& pubkey)
{
    payment_address payaddr;
    set_public_key(payaddr, pubkey);
    return payaddr;
}
hash_digest derive_seed(const ec_point& pubkey)
{
    data_chunk data(pubkey.begin(), pubkey.end());
    return bitcoin_hash(data);
}

void unlock(
    const std::string chunk_filename,
    const std::string pubkey,
    const std::string decrypt_extension)
{
    // Convert string type to point.
    ec_point pubkey_point = pubkey_to_point(pubkey);
    std::ifstream infile(chunk_filename, std::ifstream::binary);
    infile.seekg(0, std::ifstream::end);
    size_t file_size = infile.tellg();
    BITCOIN_ASSERT(file_size % 16 == 0);
    infile.seekg(0, std::ifstream::beg);
    // Read entire file in.
    data_chunk cipher(file_size);
    // Copy chunk to public chunk file.
    char* data = reinterpret_cast<char*>(cipher.data());
    infile.read(data, file_size);
    infile.close();
    // Get seed.
    payment_address bid_addr = bidding_address(pubkey_point);
    hash_digest seed = derive_seed(pubkey_point);
    // Decrypt chunk.
    aes256_context ctx; 
    BITCOIN_ASSERT(seed.size() == 32);
    aes256_init(&ctx, seed.data());
    BITCOIN_ASSERT(cipher.size() % 16 == 0);
    for (size_t i = 0; i < cipher.size(); i += 16)
        aes256_decrypt_ecb(&ctx, cipher.data() + i);
    aes256_done(&ctx);
    // Write out.
    const fs::path new_chunk_filename = (chunk_filename + ".decrypted");
    std::ofstream outfile(new_chunk_filename.native(), std::ifstream::binary);
    char* dec_data = reinterpret_cast<char*>(cipher.data());
    outfile.write(dec_data, cipher.size());
}

} // namespace darkleaks

