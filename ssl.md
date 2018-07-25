```

证书格式
证书常用的格式有三种：PFX（PKCS12标准，Publick Key Cryptography Standard #12）,PEM(Privacy Enhanced Mail  一种ASCII码的编码格式)，DER（ASN1标准）。
证书链
为验证一个用户(比如客户端验证服务端)的公钥证书，需要获得签发这个用户证书的CA的公钥，才能检查用户证书上的签名。一个公钥证书需要让其上一级CA证明它的身份，验证公钥的过程成为一个叠代过程并形成一个证书链。 例如有以下证书链： 根证书->CA证书->服务端证书。如果需要认证服务端证书，就需要用CA证书，而需要认证CA证书，就必须使用根证书。
根证书
根CA（Certification Authority）是认证体系中第一个证书颁发机构，它是所有信任的起源，是自签名CA（使用自己的私钥为自己签名）。根CA为自己颁发的证书即为根证书。根CA可以为其他CA创建证书，也可以为其他计算机、用户和服务创建证书。
公钥证书
含有公钥的数字证书。为保证公钥证书的正确性，公钥证书往往需要签发CA对其进行签名。公钥证书是可以对外发布的。
私钥文件
用户密钥对中的私钥文件。私钥文件不对外发布，为保护私钥文件的私密性，可以对私钥文件进行加密保护，即只有知道私钥的密码才能正常使用私钥。
服务端证书
包含了服务端信息和服务端公钥证书，用于客户端验证服务端的身份，并使用服务端公钥加密要发送给服务端的信息。
客户端证书
包含了客户端信息和客户端公钥证书，用于服务端验证客户端的身份，并使用客户端公钥加密要发送给客户端的信息。
单向验证
服务端需要加载“CA证书”、“服务端证书”和“服务端的私钥文件”；客户端需要加载“CA证书”。通信过程中仅客户端对服务端进行验证，服务端不对客户端进行验证。
双向验证
服务端需要加载“CA证书”、“服务端证书”和“服务端的私钥文件”；客户端需要加载“CA证书”、“客户端证书”和“客户端的私钥文件”。通信过程中客户端和服务端进行相互的验证。
带有私钥的证书
由Public Key Cryptography Standards #12，PKCS#12标准定义，包含了公钥和私钥的二进制格式的证书形式，以pfx作为证书文件后缀名。
二进制编码的证书
证书中没有私钥，DER编码二进制格式的证书文件，以cer作为证书文件后缀名。
Base64编码的证书
证书中没有私钥，BASE64编码文本格式的证书文件，也是以cer作为证书文件后缀名。
由定义可以看出，只有pfx格式的数字证书是包含有私钥的，cer格式的数字证书里面只有公钥没有私钥。
如果要导出私钥（pfx）,是需要输入密码的，这个密码就是对私钥再次加密，这样就保证了私钥的安全，别人即使拿到了你的证书备份（pfx)，不知道加密私钥的密码，也是无法导入证书的。相反，如果只是导入导出cer格式的证书，是不会提示你输入密码的。因为公钥一般来说是对外公开的，不用加密。

使用OpenSSL生成证书   
首先得安装OpenSSL软件包openssl，安装了这个软件包之后，我们可以做这些事情：
o Creation of RSA, DH and DSA Key Parameters # 创建密钥 key
o Creation of X.509 Certificates, CSRs and CRLs # 创建证书
o Calculation of Message Digests                # 
o Encryption and Decryption with Ciphers # 加密、解密
o SSL/TLS Client and Server Tests        # SSL 服务器端/客户端测试
o Handling of S/MIME signed or encrypted Mail # 处理签名或加密了的邮件
1、生成RSA密钥的方法
openssl genrsa -des3 -out privkey.pem 2048这个命令会生成一个2048位的密钥，同时有一个des3方法加密的密码，如果你不想要每次都输入密码，可以改成：openssl genrsa -out privkey.pem 2048建议用2048位密钥，少于此可能会不安全或很快将不安全。
2、生成一个证书请求
openssl req -new -key privkey.pem -out cert.csr
这个命令将会生成一个证书请求，当然，用到了前面生成的密钥privkey.pem文件
这里将生成一个新的文件cert.csr，即一个证书请求文件，你可以拿着这个文件去数字证书颁发机构（即CA）申请一个数字证书。CA会给你一个新的文件cacert.pem，那才是你的数字证书。
 
如果是自己做测试，那么证书的申请机构和颁发机构都是自己。就可以用下面这个命令来生成证书：
openssl req -new -x509 -key privkey.pem -out cacert.pem -days 1095
这个命令将用上面生成的密钥privkey.pem生成一个数字证书cacert.pem
 
3、使用数字证书和密钥
有了privkey.pem和cacert.pem文件后就可以在自己的程序中使用了，比如做一个加密通讯的服务器

OpenSSL建立自己的CA   
(1) 环境准备
首先，需要准备一个目录放置CA文件，包括颁发的证书和CRL(Certificate Revoke List)。
这里我们选择目录 /opt/myca。
然后我们在/opt/myca下建立两个目录，certs用来保存我们的CA颁发的所有的证书的副本；private用来保存CA证书的私钥匙。
CA的私钥匙很重要，至少需要2048位长度。建议保存在硬件里，或者至少不要放在网络中。
除了生成钥匙，在我们的CA体系中还需要创建三个文件。第一个文件用来跟踪最后一次颁发的证书的序列号，我们把它命名为serial，初始化为01。第二个文件是一个排序数据库，用来跟踪已经颁发的证书。我们把它命名为index.txt，文件内容为空。
$ mkdir /opt/myca
$ cd /opt/myca
$ mkdir certs private
$ chmod g-rwx,o-rwx private
$ echo "01" > serial
$ touch index.txt
第三个文件是OpenSSL的配置文件，创建起来要棘手点。示例如下：
$ vi openssl_ca.cnf
然后添加如下内容：
[ ca ]
default_ca = myca
[ myca ]
dir = /opt/myca
certificate = $dir/rootcert.pem
database = $dir/index.txt
new_certs_dir = $dir/certs
private_key = $dir/private/rootkey.pem
serial = $dir/serial
default_crl_days= 7
default_days = 365
default_md = md5
policy = myca_policy
x509_extensions = certificate_extensions
[ myca_policy ]
commonName = supplied
stateOrProvinceName = supplied
countryName = supplied
emailAddress = supplied
organizationName= supplied
organizationalUnitName = optional
[ certificate_extensions ]
basicConstraints= CA:false
 
[req]
distinguished_name = "ca_distinguished_name"
x509_extensions = ca_extensions
 
[ca_distinguished_name]
commonName = commonName
stateOrProvinceName = stateOrProvinceName
countryName = countryName
emailAddress = emailAddress
organizationName= organization
organizationalUnitName = unitName
 
[ ca_extensions ]
basicConstraints = CA:true
keyUsage = digitalSignature,keyEncipherment,cRLSign,keyCertSign
我们需要告诉OpenSSL配置文件的路径，有两种方法可以达成目的：通过config命令选项；通过环境变量OPENSSL_CONF。这里我们使用config命令选项的方式。
(2) 生成根证书(Root Certificate)
我们需要一个证书来为自己颁发的证书签名，这个证书可从其他CA获取，或者是自签名的根证书。这里我们生成一个自签名的根证书。
首先我们需要往配置文件里面添加一些信息，如下所示，节名和命令行工具的命令req一样。我们把所有必要的信息都写进配置，而不是在命令行输入，这是唯一指定X.509v3扩展的方式，也能让我们对如何创建根证书有个清晰的把握。
$ vi openssl_root.cnf
然后添加如下内容：
[ ca ]
default_ca = myca
[ myca ]
dir = /opt/myca
certificate = $dir/rootcert.pem
database = $dir/index.txt
new_certs_dir = $dir/certs
private_key = $dir/private/rootkey.pem
serial = $dir/serial
default_crl_days= 7
default_days = 365
default_md = md5
policy = myca_policy
x509_extensions = certificate_extensions
[ myca_policy ]
commonName = supplied
stateOrProvinceName = supplied
countryName = supplied
emailAddress = supplied
organizationName= supplied
organizationalUnitName = optional
[ certificate_extensions ]
basicConstraints= CA:false
 
 [ req ]
default_bits = 2048
default_keyfile = /opt/myca/private/rootkey.pem
default_md = sha1
prompt = no
distinguished_name = root_ca_distinguished_name
x509_extensions = root_ca_extensions
 
[ root_ca_distinguished_name ]
commonName = My Test CA
stateOrProvinceName = Beijing
countryName = CN
emailAddress = test@openssl.com.cn
organizationName = Root Certification Authority
 
[ root_ca_extensions ]
basicConstraints = CA:true
keyUsage = digitalSignature,keyEncipherment,cRLSign,keyCertSign
 
万事俱备，我们可以生成根证书了：
$ openssl req -x509 -newkey rsa:2048 -keyout rootkey.pem -config openssl.cnf -out rootcert.pem -outform PEM -days 3650
验证一下我们生成的文件。
$ openssl x509 -in rootcert.pem -text -noout
(3) 给客户颁发证书
在给客户颁发证书之前，需要客户提供证书的基本信息。我们需使用颁发客户端证书的OpenSSL配置文件
颁发客户端证书的命令和我们生成根证书的类似，都是req，不过需要提供一些额外的信息。如下：
$ openssl req -x509 -newkey rsa:2048 -keyout testkey.pem -keyform PEM -config openssl-ca.cnf -out testreq.pem -outform PEM
有两次提示要口令，第一次的口令用来加密私钥匙testkey.pem，第二次口令一般被OpenSSL忽略。
结果生成两个文件：testkey.pem，私钥匙；testreq.pem，请求信息，其中包括公钥匙。
我们来看看testreq.pem都有哪些信息？
$ openssl x509 -in testreq.pem -text -noout
现在，我们可以把testreq.pem提交给我们的CA生成证书了，根据客户端提供的密钥，使用root证书颁发客户端证书：
$ openssl x509 -in testreq.pem -out testcert.pem -CA rootcert.pem -CAkey rootkey.pem -CAcreateserial
有三次提示，一次是问你CA的私钥匙密码，两次是确认，输出的结果就是为客户颁发的证书。
可以通过batch选项取消命令提示，可通过notext选项取消证书的输出显示。
此外，还可以一次给多个客户颁发证书，方法是用 infiles选项替换in选项，不过这个选项必须放在最后，因为此后的任何字符均被处理为文件名称列表。
生成的证书放在certs目录，同时index.txt和serial的内容都发生了改变。
(4) 多级证书颁发
原理和“给客户颁发证书”相同，都是首先要生成自己的证书请求文件:
$ openssl req -x509 -newkey rsa:2048 -keyout ca2key.pem -keyform PEM -config openssl_ca.cnf -out ca2req.pem -outform PEM
然后将请求文件让上一级证书签发:
$ openssl x509 -in ca2req.pem -out ca2cert.pem -CA testcert.pem -CAkey testkey.pem -CAcreateserial
其中，ca2req.pem为证书请求文件名称，-CA testcert.pem -CAkey testkey.pem分别指定签发的CA证书和其私钥，-out后的为生成的设备证书文件名。
(5) 吊销证书
吊销证书的操作很简单，运行ca命令就可以。
$ openssl ca -gencrl -revoke testcert.pem -cert rootcert.pem -keyfile rootkey.pem -config openssl.cnf -out testpem.crl
输入CA私钥匙的密码就可以。
为了使你的吊销操作让别人知道，你可以将生成的CRL(Certificate Revoke List)公布到网上去。由于每个颁发的证书都有一个过期时间，一旦证书过期了，就可以把该证书从CRL中删除。因为这时候在不在CRL中已经无所谓了。
# 查看CRL列表信息
$ openssl crl -in testpem.crl -text -noout
# 验证CRL列表签名信息
$ openssl crl -in testpem.crl -noout -CAfile rootcert.pem
可以看到CRL的版本号为1，这是OpenSSL默认的，除非crl_extensions被指定在配置文件ca一节中。
(6) pem证书链制作
确定好各个证书的角色后，制作证书链
按照如下顺序制作证书链
l  服务器证书
l  中级证书（即二级证书）
l  根证书
$ cat ca2cert.pem testcert.pem rootcert.pem > servercert.pem
私钥使用的是服务器之前使用的密钥
此时服务器证书的私钥制作完成。
(7) 不同的证书格式
不同后缀扩展名的证书文件，可以简单用如下方法区分：
*.DER *.CER : 这样的证书文件是二进制格式，只含有证书信息，不包含私钥。
*.CRT : 这样的文件可以是二进制格式，也可以是文本格式，一般均为文本格式，功能与*.DER/*.CER相同。
*.PEM : 一般是文本格式，可以放证书或私钥，或者两者都包含。 *.PEM如果只包含私钥，那一般用 *.KEY代替。
*.PFX *.P12 是二进制格式，同时含证书和私钥，一般有密码保护。
怎么判断是文本格式还是二进制？
用记事本打开，如果是规则的数字字母，如
—–BEGIN CERTIFICATE—–
MIIE5zCCA8+gAwIBAgIQN+whYc2BgzAogau0dc3PtzANBgkqh......
—–END CERTIFICATE—–
就是文本的，上面的BEGIN CERTIFICATE，说明这是一个证书
 
如果是—–BEGIN RSA PRIVATE KEY—–，说明这是一个私钥
1. 将PEM/KEY转换为PFX
使用OpenSSL工具，可以将密钥文件KEY和证书文件PEM转化为PFX文件。
将密钥文件KEY和证书文件PEM放到OpenSSL目录下，打开OpenSSL执行以下命令：
$ openssl pkcs12 -export -out testcert.pfx -inkey testkey.pem -in testcert.pem
 2. 将PFX转换为PEM/KEY/CRT
使用OpenSSL工具，可以将PFX文件转化为密钥文件KEY和公钥文件CRT。
将PFX文件放到OpenSSL目录下，打开OpenSSL执行以下命令：
$ openssl pkcs12 -in testcer.pfx -nodes -out test.pem 
$ openssl rsa -in test.pem -out test.key
$ openssl x509 -in test.pem -out test.crt 
** 请注意 ** 此步骤是专用于使用keytool生成私钥和CSR申请证书，并且获取到pem格式证书公钥的情况下做分离私钥使用的，所以在实际部署证书时请使用此步骤分离出来的私钥和申请下来的公钥证书做匹配使用。
(8) 不同格式crl互相转换
我们使用到的两种crl格式是PEM和ASN1两种格式，直接生成ASN1的方式可能还需要用到ASN1格式的证书等，再加上我们设备并不支持ASN1格式的证书，因此，这里使用PEM格式crl转换的方式，新一个ASN1格式的crl，满足设备上的使用：
$ openssl crl -in testpem.crl -outform DER -out testasn1.crl
(9) 自签名证书
自签名证书，顾名思义可以理解为自已颁发给自己、自己为自己签名的证书。
官方颁发证书用的root证书就是自签名证书，我们上述生成的根证书，也是root证书，不同之处在于，官方的证书是权威的、大家公认的，而我们自己生成的证书，可以用来测试使用，拿出去使用的话，不会得到大家的认可。

```
