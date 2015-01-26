import obelisk
import sys

address = None
client = None

def extract(script):
  # Sanity check
  if len(script) == 0: return None
  # Throw away signature bytes
  skipbytes = ord(script[0])
  # Throw away sighash byte
  skipbytes += 1
  # Record byte length of pubkey
  pkbytes = script[skipbytes:skipbytes+1]
  # Record starting position of pubkey
  startpk = skipbytes + 1
  # Store pubkey
  pk = script[startpk:startpk + ord(pkbytes) ]

  # Sanity check
  if not (len(pk) == 33 or len(pk) == 65):
    # Debug
    print "\n#pkbytes: ", ord(pkbytes), "\nlengthpk: ", len(pk), "\npk: ", pk , "\n"
    raise Exception("Pubkey length mismatch, something is seriously wrong, blowing up...")

  return pk

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
        public_key = extract( input.script )
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
    client = obelisk.ObeliskOfLightClient("tcp://dev.unsystem.net:9091")
    client.fetch_history2(address, history_fetched)
    obelisk.start()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print >> sys.stderr, "Usage: check_addr ADDRESS"
    else:
        main(sys.argv[1:])

