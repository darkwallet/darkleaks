from __init__ import *

def init(self): #init form controls

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

