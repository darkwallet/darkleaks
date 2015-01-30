from __init__ import *

#global
actual_chunks = None

def check(a,b,c,text,console):
  error = None

  if a and text == "please select a chunks directory":
    error = 'try selecting a chunks directory'
  elif b and text == "please select a source file":
    error = 'try selecting a source file'
  elif c and text == None: 
    error = 'try generating chunks first'

  if error: 
    error = "Can\'t do that right now. Please " + error + " and try again."
    showError("Action failed", error)
    console.setText(error)

  print error, a, text
  return error

def getChunks(source_filename, number_chunks, chunks_directory, console):
  global actual_chunks

  if check(False, True, False, source_filename, console): return
  if check(True, False, False, chunks_directory, console): return

  #print source_filename, chunks_directory, number_chunks, '\n'

  setup_testdir(chunks_directory)
  console.setText("Digest of source file: " + checksum(source_filename))

  actual_chunks = darkleaks.start(source_filename, chunks_directory, number_chunks)
  console.setText("Created " + str(actual_chunks) + "chunks.")

def getProofs(source_filename, number_chunks, chunks_directory, block_hash, reveal_chunks, console):
  global actual_chunks

  if check(False, True, False, source_filename, console): return
  if check(True, False, False, chunks_directory, console): return
  if check(False, False, True, number_chunks, console): return

  #print source_filename, number_chunks, reveal_chunks, block_hash, '\n'

  proofs = darkleaks.prove(source_filename, number_chunks, block_hash, number_chunks)

  for i, pubkey in proofs:
      chunk_filename = chunks_directory + "/CHUNK." + str(i)
      darkleaks.unlock(chunk_filename, pubkey, ".decrypted")

  output_filename = chunks_directory + "OUTPUT"
  data = ""
  for i in range(actual_chunks):
      decrypted_file = chunks_directory + "/CHUNK." + str(i) + ".decrypted"
      data += open(decrypted_file).read()

  source_size = len(open(source_filename).read())

  data = data[:source_size]

  f = open(output_filename, "w").write(data)

  console.setText("Rebuilt file digest: " + checksum(output_filename))

def getSecrets(source_filename, number_chunks, console):
  if check(False,True, False, source_filename, console): return

  secret_keys = darkleaks.secrets(source_filename, number_chunks)
  console.setText(str(secret_keys))
