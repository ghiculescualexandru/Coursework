# Homework - Text Editor #

The main idea of this homework was creating a text editor, which is given 3 arguments: the name of the input file and output file and a string having the operations to be be applied to the text.

The operations: 

* wrap text: "W max_line_length";
* center text: "C start_line [end_line]";
* allign left: "L start_line [end_line]";
* allign right: "R start_line [end_line]";
* indent paragraphs: "P indent_length start_line [end_line]";
* format lines in list: "I list_type special_character start_line [end_line]", where list_type can be 'n'(numbered list), 'a/A'(alphabetic list with lowercase/uppercase characters) or 'b'(bullet list);
* format lines in ordered list: "O list_type special_character ordering start_line [end_line]", where ordering is either 'a' for normal alphabetic list or 'z' for reversed alphabetic list.

Main files: 

* **main.c** containing the main code where the text and commands are processed;
* **myfunctions.c** containing the functions used for the operations;
* **myfunctions.h** the header for *myfunctions.c*;
* **MAKEFILE**.
