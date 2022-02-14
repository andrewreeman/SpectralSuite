#!/bin/bash

DATE=$(date +%F-%H-%M-%S)
DIR=~/SpectralSuite-Backup/AU-${DATE}

mkdir -p $DIR

cp -r "${DSTROOT}/Bin Scrambler.component" ${DIR}/ || true
cp -r "${DSTROOT}/Frequency Magnet.component" ${DIR}/ || true
cp -r "${DSTROOT}/Frequency Shifter.component" ${DIR}/ || true
cp -r "${DSTROOT}/Morph.component" ${DIR}/ || true
cp -r "${DSTROOT}/Phase Lock.component" ${DIR}/ || true
cp -r "${DSTROOT}/Sinusoidal Shaped Filter.component" ${DIR}/ || true
cp -r "${DSTROOT}/Spectral Gate.component" ${DIR}/ || true

# if no previous plugins then DIR will be empty and rmdir will succeed
rmdir ${DIR} || true