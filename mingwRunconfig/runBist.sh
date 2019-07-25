#!/usr/bin/env bash

rm test.out
../mingwDebug/mgucc > test.out
diff golden/test.out test.out
