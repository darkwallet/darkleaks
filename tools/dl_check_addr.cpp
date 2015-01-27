#include <atomic>
#include <condition_variable>
#include <thread>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/client.hpp>
using namespace bc;

typedef std::shared_ptr<bc::client::obelisk_codec> codec_ptr;

bool stopped = false;
payment_address payaddr;
codec_ptr codec;

void error_handler(const std::error_code& ec)
{
    std::cerr << "dl_check_addr: error with query: "
        << ec.message() << std::endl;
    stopped = true;
};

void tx_fetched(const transaction_type& tx)
{
    for (const transaction_input_type& input: tx.inputs)
    {
        payment_address addr;
        if (!extract(addr, input.script))
            continue;
        if (addr.encoded() != payaddr.encoded())
            continue;
        BITCOIN_ASSERT(input.script.type() ==
            payment_type::pubkey_hash_sig);
        const ec_point& pubkey = input.script.operations().back().data;
        if (!verify_public_key(pubkey))
        {
            std::cerr << "dl_check_addr: problem with pubkey...";
            stopped = true;
            return;
        }
        std::cout << encode_base16(pubkey) << std::endl;
        stopped = true;
        return;
    }
    BITCOIN_ASSERT(false);
    stopped = true;
}

void history_fetched(const client::history_list& history)
{
    if (history.empty())
    {
        std::cerr << "dl_check_addr: No history yet at this address."
            << std::endl;
    }
    else
    {
        for (const auto& row: history)
        {
            if (row.spend.hash != null_hash)
            {
                std::cout << "Fetching transaction..." << std::endl;
                codec->fetch_transaction(error_handler, tx_fetched,
                    row.spend.hash);
                return;
            }
        }
        std::cerr << "dl_check_addr: No spends yet for this address. "
            "Keep bidding." << std::endl;
    }
    stopped = true;
}

/**
 * Unknown message callback handler.
 */
static void on_unknown(const std::string& command)
{
    std::cout << "unknown message:" << command << std::endl;
}

/**
 * Update message callback handler.
 */
static void on_update(const payment_address& address, size_t height,
    const hash_digest& blk_hash, const transaction_type&)
{
    std::cout << "update:" << address.encoded() << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: dl_check_addr ADDRESS" << std::endl;
        return -1;
    }
    if (!payaddr.set_encoded(argv[1]))
    {
        std::cerr << "dl_check_addr: invalid Bitcoin address." << std::endl;
        return -1;
    }
    std::string server = "tcp://dev.unsystem.net:9091";
    std::cout << "Connecting to: " << server << std::endl;
    czmqpp::context context;
    czmqpp::socket socket(context, ZMQ_DEALER);
    if (socket.connect(server) == -1)
    {
        std::cerr << "dl_check_addr: couldn't connect to blockchain server."
            << std::endl;
        return -1;
    }
    typedef std::shared_ptr<bc::client::socket_stream> stream_ptr;
    typedef std::shared_ptr<bc::client::message_stream> base_stream_ptr;

    stream_ptr stream = std::make_shared<bc::client::socket_stream>(socket);

    base_stream_ptr base_stream =
        std::static_pointer_cast<bc::client::message_stream>(stream);

    codec = std::make_shared<bc::client::obelisk_codec>(
        base_stream, on_update, on_unknown);

    codec->address_fetch_history(error_handler, history_fetched, payaddr);

    // Wait for the response:
    while (codec->outstanding_call_count())
    {
        czmqpp::poller poller;
        poller.add(socket);

        // Figure out how much timeout we have left:
        long delay = -1;
        auto next_wakeup = codec->wakeup();
        if (next_wakeup.count())
            delay = static_cast<long>(next_wakeup.count());

        // Sleep:
        poller.wait(delay);
        if (poller.terminated())
            break;
        if (!poller.expired())
            stream->signal_response(codec);
    }
    return 0;
}

