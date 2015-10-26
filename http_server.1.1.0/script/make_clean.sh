#!/bin/sh

function make_clean()
{
    for i in $@
    do
        if [[ "$i" != "./" && "$i" != "../" ]]
        then
            echo $i
            cd $i
            make clean
            cd -
        fi
    done
}

cd ../
filelist=`find -type d`
make_clean $filelist
rm -rf extern_lib/*
