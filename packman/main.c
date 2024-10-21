#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
int x = 15;
int y = 16;
int xG = 3;
int yG = 3;
int point = 0;
char ch = ' ';
char map[18][32];
void Ghost()
{
    int Xtemp = 0, Ytemp = 0;
    Xtemp = x - xG;
    Ytemp = y - yG;
    if (Xtemp > 0 && Ytemp > 0)
    {
        map[yG][xG] = ch;
        ch = map[yG + 1][xG + 1];
        map[yG + 1][xG + 1] = 'O';
        xG++;
        yG++;
    }
    else if (Xtemp > 0 && Ytemp < 0)
    {
        map[yG][xG] = ch;
        ch = map[yG][xG + 1];
        map[yG][xG + 1] = 'O';
        xG++;
    }
    else if (Xtemp < 0 && Ytemp > 0)
    {
        map[yG][xG] = ch;
        ch = map[yG + 1][xG - 1];
        map[yG + 1][xG - 1] = 'O';
        xG--;
        yG++;
    }
    else if (Xtemp < 0 && Ytemp < 0)
    {

        map[yG][xG] = ch;
        ch = map[yG - 1][xG];
        map[yG - 1][xG] = 'O';

        yG--;
    }
}
int main()
{
    FILE *file;
    char mode;
    printf("Chose UR Mode (E for Easy , H for Hard)\n");
    scanf("%c", &mode);
    if (mode == 'E' ||mode == 'e')
    {
        file = fopen("mapEasy.txt", "r");
    }
    if (mode == 'H' || mode == 'h')
    {
        file = fopen("mapHard.txt", "r");
    }

    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            fscanf(file, "%c", &map[i][j]);
        }
    }

    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            printf("%c", map[i][j]);
        }
    }

    while (1)
    {
        int enter = getch();

        if (enter == 'w' || enter == 'W')
        {
            if (map[y - 1][x] == '-')
            {
                printf("\nYou Lost  :(");
                break;
            }
            else if (map[y - 1][x] == '+')
            {

                point++;
                system("CLS");
                printf("\nYou Got A Point\nYour Point Is = %d\n", point);
                map[y][x] = ' ';
                map[y - 1][x] = 'U';
                y--;
            }
            else
            {
                system("CLS");
                map[y][x] = ' ';
                map[y - 1][x] = 'U';
                y--;
            }
        }
        if (enter == 's' || enter == 'S')
        {
            if (map[y + 1][x] == '-')
            {
                printf("\nYou Lost  :(");
                break;
            }
            else if (map[y + 1][x] == '+')
            {

                point++;
                system("CLS");
                printf("\nYou Got A Point\nYour Point Is = %d\n", point);
                map[y][x] = ' ';
                map[y + 1][x] = 'U';
                y++;
            }
            else
            {
                system("CLS");
                map[y][x] = ' ';
                map[y + 1][x] = 'U';
                y++;
            }
        }
        if (enter == 'a' || enter == 'A')
        {
            if (map[y][x - 1] == '|')
            {
                printf("\nYou Lost  :(");
                break;
            }
            else if (map[y][x - 1] == '+')
            {

                point++;
                system("CLS");
                printf("\nYou Got A Point\nYour Point Is = %d\n", point);
                map[y][x] = ' ';
                map[y][x - 1] = 'U';
                x--;
            }
            else
            {
                system("CLS");
                map[y][x] = ' ';
                map[y][x - 1] = 'U';
                x--;
            }
        }
        if (enter == 'd' || enter == 'D')
        {
            if (map[y][x + 1] == '|')
            {
                printf("\nYou Lost  :(");
                break;
            }
            else if (map[y][x + 1] == '+')
            {

                point++;
                system("CLS");
                printf("\nYou Got A Point\nYour Point Is = %d\n", point);
                map[y][x] = ' ';
                map[y][x + 1] = 'U';
                x++;
            }
            else
            {
                system("CLS");
                map[y][x] = ' ';
                map[y][x + 1] = 'U';
                x++;
            }
        }
        if (point == 6)
        {
            printf("You Win\n");
            break;
        }

        if (enter == 27)
        {
            system("CLS");
            break;
        }
        Ghost();
        if (xG == x && yG == y)
        {
            system("CLS");
            printf("\nYou Lost  :(");
            printf("\nYour Point = %d\nGood Luck Next Time", point);
            break;
        }
        for (int i = 0; i < 18; i++)
        {
            for (int j = 0; j < 32; j++)
            {
                printf("%c", map[i][j]);
            }
        }
    }
    printf("\nGood Bye\n");
    printf("Press Any Key to Exit");
    getch();
}