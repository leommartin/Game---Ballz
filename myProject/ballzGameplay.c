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
