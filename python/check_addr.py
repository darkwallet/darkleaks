import obelisk
import sys

address = None
client = None

def tx_fetched(ec, tx):
    if ec:
        print >> sys.stderr, "Internal error or bad DB:", ec
        obelisk.stop()
        return
    tx = obelisk.Transaction.deserialize(tx)
    for input in tx.inputs:
        # This is the input script
        print input.script.encode("hex")
        # Extract the public key from the script (begins with 02, 03 or 04)
        # Hardcoded value for now.
        public_key = "0206eaa3de705ff69e6cf15bcd3f67dfd33932010dbfb40778c3b4902038e886f9".decode("hex")
        # Use obelisk.public_key_to_bc_address(public_key) to get input_address
        input_address = obelisk.public_key_to_bc_address(public_key)
        # Skip input if not the one we're looking for.
        if input_address != address:
            continue
        # Found the public_key!
        print public_key.encode("hex")
        obelisk.stop()
        return
    print >> sys.stderr, "Internal error, couldn't find spend input!"
    obelisk.stop()

def history_fetched(ec, history):
    if ec:
        print >> sys.stderr, "Error:", ec
        obelisk.stop()
        return
    elif not history:
        print >> sys.stderr, "No history yet at this address."
        obelisk.stop()
        return
    for id, hash, index, height, value in history:
        if id == obelisk.PointIdent.Spend:
            client.fetch_transaction(hash, tx_fetched)
            return
    # Not found the hash!
    print >> sys.stderr, "No spends yet for this address. Keep bidding."
    obelisk.stop()

def main(args):
    global address
    global client
    address = args[0]
    client = obelisk.ObeliskOfLightClient("tcp://localhost:9091")
    client.fetch_history2(address, history_fetched)
    obelisk.start()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print >> sys.stderr, "Usage: check_addr ADDRESS"
    else:
        main(sys.argv[1:])

