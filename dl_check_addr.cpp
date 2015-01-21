#include <atomic>
#include <condition_variable>
#include <thread>
#include <bitcoin/bitcoin.hpp>
#include <obelisk/obelisk.hpp>
using namespace bc;

bool stopped = false;
payment_address payaddr;
obelisk::fullnode_interface* node = nullptr;

void tx_fetched(const std::error_code& ec, const transaction_type& tx)
{
    if (ec)
    {
        std::cerr << "dl_check_addr: Failed to fetch transaction: "
            << ec.message() << std::endl;
    }
    else
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
            std::cout << pubkey << std::endl;
            stopped = true;
            return;
        }
        BITCOIN_ASSERT(false);
    }
    stopped = true;
}

void history_fetched(
    const std::error_code& ec, const blockchain::history_list& history)
{
    if (ec)
    {
        std::cerr << "dl_check_addr: Failed to fetch history: "
            << ec.message() << std::endl;
    }
    else if (history.empty())
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
                node->blockchain.fetch_transaction(row.spend.hash, tx_fetched);
                return;
            }
        }
        std::cerr << "dl_check_addr: No spends yet for this address. "
            "Keep bidding." << std::endl;
    }
    stopped = true;
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
    threadpool pool(1);
    obelisk::fullnode_interface fullnode(pool,
        "tcp://obelisk.unsystem.net:9091");
    fullnode.address.fetch_history(payaddr, history_fetched);
    node = &fullnode;
    // Try to find pubkey for address.
    // Use the blockchain.
    while (!stopped)
    {
        fullnode.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    pool.stop();
    pool.join();
    return 0;
}

