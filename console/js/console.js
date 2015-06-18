Application = function() {
}

Application.prototype = {
    init: function() {
        var that = this;

        this.cpp_editor = this.setup_editor("cpp-source", false);
        this.cpp_editor.getSession().setMode("ace/mode/c_cpp");
        this.cpp_editor.getSession().on('changeScrollTop', function(scroll) {
            that.java_editor.getSession().setScrollTop(scroll);
        });

        this.java_editor = this.setup_editor("java-result", true);
        this.java_editor.getSession().setMode("ace/mode/java");
        this.java_editor.getSession().on('changeScrollTop', function(scroll) {
            that.cpp_editor.getSession().setScrollTop(scroll);
        });

        $("#cpp-source").on('keyup change', function() {
            that.translate_code();
        });

        this.output = "java";
        $("#menu-java").on('click', function() {
            that.output = "java";
            $("#label-output").text("Java");

            that.translate_code();
        });
        $("#menu-csharp").on('click', function() {
            that.output = "csharp";
            $("#label-output").text("C#");

            that.translate_code();
        });
    },

    setup_editor: function(id, readonly) {
        var editor = ace.edit($("#" + id).get(0));
        editor.$blockScrolling = Infinity;
        editor.setReadOnly(readonly);
        editor.setPrintMarginColumn(120);
        return editor;
    },

    translate_code: function() {
        var that = this;
        $.post("/api/v1/translate?output=" + this.output, this.cpp_editor.getValue(), function(response) {
            that.java_editor.setValue(response);
            that.java_editor.session.selection.clearSelection();
            that.java_editor.getSession().setScrollTop(that.java_editor.getSession().getScrollTop());
        });
    },

    load_snippet: function(name) {
        var that = this;
        $.get("/content/" + name, function(response) {
            that.cpp_editor.setValue(response);
            that.cpp_editor.session.selection.clearSelection();
            that.translate_code();
        });
    }
};

var app = new Application();
app.init();
