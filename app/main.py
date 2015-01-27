import os
import sys
include_path = os.path.dirname(os.path.realpath(__file__)) + "/../python/"
sys.path.append(include_path)
# All sub-modules below this line will now be able to import darkleaks module!
import darkleaks

import shutil
import hashlib
from PyQt4 import QtGui, QtCore

def setup_testdir(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    os.makedirs(path)

def checksum(filename):
    f = open(filename)
    return hashlib.sha1(f.read()).hexdigest()

def showDialog(parent, qlabel):
    fname = QtGui.QFileDialog.getOpenFileName(parent, 'Open file', './')
    qlabel.setText(fname)

def showDialogDir(parent, qlabel):
    fname = QtGui.QFileDialog.getExistingDirectory(parent, 'Open dir', './')
    qlabel.setText(fname)

def showDialogText(self, qlabel, titleText, prompt):
    text, ok = QtGui.QInputDialog.getText(self, titleText, prompt)
    if ok: qlabel.setText(str(text))

actual_chunks = None

def getChunks(source_filename, number_chunks, chunks_directory, console):
  global actual_chunks
  if chunks_directory == "please select a chunks directory" or source_filename == "please select a source file": return

  #print source_filename, chunks_directory, number_chunks, '\n'

  setup_testdir(chunks_directory)
  console.setText("Digest of source file: " + checksum(source_filename))

  actual_chunks = darkleaks.start(source_filename, chunks_directory, number_chunks)
  console.setText("Created " + str(actual_chunks) + "chunks.")

def getProofs(source_filename, number_chunks, chunks_directory, block_hash, reveal_chunks, console):
  global actual_chunks
  if actual_chunks == None or chunks_directory == "please select a chunks directory" or source_filename == "please select a source file": return

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
  if source_filename == "please select a source file": return

  secret_keys = darkleaks.secrets(source_filename, number_chunks)
  console.setText(str(secret_keys))

def main(args):
    #print "hello!", args

    app = QtGui.QApplication(sys.argv)

    self = QtGui.QWidget()

    openFile = QtGui.QPushButton('Choose source file', self)
    openFile.move(20, 10)

    openDir = QtGui.QPushButton('Choose chunks dir', self)
    openDir.move(20, 40)

    chooseChunks = QtGui.QPushButton('Choose # of chunks (default 100)', self)
    chooseChunks.move(20, 70)

    blockHashDialog = QtGui.QPushButton('Choose blockhash', self)
    blockHashDialog.move(20, 100)

    revealChunks = QtGui.QPushButton('Choose # of chunks to reveal (default 10)', self)
    revealChunks.move(20, 130)

    consoleText = QtGui.QTextEdit(self)
    consoleText.move(150, 400)
    consoleText.setReadOnly(True)

    generateButton = QtGui.QPushButton('Generate Chunks', self)
    generateButton.setCheckable(True)
    generateButton.move(230, 10)

    proofsButton = QtGui.QPushButton('Generate Proofs', self)
    proofsButton.setCheckable(True)
    proofsButton.move(230, 40)

    secretsButton = QtGui.QPushButton('Generate Secrets', self)
    secretsButton.setCheckable(True)
    secretsButton.move(230, 70)

    sourceFile = QtGui.QLabel('please select a source file',self)
    sourceFile.setGeometry(40,200, 500, 40)
    sourceFile.move(20,200)

    chunksDir = QtGui.QLabel('please select a chunks directory',self)
    chunksDir.setGeometry(40,200, 500, 40)
    chunksDir.move(20,220)

    numChunks = QtGui.QLabel('100',self)
    numChunks.setGeometry(40,200, 500, 40)
    numChunks.move(20,240)

    blockHash = QtGui.QLabel('please add a blockhash',self)
    blockHash.setGeometry(40,200, 500, 40)
    blockHash.move(20,260)

    numChunksReveal = QtGui.QLabel('10',self)
    numChunksReveal.setGeometry(40,200, 500, 40)
    numChunksReveal.move(20,280)

    openFile.clicked.connect(lambda : showDialog(self, sourceFile))
    openDir.clicked.connect(lambda : showDialogDir(self, chunksDir))
    chooseChunks.clicked.connect(lambda : showDialogText(self, numChunks, 'chunks', 'Enter Chunks: '))
    blockHashDialog.clicked.connect(lambda : showDialogText(self, blockHash, 'blockhash', 'Enter blockhash: '))
    revealChunks.clicked.connect(lambda : showDialogText(self, numChunksReveal, 'reveal chunks', 'Enter Chunks to reveal: '))
    generateButton.clicked[bool].connect(lambda: getChunks(str(sourceFile.text()), int(numChunks.text()), str(chunksDir.text()), consoleText ) )
    proofsButton.clicked[bool].connect(
      lambda: getProofs(str(sourceFile.text()), int(numChunks.text()), str(chunksDir.text()), 
      str(blockHash.text()), int(numChunksReveal.text()), consoleText ) )
    secretsButton.clicked[bool].connect(lambda: getSecrets(str(sourceFile.text()), int(numChunks.text()), consoleText) )

    qbtn = QtGui.QPushButton('Quit', self)
    qbtn.clicked.connect(QtCore.QCoreApplication.instance().quit)
    qbtn.move(50, 600)       
    
    self.setGeometry(50, 50, 700, 700)
    self.setWindowTitle('DarkLeaks UI v1')
    self.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main(sys.argv[1:])

