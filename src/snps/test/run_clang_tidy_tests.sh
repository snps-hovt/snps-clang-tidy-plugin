#!/bin/bash

# usage: ./run_clang_tidy_tests.sh 
# set TESTS_DIR to the path of a directory containing .cpp test files to run clang-tidy against those files.
# Each .cpp test file should have a coma separated list of checks for that file as a first line, commented in c++ comment style, like so "// chacke1, chack2"
# set CLANG_TIDY_PLUGINS environment variable to the coma separated list of paths to all clang-tidy plugins you want to load. 

RED='\033[0;31m'
NC='\033[0m'

# Path to clang-tidy
CLANG_TIDY=${CLANG_TIDY:-clang-tidy}
echo "Using clang-tidy from $CLANG_TIDY"

TESTS_DIR=${TESTS_DIR:-$(pwd)}

if [ ! -d "$TESTS_DIR" ]; then
    echo "Directory $TESTS_DIR does not exist."
    exit 1
fi

echo "Testing directory is $TESTS_DIR"

found_test=false

# Split the variable by comas
IFS=',' read -r -a plugins_array <<< "$CLANG_TIDY_PLUGINS"

tidy_load_plugins=" "

for plugin in "${plugins_array[@]}"; do
    tidy_load_plugins+="-load=${plugin} "
done

# Remove the trailing space
tidy_load_plugins=${tidy_load_plugins% }

check_return_value=0

# Enable nullglob to avoid issues with no matches
shopt -s nullglob
for test_file in "$TESTS_DIR"/*.cpp; do
    # Check if the file exists
    if [ ! -e "$test_file" ]; then
        continue
    fi
    found_test=true
    echo "Running clang-tidy on $test_file"
    checks_list=$(sed -n '1s|^[ /]*||p' $test_file |  tr -d '[:space:]')
    if [ -z "${checks_list}" ]; then
        echo "No checks in $test_file. Make sure the first line in a comment with coma separated list of checks."
        continue
    fi
    # First, check if all checkers listed in the test file are available in clang-tidy
    result_list=$($CLANG_TIDY -checks="-*,$checks_list" $tidy_load_plugins --list-checks | tail -n +2 | tr -s '[:space:]' ',' | awk "NF")
    result_list=${result_list#","}
    result_list=${result_list%","}

    # Convert lists to arrays removing spaces
    readarray -td, checks_array <<<"$checks_list,"; unset 'checks_array[-1]'
    readarray -td, result_array <<<"$result_list,"; unset 'result_array[-1]'

    # Check if all values in the $checks_array exists in the $result_array
    for check in "${checks_array[@]}"; do
        if ! [[ " ${result_array[@]} " =~ " $check " ]]; then
            check_return_value=1
            echo "Check $check not found! Used in $test_file"
            #break
        fi
    done
    # End of: check if all checkers listed in the test file are available in clang-tidy

    compile_commands=$(sed -n '2s|^[ /]*||p' $test_file |  tr -d '[:space:]')
    result_list=$($CLANG_TIDY $test_file -checks="-*,$checks_list" $tidy_load_plugins -- $compile_commands)
    while IFS= read -r line; do
        # Find warning/error line
        if [[ $line =~ ^(.*/.*\.cpp):([0-9]+):[0-9]+:\ (warning|error):\ (.*)\ \[(.*)\]$ ]]; then
            file_path="${BASH_REMATCH[1]}"
            line_number="${BASH_REMATCH[2]}"
            warning_error="${BASH_REMATCH[3]}"
            message="${BASH_REMATCH[4]}"
            check_name="${BASH_REMATCH[5]}"
            if ! [[ " ${checks_array[@]} " =~ " $check_name " ]]; then
                echo $line
                continue
            fi
            if [[ $file_path != $test_file ]]; then
                echo $line
                continue
            fi
            pass=false
            while [ "$line_number" -gt "4" ] ; do
                line_number=$((line_number - 1))
                golden_content=$(sed -n "${line_number}p" $test_file)
                if [[ "$golden_content" != *//* ]]; then
                    break
                fi
                if [[ $golden_content == *"${warning_error}"* && $golden_content == *"${message}" ]]; then
                    pass=true
                fi
            done
            if [ "$pass" = false ]; then
                echo "Test Failed: $line"
                check_return_value=1
            fi
        fi
    done <<< "$result_list"

    # TODO: call clang-tidy in the $test_file with --fix and compare results
done

if [ "$found_test" = true ]; then
    exit $check_return_value
else
    echo "No tests found in $TESTS_DIR"
    exit 1
fi