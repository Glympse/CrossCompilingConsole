import os
import signal
import tornado.httpserver
import tornado.ioloop

class File:
    @staticmethod
    def root():
        return os.path.dirname(os.path.realpath(__file__))

    @staticmethod
    def read(name):
        file = open(name, "r")
        content = file.read()
        file.close()
        return content

    @staticmethod
    def write(name, content):
        file = open(name, "w")
        file.write(content)
        file.close()

class TornadoServer:
    def __init__(self, app, port):
        self.app = app
        self.port = int(os.environ.get("PORT", port))

    def handler(self, signum, frame):
        tornado.ioloop.IOLoop.instance().add_callback(self.shutdown)

    def start(self):
        signal.signal(signal.SIGTERM, self.handler)
        signal.signal(signal.SIGINT, self.handler)

        self.server = tornado.httpserver.HTTPServer(self.app)
        self.server.listen(self.port)
        tornado.ioloop.IOLoop.instance().start()

    def shutdown(self):    
        self.server.stop()
        tornado.ioloop.IOLoop.instance().stop()
