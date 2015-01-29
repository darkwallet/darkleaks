import shutil, hashlib

def setup_testdir(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    os.makedirs(path)

def checksum(filename):
    f = open(filename)
    return hashlib.sha1(f.read()).hexdigest()
