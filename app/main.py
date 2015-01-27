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

def getChunks(source_filename, number_chunks, chunks_directory):
  print source_filename, chunks_directory, number_chunks, '\n'
  setup_testdir(chunks_directory)
  print "Digest of source file:", checksum(source_filename)

  actual_chunks = darkleaks.start(source_filename, chunks_directory, number_chunks)
  print "Created", actual_chunks, "chunks."

def main(args):
    print "hello!", args

    app = QtGui.QApplication(sys.argv)

    self = QtGui.QWidget()

    openFile = QtGui.QPushButton('Choose source file', self)
    openFile.move(200, 10)

    openDir = QtGui.QPushButton('Choose chunks dir', self)
    openDir.move(200, 40)

    generateButton = QtGui.QPushButton('Generate Chunks', self)
    generateButton.setCheckable(True)
    generateButton.move(10, 10)

    sourceFile = QtGui.QLabel('please select a source file',self)
    sourceFile.setGeometry(40,2000, 2000, 40)
    sourceFile.move(20,100)

    chunksDir = QtGui.QLabel('please select a chunks directory',self)
    chunksDir.setGeometry(40,2000, 2000, 40)
    chunksDir.move(20,120)

    openFile.clicked.connect(lambda : showDialog(self, sourceFile))
    openDir.clicked.connect(lambda : showDialogDir(self, chunksDir))
    generateButton.clicked[bool].connect(lambda: getChunks(str(sourceFile.text()), 50, str(chunksDir.text()) ) )


    qbtn = QtGui.QPushButton('Quit', self)
    qbtn.clicked.connect(QtCore.QCoreApplication.instance().quit)
    qbtn.move(50, 50)       
    
    self.setGeometry(300, 300, 250, 150)
    self.setWindowTitle('HelloWorld')
    self.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main(sys.argv[1:])

