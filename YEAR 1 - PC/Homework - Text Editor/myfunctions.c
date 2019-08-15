#include "myfunctions.h"

/**
 * Description: Comparing function used for ascending qSort.
 **/
int comparator_a(const void *a, const void *b)
{
    return strcmp((char *)a, (char *)b);
}

/**
 * Description: Comparing function used for ascending qSort.
 **/
int comparator_z(const void *a, const void *b)
{
    return -strcmp((char *)a, (char *)b);
}

/**
 * Description: This is a function used to erase the whitespace from the 
 original text
 **/
void trim(char *line)
{
    int i = strlen(line) - 1;

    while (i > 0)
    {
        if (isspace(line[i]))
        {
            line[i] = '\0';
        }
        else
        {
            li ne[i + 1] = '\n';
            break;
        }

        i--;
    }
}

/**
 * Description: Wrapping the text's lines in a given
 number of characters (max_line_length).
 **/
int wrap(
    char original[NMax][NMax],
    char result[NMax][NMax],
    int original_lines,
    int max_length)
{

    int i;
    int final_lines = 0;
    char aux[NMax];
    char *p;

    /* Verifying if Wrap operation is possible. */
    for (i = 0; i < original_lines; i++)
    {
        strcpy(aux, original[i]);
        p = strtok(aux, " ");
        while (p)
        {
            if (max_length < strlen(p))
            {
                /* Displaying error message. */
                printf("Cannot wrap!\n");
                return -2;
            }

            p = strtok(NULL, " ");
        }
    }

    for (i = 0; i < original_lines; i++)
    {
        /* Erase trailing */
        trim(original[i]);
        p = strtok(original[i], " ");

        if (!strcmp(p, "\n"))
        {
            strcpy(result[++final_lines], "\n");
            final_lines++;
            continue;
        }

        while (p)
        {
            if (strchr(p, '\n'))
            {
                p[strlen(p) - 1] = '\0';
            }
            if ((strlen(result[final_lines]) + strlen(p)) <= max_length)
            {
                strcat(result[final_lines], p);
                strcat(result[final_lines], " ");
            }

            else
            {
                strcat(result[final_lines], "\n");
                trim(result[final_lines]);
                final_lines++;
                continue;
            }

            p = strtok(NULL, " ");
        }
    }

    return final_lines;
}

/**
 * Description: Aligning the text's lines to left
 **/
void Align_Left(
    char text[NMax][NMax],
    int start_line,
    int end_line,
    int lines)
{
    int i, spaces_cnt;
    char aux[NMax];

    for (i = start_line; i <= end_line; i++)
    {
        strcpy(aux, text[i]);
        spaces_cnt = 0;

        /* Counting the spaces before. */
        while (text[i][spaces_cnt] == ' ')
            spaces_cnt++;

        /* Erase the spaces before. */
        strcpy(aux, text[i] + spaces_cnt);
        strcpy(text[i], aux);
    }
}

/**
 * Description: Aligning the text's lines to right
 **/
void Align_Right(
    char text[NMax][NMax],
    int start_line,
    int end_line,
    int lines)
{
    int i, j, max_length = -1, dif, n;
    char aux[NMax]; /* Buffer. */

    /* Finding the max length of the text.*/
    for (i = 0; i < lines; i++)
    {
        n = strlen(text[i]);
        if (n > max_length)
            max_length = n;
    }

    for (i = start_line; i <= end_line; i++)
    {
        if (!strcmp(text[i], "\n"))
            continue;

        dif = max_length - strlen(text[i]);
        strcpy(aux, text[i]);

        if (dif == 0)
            continue;

        strcpy(text[i], " ");

        for (j = 0; j < dif - 1; j++)
            strcat(text[i], " ");

        strcat(text[i], aux);
    }
}

/**
 * Description: Indenting the paragraphs.
 **/
void Paragraphs(
    char text[NMax][NMax],
    int indent_length,
    int start_line,
    int end_line)
{
    int i, j;
    char aux[NMax]; /* Buffer. */

    for (i = start_line; i <= end_line; i++)
        if ((strcmp(text[i - 1], "\n") == 0 &&
             strcmp(text[i], "\n")) ||
            (i == 0 && strcmp(text[i], "\n")))
        {
            strcpy(aux, text[i]);
            strcpy(text[i], " ");

            for (j = 1; j < indent_length; j++)
                strcat(text[i], " ");

            strcat(text[i], aux);
        }
}

/**
 * Description: Formatting the text lines into an unordered list.
 **/
void Lists(
    char text[NMax][NMax],
    char list_type,
    char special_character,
    int start_line,
    int end_line,
    int lines)
{
    int i;
    char aux[NMax]; /* Buffer. */

    /* Aligning the text to left and erasing the trailing whitspaces. */
    Align_Left(text, start_line, end_line, 1000);
    trim(text[lines]);

    /* Numerical lists. */
    if (list_type == 'n')
    {
        int cnt = 1;
        for (i = start_line; i <= end_line; i++)
        {
            strcpy(aux, text[i]);
            sprintf(text[i], "%d%c ", cnt++, special_character);
            strcat(text[i], aux);
        }
    }

    /* For a) or A) like lists. */
    if (list_type == 'a')
    {
        char cnt = 'a';
        for (i = start_line; i <= end_line; i++)
        {
            strcpy(aux, text[i]);
            sprintf(text[i], "%c%c ", cnt++, special_character);
            strcat(text[i], aux);
        }
    }

    if (list_type == 'A')
    {
        char cnt = 'A';
        for (i = start_line; i <= end_line; i++)
        {
            strcpy(aux, text[i]);
            sprintf(text[i], "%c%c ", cnt++, special_character);
            strcat(text[i], aux);
        }
    }

    /* For bullet lists */
    if (list_type == 'b')
        for (i = start_line; i <= end_line; i++)
        {
            strcpy(aux, text[i]);

            sprintf(text[i], "%c ", special_character);
            strcat(text[i], aux);
        }
}

/**
 * Description: Formatting the text lines into an ordered list.
 **/
void Ordered_Lists(
    char text[NMax][NMax],
    char list_type,
    char special_character,
    char ordering,
    int start_line,
    int end_line,
    int lines)
{
    trim(text[lines - 1]);
    int n = end_line - start_line + 1;

    if (ordering == 'a')
    {
        /* Using qSort for alphabetical ordering */
        qsort(text[start_line], n, sizeof(text[0]), comparator_a);
        Lists(text, list_type, special_character, start_line, end_line, lines);
    }

    if (ordering == 'z')
    {
        /* Using qSort for reversed alphabetical ordering */
        qsort(text[start_line], n, sizeof(text[0]), comparator_z);
        Lists(text, list_type, special_character, start_line, end_line, lines);
    }
}

/**
 * Description: Centering the text original lines.
 **/
void Center(
    char text[NMax][NMax],
    int start_line,
    int end_line,
    int lines)
{
    int i, j, max_length = -1, dif, n;
    char aux[NMax]; /* Buffer. */

    /* Finding the max length of the text.*/
    for (i = 0; i <= lines; i++)
    {
        n = strlen(text[i]);
        if (n > max_length)
            max_length = n;
    }

    for (i = start_line; i <= end_line; i++)
    {
        if (!strcmp(text[i], "\n"))
            continue;

        dif = max_length - strlen(text[i]);
        strcpy(aux, text[i]);

        if (dif == 0)
            continue;

        strcpy(text[i], "");

        if (dif % 2 != 0)

            strcpy(text[i], " ");

        for (j = 0; j < dif / 2; j++)
            strcat(text[i], " ");

        strcat(text[i], aux);
    }
}