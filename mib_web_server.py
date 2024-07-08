'''
Implementação de um servidor web para o gerenciamento de dispositivos SNMP

Objetos:
    - MibWebServer: Servidor web para o gerenciamento de dispositivos SNMP

Informações gerais do servidor:
1.Nome do servidor (string, read-only)
    - Versão do software do servidor (string, read-only)
    - Tempo de atividade (uptime) (integer, read-only)
2. Métricas de desempenho:
    - Uso de CPU (integer, read-only)
    - Uso de memória (integer, read-only)
    - Uso de disco (integer, read-only)
    - Número de requisições por segundo (integer, read-only)
    - Tempo médio de resposta (integer, read-only)
3. Configurações do servidor:
    - Porta HTTP (integer, read-write)
    - Porta HTTPS (integer, read-write)
    - Diretório raiz do servidor (string, read-write)
    - Tamanho máximo de upload (integer, read-write)
4. Tabela de URLs mais acessados:
    - URL (string, read-only)
    - Contador de acessos (integer, read-only)
    - Último acesso (timestamp, read-only)
'''

from http.server import BaseHTTPRequestHandler, HTTPServer

class MibWebServer(HTTPServer):
    def __init__(self, server_address, RequestHandlerClass):
        super().__init__(server_address, RequestHandlerClass)
        self.server_name = 'MIB Web Server'
        self.server_version = '1.0'
        self.uptime = 0
        self.cpu_usage = 0
        self.mem_usage = 0
        self.disk_usage = 0
        self.requests_per_second = 0
        self.avg_response_time = 0
        self.http_port = 80
        self.https_port = 443
        self.root_dir = '/'
        self.max_upload_size = 1024
        self.urls = {}
        self.urls['/'] = {'counter': 0, 'last_access': 0}
        self.urls['/index.html'] = {'counter': 0, 'last_access': 0}

class MibWebServerHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(b'Hello, world!')

if __name__ == '__main__':
    server_address = ('', 8080)
    server = MibWebServer(server_address, MibWebServerHandler)
    server.serve_forever()

