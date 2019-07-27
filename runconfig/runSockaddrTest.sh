#!/usr/bin/env bash

outfile=sockaddrTest.out
rm $outfile
../Debug/mgucc sockaddrTest | tee $outfile
diff golden/$outfile $outfile
