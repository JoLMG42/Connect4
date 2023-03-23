/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-faou <mle-faou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 14:45:09 by jtaravel          #+#    #+#             */
/*   Updated: 2022/06/12 19:57:27 by mle-faou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"

t_graph graph;

void freeMLX(void) {
    if (graph.empty)
        mlx_destroy_image(graph.mlx, graph.empty);
    if (graph.red_piece)
        mlx_destroy_image(graph.mlx, graph.red_piece);
    if (graph.yellow_piece)
        mlx_destroy_image(graph.mlx, graph.yellow_piece);
    mlx_destroy_window(graph.mlx, graph.win);
    mlx_destroy_display(graph.mlx);
    free(graph.mlx);
    freeTab();
    exit(0);
}

int exit_mlx(int key, void *param) {
    (void)key;
    (void)param;
    freeMLX();
    return (0);
}

int mouse_hook(int button, int x, int y, void *param) {
    (void)param;
    (void)y;
    if (button != 1 || !gameInfo.isWaiting || gameInfo.isPlayerAI)
        return (0);
    if (addPiece((x / graph.image_width), 1))
        return(0);
    gameInfo.isWaiting = 0;
    gameloopSecond();
    return (0);
}

int initMLX(void) {
    if (!(graph.mlx = mlx_init())) {
        write(1, "Error: mlx_init() failed\n", 26);
        return (1);
    }
    if (!(graph.win = mlx_new_window(graph.mlx, gameInfo.width * GRAPHIC_SIZE_X, gameInfo.height * GRAPHIC_SIZE_Y, "Connect4"))) {
        write(1, "Error: mlx_new_window() failed\n", 32);
        return (1);
    }
    if (!(graph.yellow_piece = mlx_xpm_file_to_image(graph.mlx, "assets/yellow_piece.xpm", &(graph.image_width), &(graph.image_height)))) {
        write(1, "Error: mlx_xpm_file_to_image() for yellow_piece.xpm failed\n", 60);
        return (1);
    }
    if (graph.image_width != GRAPHIC_SIZE_X || graph.image_height != GRAPHIC_SIZE_Y) {
        write(1, "Error: wrong image size for yellow_piece.xpm\n", 46);
        return (1);
    }
    if (!(graph.red_piece = mlx_xpm_file_to_image(graph.mlx, "assets/red_piece.xpm", &(graph.image_width), &(graph.image_height))))
    {
        write(1, "Error: mlx_xpm_file_to_image() for red_piece.xpm failed\n", 57);
        return (1);
    }
    if (graph.image_width != GRAPHIC_SIZE_X || graph.image_height != GRAPHIC_SIZE_Y) {
        write(1, "Error: wrong image size for red_piece.xpm\n", 43);
        return (1);
    }
    if (!(graph.empty = mlx_xpm_file_to_image(graph.mlx, "assets/empty.xpm", &(graph.image_width), &(graph.image_height))))
    {
        write(1, "Error: mlx_xpm_file_to_image() for empty.xpm failed\n", 53);
        return (1);
    }
    if (graph.image_width != GRAPHIC_SIZE_X || graph.image_height != GRAPHIC_SIZE_Y) {
        write(1, "Error: wrong image size for empty.xpm\n", 39);
        return (1);
    }
    return (0);
}

void initGraphicTab(void) {
    for (int j = 0; j < gameInfo.height; j++)
    {
        for (int i = 0; i < gameInfo.width; i++)
        {
            mlx_put_image_to_window(graph.mlx, graph.win, graph.empty, i * GRAPHIC_SIZE_X, j * GRAPHIC_SIZE_Y);
        }
    }
}

void updateGraphicTab(int x, int y, int player) {
    if (player == 1)
    {
        mlx_put_image_to_window(graph.mlx, graph.win, graph.red_piece, x* GRAPHIC_SIZE_Y, (gameInfo.height - y - 1) * GRAPHIC_SIZE_X);
    }
    if (player == 2)
    {
        mlx_put_image_to_window(graph.mlx, graph.win, graph.yellow_piece, x * GRAPHIC_SIZE_Y, (gameInfo.height - y - 1) * GRAPHIC_SIZE_X);
    }
}
