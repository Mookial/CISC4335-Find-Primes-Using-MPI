#!/bin/bash
for i in `ypcat hosts | grep -e websol25 -e websol26  | cut -f2 | cut -f1 -d' ' | sort | uniq`
    do
        echo $i
        ssh -qx $i /bin/date
    done 

