import os
import sys
include_path = os.path.dirname(os.path.realpath(__file__)) + "/../python/"
sys.path.append(include_path)
# All sub-modules below this line will now be able to import darkleaks module!
import darkleaks

import shutil
import hashlib
from PyQt4 import QtGui, QtCore

def checksum(filename):
    f = open(filename)
    return hashlib.sha1(f.read()).hexdigest()

def showDialog(parent):
    fname = QtGui.QFileDialog.getOpenFileName(parent, 'Open file', '/home')

def getChunks(source_filename, number_chunks, chunks_directory):
  setup_testdir(chunks_directory)
  print "Digest of source file:", checksum(source_filename)

  actual_chunks = darkleaks.start(source_filename, chunks_directory, number_chunks)
  print "Created", actual_chunks, "chunks."

def main(args):
    print "hello!", args

    app = QtGui.QApplication(sys.argv)

    self = QtGui.QWidget()

    openButton = QtGui.QPushButton('Choose leaking document', self)
    openButton.move(200, 10)

    generateButton = QtGui.QPushButton('Generate Chunks', self)
    generateButton.setCheckable(True)
    generateButton.move(10, 10)

    #openFile = QtGui.QAction('Open', self)
    #openFile.triggered.connect(showDialog)

    openButton.clicked.connect(lambda : showDialog(self))
    generateButton.clicked[bool].connect(getChunks)

    qbtn = QtGui.QPushButton('Quit', self)
    qbtn.clicked.connect(QtCore.QCoreApplication.instance().quit)
    qbtn.move(50, 50)       
    
    self.setGeometry(300, 300, 250, 150)
    self.setWindowTitle('HelloWorld')
    self.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main(sys.argv[1:])

