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

#define ROUNDED_SIZE 20
#define LETTER_HEIGHT (DISPLAY_HEIGHT / 3.5)

#define BOTAO_ROSA 234, 35, 94
#define CONST 20


int main(int argc, char *argv[])
{
    gameWindow window;

    bool redraw = true, sair = false;

    window = gameGraphicInit(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    ALLEGRO_FONT *fontGame = al_load_font("resources/game_over.ttf", FONT_NUM_SIZE, 0);
    ALLEGRO_FONT *fontGameOver = al_load_font("resources/game_over.ttf", FONT_GAMEOVER_SIZE / 2, 0);
    ALLEGRO_FONT *fontBall = al_load_font("resources/font_ball.otf", FONT_BALL_SIZE, 0);
    ALLEGRO_FONT *logoFont = al_load_font("resources/desenho.ttf", FONT_LOGO_SIZE, 0);
    ALLEGRO_FONT *fontButton = al_load_font("resources/font_button.otf", 20, 0);
    ALLEGRO_FONT *fontScore = al_load_font("resources/font_score.ttf", 50, 0);
    ALLEGRO_FONT *fontScoreEnd = al_load_font("resources/font_score.ttf", FONT_SCORE_GAMEOVER_SIZE, 0);
    ALLEGRO_FONT *fontSquare = al_load_font("resources/font_score.ttf", 25, 0);

    ALLEGRO_EVENT ev;

    al_init_image_addon();
    ALLEGRO_BITMAP *info1 = al_load_bitmap("resources/info1.jpg");
    ALLEGRO_BITMAP *info2 = al_load_bitmap("resources/info2.jpg");

    square_t square[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS];
    ballBlock_t ballBlock[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS];
    ballSight_t sight;
    mouse_t mouse;
    distance_t distance;

    float distX[CONST], distY[CONST];
    char strNum[10];
    int i, l, c;
    int gameover, menu, info, playing;
    int score, bestScore;
    int qtdBalls, newBalls;
    int initialBallsX, allGround;
    int timer;
    
    bool firstBallOnTheFloor, touchGround;

    blocksInit(square, ballBlock);

    // aqui cabe uma função para inicializar as variáveis
    menu = 1;
    info = 0;
    score = 1;
    bestScore = 0;
    gameover = 0;
    playing = 0;
    qtdBalls = 1;

    mouse.button_state = 0;

    ball_t *b;
    b = createBall(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - BALL_SIZE);

    if (!b)
        printf("erro na criação da bola");

    ball_t **balls = initBallsArray(b);
    if (!balls)
        printf("erro na criação do array bola");

    bestScore = scoreCompare(score);

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

            /* Menu -> PLAY button */
            if (click_centre_button(&mouse_state) && menu)
            {
                menu = 0;
                playing = 1;
            }
            /* Menu -> INFO button */
            else if (click_side_button(&mouse_state) && menu)
            {
                menu = 0;
                info = 1;
            }
            /* Info -> MENU button */
            else if (click_side_button(&mouse_state) && info)
            {
                menu = 1;
                info = 0;
            }
            /* Gameover -> MENU button */
            else if (click_centre_button(&mouse_state) && gameover)
            {
                gameover = 0;
                menu = 1;
                score = 1;
                blocksInit(square, ballBlock);
                // ball = ballInit(ball);
            }
            /* Playing -> MENU button */
            else if (click_side_button(&mouse_state) && playing)
            {
                menu = 1;
                playing = 0;
            }
            /* Jogada válida*/
            else if ((ev.mouse.y <= DISPLAY_HEIGHT - 6 * BALL_SIZE) && playing)
                mouse.button_state = 1;

            allGround = ballsGround(balls, qtdBalls);

            if (allGround && mouse.button_state)
            {
                mouse.x = ev.mouse.x;
                mouse.y = ev.mouse.y;

                distance.x = mouse.x - balls[0]->x;
                distance.y = mouse.y - balls[0]->y;
                distance.angle = atan2(distance.y, distance.x);

                for (int i = 0; i < qtdBalls; i++)
                {
                    balls[i]->dx = SPEED * cos(distance.angle);
                    balls[i]->dy = SPEED * sin(distance.angle);
                    balls[i]->launched = false;
                    balls[i]->moving = true;
                    balls[i]->ground = false;
                }

                timer = -MARGIN_TIMER;
                allGround = false;
                firstBallOnTheFloor = true;
                touchGround = false;
                newBalls = 0;
            }
            break;

        case ALLEGRO_EVENT_TIMER:

            timer++;

            /* Talvez o estado playing seja melhor para ser usado no if */
            if (mouse.button_state && playing)
            {
                for (int i = 0; i < qtdBalls; i++)
                {
                    if (balls[i]->launched)
                    {
                        /* Verifica os limites laterais do Display */
                        if (balls[i]->x <= (BALL_SIZE + MARGIN) || balls[i]->x >= (DISPLAY_WIDTH - BALL_SIZE - MARGIN))
                            balls[i]->dx = -balls[i]->dx;

                        /* Verifica o limite superior do Display */
                        if (balls[i]->y <= LIMIT_Y_GAME + BALL_SIZE + MARGIN)
                            balls[i]->dy = -balls[i]->dy;

                        /* Verifica o limite inferior do Display e reseta estrutura e a jogada */
                        if (balls[i]->y > DISPLAY_HEIGHT - BALL_SIZE)
                        {
                            balls[i]->moving = false;
                            balls[i]->launched = false;
                            balls[i]->ground = true;

                            if (firstBallOnTheFloor)
                            {
                                initialBallsX = balls[i]->x;
                                firstBallOnTheFloor = false;
                                touchGround = true;
                            }

                            balls[i]->x = initialBallsX;
                            balls[i]->y = BALL_FLOOR * 1.5;
                            balls[i]->dx = 0.0;
                            balls[i]->dy = 0.0;

                            allGround = ballsGround(balls, qtdBalls);

                            /* Reseta jogada */
                            if (allGround)
                            {
                                score++;

                                mouse.button_state = 0;

                                resetBalls(balls, initialBallsX, qtdBalls);

                                /* Attention: acho que não precisa do laço */
                                for (int cont = 0; cont < newBalls; cont++)
                                {
                                    ball_t *aux = createBall(balls[cont]->x, DISPLAY_HEIGHT - BALL_SIZE);
                                    balls = addBalls(balls, aux, qtdBalls);
                                    qtdBalls++;
                                }

                                /* Verificações para que a bola ao início do jogo não esteja em posições indevidas */
                                for (int indice = 0; indice < qtdBalls; indice++)
                                {
                                    if (balls[indice]->x <= BALL_SIZE + MARGIN)
                                        balls[indice]->x = BALL_SIZE * 2;

                                    if (balls[indice]->x >= DISPLAY_WIDTH - BALL_SIZE - MARGIN)
                                        balls[indice]->x = DISPLAY_WIDTH - BALL_SIZE * 2;
                                }

                                /* Copiar os quadrados ainda vivos para linha debaixo */
                                for (l = SIZE_BLOCK_LINES - 1; l >= 1; l--)
                                {
                                    for (c = 0; c < SIZE_BLOCK_COLUMNS; c++)
                                    {
                                        if (square[l - 1][c].alive)
                                        {
                                            square[l][c].alive = true;
                                            square[l][c].life = square[l - 1][c].life;
                                        }
                                        else
                                        {
                                            square[l][c].alive = false;
                                            square[l][c].life = 0;

                                            if(ballBlock[l-1][c].alive)
                                            {
                                                ballBlock[l-1][c].alive = false;
                                                ballBlock[l][c].alive = true;
                                            }
                                            else
                                                ballBlock[l][c].alive = false;
                                        }
                                    }
                                }

                                /* Se restaram quadrados na penúltima linha da matriz, então é o fim do jogo */
                                /* To Do: Tentar implementar o efeito de desenhar última linha(sem que haja uma jogada) e encerrar */
                                /* Talvez isso esteja no lugar errado!! */
                                l = SIZE_BLOCK_LINES - 2;
                                for (c = 0; c < SIZE_BLOCK_COLUMNS; c++)
                                {
                                    if (square[l][c].alive)
                                        gameover = true;
                                }

                                if (gameover)
                                    bestScore = scoreCompare(score);

                                /* Gerar uma nova linha inicial com no máximo 4 quadrados e 1 bloco de bola */
                                l = 0;
                                int alives = 0;
                                int blocks = 0;
                                for (c = 0; c < SIZE_BLOCK_COLUMNS; c++)
                                {
                                    if (alives < MAX_ALIVE)
                                    {
                                        square[l][c].alive = rand() % 2;

                                        if (square[l][c].alive)
                                        {
                                            if (((score >= 3) && (rand() % 2)) && ((rand() % 2) && (rand() % 2)))
                                                square[l][c].life = score * 2;
                                            else
                                                square[l][c].life = score;
                                            alives++;
                                        }
                                        else
                                        {
                                            square[l][c].life = 0;

                                            if(blocks < 1)
                                            {
                                                ballBlock[l][c].alive = rand() % 2;

                                                if(ballBlock[l][c].alive)
                                                    blocks++;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        square[l][c].alive = false;
                                        square[l][c].life = 0;

                                        if(blocks < 1)
                                        {
                                            ballBlock[l][c].alive = rand() % 2;

                                            if(ballBlock[l][c].alive)
                                                blocks++;
                                        }
                                    }
                                }
                            }
                        }
                        else /* Bola não está no chão */
                        {
                            for (l = 0; l < SIZE_BLOCK_LINES; l++)
                            {
                                for (c = 0; c < SIZE_BLOCK_COLUMNS; c++)
                                {
                                    if (square[l][c].alive)
                                    {
                                        if (collide_vertical(square[l][c].x1, square[l][c].y1, square[l][c].x2, square[l][c].y2, balls[i]->x, balls[i]->y, balls[i]->dx, balls[i]->dy))
                                        {
                                            balls[i]->dy = -balls[i]->dy;

                                                square[l][c].life--;

                                            if (square[l][c].life <= 0)
                                                square[l][c].alive = 0;
                                        }

                                        else if (collide_lateral(square[l][c].x1, square[l][c].y1, square[l][c].x2, square[l][c].y2, balls[i]->x, balls[i]->y, balls[i]->dx, balls[i]->dy))
                                        {
                                            balls[i]->dx = -balls[i]->dx;

                                                square[l][c].life--;

                                            if (square[l][c].life <= 0)
                                                square[l][c].alive = 0;
                                        }
                                    }

                                    if (ballBlock[l][c].alive)
                                    {
                                        if(collide_vertical(square[l][c].x1+20, square[l][c].y1+20, square[l][c].x2-20, square[l][c].y2-20, balls[i]->x, balls[i]->y, balls[i]->dx, balls[i]->dy))
                                        {
                                            ballBlock[l][c].alive = false;
                                            newBalls++;
                                        }
                                        else if(collide_lateral(square[l][c].x1+20, square[l][c].y1+20, square[l][c].x2-20, square[l][c].y2-20, balls[i]->x, balls[i]->y, balls[i]->dx, balls[i]->dy))
                                        {
                                            ballBlock[l][c].alive = false;
                                            newBalls++;
                                        }
                                            
                                    }
                                }
                            }

                            balls[i]->x += balls[i]->dx;
                            balls[i]->y += balls[i]->dy;
                        }
                    }
                    else
                    {
                        if (timer >= (i * 10) - MARGIN_TIMER && timer <= (i * 10) + MARGIN_TIMER)
                        {
                            balls[i]->x += balls[i]->dx;
                            balls[i]->y += balls[i]->dy;
                            balls[i]->launched = true;
                        }
                    }
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

        if (redraw && al_is_event_queue_empty(window.event_queue))
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(30, 30, 30));

            if (gameover)
            {
                playing = 0;
                qtdBalls = 1;
                al_draw_text(fontGameOver, BRANCO, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 5, ALLEGRO_ALIGN_CENTER, "Game Over");

                /* Desenha botão de MENU */
                al_draw_filled_rounded_rectangle(PLAY_BUTTON_POS_X1, PLAY_BUTTON_POS_Y1, PLAY_BUTTON_POS_X2, PLAY_BUTTON_POS_Y2, ROUNDED_SIZE, ROUNDED_SIZE, al_map_rgb(BOTAO_ROSA));

                /* Desenha score */
                sprintf(strNum, "%d", score);
                al_draw_text(
                    fontScoreEnd,
                    al_map_rgb(255, 255, 255),
                    (DISPLAY_WIDTH / 2),
                    (LETTER_HEIGHT),
                    ALLEGRO_ALIGN_CENTER,
                    strNum);

                if (bestScore < score)
                    bestScore = score;

                al_draw_textf(
                    fontScore,
                    al_map_rgb(255, 255, 255),
                    (PLAY_BUTTON_POS_X1 + (PLAY_BUTTON_POS_X2 - PLAY_BUTTON_POS_X1) / 2),
                    (LETTER_HEIGHT + FONT_GAMEOVER_SIZE / 2 + MARGIN * 2),
                    ALLEGRO_ALIGN_CENTER,
                    "BEST: %d",
                    bestScore);

                al_draw_text(
                    fontButton,
                    al_map_rgb(255, 255, 255),
                    (PLAY_BUTTON_POS_X1 + (PLAY_BUTTON_POS_X2 - PLAY_BUTTON_POS_X1) / 2),
                    (PLAY_BUTTON_POS_Y1 + (PLAY_BUTTON_POS_Y2 - PLAY_BUTTON_POS_Y1) / 5),
                    ALLEGRO_ALIGN_CENTER,
                    "MENU");
            }
            else if (menu)
            {
                /* Desenha botão de INFO */
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

                /* Desenha a logo do game */
                al_draw_text(logoFont, al_map_rgb(234, 35, 94), DISPLAY_WIDTH/2-80, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "B");
                al_draw_text(logoFont, al_map_rgb(255, 165, 0), DISPLAY_WIDTH/2-20, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "a");
                al_draw_text(logoFont, al_map_rgb(0, 0, 205), DISPLAY_WIDTH/2+20, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "l");
                al_draw_text(logoFont, al_map_rgb(0, 255, 255), DISPLAY_WIDTH/2+50, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "l");
                al_draw_text(logoFont, al_map_rgb(124,252,0), DISPLAY_WIDTH/2+90, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "z");

                /* Desenha botão de PLAY */
                al_draw_filled_rounded_rectangle(PLAY_BUTTON_POS_X1, PLAY_BUTTON_POS_Y1, PLAY_BUTTON_POS_X2, PLAY_BUTTON_POS_Y2, ROUNDED_SIZE, ROUNDED_SIZE, al_map_rgb(BOTAO_ROSA));
                
                /* Desenha palavra PLAY usando o botão como referência */
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
            else if (info)
            {
                al_draw_filled_rounded_rectangle(MARGIN * 2, LIMIT_Y_GAME / 7, DISPLAY_WIDTH / 5, LIMIT_Y_GAME / 2, 15, 15, al_map_rgb(BOTAO_ROSA));

                al_draw_text(
                    fontButton,
                    al_map_rgb(255, 255, 255),
                    (MARGIN + DISPLAY_WIDTH / 10),
                    (MARGIN + LIMIT_Y_GAME / 8),
                    ALLEGRO_ALIGN_CENTER,
                    "MENU");

                al_draw_text(
                    fontButton,
                    al_map_rgb(255, 255, 255),
                    DISPLAY_WIDTH / 2,
                    DISPLAY_HEIGHT / 12,
                    ALLEGRO_ALIGN_CENTER,
                    "HOW TO PLAY");
                al_draw_bitmap(info1, DISPLAY_WIDTH / 10, DISPLAY_HEIGHT / 8, 0);
                al_draw_bitmap(info2, DISPLAY_WIDTH / 10, DISPLAY_HEIGHT * 0.56, 0);
            }
            else if (playing)
            {
                /* Desenha o jogo */

                al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, LIMIT_Y_GAME, al_map_rgb(38, 38, 38));

                al_draw_filled_rounded_rectangle(MARGIN * 2, LIMIT_Y_GAME / 7, DISPLAY_WIDTH / 5, LIMIT_Y_GAME / 2, 15, 15, al_map_rgb(BOTAO_ROSA));

                al_draw_text(
                    fontButton,
                    al_map_rgb(255, 255, 255),
                    (MARGIN + DISPLAY_WIDTH / 10),
                    (MARGIN + LIMIT_Y_GAME / 8),
                    ALLEGRO_ALIGN_CENTER,
                    "MENU");

                sprintf(strNum, "%d", score);
                al_draw_text(fontScore, BRANCO, DISPLAY_WIDTH / 2, LIMIT_Y_GAME / 12, ALLEGRO_ALIGN_CENTER, strNum);

                al_draw_text(
                    fontButton,
                    al_map_rgb(255, 255, 255),
                    (DISPLAY_WIDTH * 0.9),
                    (LIMIT_Y_GAME / 7),
                    ALLEGRO_ALIGN_CENTER,
                    "BEST");

                if (bestScore < score)
                    bestScore = score;

                sprintf(strNum, "%d", bestScore);
                al_draw_text(
                    fontButton,
                    al_map_rgb(255, 255, 255),
                    (DISPLAY_WIDTH * 0.9),
                    (LIMIT_Y_GAME / 3 + MARGIN),
                    ALLEGRO_ALIGN_CENTER,
                    strNum);

                /* Desenha os quadrados com cor baseado no life */
                for (int i = 0; i < SIZE_BLOCK_LINES; i++)
                {
                    for (int j = 0; j < SIZE_BLOCK_COLUMNS; j++)
                    {
                        if (square[i][j].alive)
                        {
                            if (square[i][j].life <= 5)
                                al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, DOURADO_GOLD);
                            else if (square[i][j].life <= 10)
                                al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, VERDE_LIMAO);
                            else if (square[i][j].life <= 30)
                                al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, ROSA_DEEP_PINK);
                            else if (square[i][j].life <= 50)
                                al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, ROXO_PINK);
                            else if (square[i][j].life <= 100)
                                al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, AZUL);
                            else
                                al_draw_filled_rectangle(square[i][j].x1, square[i][j].y1, square[i][j].x2, square[i][j].y2, VERDE_AGUA);

                            sprintf(strNum, "%d", square[i][j].life);

                            al_draw_text(fontSquare, al_map_rgb(0, 0, 0), square[i][j].text.x, square[i][j].text.y, ALLEGRO_ALIGN_CENTER, strNum);
                        }
                        else
                        {
                            if(ballBlock[i][j].alive)
                            {
                                if(timer % 20 <= 10 )
                                    al_draw_circle( ballBlock[i][j].x, ballBlock[i][j].y , 15, BRANCO, 3);
                                else
                                    al_draw_circle( ballBlock[i][j].x, ballBlock[i][j].y , 15+(15*0.125), BRANCO, 3);

                                al_draw_filled_circle( ballBlock[i][j].x, ballBlock[i][j].y , 7.5, BRANCO);
                            }
                        }   

                    }
                }

                /* Desenha as bolas */
                for (i = 0; i < qtdBalls; i++)
                    al_draw_filled_circle(balls[i]->x, balls[i]->y, BALL_SIZE, al_map_rgb(255, 255, 255));

                /* Desenha mira caso todas as bolas estejam no chão */
                if (allGround)
                {
                    al_draw_textf(fontBall, BRANCO, balls[0]->x - BALL_SIZE, balls[0]->y - BALL_SIZE * 3, ALLEGRO_ALIGN_CENTER, "%dx", qtdBalls);

                    if ((DISPLAY_HEIGHT - (BALL_SIZE * 6)) >= sight.y && sight.y >= LIMIT_Y_GAME)
                    {
                        float cont = 0;
                        int mult = 3;
                        for (int c = 0; c < CONST; c++)
                        {
                            if ((DISPLAY_HEIGHT - (BALL_SIZE * mult) >= sight.y))
                            {
                                distY[c] = BALL_SIZE * mult;
                                mult += 3;
                                cont++;
                            }
                        }

                        float distBallX = 100 / cont / 100;

                        for (int c = 0; c < cont; c++)
                        {
                            distX[c] = (abs(sight.x - balls[0]->x)) * distBallX;
                            distBallX += 100 / cont / 100;
                        }

                        if (balls[0]->x >= sight.x)
                            for (int c = 0; c < cont; c++)
                                al_draw_filled_circle(balls[0]->x - distX[c], balls[0]->y - distY[c], BALL_SIZE, al_map_rgb(255, 255, 255));
                        else
                            for (int c = 0; c < cont; c++)
                                al_draw_filled_circle(balls[0]->x + distX[c], balls[0]->y - distY[c], BALL_SIZE, al_map_rgb(255, 255, 255));
                    }
                }

                if(touchGround)
                    al_draw_filled_circle(initialBallsX, BALL_FLOOR, BALL_SIZE, al_map_rgb(255, 255, 255));

                    
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