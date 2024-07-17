#!/bin/bash 

# Exit early if any command fails
set -e

# Frequency shifter
auval -strict -v aumf SPFS STPW 

# Bin Scrambler
auval -strict -v aumf SPBS STPW

# Frequency magnet
auval -strict -v aumf SPFM STPW

# Morph
auval -strict -v aumf SPMO STPW

# Phase lock
auval -strict -v aumf SPPL STPW

# Sinusoidal Sharped Filer
auval -strict -v aumf SPSS STPW

# Spectral Gate
auval -strict -v aumf SPSG STPW
