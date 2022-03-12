#!/usr/bin/env bash

outfile=intUtilsTest.out
rm $outfile
../Debug/mgucc intUtilsTest | tee $outfile
diff golden/$outfile $outfile
