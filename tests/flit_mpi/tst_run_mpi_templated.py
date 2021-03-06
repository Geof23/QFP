# -- LICENSE BEGIN --
#
# Copyright (c) 2015-2020, Lawrence Livermore National Security, LLC.
#
# Produced at the Lawrence Livermore National Laboratory
#
# Written by
#   Michael Bentley (mikebentley15@gmail.com),
#   Geof Sawaya (fredricflinstone@gmail.com),
#   and Ian Briggs (ian.briggs@utah.edu)
# under the direction of
#   Ganesh Gopalakrishnan
#   and Dong H. Ahn.
#
# LLNL-CODE-743137
#
# All rights reserved.
#
# This file is part of FLiT. For details, see
#   https://pruners.github.io/flit
# Please also read
#   https://github.com/PRUNERS/FLiT/blob/master/LICENSE
#
# Redistribution and use in source and binary forms, with or
# without modification, are permitted provided that the following
# conditions are met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the disclaimer below.
#
# - Redistributions in binary form must reproduce the above
#   copyright notice, this list of conditions and the disclaimer
#   (as noted below) in the documentation and/or other materials
#   provided with the distribution.
#
# - Neither the name of the LLNS/LLNL nor the names of its
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL
# SECURITY, LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
# Additional BSD Notice
#
# 1. This notice is required to be provided under our contract
#    with the U.S. Department of Energy (DOE). This work was
#    produced at Lawrence Livermore National Laboratory under
#    Contract No. DE-AC52-07NA27344 with the DOE.
#
# 2. Neither the United States Government nor Lawrence Livermore
#    National Security, LLC nor any of their employees, makes any
#    warranty, express or implied, or assumes any liability or
#    responsibility for the accuracy, completeness, or usefulness of
#    any information, apparatus, product, or process disclosed, or
#    represents that its use would not infringe privately-owned
#    rights.
#
# 3. Also, reference herein to any specific commercial products,
#    process, or services by trade name, trademark, manufacturer or
#    otherwise does not necessarily constitute or imply its
#    endorsement, recommendation, or favoring by the United States
#    Government or Lawrence Livermore National Security, LLC. The
#    views and opinions of authors expressed herein do not
#    necessarily state or reflect those of the United States
#    Government or Lawrence Livermore National Security, LLC, and
#    shall not be used for advertising or product endorsement
#    purposes.
#
# -- LICENSE END --

'''
Tests FLiT's capabilities to compile and run MPI tests

The tests are below using doctest

Let's now make a temporary directory and test that we can successfully compile
and run the FLiT test with MPI

>>> import glob
>>> import os
>>> import shutil
>>> import subprocess as subp

>>> class TestError(RuntimeError): pass

Delete MAKEFLAGS so that silent mode does not propogate
>>> if 'MAKEFLAGS' in os.environ:
...     del os.environ['MAKEFLAGS']

>>> with th.tempdir() as temp_dir:
...     retval = th.flit.main(['init', '-C', temp_dir]) # doctest:+ELLIPSIS
...     if retval != 0:
...         raise TestError('Main #1 returned with {}, failed to initialize'
...                         .format(retval))
...     _ = shutil.copy(os.path.join('data', 'MpiFloat.cpp'),
...                     os.path.join(temp_dir, 'tests'))
...     _ = shutil.copy(os.path.join('data', 'flit-config.toml'), temp_dir)
...     os.remove(os.path.join(temp_dir, 'tests', 'Empty.cpp'))
...     retval = th.flit.main(['update', '-C', temp_dir])
...     if retval != 0:
...         raise TestError('Main #2 returned with {}, failed to update'
...                         .format(retval))
...     compile_str = subp.check_output(['make', '-C', temp_dir, 'gt',
...                                      'VERBOSE=1'],
...                                     stderr=subp.STDOUT)
...     run_str = subp.check_output(['make', '-C', temp_dir,
...                                  'ground-truth.csv', 'VERBOSE=1'],
...                                 stderr=subp.STDOUT)
...     file_f = os.path.join(temp_dir, 'ground-truth.csv_MpiFloat_f.dat')
...     file_d = os.path.join(temp_dir, 'ground-truth.csv_MpiFloat_d.dat')
...     file_e = os.path.join(temp_dir, 'ground-truth.csv_MpiFloat_e.dat')
...     with open(file_f, 'r') as fin: contents_f = fin.read()
...     with open(file_d, 'r') as fin: contents_d = fin.read()
...     with open(file_e, 'r') as fin: contents_e = fin.read()
Creating /.../flit-config.toml
Creating /.../custom.mk
Creating /.../main.cpp
Creating /.../tests/Empty.cpp
Creating /.../Makefile
>>> lines_f = contents_f.splitlines()
>>> lines_d = contents_d.splitlines()
>>> lines_e = contents_e.splitlines()
>>> compile_str = compile_str.decode('utf-8').strip().splitlines()
>>> run_str = run_str.decode('utf-8').strip().splitlines()

Make sure the correct arguments are passed to mpirun

>>> './gtrun --output ground-truth.csv --no-timing' in run_str
True

Make sure the console messages are there, but they can be out of order

>>> all_lines = (lines_f, lines_d, lines_e)
>>> [x.count('hello from rank 0 of 2') for x in all_lines]
[1, 1, 1]
>>> [x.count('hello from rank 1 of 2') for x in all_lines]
[1, 1, 1]
>>> [x.count("Sending '3.14159' from rank 0") for x in all_lines]
[1, 1, 1]
>>> [x.count("Received '3.14159' from rank 0 to rank 1") for x in all_lines]
[1, 1, 1]

>>> lines_f.count('mpi_main_F<f>(3, {mympi, remaining, arguments})')
2
>>> lines_f.count('mpi_main_F<d>(3, {mympi, remaining, arguments})')
0
>>> lines_f.count('mpi_main_F<e>(3, {mympi, remaining, arguments})')
0

>>> lines_d.count('mpi_main_F<f>(3, {mympi, remaining, arguments})')
0
>>> lines_d.count('mpi_main_F<d>(3, {mympi, remaining, arguments})')
2
>>> lines_d.count('mpi_main_F<e>(3, {mympi, remaining, arguments})')
0

>>> lines_e.count('mpi_main_F<f>(3, {mympi, remaining, arguments})')
0
>>> lines_e.count('mpi_main_F<d>(3, {mympi, remaining, arguments})')
0
>>> lines_e.count('mpi_main_F<e>(3, {mympi, remaining, arguments})')
2
'''

# Test setup before the docstring is run.
import sys
before_path = sys.path[:]
sys.path.append('..')
import test_harness as th
sys.path = before_path

if __name__ == '__main__':
    from doctest import testmod
    failures, tests = testmod()
    sys.exit(failures)
