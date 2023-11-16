#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIM_MIN 3
#define DIM_MAX 9

#ifndef INT_MAX
    #define INT_MAX 12345678
#endif // INT_MAX

// prototypes
void greet(void);
int** init(int d);
void draw(int** board, int d);
short move(int** board, int d, int tile);
short won(int** board, int d);
int get_int();

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    int d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }
    int** board = init(d);

    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    greet();

    while (1)
    {
        draw(board, d);

        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        if (won(board, d))
        {
            printf("ftw!\n");
            break;
        }

        printf("Tile to move: ");
        int tile = get_int();
        printf("\n");

        if (tile == 0)
        {
            break;
        }

        fprintf(file, "%i\n", tile);
        fflush(file);

        if (!move(board, d, tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }
        usleep(500000);
    }
    fclose(file);

    for (int i = 0; i < d; i++)
    {
        free(board[i]);
    }
    free(board);

    return 0;
}

int** init(int d)
{
    int** board = (int**)malloc(d * sizeof(int*));
    for (int i = 0; i < d; i++)
    {
        board[i] = (int*)malloc(d * sizeof(int));
    }

    int num = d * d - 1;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = num;
            num--;
        }
    }

    if (d % 2 == 0)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp;
    }

    return board;
}