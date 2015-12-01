#!/usr/bin/python3

# this is the launcher for QFP_gdb
# where: testid is one of:
#  0: DoOrthoPerturbTestP2
#  1: DoOrthoPerturbTestN2
#  2: DoMatrixMultSanity
#  3: DoSimpleRotate90
#  4: RotateAndUnrotate
#  5: RotateFullCircle
#  6: DoSkewSymPRotationTest
#  7: DoHariGSBasic
#  8: DoHariGSImproved



import sys
from subprocess import check_output
from os import path

def usage():             #  1         2           3        4              5
    print(sys.argv[0] + '[bin1 path] [bin2 path] [testid] [prec = f|d|e] [sort = lt|gt|ns|bi]')


if len(sys.argv != 6):
    usage()
    exit(1)

cmd_out = ""
emacs = check_output('which emacs', shell=True)[:-1]  #remove newline from which

topDir = os.path.realpath(__file__)
inf1 = os.path.realpath(sys.argv[1])
inf2 = os.path.realpath(sys.argv[2])
os.environ['TEST'] = sys.argv[3]
os.environ['PRECISION'] = sys.argv[4]
os.environ['SORT'] = sys.argv[5]

if os.path.isfile(inf1):
    os.remove(inf1)
if os.path.isfile(inf2):
    os.remove(inf2)
chdir(topDir + '/gdb') #this has secret .gdbinit script

try:
    print('creating symlinks to binaries...')
    print(check_output('ln', '-s', inf1, 'inf1'))
    print(check_output('ln', '-s', inf2, 'inf2'))
    
    cmd_out = check_output(emacs, '--eval="(gdb \"gdb -i=mi "' + inf1)
    
except CalledProcessError:
    print(cmd_out)
    exit(1)

exit(0)