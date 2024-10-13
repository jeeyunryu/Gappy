#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_input(int row_value[], int col_value[], int x, int y);
void print_output(char gappy[][9], int x, int y);

const int W = 9;
const int H = 9;
int main()
{
    int x = 9;
    int y = 9;

    int i, j, k, l;

    char gappy[9][9];
    int row_value[9];
    int col_value[9];

    get_input(row_value, col_value, x, y);

    FILE *fp = fopen("formula", "w");

    for (i = 1; i <= y; i++)
    {
        for (j = 1; j <= x; j++)
        {
            fprintf(fp, "(declare-const p%d%d Bool)\n", i, j);
        }
    }
    fprintf(fp, "\n");

    // There must be exactly tow black cells in each row
    fprintf(fp, "; S0\n");
    fprintf(fp, "(assert ");
    fprintf(fp, "(and ");
    for (i = 1; i <= y; i++)
    {
        int k = row_value[i - 1];
        fprintf(fp, "(or ");
        for (j = 1; j <= x - k - 1; j++)
        {
            fprintf(fp, "(and ");
            fprintf(fp, "(not (or ");
            for (l = 1; l <= x; l++)
            {
                if (l != j && l != j + k + 1)
                {
                    fprintf(fp, "p%d%d ", i, l);
                }
            }
            fprintf(fp, ")) ");
            fprintf(fp, "(and ");
            fprintf(fp, "p%d%d ", i, j);
            fprintf(fp, "p%d%d", i, j + k + 1);
            fprintf(fp, "))");
        }
        fprintf(fp, ")\n");
    }
    fprintf(fp, "))\n");

    // There must be exactly tow black cells in each column
    fprintf(fp, "; S1\n");
    fprintf(fp, "(assert ");
    fprintf(fp, "(and ");
    for (j = 1; j <= x; j++)
    {
        int k = col_value[j - 1];
        fprintf(fp, "(or ");
        for (i = 1; i <= y - k - 1; i++)
        {
            fprintf(fp, "(and ");
            fprintf(fp, "(not (or ");
            for (l = 1; l <= y; l++)
            {
                if (l != i && l != i + k + 1)
                {
                    fprintf(fp, "p%d%d ", l, j);
                }
            }
            fprintf(fp, ")) ");
            fprintf(fp, "(and ");
            fprintf(fp, "p%d%d ", i, j);
            fprintf(fp, "p%d%d", i + k + 1, j);
            fprintf(fp, ")) ");
        }
        fprintf(fp, ")\n");
    }
    fprintf(fp, "))\n");

    // No two black cells are adjacent to each other diagonally(1) "\"
    fprintf(fp, "; S2\n");
    fprintf(fp, "(assert ");
    fprintf(fp, "(and ");
    for (i = 1; i < y; i++)
    {
        for (j = 1; j < x; j++)
        {
            fprintf(fp, "(not (and ");
            fprintf(fp, "p%d%d ", i, j);
            fprintf(fp, "p%d%d", i + 1, j + 1);
            fprintf(fp, ")) ");
        }
    }
    fprintf(fp, "))\n");

    // No two black cells are adjacent to each other diagonally(2) "/"
    fprintf(fp, "; S3\n");
    fprintf(fp, "(assert ");
    fprintf(fp, "(and ");
    for (i = 1; i < y; i++)
    {
        for (j = 2; j <= x; j++)
        {
            fprintf(fp, "(not (and ");
            fprintf(fp, "p%d%d ", i, j);
            fprintf(fp, "p%d%d", i + 1, j - 1);
            fprintf(fp, ")) ");
        }
    }
    fprintf(fp, "))");

    fprintf(fp, "\n(check-sat)\n(get-model)\n");

    fclose(fp);

    FILE *fin = popen("z3 formula", "r");
    char buf[128];
    fscanf(fin, "%s %s", buf, buf);
    char temp[128];
    while (!feof(fin))
    {
        temp[0] = '\0';

        fscanf(fin, "%s", buf); // printf("%s ", buf) ;
        fscanf(fin, "%s", buf); // printf("%s ", buf) ;
        strcpy(temp, buf);

        fscanf(fin, "%s", buf); // printf("%s ", buf) ;
        fscanf(fin, "%s", buf); // printf("%s ", buf) ;
        fscanf(fin, "%s", buf); // printf("%s\n", buf) ;

        if (strcmp(buf, "true)") == 0)
        {
            int x = temp[1] - '0';
            int y = temp[2] - '0';

            gappy[x - 1][y - 1] = 'B';
        }
        else
        {
            int x = temp[1] - '0';
            int y = temp[2] - '0';

            gappy[x - 1][y - 1] = 'W';
        }
    }

    print_output(gappy, x, y);

    pclose(fin);

    return 0;
}

void get_input(int row_value[], int col_value[], int x, int y)
{
    for (int i = 0; i < y; i++)
    {
        scanf("%d", &row_value[i]);
    }

    for (int j = 0; j < x; j++)
    {
        scanf("%d", &col_value[j]);
    }
}

void print_output(char gappy[][9], int x, int y)
{
    printf("\n");

    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            if (j != x - 1)
            {
                printf("%c ", gappy[i][j]);
            }
            else
            {
                printf("%c", gappy[i][j]);
            }
        }
        printf("\n");
    }
}
