#!/usr/bin/env bash
outfile=getoptTest.out
../Debug/mgucc getoptTest -hs --longOnly -abcd -e -a not a option | tee $outfile

echo comparing with golden results...
diff golden/$outfile $outfile
