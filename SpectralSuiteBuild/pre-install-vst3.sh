#!/bin/bash

DATE=$(date +%F-%H-%M-%S)
DIR=~/SpectralSuite-Backup/VST3-${DATE}
mkdir -p $DIR

cp -r ${DSTROOT} ${DIR}/ || true

# if no previous plugins then DIR will be empty and rmdir will succeed
rmdir ${DIR} || true