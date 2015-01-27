import os
import sys
include_path = os.path.dirname(os.path.realpath(__file__)) + "/../python/"
sys.path.append(include_path)
# All sub-modules below this line will now be able to import darkleaks module!
import darkleaks
from PyQt4 import QtGui


def main(args):
    print "hello!", args

    app = QtGui.QApplication(sys.argv)

    w = QtGui.QWidget()
    w.resize(250, 150)
    w.move(300, 300)
    w.setWindowTitle('HelloWorld')
    w.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main(sys.argv[1:])

