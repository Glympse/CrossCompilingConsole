import uuid
import os
import sys
import json
import tornado.web
import utilities

sys.path.append(utilities.File.root() + "/../translator/tools")

import translator.tools.cpp2java
import translator.tools.cpp2csharp
import translator.tools.java2objc


class Config:

    def __init__(self, filename):
        config_file = utilities.File.root() + "/content/" + filename
        self.data = json.loads(utilities.File.read(config_file))
        self.package = self.data["packages"][0]


class TranslateHandler(tornado.web.RequestHandler):

    __DEFAULT_OUTPUT = "java"

    __FORMATS = {
        "java": {
            "factory": translator.tools.cpp2java.Factory(),
            "config": "java.json",
            "extension": ""
        },
        "csharp": {
            "factory": translator.tools.cpp2csharp.Factory(),
            "config": "csharp.json",
            "extension": ""
        },
        "objch": {
            "factory": translator.tools.java2objc.Factory(),
            "config": "objc.json",
            "extension": "h"
        },
        "objcmm": {
            "factory": translator.tools.java2objc.Factory(),
            "config": "objc.json",
            "extension": "mm"
        }
    }

    def post(self):
        output_name = self.get_argument("output", TranslateHandler.__DEFAULT_OUTPUT)
        output = TranslateHandler.__FORMATS[output_name]

        # Save request body to a file. Translator operates with files currently.
        id = str(uuid.uuid1())
        src_filename = "./" + id + ".dat"
        dst_filename = "./" + id + "."
        utilities.File.write(src_filename, self.request.body)

        # Load config
        config = Config(output["config"])

        # Translate the code
        translator = output["factory"].translator()
        translator.translate(src_filename, dst_filename, config, config.package)

        # Read the result
        try:
            result = utilities.File.read("{}{}".format(dst_filename, output["extension"]))
        except Exception as e:
            result = ""

        # Do some cleanup
        for file in os.listdir("./"):
            if file.startswith(id):
                os.remove("./" + file)

        # Respond
        self.set_header("Access-Control-Allow-Origin", "*")
        self.write(result)
        self.finish()


app = tornado.web.Application([
    (r"/api/v1/translate", TranslateHandler),
    (r"/(.*)", tornado.web.StaticFileHandler, {
        "path": utilities.File.root() + "/../console/",
        "default_filename": "cpp2x.htm"
    }),
])

server = utilities.TornadoServer(app, 8080)
server.start()
