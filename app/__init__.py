# darkleaks v1
from PyQt4 import QtGui, QtCore
import os,sys, shutil, hashlib, time
include_path = os.path.dirname(os.path.realpath(__file__)) + "/../python/"
sys.path.append(include_path)

# Init QtGui
app = QtGui.QApplication(sys.argv)
# Init QWidget
self = QtGui.QWidget(None, QtCore.Qt.WindowFlags(3))

# All sub-modules below this line will now be able to import darkleaks module!
import darkleaks

from util import *
from dark import *
from form import *
