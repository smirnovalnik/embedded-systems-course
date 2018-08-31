#!/usr/bin/python
"""
Generate header and source files for lab3_ex with sin samples from 0 to 2pi.
Usage: sin_gen n min max,
where n - number of samples,
min - minimum value of samples,
max - maximum value of samples
Example:
sin_gen 100 0 1000
Default parameters:
n = 200, min = 0, max = 4095
"""

import sys
from math import sin, pi

FILE_NAME = "sin"

def sin_gen(n, mn, mx):
    """ Generate n sin samples in (mx - mn) range """
    return [int((mx - mn)*(sin(i*2*pi/n) + 1)/2) for i in range(n)]

if __name__ == "__main__":
    # Default parameters
    n, mn, mx = 200, 0, 2**12 - 1
    # Custom parameters
    if len(sys.argv) == 4:
        n, mn, mx = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
    # Generate header file
    with open(FILE_NAME + ".h", "w") as f:
        f.write(
"""/**
  ******************************************************************************
  * \\file  {0}.h
  * \\brief Sin samples n = {1}, min = {2}, max = {3}
  ******************************************************************************
  */

#ifndef _SIN_H_
#define _SIN_H_

#include <stdint.h>

#define SIN_LEN {1}

extern uint16_t sin_arr[SIN_LEN];

#endif /* _SIN_H_ */
""".format(FILE_NAME, n, mn, mx))

    # Generate source file
    samp = sin_gen(n, mn, mx)
    with open(FILE_NAME + ".c", "w") as f:
        f.write(
"""/**
  ******************************************************************************
  * \\file  {0}.c
  * \\brief Sin samples n = {1}, min = {2}, max = {3}
  ******************************************************************************
  */

#include "{0}.h"

uint16_t sin_arr[SIN_LEN] = {{
""".format(FILE_NAME, n, mn, mx))
        f.write("    ")
        for i, s in enumerate(samp):
            f.write("{0:5}, ".format(s))
            if (i + 1) % 8 == 0:
                f.write("\n    ")
        f.write("};\n")
