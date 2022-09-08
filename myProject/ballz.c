#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "graphicStructure.h"
#include "ballzGameplay.h"

/* Posições */
#define POS_START_SQUARE_Y (MARGIN + SQUARE_HEIGHT)

#define PLAY_BUTTON_POS_X1 (DISPLAY_WIDTH/2-100)
#define PLAY_BUTTON_POS_Y1 (DISPLAY_HEIGHT/2)
#define PLAY_BUTTON_POS_X2 (DISPLAY_WIDTH/2+100)
#define PLAY_BUTTON_POS_Y2 (DISPLAY_HEIGHT/2+40)
#define ROUNDED_SIZE       20
#define LETTER_HEIGHT      (DISPLAY_HEIGHT/3.5)

#define BOTAO_ROSA         234, 35, 94
#define CONST 20

ball_t ballInit(ball_t ball)
{
    ball.x = DISPLAY_WIDTH / 2;
    ball.y = DISPLAY_HEIGHT - BALL_SIZE;
    ball.ground = true;
    ball.shooted = false;

    return ball;
}

int main(int argc, char *argv[])
{
    gameWindow window;
    
    bool redraw = true, sair = false;
    
    window = gameGraphicInit(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    
    ALLEGRO_EVENT ev;
    ALLEGRO_FONT* fontGame = al_load_font("resources/game_over.ttf", FONT_NUM_SIZE, 0);
    ALLEGRO_FONT* fontGameOver = al_load_font("resources/game_over.ttf", FONT_GAMEOVER_SIZE/2, 0);
    ALLEGRO_FONT* fontBall = al_load_font("resources/font_ball.otf", FONT_BALL_SIZE, 0);
    ALLEGRO_FONT* logoFont = al_load_font("resources/desenho.ttf", FONT_LOGO_SIZE, 0);
    ALLEGRO_FONT* fontButton = al_load_font("resources/font_button.otf", 20, 0);
    ALLEGRO_FONT* fontScore = al_load_font("resources/font_score.ttf", 40, 0);
    ALLEGRO_FONT* fontScoreEnd = al_load_font("resources/font_score.ttf", FONT_SCORE_GAMEOVER_SIZE, 0);
    ALLEGRO_FONT* fontSquare = al_load_font("resources/font_score.ttf", 25, 0);

    al_init_image_addon();
    ALLEGRO_BITMAP* info1 = al_load_bitmap("resources/info1.jpg");
    ALLEGRO_BITMAP* info2 = al_load_bitmap("resources/info2.jpg");
    

    ball_t ball;
    square_t square[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS];
    ballSight_t sight;
    mouse_t mouse;
    distance_t distance;

    float distX[CONST], distY[CONST];
    char strNum[10];
    int i, j, l;
    int gameover, menu, info, score;

    squaresInit(square);

    menu = 1;
    info = 0;
    score = 1;
    gameover = 0;
  
    mouse.button_state = 0;

    ball = ballInit(ball);
    
    ALLEGRO_MOUSE_STATE mouse_state;
    
    while (!sair) 
    {
        al_wait_for_event(window.event_queue, &ev);

        switch (ev.type) 
        {
            case ALLEGRO_EVENT_MOUSE_AXES:

                sight.x = ev.mouse.x;
                sight.y = ev.mouse.y;

            break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                al_get_mouse_state(&mouse_state);

                // To do: add estado para quando estiver jogando e quiser ir para o menu pelo botao lateral.

                if(click_centre_button(&mouse_state) && menu) 
                {
                    printf("Botão PLAY foi acionado.\n");
                    menu = 0;
                }
                else if(click_side_button(&mouse_state) && menu) 
                {
                    printf("Botão INFO acionado.\n");
                    // gameover = 0;
                    menu = 0;
                    info = 1;
                    // mouse.button_state = 0;
                }
                else if(click_side_button(&mouse_state) && info) 
                {
                    printf("Botão INFO acionado.\n");
                    // gameover = 0;
                    menu = 1;
                    info = 0;
                    // mouse.button_state = 0;
                }
                else if(click_centre_button(&mouse_state) && gameover) 
                {
                    printf("Botão MENU foi acionado.\n");
                    gameover = 0;
                    menu = 1;
                    // mouse.button_state = 0;
                    score = 1;
                    squaresInit(square);
                    ball = ballInit(ball);
                }
                else if( (ev.mouse.y <= DISPLAY_HEIGHT - 6 * BALL_SIZE) && !menu )
                    mouse.button_state = 1;

                if(ball.ground && mouse.button_state)
                {
                    mouse.x = ev.mouse.x;
                    mouse.y = ev.mouse.y;

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
                    if(ball.x <= (BALL_SIZE + MARGIN) || ball.x >= (DISPLAY_WIDTH - BALL_SIZE - MARGIN))
                        ball.dx = -ball.dx;

                    /* Verifica o limite superior do Display */
                    if(ball.y <= LIMIT_Y_GAME + BALL_SIZE + MARGIN)
                        ball.dy = -ball.dy;

                    /* Verifica o limite inferior do Display e reseta estrutura e a jogada */
                    else if(ball.y > DISPLAY_HEIGHT - BALL_SIZE)
                    {
                        ball.ground = true;
                        printf("ground: %d\n", ball.ground);
                    }
                    
                    /* Reseta jogada */
                    if(ball.ground)
                    {
                        ball.y = BALL_FLOOR;
                        mouse.button_state = 0;
                        ball.shooted = false;
                        score++;

                        /* Se restaram quadrados na penúltima linha da matriz, então é o fim do jogo */
                        /* To Do: Tentar implementar o efeito de desenhar última linha(sem que haja uma jogada) e encerrar */
                        /* Talvez isso esteja no lugar errado!! */
                        l = SIZE_BLOCK_LINES-3;
                        for(int c = 0; c < SIZE_BLOCK_COLUMNS; c++)
                        {
                            if(square[l][c].alive)
                                gameover = true;
                        }

                        /* Verificações para que a bola ao início do jogo não esteja em posições indevidas */
                        if(ball.x <= BALL_SIZE + MARGIN)
                            ball.x = BALL_SIZE * 2;
                        
                        if(ball.x >= DISPLAY_WIDTH - BALL_SIZE - MARGIN)
                            ball.x = DISPLAY_WIDTH - BALL_SIZE*2;

                        /* Copiar os quadrados ainda vivos para linha debaixo */
                        for(l = SIZE_BLOCK_LINES-1; l >= 1; l--)
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
                        
                        /* Gerar uma nova linha inicial com no máximo 5 quadrados */
                        i = 0;
                        int alives = 0;
                        for(j = 0; j < SIZE_BLOCK_COLUMNS; j++)
                        {
                            if(alives < MAX_ALIVE)
                            {
                                square[i][j].alive = rand() % 2;

                                if(square[i][j].alive)
                                {
                                    if(((score >= 3) && (rand() % 2)) && ((rand() % 2) && (rand()%2)))
                                        square[i][j].life = score * 2;
                                    else
                                        square[i][j].life = score; 
                                    alives++;
                                }
                                else 
                                    square[i][j].life = 0;
                            }
                            else
                            {
                                square[i][j].alive = 0;
                                square[i][j].life  = 0;
                            }
                        }
                    }
                    else // bola não está no chão.
                    {                    
                        bool cv = false;
                        bool ch = false;
                        for(i = 0; i < SIZE_BLOCK_LINES; i++)
                        {
                            for(j = 0; j < SIZE_BLOCK_COLUMNS; j++)
                            {
                                if(square[i][j].alive)
                                {
                                    if(collide_vertical(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, ball.x, ball.y))
                                    {
                                        ball.dy = -ball.dy; 
                                        
                                        if(! ch)
                                            square[i][j].life--;

                                        if(square[i][j].life <= 0)
                                            square[i][j].alive = 0;

                                        printf("colisão vertical\n");

                                        cv = true;
                                    }
                                    
                                    ch = false;
                                    if(collide_lateral(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, ball.x, ball.y))
                                    {
                                        ball.dx = -ball.dx;

                                        if( ! cv )
                                            square[i][j].life--;

                                        if(square[i][j].life <= 0)
                                            square[i][j].alive = 0;
                                        
                                        printf("colisão lateral\n");

                                        ch = true;
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
            al_clear_to_color(al_map_rgb(30,30,30));

            if(gameover)
            {
                al_draw_text(fontGameOver, BRANCO, DISPLAY_WIDTH/2, DISPLAY_HEIGHT/5, ALLEGRO_ALIGN_CENTER, "Game Over");
                // printf("score: %d", score);

                // Desenha botão de MENU
                al_draw_filled_rounded_rectangle(PLAY_BUTTON_POS_X1, PLAY_BUTTON_POS_Y1, PLAY_BUTTON_POS_X2, PLAY_BUTTON_POS_Y2, ROUNDED_SIZE, ROUNDED_SIZE, al_map_rgb(BOTAO_ROSA));
            
                // Desenha o SCORE na tela
                sprintf(strNum, "%d", score);
                al_draw_text
                (
                    fontScoreEnd, 
                    al_map_rgb(255, 255, 255),
                    (DISPLAY_WIDTH/2), 
                    (LETTER_HEIGHT),
                    ALLEGRO_ALIGN_CENTER, 
                    strNum
                );

                 al_draw_text
                (
                    fontScore, 
                    al_map_rgb(255, 255, 255),
                    (PLAY_BUTTON_POS_X1 + (PLAY_BUTTON_POS_X2 - PLAY_BUTTON_POS_X1) / 2), 
                    (LETTER_HEIGHT + FONT_GAMEOVER_SIZE/2 + MARGIN*2),
                    ALLEGRO_ALIGN_CENTER, 
                    "BEST:"
                );

                al_draw_text
                (
                    fontButton, 
                    al_map_rgb(255, 255, 255),
                    (PLAY_BUTTON_POS_X1 + (PLAY_BUTTON_POS_X2 - PLAY_BUTTON_POS_X1) / 2), 
                    (PLAY_BUTTON_POS_Y1 + (PLAY_BUTTON_POS_Y2 - PLAY_BUTTON_POS_Y1) / 5),
                    ALLEGRO_ALIGN_CENTER, 
                    "MENU"
                );
            }
            else if (menu)
            {   
                al_draw_filled_rounded_rectangle(MARGIN*2, LIMIT_Y_GAME/7, DISPLAY_WIDTH/5, LIMIT_Y_GAME/2, 15,15, al_map_rgb(50,50,50));

                al_draw_text
                (
                    fontButton, 
                    al_map_rgb(255, 255, 255),
                    (MARGIN + DISPLAY_WIDTH/10), 
                    (MARGIN + LIMIT_Y_GAME/7),
                    ALLEGRO_ALIGN_CENTER, 
                    "INFO"
                );

                // Desenha texto com a fonte logo ttf
                al_draw_text(logoFont, al_map_rgb(234, 35, 94), DISPLAY_WIDTH/2-80, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "B");
                al_draw_text(logoFont, al_map_rgb(255, 165, 0), DISPLAY_WIDTH/2-20, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "a");
                al_draw_text(logoFont, al_map_rgb(0, 0, 205), DISPLAY_WIDTH/2+20, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "l");
                al_draw_text(logoFont, al_map_rgb(0, 255, 255), DISPLAY_WIDTH/2+50, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "l");
                al_draw_text(logoFont, al_map_rgb(124,252,0), DISPLAY_WIDTH/2+90, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "z");

                // Desenha botão de PLAY
                al_draw_filled_rounded_rectangle(PLAY_BUTTON_POS_X1, PLAY_BUTTON_POS_Y1, PLAY_BUTTON_POS_X2, PLAY_BUTTON_POS_Y2, ROUNDED_SIZE, ROUNDED_SIZE, al_map_rgb(BOTAO_ROSA));
                
                // Desenha palavra PLAY usando o retangulo como referência
                al_draw_text
                (
                    fontButton, 
                    al_map_rgb(255, 255, 255),
                    (PLAY_BUTTON_POS_X1 + (PLAY_BUTTON_POS_X2 - PLAY_BUTTON_POS_X1) /  2), 
                    (PLAY_BUTTON_POS_Y1 + (PLAY_BUTTON_POS_Y2 - PLAY_BUTTON_POS_Y1) /  5),
                    ALLEGRO_ALIGN_CENTER, 
                    "PLAY"
                );
            }
            else if(info)
            {   
                al_draw_filled_rounded_rectangle(MARGIN*2, LIMIT_Y_GAME/7, DISPLAY_WIDTH/5, LIMIT_Y_GAME/2, 15,15, al_map_rgb(BOTAO_ROSA));
                
                al_draw_text
                (
                    fontButton, 
                    al_map_rgb(255, 255, 255),
                    (MARGIN + DISPLAY_WIDTH/10), 
                    (MARGIN + LIMIT_Y_GAME/7),
                    ALLEGRO_ALIGN_CENTER, 
                    "MENU"
                );

                al_draw_text
                (
                    fontButton, 
                    al_map_rgb(255, 255, 255),
                    DISPLAY_WIDTH/2, 
                    DISPLAY_HEIGHT/12,
                    ALLEGRO_ALIGN_CENTER, 
                    "HOW TO PLAY"
                );
                al_draw_bitmap(info1, DISPLAY_WIDTH/10, DISPLAY_HEIGHT/8, 0);
                al_draw_bitmap(info2, DISPLAY_WIDTH/10, DISPLAY_HEIGHT*0.56, 0);
            }
            else // Desenha o jogo
            {
                if(click_side_button(&mouse_state)) 
                {
                    printf("Botão MENU foi acionado.\n");
                    gameover = 0;
                    menu = 1;
                    mouse.button_state = 0;
                    score = 1;
                    squaresInit(square);
                    ball = ballInit(ball);
                }

                al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, LIMIT_Y_GAME, al_map_rgb(38,38,38));

                al_draw_filled_rounded_rectangle(MARGIN*2, LIMIT_Y_GAME/7, DISPLAY_WIDTH/5, LIMIT_Y_GAME/2, 15,15, al_map_rgb(BOTAO_ROSA));
                
                al_draw_text
                (
                    fontButton, 
                    al_map_rgb(255, 255, 255),
                    (MARGIN + DISPLAY_WIDTH/10), 
                    (MARGIN + LIMIT_Y_GAME/7),
                    ALLEGRO_ALIGN_CENTER, 
                    "MENU"
                );

                sprintf(strNum, "%d", score);
                al_draw_text(fontScore, BRANCO, DISPLAY_WIDTH/2, LIMIT_Y_GAME/10, ALLEGRO_ALIGN_CENTER, strNum);

                for(int i = 0; i < SIZE_BLOCK_LINES; i++)
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

                            al_draw_text(fontSquare, al_map_rgb(0,0,0), square[i][j].text.x, square[i][j].text.y, ALLEGRO_ALIGN_CENTER, strNum);
                        }
                    }
                }

                al_draw_filled_circle(ball.x, ball.y, BALL_SIZE, al_map_rgb(255,255,255));
                
                if(ball.ground)
                {
                    al_draw_text(fontBall, BRANCO, ball.x - BALL_SIZE, ball.y - BALL_SIZE*3, ALLEGRO_ALIGN_CENTER, "x1");
                    
                    if( (DISPLAY_HEIGHT - (BALL_SIZE * 6)) >= sight.y)
                    {
                        float cont = 0;
                        int mult = 3;
                        for(int c = 0; c < CONST; c++)
                        {
                            if(( DISPLAY_HEIGHT - (BALL_SIZE * mult) >= sight.y))
                            {
                                distY[c] = BALL_SIZE * mult;
                                mult+= 3;
                                cont++;
                            }
                        }

                        float distBallX = 100/cont/100;

                        for(int c = 0; c < cont; c++)
                        {
                            distX[c] = (abs(sight.x - ball.x)) * distBallX;
                            distBallX += 100/cont/100;
                        }

                        if( ball.x >= sight.x )
                        {
                            for(int c = 0; c < cont; c++)
                                // if( ((ball.x - distX[c]) <= (DISPLAY_WIDTH)) && (ball.x - distX[c] >= BALL_SIZE) )
                                    al_draw_filled_circle(ball.x - distX[c], ball.y - distY[c], BALL_SIZE, al_map_rgb(255,255,255));
                        }
                        else
                            for(int c = 0; c < cont; c++)
                            // if( ((ball.x - distX[c]) <= (DISPLAY_WIDTH)) && (ball.x - distX[c] >= BALL_SIZE) )
                                al_draw_filled_circle(ball.x + distX[c], ball.y - distY[c], BALL_SIZE, al_map_rgb(255,255,255));
                    }
                }
            }
            
            al_flip_display();
        }
    } 
    
    al_destroy_bitmap(info1);
    al_destroy_bitmap(info2);
    al_destroy_font(fontGame);
    al_destroy_font(fontGameOver);
    al_destroy_font(fontBall);
    al_destroy_font(fontButton);
    al_destroy_font(fontSquare);
    al_destroy_font(fontScore);
    al_destroy_font(fontScoreEnd);
    gameGraphicDeinit(window);
    
    return 0;
}