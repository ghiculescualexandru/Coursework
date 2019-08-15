#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define NMax 1000

/**
 * Description: This is the main function where I used structures for the 
 * operations to remember the line from which the operations starts and 
 * the line where it ends. In addition, for lists and indenting paragraphs
 * I also held the list type and the special character or the indent lenght.
 **/
int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[2], "r"); //fisierul de input
    FILE *g = fopen(argv[3], "w"); //fisierul de output

    /* A buffer, the original text, the result text and one backup text used
    for errors. */
    char buf[NMax], text[NMax][NMax], original[NMax][NMax], backup[NMax][NMax];
    char result[NMax][NMax], type;

    int i, j, operations = 0, original_lines = 0;
    char *p;

    int W_length;

    /* This structure is used for center, left and right aligning and for justify. */
    struct C_L_R_J
    {
        int start_line;
        int end_line;
    } C_L_R_J;

    /* This structure is used for paragraphs */
    struct P
    {
        int indent_length;
        int start_line;
        int end_line;
    } P;

    /* These structures are used for lists */
    struct I
    {
        char list_type;
        char special_character;
        int start_line;
        int end_line;
    } I;

    struct O
    {
        char list_type;
        char special_character;
        char ordering;
        int start_line;
        int end_line;
    } O;

    /* I used these to check if the start operation line is higher 
    than the end line. */
    int ok1 = 0;
    int ok2 = 0;

    /* This was used for invalid operations. */
    int ok = 0;

    while (fgets(buf, NMax, f))
    {
        strcpy(original[original_lines], buf);
        original_lines++;
    }

    for (i = 0; i <= original_lines; i++)
        strcpy(backup[i], original[i]);

    int lines_backup = original_lines;

    fclose(f);

    strcpy(buf, argv[1]);

    p = strtok(buf, ",");

    while (p)
    {
        strcpy(text[operations++], p);
        p = strtok(NULL, ",");
    }

    if (operations > 10)
    {
        printf("Too many operations! Only the first 10 will be applied.\n");
        operations = 10;
    }

    for (i = 0; i < operations && !ok; i++)
    {
        j = 0;
        while (text[i][j] == ' ')
            strcpy(text[i], text[i] + 1);

        /* Here is the type of the operation. */
        type = text[i][0];

        char valid_operations[] = "WJCIOPLRwjcioplr";

        if (strchr(valid_operations, type) == 0)
        {
            printf("Invalid operation!\n");
            ok = 1;
            break;
        }

        if (type <= 'z' && type >= 'a')
            type -= 32;

        strcpy(text[i], text[i] + 1);
        j = 0;
        while (text[i][j] == ' ')
            strcpy(text[i], text[i] + 1);

        switch (type)
        {
        /* Wrap case. */
        case 'W':
            j = 0;
            W_length = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                W_length = W_length * 10 + (text[i][j] - '0');
                j++;
            }

            if (text[i][j] != '\0')
            {
                printf("Invalid operation!\n");
                ok = 1;
                break;
            }

            if (W_length == 0)
            {
                printf("Invalid operation!\n");
                ok = 1;
                break;
            }

            break;
        /* Center, justify, left and right case. */
        case 'C':
        case 'L':
        case 'R':
        case 'J':
            j = 0;
            ok1 = 0;
            ok2 = 0;
            C_L_R_J.start_line = 0;

            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                ok1 = 1;
                C_L_R_J.start_line = C_L_R_J.start_line * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            j = 0;
            C_L_R_J.end_line = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                ok2 = 1;
                C_L_R_J.end_line = C_L_R_J.end_line * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            if (text[i][j] != '\0')
            {
                printf("Invalid operation!\n");
                ok = 1;
                break;
            }

            if (!ok1)
                C_L_R_J.start_line = 0;
            if (!ok2 || C_L_R_J.end_line > original_lines)
                C_L_R_J.end_line = original_lines - 1;

            /* The case in which start line > end line. */
            if (C_L_R_J.start_line > C_L_R_J.end_line)
            {
                printf("Invalid operation!\n");
                ok = 1;
                break;
            }

            break;

            /* Paragraph case. */
        case 'P':
            j = 0;
            P.indent_length = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                P.indent_length = P.indent_length * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            if (P.indent_length == 0)
            {
                printf("Invalid operation!\n");
                ok = 1;
                break;
            }

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            P.start_line = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                P.start_line = P.start_line * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            P.end_line = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                P.end_line = P.end_line * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            if (P.start_line == 0)
                P.start_line = 0;
            if (P.end_line == 0 || P.end_line > original_lines)
                P.end_line = original_lines - 1;

            break;

        case 'I':

            /* The list type. */
            I.list_type = text[i][j];

            char liste_valide[] = "nAab";
            if (strchr(liste_valide, I.list_type) == 0)
            {
                printf("Invalid operation!\n");
                ok = 1;
                break;
            }

            strcpy(text[i], text[i] + 1);

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            /* The special character. */
            I.special_character = text[i][j];
            strcpy(text[i], text[i] + 1);

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            if (I.special_character == NULL ||
                (text[i][j] < '0' || text[i][j] > '9') &&
                    strlen(text[i] + j) > 1)
            {
                printf("Invalid operation!\n");
                ok = 1;
                break;
            }

            I.start_line = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                I.start_line = I.start_line * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            I.end_line = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                I.end_line = I.end_line * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            if (I.start_line == 0)
                I.start_line = 0;
            if (I.end_line == 0 || I.end_line > original_lines)
                I.end_line = original_lines - 1;

            break;

        case 'O':

            /* The list type. */
            O.list_type = text[i][j];
            strcpy(text[i], text[i] + 1);

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            /* The special character. */
            O.special_character = text[i][j];
            strcpy(text[i], text[i] + 1);

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            /* The ordering type. */
            O.ordering = text[i][j];
            strcpy(text[i], text[i] + 1);

            if (O.special_character == NULL ||
                O.ordering == NULL ||
                (strchr("aAzZ", O.ordering) == 0))
            {
                printf("Invalid operation!\n");
                ok = 1;
            }

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            O.start_line = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                O.start_line = O.start_line * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            while (text[i][j] == ' ')
                strcpy(text[i], text[i] + 1);

            O.end_line = 0;
            while (text[i][j] <= '9' && text[i][j] >= '0')
            {
                O.end_line = O.end_line * 10 + (text[i][j] - '0');
                strcpy(text[i], text[i] + 1);
            }

            if (O.start_line == 0)
                O.start_line = 0;
            if (O.end_line == 0 || O.end_line > original_lines)
                O.end_line = original_lines - 1;

            break;
        }

        if (!ok)

            /* Here I used my functions for every operation. */
            switch (type)
            {
            case 'W':
                original_lines = wrap(original,
                                      result, original_lines, W_length) +
                                 1;
                if (original_lines == -1)
                    ok = 1;
                for (j = 0; j < original_lines; j++)
                    strcpy(original[j], result[j]);
                break;

            case 'C':
                Center(original,
                       C_L_R_J.start_line,
                       C_L_R_J.end_line,
                       original_lines);
                break;

            case 'L':
                Align_Left(original,
                           C_L_R_J.start_line,
                           C_L_R_J.end_line,
                           original_lines);
                break;
            case 'R':
                Align_Right(original,
                            C_L_R_J.start_line,
                            C_L_R_J.end_line,
                            original_lines);
                break;

            case 'J':
                break;

            case 'P':
                Paragraphs(original,
                           P.indent_length,
                           P.start_line,
                           P.end_line);
                break;
            case 'I':
                Lists(original,
                      I.list_type,
                      I.special_character,
                      I.start_line,
                      I.end_line,
                      original_lines);
                break;
            case 'O':
                Ordered_Lists(original,
                              O.list_type,
                              O.special_character,
                              O.ordering,
                              O.start_line,
                              O.end_line,
                              original_lines);
                break;

            default:;
            }
    }

    /* The case where errors occur and I use my backup */
    if (ok)
        for (i = 0; i <= lines_backup; i++)
            fputs(backup[i], g);

    else

        for (i = 0; i < original_lines; i++)
        {
            trim(original[i]);
            fputs(original[i], g);
        }

    fclose(g);

    return 0;
}