import uuid
import os
import json
import tornado.web
import utilities

import translator.tools.cpp2java
import translator.tools.cpp2csharp

class Config:
    def __init__(self, output):
        config_file = utilities.File.root() + "/content/" + output + ".json"
        self.data = json.loads(utilities.File.read(config_file))

class ConsoleIndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render(utilities.File.root() + "/../console/index.htm")

class TranslateHandler(tornado.web.RequestHandler):
    TRANSLATORS = {
        "java": translator.tools.cpp2java.CppFactory(),
        "csharp": translator.tools.cpp2csharp.CSharpFactory()
    }

    def post(self):
        output = self.get_argument("output", "java")

        # Save request body to a file. Translator operates with files currently.
        src_filename = "./" + str(uuid.uuid1()) + ".cpp"
        dst_filename = "./" + str(uuid.uuid1()) + ".java"
        utilities.File.write(src_filename, self.request.body)

        # Load config
        config = Config(output)

        # Translate the code
        tr = TranslateHandler.TRANSLATORS[output].translator()
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
