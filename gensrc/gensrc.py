#!/usr/bin/env python3

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
This script generates random FLiT tests
'''

from environment import Environment, Variable
from expression import random_expression
from testcase import TestCase

import argparse
import math
import numpy as np
import os
import random
import sys


# TODO: write unit tests and put them somewhere else
def test_random_expression():
   print('Running test_random_expression()')
   env = Environment({
      'x': Variable('x', 'int'),
      'y': Variable('y', 'float'),
      'z': Variable('z', 'T'),
      })
   for i in range(10):
      print(random_expression(env, random.randint(0, 15), True))
   print()

def test_TestCase():
   print('Running test_TestCase()')
   tc = TestCase('TestCaseNumberOne', [6, 4, 2])
   print(tc)
   print()

def create_many_test_cases(directory, n):
    digits = math.ceil(math.log10(n + 1))
    name_template = 'RandomArithmeticTestCase_{0:0' + str(digits) + 'd}'
    for i in range(n):
        input_count = max(1, np.random.poisson(3))
        tc = TestCase(name_template.format(i+1), np.random.exponential(20, input_count))
        with open(os.path.join(directory, tc.name + '.cpp'), 'w') as outfile:
            outfile.write(str(tc))

def main(arguments):
   'Main logic here.  Returns 0 on success.'
   parser = argparse.ArgumentParser()
   parser.add_argument('-d', '--outdir', default='.', help='Directory to output random tests')
   parser.add_argument('number', type=int, help='How many test cases to generate')
   args = parser.parse_args(arguments)
   #test_random_expression()
   #test_TestCase()
   create_many_test_cases(args.outdir, args.number)
   return 0

if __name__ == '__main__':
   sys.exit(main(sys.argv[1:]))

