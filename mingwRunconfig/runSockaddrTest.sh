#!/usr/bin/env bash

outfile=sockaddrTest.out
rm $outfile
../mingwDebug/mgucc sockaddrTest -b | tee $outfile
diff golden/$outfile $outfile
