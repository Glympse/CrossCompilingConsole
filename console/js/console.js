Application = function() {
}

Application.prototype = {
    init: function(input, output, sync) {
        var that = this;

        this.input = input;
        this.output = output;
        this.update_outut_label();

        this.input_editor = this.setup_editor("input-source", false, this.input);
        $("#input-source").on('keyup change', function() {
            that.translate_code();
        });
        this.output_editor = this.setup_editor("output-source", true, this.output);

        if ( sync ) {
            this.input_editor.getSession().on('changeScrollTop', function(scroll) {
                that.output_editor.getSession().setScrollTop(scroll);
            });
            this.output_editor.getSession().on('changeScrollTop', function(scroll) {
                that.input_editor.getSession().setScrollTop(scroll);
            });
        }

        $("#menu-java").on('click', function() {
            that.output = "java";
            that.update_outut_label();
            that.update_mode(that.output_editor, that.output);
            that.translate_code();
        });
        $("#menu-csharp").on('click', function() {
            that.output = "csharp";
            that.update_outut_label();
            that.update_mode(that.output_editor, that.output);
            that.translate_code();
        });
        $("#menu-objch").on('click', function() {
            that.output = "objch";
            that.update_outut_label();
            that.update_mode(that.output_editor, that.output);
            that.translate_code();
        });
        $("#menu-objcmm").on('click', function() {
            that.output = "objcmm";
            that.update_outut_label();
            that.update_mode(that.output_editor, that.output);
            that.translate_code();
        });
    },

    setup_editor: function(id, readonly, language) {
        var editor = ace.edit($("#" + id).get(0));
        editor.$blockScrolling = Infinity;
        editor.setReadOnly(readonly);
        editor.setPrintMarginColumn(120);
        this.update_mode(editor, language);
        return editor;
    },

    update_outut_label: function() {
        if ( "java" == this.output ) {
            $("#label-output").text("Java");
        } else if ( "csharp" == this.output ) {
            $("#label-output").text("C#");
        } else if ( "objch" == this.output ) {
            $("#label-output").text("Objective-C - Header");
        } else if ( "objcmm" == this.output ) {
            $("#label-output").text("Objective-C - Source");
        }
    },

    update_mode(editor, language) {
        if ( "cpp" == language ) {
            editor.getSession().setMode("ace/mode/c_cpp");
        } else if ( "java" == language ) {
            editor.getSession().setMode("ace/mode/java");
        } else if ( "csharp" == language ) {
            editor.getSession().setMode("ace/mode/csharp");
        } else if ( "objch" == language || "objcmm" == language ) {
            editor.getSession().setMode("ace/mode/objectivec");
        }
    },

    translate_code: function() {
        var that = this;
        $.post("/api/v1/translate?output=" + this.output, this.input_editor.getValue(), function(response) {
            if ( !response ) {
                return;
            }
            that.output_editor.setValue(response);
            that.output_editor.session.selection.clearSelection();
            that.output_editor.getSession().setScrollTop(that.output_editor.getSession().getScrollTop());
        });
    },

    load_snippet: function(name) {
        var that = this;
        $.get("/content/" + name, function(response) {
            that.input_editor.setValue(response);
            that.input_editor.session.selection.clearSelection();
            that.translate_code();
        });
    }
};
