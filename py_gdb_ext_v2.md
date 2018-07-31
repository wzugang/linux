```

# Python gdb extensions for python stack viewing

set confirm off
def showframe
    if $arg0 < 0
        she echo "Usage: showframe frame_object_address"
        return
    end
    set $frame_obj=(long*)$arg0
    set $frame_no=0
    while $frame_obj != 0
        set $code_obj=(long*)(*($frame_obj+4))
        set $file_name=(char*)(*($code_obj+10))
        set $func_name=(char*)(*($code_obj+11))
        
        set $trace_obj=(long*)(*($frame_obj+10))
        if $trace_obj != 0
            set $lineno = *(((int*)$frame_obj)+31)
        else
            set $lastI  = *(((int*)$frame_obj)+30)
            set $lnotab = (long*)(*($code_obj+13))
            set $lnobuf = ((char*)$lnotab)+36
            set $lineno = *((int*)$code_obj+24)
            set $bufsize = *($lnotab+2)/2
            set $curaddr = (int)0
            while $bufsize > 0
                set $curaddr = $curaddr + (int)(*$lnobuf)
                set $lnobuf = $lnobuf+1
                if $curaddr > $lastI
                    loop_break
                end
                set $lineno = $lineno + (int)(*$lnobuf)
                set $lnobuf = $lnobuf+1
                set $bufsize = $bufsize - 1
            end
        end
        printf "    #%d %s:%d in %s\n", $frame_no, $file_name+36, $lineno, $func_name+36
        set $frame_obj=(long*)(*($frame_obj+3))
        set $frame_no=$frame_no+1
    end
end

def pystacks
    set $interp=(long*)$arg0
    while $interp != 0
        set $thread_state=(long*)(*($interp+1))
        while $thread_state != 0
            set $frame=(long*)(*($thread_state+2))
            set $id=*($thread_state+18)
            printf "\nThread:%ld, frame: 0x%lx\n", $id, $frame
            showframe $frame
            set $thread_state=(long*)(*$thread_state)
        end
        set $interp=(long*)(*$interp)
    end
end

define build_pystack_7
    set confirm off
    set logging off
    set logging file /tmp/disas__PyThread_CurrentFrames.txt
    set logging overwrite on
    set logging redirect on
    set logging on
    #for gdb 6.x
    # disas _PyThread_CurrentFrames _PyThread_CurrentFrames+80
    #for gdb 7.x
    disas _PyThread_CurrentFrames,_PyThread_CurrentFrames+80
    set logging off
    set logging redirect off
    set logging overwrite off
    # r14 for python 2.7 and r13 for python 2.6
    she grep '(%rip),%r14' /tmp/disas__PyThread_CurrentFrames.txt|cut -f2 -d'#'|awk '{print "def showpystacks\n    pystacks "$1"\nend\n"}' > /tmp/build_pystacks_cmd.txt
    source /tmp/build_pystacks_cmd.txt
    #she rm -f /tmp/build_pystacks_cmd.txt /tmp/disas__PyThread_CurrentFrames.txt
end

define build_pystack_6
    set confirm off
    set logging off
    set logging file /tmp/disas__PyThread_CurrentFrames.txt
    set logging overwrite on
    set logging redirect on
    set logging on
    #for gdb 6.x
    disas _PyThread_CurrentFrames _PyThread_CurrentFrames+80
    #for gdb 7.x
    # disas _PyThread_CurrentFrames,_PyThread_CurrentFrames+80
    set logging off
    set logging redirect off
    set logging overwrite off
    # r14 for python 2.7 and r13 for python 2.6
    she grep '(%rip),%r14' /tmp/disas__PyThread_CurrentFrames.txt|cut -f2 -d'#'|awk '{print "def showpystacks\n    pystacks "$1"\nend\n"}' > /tmp/build_pystacks_cmd.txt
    source /tmp/build_pystacks_cmd.txt
    she rm -f /tmp/build_pystacks_cmd.txt /tmp/disas__PyThread_CurrentFrames.txt
end

# the following two functions are used for debuging
def showframeobj
    set $frame_obj=(long*)$arg0
    if $frame_obj == 0
        return
    end

    set $f_back=(long*)(*($frame_obj+3))
    set $f_code=(long*)(*($frame_obj+4))
    set $f_builtins=(long*)(*($frame_obj+5))
    set $f_globals=(long*)(*($frame_obj+6))
    set $f_locals=(long*)(*($frame_obj+7))
    set $f_valuestack=(long*)(*($frame_obj+8))
    set $f_stacktop=(long*)(*($frame_obj+9))
    set $f_trace=(long*)(*($frame_obj+10))
    set $f_exc_type=(long*)(*($frame_obj+11))
    set $f_exc_value=(long*)(*($frame_obj+12))
    set $f_exc_traceback=(long*)(*($frame_obj+13))
    set $f_tstate=(long*)(*($frame_obj+14))
    set $f_lasti=*((int*)($frame_obj+15))

    printf "frame: 0x%x {\
        \n\t f_back: 0x%x \
        \n\t f_code: 0x%x \
        \n\t f_builtins: 0x%x \
        \n\t f_globals: 0x%x \
        \n\t f_locals: 0x%x \
        \n\t f_valuestack: 0x%x \
        \n\t f_stacktop: 0x%x \
        \n\t f_trace: 0x%x \
        \n\t f_exc_type: 0x%x \
        \n\t f_exc_value: 0x%x \
        \n\t f_exc_traceback: 0x%x \
        \n\t f_tstate: 0x%x \
        \n\t f_lasti: 0x%x\n}\n", \
        $frame_obj, \
        $f_back, \
        $f_code, \
        $f_builtins, \
        $f_globals, \
        $f_locals, \
        $f_valuestack, \
        $f_stacktop, \
        $f_trace, \
        $f_exc_type, \
        $f_exc_value, \
        $f_exc_traceback, \
        $f_tstate, \
        $f_lasti
end

def showcode_obj
    set $code_obj=(long*)$arg0
    if $code_obj == 0
        return
    end

    set $co_argcount=*((int*)$code_obj+4)
    set $co_nlocals=*((int*)$code_obj+5)
    set $co_stacksize=*((int*)$code_obj+6)
    set $co_flags=*((int*)$code_obj+7)
    
    set $co_code=(long*)(*($code_obj+4))
    set $co_consts=(long*)(*($code_obj+5))
    set $co_names=(long*)(*($code_obj+6))
    set $co_varnames=(long*)(*($code_obj+7))
    set $co_freevars=(long*)(*($code_obj+8))
    set $co_cellvars=(long*)(*($code_obj+9))
    set $co_filename=(long*)(*($code_obj+10))
    set $co_name=(long*)(*($code_obj+11))
    set $co_firstlineno=*((int*)($code_obj+12))
    set $co_lnotab=(long*)(*($code_obj+13))

    printf "code: 0x%lx {\
        \n\t co_argcount: %d \
        \n\t co_nlocals: %d \
        \n\t co_stacksize: %d \
        \n\t co_flags: %d \
        \n\t co_code: 0x%lx \
        \n\t co_consts: 0x%lx \
        \n\t co_names: 0x%lx \
        \n\t co_varnames: 0x%lx \
        \n\t co_freevars: 0x%lx \
        \n\t co_cellvars: 0x%lx \
        \n\t co_filename: 0x%lx \
        \n\t co_name: 0x%lx \
        \n\t co_firstlineno: %d \
        \n\t co_lnotab: 0x%lx\n}\n", \
        $code_obj, \
        $co_argcount, \
        $co_nlocals, \
        $co_stacksize, \
        $co_flags, \
        $co_code, \
        $co_consts, \
        $co_names, \
        $co_varnames, \
        $co_freevars, \
        $co_cellvars, \
        $co_filename, \
        $co_name, \
        $co_firstlineno, \
        $co_lnotab
end



```
