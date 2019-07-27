#!/usr/bin/env bash

outfile=intUtilsTest.out
rm $outfile
../mingwDebug/mgucc intUtilsTest | tee $outfile
diff golden/$outfile $outfile
