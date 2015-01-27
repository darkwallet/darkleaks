import darkleaks
import shutil
import os
import hashlib

source_filename = "ROJAVA"
chunks_directory = "./test/"
number_chunks = 20

def setup_testdir(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    os.makedirs(path)

def checksum(filename):
    f = open(filename)
    return hashlib.sha1(f.read()).hexdigest()

setup_testdir(chunks_directory)
print "Digest of source file:", checksum(source_filename)

actual_chunks = darkleaks.start(source_filename, chunks_directory, 20)
print "Created", actual_chunks, "chunks."

# Now the chunks are passed around on BitTorrent...

# The block comes and is mined. Now we have the block_hash.
block_hash = "000000000000000014d8d1db40f33e87e8c9c5f37accb2718d40a7a1860b98c4".decode("hex")
# The leaker promised beforehand to release 10 chunks, so based
# off the block hash he now releases the pubkeys for those.
# Users can:
#   A) see that the donate addresses are generated from these
#   pubkeys as specified per chunk.
#   B) see that the pubkeys are actually used to decrypt the chunks.
# The leaker cannot preselect which chunks will get selected for
# revealing, only the number (which he announced publically).

proofs = darkleaks.prove(
    source_filename, number_chunks, block_hash, number_chunks)

# NOTE: in this example we simply will publish the proofs for all
# chunks. Normally you use check_address.py to get the pubkeys you
# need to unlock chunks.

# Just iterate chunk files.
for i, pubkey in proofs:
    chunk_filename = chunks_directory + "CHUNK." + str(i)
    darkleaks.unlock(chunk_filename, pubkey, ".decrypted")

# Now lets stitch the files back together to compare the result.
output_filename = chunks_directory + "OUTPUT"
f = open(output_filename, "a")
for i in range(actual_chunks):
    decrypted_file = chunks_directory + "CHUNK." + str(i) + ".decrypted"
    f.write(open(decrypted_file).read())

print "Rebuilt file digest:", checksum(output_filename)

# Finally the leaker can run this to get the private keys so he
# can claim the Bitcoins when people send for unlocking chunks.
secret_keys = darkleaks.secrets(source_filename, number_chunks)

