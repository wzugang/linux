```

#!/bin/bash

if [ $# -ne 1 ];then
    echo "Usage: $0 pid"
    exit 1
fi

pid=$1

gdb_path=`which gdb 2>/dev/null`
if [ -z "$gdb_path" ];then
    echo "gdb not found."
    exit 1
fi

disas_help=`gdb -ex "help disas" -ex "quit"|grep "Two arg"`
if [ -z "$disas_help" ];then
    echo "can't determine disassemble syntax."
    exit 1
fi
comma_needed=`echo "$disas_help"|grep "separated by a comma"`
if [ -z "$comma_needed" ];then
    cmd=build_pystack_6
else
    cmd=build_pystack_7
fi

gdb --quiet -nx -p $pid <<EOF 2>&1 |
source py_gdb_ext_v2
$cmd
showpystacks
set confirm off
quit
EOF
/bin/sed -n \
    -e 's/^\((gdb) \)*//' \
    -e '/^    #/p' \
    -e '/^Thread/p'

```
