#include <stdio.h>
#include <math.h>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include "graphicStructure.h"
#include "ballzGameplay.h"

/* Largura, altura e margem dos blocos */
#define SQUARE_WIDTH    112.5
#define SQUARE_HEIGHT   70
#define MARGIN   12.5

/* Tamanho da matriz de Blocos*/
#define SIZE_BLOCK_COLUMNS 7
#define SIZE_BLOCK_LINES   8

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

    ball_t ball;
    square_t square;
    // square_t square[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS];
    mouse_t mouse;
    distance_t distance;
    int speed;
    mouse.button_state = false;

    ball.x = DISPLAY_WIDTH / 2;
    ball.y = DISPLAY_HEIGHT - BALL_SIZE;
    ball.ground = true;
    ball.shooted = false;

    square.x1 = 0;
    square.x2 = 0;
    square.y1 = 0;
    square.y2 = 0;

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
                
                if(mouse.button_state)
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

                        /* Verificações para que a bola ao início do jogo não esteja em posições indevidas */
                        if(ball.x <= BALL_SIZE)
                            ball.x = BALL_SIZE * 2;
                        
                        if(ball.x >= DISPLAY_WIDTH - BALL_SIZE)
                            ball.x = DISPLAY_WIDTH - BALL_SIZE*2;
                    }
                    else
                    {                    
                        printf("squareX1: %.2f, squareY1: %.2f, squareX2: %.2f, squareY2: %.2f, ballX: %.2f, ballY: %.2f\n", 
                        square.x1, square.y1, square.x2, square.y2, ball.x, ball.y);

                        // if(collide_vertical(square.x1, square.y1, square.x2, square.y2, ball.x, ball.y))
                        //     ball.dy = -ball.dy; 
                        // if(collide_lateral(square.x1, square.y1, square.x2, square.y2, ball.x, ball.y))
                        //     ball.dx = -ball.dx;
                        
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
            
            for(int i = 1; i < 7; i++)
                for(int j = 0; j < 7; j++)
                    al_draw_filled_rectangle
                    (
                        square.x1 + MARGIN + SQUARE_WIDTH  * j, 
                        square.y1 + MARGIN + SQUARE_HEIGHT * i,
                        square.x2 + SQUARE_WIDTH  * (j+1),
                        square.y2 + SQUARE_HEIGHT * (i+1), 
                        al_map_rgb(255,255,0)
                    );
            
            al_draw_filled_circle(ball.x, ball.y, BALL_SIZE, al_map_rgb(255,255,255));

            al_flip_display();
        }
    } 
    gameGraphicDeinit(window);
    
    return 0;
}