```

#!/bin/bash

help()
{
    echo """Usage:
    ./sfind.sh dir suffix cmd str1 str2 str3 ...
Options:
    -o search if one of str is exists.
    -a search the file that have all strs."""
    exit 1
}

DIR=$1
CMD=$2
NAME=$3
STR=$4
STRS=$*
ARGN=$#
OLD_IFS="$IFS" 
IFS=" "
ARR=($STRS)
IFS=OLD_IFS


[ $# -lt 4 ] && help
! [ -d "${DIR}" ] && echo "${DIR} not exists." && exit 
[ "$CMD" != "-a" ] && [ "$CMD" != "-o" ] && help

unset ARR[0]
unset ARR[1]
unset ARR[2]

if_have_all()
{
i=3
OBJ=$1
[ "${OBJ}" = "" ] && return
for str in ${ARR[@]} ; do
    RES=$(grep ${str} ${OBJ})
    if [ "$RES" = "" ] ;then
        break
    fi
    let i=$i+1
    if [ ${i} -ge ${ARGN} ] ;then
        echo "${OBJ}"
    fi
done
}

if_have()
{
OBJ=$1
[ "${OBJ}" = "" ] && return
for str in ${ARR[@]} ;do
    RES=$(grep ${str} ${OBJ})
    if [ "$RES" != "" ]; then
        echo "${OBJ}"
        break
    fi
done
}


OBJS=`find $DIR -name "*${NAME}"|xargs echo`
OLD_IFS="$IFS" 
IFS=" "
ARRS=($OBJS)
IFS=OLD_IFS

find_and()
{
for file in ${ARRS[@]} ;do
    if_have_all $file
done
}

find_or()
{
for file in ${ARRS[@]} ;do
    if_have $file
done
}

[ "$CMD" = "-a" ] && find_and
[ "$CMD" = "-o" ] && find_or

```
