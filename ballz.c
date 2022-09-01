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

#define PLAY_BUTTON_POS_X1 (DISPLAY_WIDTH/2-100)
#define PLAY_BUTTON_POS_Y1 (DISPLAY_HEIGHT/2)
#define PLAY_BUTTON_POS_X2 (DISPLAY_WIDTH/2+100)
#define PLAY_BUTTON_POS_Y2 (DISPLAY_HEIGHT/2+40)
#define ROUNDED_SIZE       20

#define BOTAO_ROSA               234, 35, 94

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
    square_t square[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS];
    mouse_t mouse;
    distance_t distance;

    
    char strNum[10];
    int i, j, l;
    bool gameover;
    int score;

    squaresInit(square);

    ball.x = DISPLAY_WIDTH / 2;
    ball.y = DISPLAY_HEIGHT - BALL_SIZE;
    ball.ground = true;
    ball.shooted = false;
    gameover = false;
    score = 1;

    mouse.button_state = false;

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
                    // if((mouse.x <= DISPLAY_WIDTH * 0.25) || (mouse.x >= DISPLAY_WIDTH * 0.75))
                    //     speed = SPEED2;
                    // else 
                    //     speed = SPEED; 

                    distance.x = mouse.x - ball.x;
                    distance.y = mouse.y - ball.y;
                    distance.angle = atan2(distance.y, distance.x);
                    ball.dx = SPEED * cos(distance.angle);
                    ball.dy = SPEED * sin(distance.angle);
            
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
                        score++;

                        /* Se restaram quadrados na penúltima linha da matriz, então é o fim do jogo */
                        /* To Do: Tentar implementar o efeito de desenhar última linha(sem que haja uma jogada) e encerrar */
                        /* Talvez isso esteja no lugar errado!! */
                        l = SIZE_BLOCK_LINES-2;
                        for(int c = 0; c < SIZE_BLOCK_COLUMNS; c++)
                        {
                            if(square[l][c].alive)
                                gameover = true;
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
                            square[i][j].alive = rand() % 2 && rand() % 2;

                            if(square[i][j].alive)
                            {
                                if(score % 4 == 0 && rand() % 2)
                                    square[i][j].life = score * 2;
                                else
                                    square[i][j].life = score; 
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
                                    if(collide_lateral(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, ball.x, ball.y))
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
            al_clear_to_color(al_map_rgb(32,32,32));

            for(int i = 1; i <= SIZE_BLOCK_LINES; i++)
            {
                for(int j = 0; j < SIZE_BLOCK_COLUMNS; j++)
                {   
                    if(square[i][j].alive)
                    {
                        if(square[i][j].life <= 5)
                            al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2 , square[i][j].y2, DOURADO_GOLD);
                        else if(square[i][j].life <= 10)
                            al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2 , square[i][j].y2, VERDE_LIMAO);
                        else if(square[i][j].life <= 30)
                            al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2 , square[i][j].y2, ROSA_DEEP_PINK);
                        else if(square[i][j].life <= 50)
                            al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2 , square[i][j].y2, ROXO_PINK);
                        else if(square[i][j].life <= 100)
                            al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2 , square[i][j].y2, AZUL);
                        else
                            al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2 , square[i][j].y2, VERDE_AGUA);
                        
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
                al_draw_text(fontGameOver, BRANCO, DISPLAY_WIDTH/2, DISPLAY_HEIGHT/4, ALLEGRO_ALIGN_CENTER, "Game Over");
                // printf("score: %d", score);

                // Desenha botão de PLAY
                al_draw_filled_rounded_rectangle(PLAY_BUTTON_POS_X1, PLAY_BUTTON_POS_Y1, PLAY_BUTTON_POS_X2, PLAY_BUTTON_POS_Y2, ROUNDED_SIZE, ROUNDED_SIZE, al_map_rgb(BOTAO_ROSA));
            
                // Desenha palavra PLAY usando o retangulo como referência
                al_draw_text
                (
                    fontBall, 
                    al_map_rgb(255, 255, 255),
                    (PLAY_BUTTON_POS_X1 + (PLAY_BUTTON_POS_X2 - PLAY_BUTTON_POS_X1)/2), 
                    (PLAY_BUTTON_POS_Y1 + (PLAY_BUTTON_POS_Y2 - PLAY_BUTTON_POS_Y1)/4),
                    ALLEGRO_ALIGN_CENTER, 
                    "MENU"
                );
            }

            al_flip_display();
        }
    } 

    free(fontGame);
    free(fontGameOver);
    gameGraphicDeinit(window);
    
    return 0;
}