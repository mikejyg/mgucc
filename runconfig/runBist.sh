#!/usr/bin/env bash

rm test.out
../Debug/mgucc > test.out
diff golden/test.out test.out
