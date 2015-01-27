#ifndef DARKLEAKS_UTILITY
#define DARKLEAKS_UTILITY

namespace darkleaks {

template <typename DataBuffer>
std::string data_to_string(const DataBuffer& in)
{
    std::string out;
    out.resize(in.size());
    for (size_t i = 0; i < in.size(); ++i)
        out[i] = static_cast<char>(in[i]);
    return out;
}

inline bc::ec_point pubkey_to_point(const std::string& pubkey)
{
    BITCOIN_ASSERT(pubkey.size() == bc::ec_compressed_size);
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(pubkey.data());
    bc::ec_point point;
    point.resize(bc::ec_compressed_size);
    std::copy(bytes, bytes + pubkey.size(), point.begin());
    return point;
}

} // namespace darkleaks

#endif

