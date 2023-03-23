/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect4.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-faou <mle-faou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 00:53:52 by mle-faou          #+#    #+#             */
/*   Updated: 2022/06/11 13:29:01 by mle-faou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECT4_H
#define CONNECT4_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mlx/mlx.h"

#include "mlx/mlx.h"

#define MAX_WIDTH 93
#define MAX_HEIGHT 34

#define MAX_GRAPHIC_WIDTH 20 // todo update
#define MAX_GRAPHIC_HEIGHT 10 // todo update
#define GRAPHIC_SIZE_X 64
#define GRAPHIC_SIZE_Y 64

#define _YELLOW "\e[33m"
#define _RED "\e[31m"
#define _BLUE "\e[34m"

#define _BLINK "\e[5m"
#define _RESET "\e[0m"

#define _CLEAR_CONSOLE "\e[1;1H\e[2J"

#define ALGO_MAX_DEPTH 5
#define MAX_WIDTH_AI 20

//main.c
typedef struct s_gameInfo
{
    int **tab;
    int width;
    int height;
    int aiFlag;

    int isPlayerAI;
    int isGraphic;
    int isTextOnly;
    
    int lastX;
    int lastY;
    int winPiece2X;
    int winPiece2Y;
    int winPiece3X;
    int winPiece3Y;
    int winPiece4X;
    int winPiece4Y;

    int isWaiting;
} t_gameInfo;

extern t_gameInfo gameInfo;

void freeTab(void);
void writeInteger(int nb);
int gameloopSecond(void);
int addPiece(int column, int player);

//graphic.c
typedef struct s_graph
{
    void *mlx;
    void *win;
    int image_width;
    int image_height;
    char *red_piece;
    char *yellow_piece;
    char *empty;
} t_graph;

extern t_graph graph;

void freeMLX(void);
int exit_mlx(int key, void *param);
int initMLX(void);
void initGraphicTab(void);
void updateGraphicTab(int x, int y, int player);

int mouse_hook(int button, int x, int y, void *param);

#endif // CONNECT4_H
