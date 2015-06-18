import uuid
import os
import tornado.web
import utilities

import translator.tools.cpp2java

class ConsoleIndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render(utilities.File.root() + "/../console/index.htm")

class Config:
    pass

class TranslateHandler(tornado.web.RequestHandler):
    def post(self):
        # Save request body to a file. Translator operates with files currently.
        src_filename = "./" + str(uuid.uuid1()) + ".cpp"
        dst_filename = "./" + str(uuid.uuid1()) + ".java"
        utilities.File.write(src_filename, self.request.body)

        # Hardcode config for now
        config = Config()
        config.data = {
            "dependencies": [
                "java.lang.*",
                "java.util.*",
                "java.io.*"
            ]
        }

        # Translate the code
        tr = translator.tools.cpp2java.CppToJava()
        tr.translate(src_filename, dst_filename, config)

        # Read the result
        result = utilities.File.read(dst_filename)

        # Do some cleanup
        os.remove(src_filename)
        os.remove(dst_filename)

        # Respond
        self.set_header("Access-Control-Allow-Origin", "*")
        self.write(result)
        self.finish()

app = tornado.web.Application([
    (r'/api/v1/translate', TranslateHandler),
    (r'/', ConsoleIndexHandler),
    (r'/(.*)', tornado.web.StaticFileHandler, {
        'path': utilities.File.root() + "/../console/"
    }),
])

if __name__ == '__main__':    
    server = utilities.TornadoServer(app, 8080)
    server.start()
