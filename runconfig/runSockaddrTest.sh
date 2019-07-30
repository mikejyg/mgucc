#!/usr/bin/env bash

outfile=sockaddrTest.out
rm $outfile
../Debug/mgucc sockaddrTest -b | tee $outfile
diff golden/$outfile $outfile
