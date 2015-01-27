import os
import sys
include_path = os.path.dirname(os.path.realpath(__file__)) + "/../python/"
sys.path.append(include_path)
# All sub-modules below this line will now be able to import darkleaks module!
import darkleaks

def main(args):
    print "hello!", args

if __name__ == "__main__":
    main(sys.argv[1:])

