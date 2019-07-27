#!/usr/bin/env bash

outfile=sockaddrTest.out
rm $outfile
../mingwDebug/mgucc sockaddrTest | tee $outfile
diff golden/$outfile $outfile
