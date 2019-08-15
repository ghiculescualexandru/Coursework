#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define NMax 1000

int comparator_a(const void *a, const void *b);
int comparator_z(const void *a, const void *b);
void trim(char *line);
int wrap(
    char original[NMax][NMax],
    char result[NMax][NMax],
    int original_lines,
    int max_length);
void Align_Left(
    char text[NMax][NMax],
    int start_line,
    int end_line,
    int lines);
void Align_Right(
    char text[NMax][NMax],
    int start_line,
    int end_line,
    int lines);
void Paragraphs(
    char text[NMax][NMax],
    int indent_length,
    int start_line,
    int end_line);
void Lists(
    char text[NMax][NMax],
    char list_type,
    char special_character,
    int start_line,
    int end_line,
    int lines);
void Ordered_Lists(
    char text[NMax][NMax],
    char list_type,
    char special_character,
    char ordering,
    int start_line,
    int end_line,
    int lines);
void Center(
    char text[NMax][NMax],
    int start_line,
    int end_line,
    int lines);