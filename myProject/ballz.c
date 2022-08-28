#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include "graphicStructure.h"
#include "ballzGameplay.h"

/* Posições */
#define POS_START_SQUARE_Y (MARGIN + SQUARE_HEIGHT)

// #define SQUARE_WIDTH 113.75
// #define MARGIN 3.75

int main(int argc, char *argv[])
{
    gameWindow window;
    
    bool redraw = true, sair = false;
    
    window = gameGraphicInit(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    
    ALLEGRO_EVENT ev;
    ALLEGRO_FONT* fontGame = al_load_font("resources/game_over.ttf", FONT_NUM_SIZE, 0);
    ALLEGRO_FONT* fontGameOver = al_load_font("resources/game_over.ttf", FONT_GAMEOVER_SIZE, 0);
    ALLEGRO_FONT* fontBall = al_load_font("resources/font_ball.otf", FONT_BALL_SIZE, 0);

    ball_t ball;
    // square_t square;
    square_t square[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS];
    mouse_t mouse;
    distance_t distance;
    int speed;
    mouse.button_state = false;
    char strNum[10];
    int i, j, l;
    bool gameover;

    ball.x = DISPLAY_WIDTH / 2;
    ball.y = DISPLAY_HEIGHT - BALL_SIZE;
    ball.ground = true;
    ball.shooted = false;
    gameover = false;

    squaresInit(square);

    l = 0;

    // square.x1 = 0;
    // square.x2 = 0;
    // square.y1 = 0;
    // square.y2 = 0;

    while (!sair) 
    {
        al_wait_for_event(window.event_queue, &ev);

        switch (ev.type) 
        {

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                if((ev.mouse.y <= DISPLAY_HEIGHT - 4 * BALL_SIZE))
                    mouse.button_state = true;

                if(ball.ground && mouse.button_state)
                {
                    mouse.x = ev.mouse.x;
                    mouse.y = ev.mouse.y;

                    // To do: melhorar isso.
                    if((mouse.x <= DISPLAY_WIDTH * 0.25) || (mouse.x >= DISPLAY_WIDTH * 0.75))
                        speed = SPEED2;
                    else 
                        speed = SPEED; 

                    distance.x = mouse.x - ball.x;
                    distance.y = mouse.y - ball.y;
                    distance.angle = atan2(distance.y, distance.x);
                    ball.dx = speed * cos(distance.angle);
                    ball.dy = speed * sin(distance.angle);
            
                    // To do: verificar qual o melhor lugar para colocar a linha abaixo.
                    ball.ground = false;  
                }

                break;
        
            case ALLEGRO_EVENT_TIMER:
                
                if(mouse.button_state && !gameover)
                {
                    /* Verifica os limites laterais do Display */
                    if(ball.x <= BALL_SIZE || ball.x >= DISPLAY_WIDTH - BALL_SIZE)
                    {
                        ball.dx = -ball.dx;
                    }
                    /* Verifica o limite superior do Display */
                    if(ball.y <= BALL_SIZE)
                    ball.dy = -ball.dy;

                    /* Verifica o limite inferior do Display e reseta estrutura e a jogada */
                    if(ball.y > DISPLAY_HEIGHT - BALL_SIZE)
                    {
                        ball.ground = true;
                        printf("ground: %d\n", ball.ground);
                        mouse.button_state = false;
                        ball.y = BALL_FLOOR;
                        ball.shooted = false;

                        /* Se restaram quadrados na penúltima linha da matriz, então é o fim do jogo */
                        /* To Do: Tentar implementar o efeito de desenhar última linha(sem que haja uma jogada) e encerrar */
                        /* Talvez isso esteja no lugar errado!! */
                        l = SIZE_BLOCK_LINES-2;
                        for(int c = 0; c < SIZE_BLOCK_COLUMNS; c++)
                        {
                            if(square[l][c].alive)
                            {
                                gameover = true;
                                printf("%d, %d\n", l, c);
                            }
                            printf("[%d][%d] : %d\n", l, c, square[i][j].alive);
                        }

                        /* Verificações para que a bola ao início do jogo não esteja em posições indevidas */
                        if(ball.x <= BALL_SIZE)
                            ball.x = BALL_SIZE * 2;
                        
                        if(ball.x >= DISPLAY_WIDTH - BALL_SIZE)
                            ball.x = DISPLAY_WIDTH - BALL_SIZE*2;

                        /* Copiar os quadrados ainda vivos para linha debaixo */
                        for(l = SIZE_BLOCK_LINES; l >= 2; l--)
                        {
                            for(j = 0; j < SIZE_BLOCK_COLUMNS; j++)
                            {
                                if(square[l-1][j].alive)
                                {
                                    square[l][j].alive = true;
                                    square[l][j].life = square[l-1][j].life;
                                }
                                else 
                                {
                                    square[l][j].alive = false;
                                    square[l][j].life = 0;
                                }

                            }
                        }
                        
                        /* Gerar uma nova linha inicial */
                        i = 1;
                        for(j = 0; j < SIZE_BLOCK_COLUMNS; j++)
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
                    else
                    {                    
                        bool cv = false;
                        for(i = 1; i < SIZE_BLOCK_LINES; i++)
                        {
                            for(j = 0; j < SIZE_BLOCK_COLUMNS; j++)
                            {
                                if(square[i][j].alive)
                                {
                                    if(collide_vertical(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, ball.x, ball.y))
                                    {
                                        ball.dy = -ball.dy; 

                                        square[i][j].life--;
                                        if(square[i][j].life <= 0)
                                            square[i][j].alive = 0;

                                        printf("colisão vertical\n");

                                        cv = true;
                                    }
                                    else if(collide_lateral(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, ball.x, ball.y))
                                    {
                                        ball.dx = -ball.dx;

                                        if( ! cv )
                                        square[i][j].life--;

                                        if(square[i][j].life <= 0)
                                            square[i][j].alive = 0;
                                        
                                        printf("colisão lateral\n");
                                    }   
                                    cv = false;
                                }
                            }
                        }
                        
                        ball.x += ball.dx;
                        ball.y += ball.dy;
                        ball.shooted = true;
                    } 
                }
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE: 

                sair = true; 
                break;
        
            case ALLEGRO_EVENT_KEY_UP:

                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) 
                    sair = true;
                break;
        }

        if(redraw && al_is_event_queue_empty(window.event_queue)) 
        {
            redraw = false;
            al_clear_to_color(PRETO);

            for(int i = 1; i <= SIZE_BLOCK_LINES; i++)
            {
                for(int j = 0; j < SIZE_BLOCK_COLUMNS; j++)
                {   
                    if(square[i][j].alive)
                    {
                        al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2 , square[i][j].y2, al_map_rgb(255,255,0));
                        
                        sprintf(strNum, "%d", square[i][j].life);

                        al_draw_text(
                            fontGame,
                            al_map_rgb(0,0,0),
                            square[i][j].text.x, 
                            square[i][j].text.y,
                            ALLEGRO_ALIGN_CENTER,
                            strNum
                        );
                    }
                }
            }

            al_draw_filled_circle(ball.x, ball.y, BALL_SIZE, al_map_rgb(255,255,255));

            if(ball.ground)
                al_draw_text(fontBall, BRANCO, ball.x - BALL_SIZE, ball.y - BALL_SIZE*3, ALLEGRO_ALIGN_CENTER, "x1");

            if(gameover)
            {
                al_clear_to_color(PRETO);
                al_draw_text(fontGameOver, BRANCO, DISPLAY_WIDTH/2, DISPLAY_HEIGHT/3, ALLEGRO_ALIGN_CENTER, "Game Over");
            }

            al_flip_display();
        }
    } 
    gameGraphicDeinit(window);
    
    return 0;
}