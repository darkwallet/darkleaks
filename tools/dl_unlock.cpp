#include <darkleaks.hpp>
#include <bitcoin/bitcoin.hpp>
#include "utility.hpp"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: dl_unlock CHUNKFILE PUBKEY"
            << std::endl;
        return -1;
    }
    const std::string chunk_filename = argv[1];
    const std::string pubkey_hex = argv[2];
    std::string pubkey;
    if (!hex_to_pubkey(pubkey_hex, pubkey))
    {
        std::cerr << "dl_unlock: not a valid PUBKEY." << std::endl;
        return -1;
    }
    darkleaks::unlock(chunk_filename, pubkey);
    return 0;
}


