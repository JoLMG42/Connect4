/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-faou <mle-faou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 00:53:49 by mle-faou          #+#    #+#             */
/*   Updated: 2022/06/12 23:23:20 by mle-faou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"

t_gameInfo gameInfo;

void ft_bzero(void *s, size_t n)
{
    size_t i;

    i = 0;
    while (i < n)
    {
        ((char *)s)[i] = 0;
        i++;
    }
}

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

int getTabSize(char *str)
{
    int nb = 0;
    
    if (ft_strlen(str) > 4)
        return (-1);
    int i = 0;
    while (str[i] && str[i] != '\n')
    {
        if (str[i] < '0' || str[i] > '9')
            return (-1);
        nb = nb * 10 + (str[i] - '0');
        i++;
    }
    return (nb);
}

void writeInteger(int nb) {
    char str[10];
    int i = 0;
    if (nb == 0)
        str[i++] = '0';
    while (nb > 0)
    {
        str[i] = nb % 10 + '0';
        nb = nb / 10;
        i++;
    }
    str[i] = '\0';
    write(1, str, i);
}

int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return (s1[i] - s2[i]);
        i++;
    }
    return (s1[i] - s2[i]);
}

int **createTab(void) {
    int **tab;
    if (!(tab = malloc(sizeof(int *) * gameInfo.width))) {
        write(1, "Error: malloc failed\n", 22);
        return (NULL);
    }
    for (int i = 0; i < gameInfo.width; i++) {
        if (!(tab[i] = malloc(sizeof(int) * gameInfo.height))) {
            free(tab);
            write(1, "Error: malloc failed\n", 22);
            return (NULL);
        }
        for (int j = 0; j < gameInfo.height; j++)
            tab[i][j] = 0;
    }
    return (tab);
}

void freeTab(void) {
    for (int i = 0; i < gameInfo.width; i++)
        free(gameInfo.tab[i]);
    free(gameInfo.tab);
}

int isColumnFull(int column) {
    if (gameInfo.tab[column][gameInfo.height - 1] != 0)
        return (1);
    return (0);
}

int isTabFull(void) {
    for (int i = 0; i < gameInfo.width; i++) {
        if (isColumnFull(i) == 0)
            return (0);
    }
    return (1);
}

int isBlinking(int i, int j) {
    if ((gameInfo.lastX == i && gameInfo.lastY == j) ||
        (gameInfo.winPiece2X == i && gameInfo.winPiece2Y == j) ||
        (gameInfo.winPiece3X == i && gameInfo.winPiece3Y == j) ||
        (gameInfo.winPiece4X == i && gameInfo.winPiece4Y == j))
        return (1);
    return (0);
}

void displayTab(void) {
    if (gameInfo.isTextOnly)
        return ;
    write(1, _CLEAR_CONSOLE, 11);
    for (int j = gameInfo.height - 1; j>= 0; j--) {
        write (1, _BLUE, 6);
        write(1, "║ ", 5);
        write(1, _RESET, 5);
        for (int i = 0; i < gameInfo.width; i++) {
            if (gameInfo.tab[i][j] == 0)
                write(1, "   ", 4);
            else {
                if (gameInfo.tab[i][j] == 1)
                    write(1, _RED, 6);
                if (gameInfo.tab[i][j] == 2)
                    write(1, _YELLOW, 6);
                if (isBlinking(i, j))
                    write(1, _BLINK, 5);
                write(1, "╔╗ ", 8);
                write(1, _RESET, 5);
            }
        }
        write(1, _BLUE, 6);
        write(1, "║\n║ ", 9);
        write(1, _RESET, 5);
        for (int i = 0; i < gameInfo.width; i++) {
            if (gameInfo.tab[i][j] == 0)
                write(1, "   ", 4);
            else {
                if (gameInfo.tab[i][j] == 1)
                    write(1, _RED, 6);
                if (gameInfo.tab[i][j] == 2)
                    write(1, _YELLOW, 6);
                if (isBlinking(i, j))
                    write(1, _BLINK, 5);
                write(1, "╚╝ ", 8);
                write(1, _RESET, 5);
            }
        }
        write(1, _BLUE, 6);
        write(1, "║\n", 5);
        write(1, _RESET, 5);
    }
    write(1, _BLUE, 6);
    write(1, "╚", 4);

    for (int j = 0; j < gameInfo.width; j++) {
        write(1, "═══", 10);
    }
    write(1, "═╝\n  ", 10);
    write(1, _RESET, 5);
    int tenth;
    int unit;
    for (int j = 0; j < gameInfo.width; j++) {
        tenth = (j / 10) + '0';
        unit = (j % 10) + '0';
        write(1, &(tenth), 1);
        write(1, &(unit), 1);
        write(1, " ", 2);
    }
    write(1, "\n", 2);
}

int addPiece(int column, int player) {
    for (int i = 0; i < gameInfo.height; i++) {
        if (gameInfo.tab[column][i] == 0) {
            gameInfo.tab[column][i] = player;
            gameInfo.lastX = column;
            gameInfo.lastY = i;
            if (gameInfo.isGraphic) {
                updateGraphicTab(column, i, player);
            }
            if (gameInfo.isTextOnly)
            {
                if (player == 1)
                {
                    write(1, _RED, 6);
                    write(1, "Player", 7);
                }
                if (player == 2) {
                    write(1, _YELLOW, 6);
                    write(1, "AI", 3);
                }
                write(1, _RESET, 5);
                write(1, " placed a piece in column ", 27);
                writeInteger(column);
                write(1, "\n", 1);
            }
            return (0);
        }
    }
    if (player == 1)
        write(1, "Error: column's full, choose another one\n", 42);
    return (1);
}

int branchAICheckTab(int testingTab[gameInfo.width][gameInfo.height]) {
    for (int i = 0; i < gameInfo.width; i++) {
        for (int j = 0; j < gameInfo.height; j++) {
            if (testingTab[i][j] == 0)
                continue;
            // check horizontal
            if (i + 3 < gameInfo.width) {
                if (testingTab[i][j] == testingTab[i + 1][j] &&
                    testingTab[i][j] == testingTab[i + 2][j] &&
                    testingTab[i][j] == testingTab[i + 3][j]) {
                    return (testingTab[i][j]);
                }
            }
            // check vertical
            if (j + 3 < gameInfo.height) {
                if (testingTab[i][j] == testingTab[i][j + 1] &&
                    testingTab[i][j] == testingTab[i][j + 2] &&
                    testingTab[i][j] == testingTab[i][j + 3]) {

                    return (testingTab[i][j]);
                }
            }
            // check diagonal up
            if (i + 3 < gameInfo.width && j + 3 < gameInfo.height) {
                if (testingTab[i][j] == testingTab[i + 1][j + 1] &&
                    testingTab[i][j] == testingTab[i + 2][j + 2] &&
                    testingTab[i][j] == testingTab[i + 3][j + 3]) {
                    return (testingTab[i][j]);
                }
            }
            // check diagonal down
            if (i - 3 >= 0 && j + 3 < gameInfo.height) {
                if (testingTab[i][j] == testingTab[i - 1][j + 1] &&
                    testingTab[i][j] == testingTab[i - 2][j + 2] &&
                    testingTab[i][j] == testingTab[i - 3][j + 3]) {
                    return (testingTab[i][j]);
                }
            }
        }
    }
    return (0);
}

void branchAI(int testingTab[gameInfo.width][gameInfo.height], int initialMove, int move, int depth, int *value, int *bestMove, int player) {
    if (depth >= ALGO_MAX_DEPTH)
        return ;
    if (isColumnFull(move))
        return;
    for (int i = 0; i < gameInfo.height; i++) {
        if (testingTab[move][i] == 0) {
            testingTab[move][i] = player;
            break;
        }
    }
    int result = branchAICheckTab(testingTab);
    if (result == player)
        result = ALGO_MAX_DEPTH + 1 - depth;
    if (result > *value || (result == *value && rand() % gameInfo.width == 0)) {
        *value = result;
        *bestMove = initialMove;
    }

    int childTab[gameInfo.width][gameInfo.height];
    for (int i = 0; i < gameInfo.width; i++) {
        for (int i = 0; i < gameInfo.width; i++) {
            for (int j = 0; j < gameInfo.height; j++) {
                childTab[i][j] = testingTab[i][j];
            }
        }
        branchAI(childTab, initialMove, i, depth + 1, value, bestMove, player);
    }
}

void defenseAI(int testingTab[gameInfo.width][gameInfo.height], int move, int *value, int *bestMove, int opponent) {
    if (isColumnFull(move))
        return;
    for (int i = 0; i < gameInfo.height; i++) {
        if (testingTab[move][i] == 0) {
            testingTab[move][i] = opponent;
            break;
        }
    }
    int result = branchAICheckTab(testingTab);
    if (result == opponent)
        result = ALGO_MAX_DEPTH;
    if (result > *value || (result == *value && rand() % gameInfo.width == 0)) {
        *value = result;
        *bestMove = move;
    }
}

void connect4AI(int player) {
    // random AI
    if (gameInfo.width > MAX_WIDTH_AI) {
        while (addPiece(rand() % gameInfo.width, player) == 1){
            if (isTabFull())
                return ;
            ; // careful infinite loop
        }
        return ;
    }

    //semi trying ai
    int value = -6; // >0 = win in x turns, <0 = lose in x turns
    int bestMove = 0;
    int testingTab[gameInfo.width][gameInfo.height];
    //attack
    for (int i = 0; i < gameInfo.width; i++) {
        for (int i = 0; i < gameInfo.width; i++) {
            for (int j = 0; j < gameInfo.height; j++) {
                testingTab[i][j] = gameInfo.tab[i][j];
            }
        }
        branchAI(testingTab, i, i, 1, &value, &bestMove, player);
    }
    //defense
    for (int i = 0; i < gameInfo.width; i++) {
        for (int i = 0; i < gameInfo.width; i++) {
            for (int j = 0; j < gameInfo.height; j++) {
                testingTab[i][j] = gameInfo.tab[i][j];
            }
        }
        defenseAI(testingTab, i, &value, &bestMove, (player == 1) ? 2 : 1);
    }
    addPiece(bestMove, player);
}

// -1 : draw, 0 : still going, 1 : player 1 win, 2 : player 2 win
int isGameOver(void) {
    // check if tab is full
    if (isTabFull() == 1)
        return (-1);
    for (int i = 0; i < gameInfo.width; i++) {
        for (int j = 0; j < gameInfo.height; j++) {
            if (gameInfo.tab[i][j] == 0)
                continue;
            // check horizontal
            if (i + 3 < gameInfo.width) {
                if (gameInfo.tab[i][j] == gameInfo.tab[i + 1][j] &&
                    gameInfo.tab[i][j] == gameInfo.tab[i + 2][j] &&
                    gameInfo.tab[i][j] == gameInfo.tab[i + 3][j]) {
                    gameInfo.lastX = i;
                    gameInfo.lastY = j;
                    gameInfo.winPiece2X = i + 1;
                    gameInfo.winPiece2Y = j;
                    gameInfo.winPiece3X = i + 2;
                    gameInfo.winPiece3Y = j;
                    gameInfo.winPiece4X = i + 3;
                    gameInfo.winPiece4Y = j;
                    return (gameInfo.tab[i][j]);
                }
            }
            // check vertical
            if (j + 3 < gameInfo.height) {
                if (gameInfo.tab[i][j] == gameInfo.tab[i][j + 1] &&
                    gameInfo.tab[i][j] == gameInfo.tab[i][j + 2] &&
                    gameInfo.tab[i][j] == gameInfo.tab[i][j + 3]) {
                    gameInfo.lastX = i;
                    gameInfo.lastY = j;
                    gameInfo.winPiece2X = i;
                    gameInfo.winPiece2Y = j + 1;
                    gameInfo.winPiece3X = i;
                    gameInfo.winPiece3Y = j + 2;
                    gameInfo.winPiece4X = i;
                    gameInfo.winPiece4Y = j + 3;
                    return (gameInfo.tab[i][j]);
                }
            }
            // check diagonal up
            if (i + 3 < gameInfo.width && j + 3 < gameInfo.height) {
                if (gameInfo.tab[i][j] == gameInfo.tab[i + 1][j + 1] &&
                    gameInfo.tab[i][j] == gameInfo.tab[i + 2][j + 2] &&
                    gameInfo.tab[i][j] == gameInfo.tab[i + 3][j + 3]) {
                    gameInfo.lastX = i;
                    gameInfo.lastY = j;
                    gameInfo.winPiece2X = i + 1;
                    gameInfo.winPiece2Y = j + 1;
                    gameInfo.winPiece3X = i + 2;
                    gameInfo.winPiece3Y = j + 2;
                    gameInfo.winPiece4X = i + 3;
                    gameInfo.winPiece4Y = j + 3;
                    return (gameInfo.tab[i][j]);
                }
            }
            // check diagonal down
            if (i - 3 >= 0 && j + 3 < gameInfo.height) {
                if (gameInfo.tab[i][j] == gameInfo.tab[i - 1][j + 1] &&
                    gameInfo.tab[i][j] == gameInfo.tab[i - 2][j + 2] &&
                    gameInfo.tab[i][j] == gameInfo.tab[i - 3][j + 3]) {
                    gameInfo.lastX = i;
                    gameInfo.lastY = j;
                    gameInfo.winPiece2X = i - 1;
                    gameInfo.winPiece2Y = j + 1;
                    gameInfo.winPiece3X = i - 2;
                    gameInfo.winPiece3Y = j + 2;
                    gameInfo.winPiece4X = i - 3;
                    gameInfo.winPiece4Y = j + 3;
                    return (gameInfo.tab[i][j]);
                }
            }
        }
    }
    return (0);
}

int checkGameOver(void) {
    int gameOver = isGameOver();
    if (gameOver == 0)
        return (0);
    if (!gameInfo.isTextOnly)
        displayTab();
    if (gameOver == 1) {
        write(1, _RED, 6);
        write(1, "Player", 7);
    }
    if (gameOver == 2) {
        write(1, _YELLOW, 6);
        write(1, "AI", 3);
    }
    if (gameOver == 1 || gameOver == 2) {
        write(1, _RESET, 5);
        write(1, " wins\n", 7);
        if (gameInfo.isTextOnly) {
            write(1, "The winning line is :\n[", 24);
            writeInteger(gameInfo.lastX);
            write(1, ":", 2);
            writeInteger(gameInfo.lastY);
            write(1, "], [", 5);
            writeInteger(gameInfo.winPiece2X);
            write(1, ":", 2);
            writeInteger(gameInfo.winPiece2Y);
            write(1, "], [", 5);
            writeInteger(gameInfo.winPiece3X);
            write(1, ":", 2);
            writeInteger(gameInfo.winPiece3Y);
            write(1, "], [", 5);
            writeInteger(gameInfo.winPiece4X);
            write(1, ":", 2);
            writeInteger(gameInfo.winPiece4Y);
            write(1, "]\n", 3);
        }
    }
    if (gameOver == -1)
        write(1, "Draw\n", 6);
    return (1);
}

void getPlayerInput(void) {
    char input[4242];
    write(1, "Type the column's number and press Enter\n>> ", 45);
    int res = 0;
    while (1) {
        ft_bzero(input, 4242);
        read(0, input, 4242);
        res = getTabSize(input);
        if (res != -1 && res < gameInfo.width && addPiece(res, 1) == 0) {
            break;
        }
        write(1, "Type the column's number and press Enter\n>> ", 45);
    }
}

void fake_sleep(void) {
    time_t begin = time(NULL);
    // sleep for 1 second
    while (time(NULL) == begin)
        ;
}

int gameloopSecond(void) {
    if (checkGameOver()) {
        if (gameInfo.isGraphic)
            freeMLX();
        return (1);
    }
    displayTab();

    if (gameInfo.isPlayerAI) {
        // fake_sleep();
    }
    connect4AI(2);
    if (checkGameOver()) {
        if (gameInfo.isGraphic)
            freeMLX();
        return (1);
    }
    displayTab();
    return (0);
}

int gameLoop(void) {
    if (gameInfo.isPlayerAI) {
        connect4AI(1);
        if (gameInfo.isGraphic)
            gameloopSecond();
    }
    if (gameInfo.isGraphic) {
        gameInfo.isWaiting = 1;
        return (0);
    }
    if (!gameInfo.isPlayerAI && !gameInfo.isGraphic) {
        getPlayerInput();
    }
    if (checkGameOver()) {
        if (gameInfo.isGraphic)
            exit(0);
        return (1);
    }
    displayTab();
    connect4AI(2);
    if (checkGameOver()) {
        if (gameInfo.isGraphic)
            exit(0);
        return (1);
    }
    displayTab();
    return (0);
}

void printUsage(void) {
    write(1, "Usage: ./connect4 <width> <height> [OPTION]...\n", 48);
    write(1, "This program is a game of connect 4. Try to win againt the AI,\nlet an AI play in your place, or even display the game with a graphic library !\n\n", 145);
    write (1, "\twidth\t\tnumber de columns, min 7, max 93\n", 42);
    write (1, "\theight\t\tnumber de lines, min 6, max 34\n", 41);
    write(1, "\t--ai\t\tplayer is an AI\n", 24);
    // check updates
    write(1, "\t--graphic\tactivate the mlx renderer(max size is 20/10)\n", 57);
    write(1, "\nExample: ./connect4 7 6\n", 26);
}

int main(int argc, char **argv) {
    if (argc < 3 || argc > 5)
    {
        printUsage();
        return (1);
    }
    gameInfo.isPlayerAI = 0;
    gameInfo.isGraphic = 0;
    gameInfo.isWaiting = 0;
    if (argc > 3) {
        if (ft_strcmp(argv[3], "--ai") == 0)
            gameInfo.isPlayerAI = 1;
        else if (ft_strcmp(argv[3], "--graphic") == 0)
            gameInfo.isGraphic = 1;
        else {
            printUsage();
            return (1);
        }
    }
    if (argc > 4) {
        if (ft_strcmp(argv[4], "--ai") == 0 && gameInfo.isPlayerAI == 0)
            gameInfo.isPlayerAI = 1;
        else if (ft_strcmp(argv[4], "--graphic") == 0 && gameInfo.isGraphic == 0)
            gameInfo.isGraphic = 1;
        else {
            printUsage();
            return (1);
        }
    }
    
    gameInfo.width = getTabSize(argv[1]);
    gameInfo.height = getTabSize(argv[2]);
    gameInfo.isTextOnly = 0;
    if (gameInfo.width < 7 || gameInfo.height < 6)
    {
        write(1, "Error: width and height must be at least 7 and 6\n", 50);
        return (1);
    }
    
    if (gameInfo.width > MAX_WIDTH || gameInfo.height > MAX_HEIGHT) {
        write(1, "The grid size is too big to be displayed.\nThe game will then play as intended in text-mode.\n", 93);
        gameInfo.isTextOnly = 1;
    }

    if (gameInfo.isGraphic &&
        (gameInfo.width > MAX_GRAPHIC_WIDTH ||
        gameInfo.height > MAX_GRAPHIC_HEIGHT)) {
        write(1, "Graphic-mode isn't available for this grid size\nThe game will play as intended but the graphic Grid won't be displayed\n", 120);
        gameInfo.isGraphic = 0;
    }

    gameInfo.lastX = -1;
    gameInfo.lastY = -1;
    gameInfo.winPiece2X = -1;
    gameInfo.winPiece2Y = -1;
    gameInfo.winPiece3X = -1;
    gameInfo.winPiece3Y = -1;
    gameInfo.winPiece4X = -1;
    gameInfo.winPiece4Y = -1;

    if (!(gameInfo.tab = createTab()))
        return (1);
    srand(time(NULL));


    if (gameInfo.isGraphic) {
        if (initMLX())
            return (1);
        initGraphicTab();
        mlx_hook(graph.win, 17, (1L << 17), exit_mlx, NULL);
        mlx_mouse_hook(graph.win, mouse_hook, NULL);
        if (rand() % 2 == 1)
            connect4AI(2);
        displayTab();
        mlx_loop_hook(graph.mlx, gameLoop, NULL);
        mlx_loop(graph.mlx);
    }
    else {
        if (rand() % 2 == 1)
            connect4AI(2);
        displayTab();
        while (gameLoop() == 0)
            ;
    }

    freeTab();
    return(0);
}
