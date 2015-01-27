#ifndef DL_UTILITY_HPP
#define DL_UTILITY_HPP

#include <darkleaks.hpp>

#include <iomanip>
#include <bitcoin/bitcoin.hpp>
using namespace bc;

inline bool hex_to_hash(const std::string hex, std::string& hash)
{
    hash_digest result;
    if (!decode_hash(result, hex))
        return false;
    const char* chars = reinterpret_cast<const char*>(result.data());
    hash.resize(result.size());
    std::copy(chars, chars + result.size(), hash.begin());
    std::reverse(hash.begin(), hash.end());
    return true;
}

inline std::string hexify(const std::string hash)
{
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(hash.data());
    std::stringstream ss;
    for (size_t i = 0; i < hash.size(); ++i)
    {
        int val = bytes[i];
        ss << std::hex << std::setfill('0') << std::setw(2) << val;
    }
    return ss.str();
}

#endif

