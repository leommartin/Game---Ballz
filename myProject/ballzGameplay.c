#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Allegro */
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

/* Gameplay */
#include "ballzGameplay.h"

/* Inicializa os quadrados com uma posição inicial */
void squaresInit(square_t square[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS])
{
    srand(time(NULL));

    for(int i = 1; i <= SIZE_BLOCK_LINES; i++)
    {
        for(int j= 0; j < SIZE_BLOCK_COLUMNS; j++)
        {
            square[i][j].x1 = MARGIN + SQUARE_WIDTH  * j;
            square[i][j].y1 = MARGIN + SQUARE_HEIGHT * i;
            square[i][j].x2 = SQUARE_WIDTH * (j+1);
            square[i][j].y2 = SQUARE_HEIGHT * (i+1);
            
            square[i][j].alive = false;
            square[i][j].life  = 0;

            square[i][j].text.x = (square[i][j].x1 + square[i][j].x2) / 2;
            square[i][j].text.y = ((square[i][j].y1 + square[i][j].y2) / 2) - FONT_NUM_SIZE/3;

            // To do: fazer com que o life apareça no meio do quadrado   
        }
    }

    int i = 1;
    for(int j= 0; j < SIZE_BLOCK_COLUMNS; j++)
    {
        square[i][j].alive = rand() % 2;
                
        if(square[i][j].alive)
        {
            if( ! (square[i][j].life = rand() % 4))
                square[i][j].life = 1;
        }
        else 
            square[i][j].life = 0;
    }
}

/* Collide Functions */

bool collide(int p1_x1, int p1_y1, int p1_x2, int p1_y2, int p2_x1, int p2_y1, int p2_x2, int p2_y2)
{
    /* Testa colisao a esquerda */
    if(p1_x1 > p2_x2)
        return false;

    /* Testa colisao a direita */
    if(p1_x2 < p2_x1)
        false;
    
    /* Testa colisao por cima */
    if(p1_y2 < p2_y1)
        return false;
    
    /* Testa colisao por baixo */
    if(p1_y1 > p2_y2)
        return false;

    return true;
}

bool collide_vertical(float square_x1, float square_y1, float square_x2, float square_y2, float ball_x, float ball_y)
{
    /* Testa colisao por baixo */
    if((ball_x >= square_x1 - BALL_SIZE) && (ball_x <= square_x2 + BALL_SIZE))
    {
        if(abs(ball_y - square_y2) <= BALL_SIZE)
            return true;
    }

    /* Testa colisao por cima */
    if((ball_x >= square_x1 - BALL_SIZE) && (ball_x <= square_x2 + BALL_SIZE))
    {
        if(abs(ball_y - square_y1) <= BALL_SIZE)
            return true;
    }        

    return false;
}

bool collide_lateral(float square_x1, float square_y1, float square_x2, float square_y2, float ball_x, float ball_y)
{
    /* Testa colisao pela esquerda */
    if((ball_y >= square_y1 - BALL_SIZE) && (ball_y <= square_y2 + BALL_SIZE))
    {
        if(abs(ball_x - square_x1) <= BALL_SIZE)
            return true;
    }
    
    /* Testa colisao pela direita */
    if((ball_y >= square_y1 - BALL_SIZE) && (ball_y <= square_y2 + BALL_SIZE))
    {
        if( (ball_x >= square_x2) && (abs(ball_x - square_x2) <= BALL_SIZE) )
            return true;
    }

    return false;
}


