from __init__ import *

def showError(title, msg):
    QtGui.QMessageBox(QtGui.QMessageBox.Warning, title, msg, QtGui.QMessageBox.No, self).show()

def showDialog(parentnode, label=None):
    name = QtGui.QFileDialog.getOpenFileName(parentnode, 'Open file', './')
    if label:
      label.setText(name)

def showDialogDir(parentnode, label=None):
    name = QtGui.QFileDialog.getExistingDirectory(parentnode, 'Open dir', './')
    if label: 
      label.setText(name)

def showDialogText(parentnode, label, titleText, prompt):
    text, ok = QtGui.QInputDialog.getText(parentnode, titleText, prompt)
    if ok: label.setText(str(text))

def setup_testdir(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    os.makedirs(path)

def checksum(filename):
    f = open(filename)
    return hashlib.sha1(f.read()).hexdigest()
