from __init__ import *

def init(): #init form controls

    background = QtGui.QPixmap()
    background.load('./app/assets/walletlogo.png')
    bigone = QtGui.QLabel('',self)
    bigone.setPixmap(background)
    bigone.move(0,0)

    #self.render(bigone,QtCore.QPoint(), QtGui.QRegion(self.rect()), QtGui.QWidget.DrawWindowBackground)
    apptitle = QtGui.QLabel('DarkLeaks v0.0601',self)
    titlefont = QtGui.QFont("Times", 36, QtGui.QFont.Bold)
    apptitle.setFont(titlefont)
    apptitle.move(100, 20)

    normalfont = QtGui.QFont("Times", 12, QtGui.QFont.Bold)

    openFile = QtGui.QPushButton('Choose source file', self)
    openFile.move(30, 350)

    openDir = QtGui.QPushButton('Choose chunks dir', self)
    openDir.move(30, 380)

    chooseChunks = QtGui.QPushButton('Choose # of chunks (default 100)', self)
    chooseChunks.move(30, 410)

    blockHashDialog = QtGui.QPushButton('Choose blockhash', self)
    blockHashDialog.move(30, 440)

    revealChunks = QtGui.QPushButton('Choose # of chunks to reveal (default 10)', self)
    revealChunks.move(30, 470)

    consoleText = QtGui.QTextEdit(self)
    consoleText.setGeometry(325,325, 465, 225)
    consoleText.setReadOnly(True)

    generateButton = QtGui.QPushButton('Generate Chunks', self)
    generateButton.setCheckable(True)
    generateButton.move(335, 290)

    proofsButton = QtGui.QPushButton('Generate Proofs', self)
    proofsButton.setCheckable(True)
    proofsButton.move(455, 290)

    secretsButton = QtGui.QPushButton('Generate Secrets', self)
    secretsButton.setCheckable(True)
    secretsButton.move(565, 290)

    sourceFile = QtGui.QLabel('please select a source file',self)
    sourceFile.setFont(normalfont)
    sourceFile.setGeometry(50,100, 800, 40)

    chunksDir = QtGui.QLabel('please select a chunks directory',self)
    chunksDir.setFont(normalfont)
    chunksDir.setGeometry(50,125, 800, 40)

    numChunks = QtGui.QLabel('100',self)
    numChunks.setFont(normalfont)
    numChunks.setGeometry(50,150, 800, 40)

    blockHash = QtGui.QLabel('please add a blockhash',self)
    blockHash.setFont(normalfont)
    blockHash.setGeometry(50,175, 800, 40)

    numChunksReveal = QtGui.QLabel('10',self)
    numChunksReveal.setFont(normalfont)
    numChunksReveal.setGeometry(50,200, 800, 40)

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
    #grid.addWidget(qbtn,2,2)
    qbtn.move(685, 550)       
    
    screen = QtGui.QDesktopWidget().screenGeometry()      
    width = 800
    height = 600

    self.setGeometry( (screen.width() - width) / 2 , (screen.height() - height) / 2 , width, height)
    self.setWindowTitle('DarkLeaks UI v0.0601')

