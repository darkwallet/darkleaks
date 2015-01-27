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

inline bc::payment_address bidding_address(const bc::ec_point& pubkey)
{
    bc::payment_address payaddr;
    bc::set_public_key(payaddr, pubkey);
    return payaddr;
}
inline bc::hash_digest derive_seed(const bc::ec_point& pubkey)
{
    bc::data_chunk data(pubkey.begin(), pubkey.end());
    return bitcoin_hash(data);
}

} // namespace darkleaks

#endif

