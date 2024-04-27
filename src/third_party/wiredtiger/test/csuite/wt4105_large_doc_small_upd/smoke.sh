#! /bin/sh

set -e

# Smoke-test wt4105_large_doc_small_upd as part of running "make check".

if [ -n "$1" ]
then
    # If the test binary is passed in manually.
    test_bin=$1
else
    # If $binary_dir isn't set, default to using the build directory
    # this script resides under. Our CMake build will sync a copy of this
    # script to the build directory. Note this assumes we are executing a
    # copy of the script that lives under the build directory. Otherwise
    # passing the binary path is required.
    binary_dir=${binary_dir:-`dirname $0`}
    test_bin=$binary_dir/test_wt4105_large_doc_small_upd
fi

$TEST_WRAPPER $test_bin -t r
$TEST_WRAPPER $test_bin -t c
