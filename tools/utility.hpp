#ifndef DL_UTILITY_HPP
#define DL_UTILITY_HPP

#include <darkleaks.hpp>

#include <iomanip>
#include <bitcoin/bitcoin.hpp>
using namespace bc;

template <typename DataBuffer>
void convert_buffer(const DataBuffer& buffer, std::string& value)
{
    const char* chars = reinterpret_cast<const char*>(buffer.data());
    value.resize(buffer.size());
    std::copy(chars, chars + buffer.size(), value.begin());
}

inline bool hex_to_hash(const std::string hex, std::string& hash)
{
    hash_digest result;
    if (!decode_hash(result, hex))
        return false;
    convert_buffer(result, hash);
    std::reverse(hash.begin(), hash.end());
    return true;
}

inline bool hex_to_pubkey(const std::string hex, std::string& pubkey)
{
    const ec_point point = decode_hex(hex);
    if (point.empty() || point.size() != ec_compressed_size)
        return false;
    convert_buffer(point, pubkey);
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

