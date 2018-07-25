```
import BaseHTTPServer,SimpleHTTPServer
import SocketServer
import re,sys
import ssl

def htc(m):
    return chr(int(m.group(1),16))

def urldecode(url):
    rex=re.compile('%([0-9a-hA-H][0-9a-hA-H])',re.M)
    return rex.sub(htc,url)

postdata = open("get_SMN_postdata.txt", 'w+')
	
class SETHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def createHTML(self):
        #html = file("index.html", "r")
        html = "<html>hello world</html>"
        for line in html:
            self.wfile.write(line)
            
    def do_GET(self):
        print "\n \n GET \n"
        print self.headers;
        
        self.createHTML()
        self.send_response(200)
        
    def do_POST(self):
        print "\n \n POST \n"
        print self.headers;
        length = int(self.headers.getheader('content-length'))
        qs = self.rfile.read(length)
        url=urldecode(qs)
        print "print body: url=",url
        postdata.write(url+'\n')
        postdata.flush()
        
        self.createHTML()
        self.send_response(200)
        
if __name__ == "__main__":
    
    if len(sys.argv)<2:print "need params,like <http|https> [port]",sys.exit()
    servertype = sys.argv[1]
    try:
        http_port = int(sys.argv[2])
        https_port = int(sys.argv[2])
    except:
        http_port = 11113
        https_port = 4443
    
    if servertype == "http":
        Handler = SETHandler
        httpd = SocketServer.TCPServer(("", http_port), Handler)
        
        
    elif(servertype == "https"):
        httpd = BaseHTTPServer.HTTPServer(('', https_port), SETHandler)
        httpd.socket = ssl.wrap_socket (httpd.socket, certfile='server.pem', server_side=True)
        
    print servertype," server:serving at port", http_port
    httpd.serve_forever()


```

openssl req -new -x509 -keyout server.pem -out server.pem -days 365 -nodes
