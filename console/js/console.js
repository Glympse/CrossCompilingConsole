Application = function() {
}

Application.prototype = {
    init: function() {
        var that = this;

        this.cpp_editor = this.setup_editor("cpp-source", false);
        this.cpp_editor.getSession().setMode("ace/mode/c_cpp");
        this.cpp_editor.getSession().on('changeScrollTop', function(scroll) {
            that.output_editor.getSession().setScrollTop(scroll);
        });

        this.output_editor = this.setup_editor("output-source", true);
        this.output_editor.getSession().setMode("ace/mode/java");
        this.output_editor.getSession().on('changeScrollTop', function(scroll) {
            that.cpp_editor.getSession().setScrollTop(scroll);
        });

        $("#cpp-source").on('keyup change', function() {
            that.translate_code();
        });

        this.output = "java";
        $("#menu-java").on('click', function() {
            $("#label-output").text("Java");
            that.output_editor.getSession().setMode("ace/mode/java");

            that.translate_code("java");
        });
        $("#menu-csharp").on('click', function() {
            $("#label-output").text("C#");
            that.output_editor.getSession().setMode("ace/mode/csharp");

            that.translate_code("csharp");
        });
    },

    setup_editor: function(id, readonly) {
        var editor = ace.edit($("#" + id).get(0));
        editor.$blockScrolling = Infinity;
        editor.setReadOnly(readonly);
        editor.setPrintMarginColumn(120);
        return editor;
    },

    translate_code: function(output) {
        if ( output ) {
            this.output = output;
        }
        var that = this;
        $.post("/api/v1/translate?output=" + this.output, this.cpp_editor.getValue(), function(response) {
            that.output_editor.setValue(response);
            that.output_editor.session.selection.clearSelection();
            that.output_editor.getSession().setScrollTop(that.output_editor.getSession().getScrollTop());
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
