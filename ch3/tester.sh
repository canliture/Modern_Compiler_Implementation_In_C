#!/bin/bash

function test(){
    for element in `ls $1`
    do
        dir_or_file=$1"/"$element
        if [ -d $dir_or_file ]
        then
            test $dir_or_file
        else
            ./a.out $dir_or_file
        fi
    done
}

if [ "$#" -ne 1 ]; then
  echo Usage: tester.sh TIG_FILE_DIRECTORY
  exit
fi

test $1