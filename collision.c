#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Allegro */
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_primitives.h>

/* Graficos */
#include "grafico.h"

#define RES_WIDTH    800
#define RES_HEIGHT   600

#define BALL_SIZE    8
#define GRID         4.0
#define SPEED        8
#define SPEED2       (SPEED * 1.05)
#define TAM          2

#define BALL_FLOOR   (RES_HEIGHT - BALL_SIZE)


/* STRUCTS */
typedef struct ball
{
    float x,y;
    float dx,dy;
    bool shooted, ground;
} 
ball_t;

typedef struct square
{
    float x1,y1,x2,y2;
    int life;
} 
square_t;

typedef struct mouse 
{
    float x,y;
    bool button_state;
} 
mouse_t;

typedef struct distance
{
    float x,y;
    float angle;
} 
distance_t;

/* Aux Functions */

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
    if((ball_x >= square_x1 - SPEED) && (ball_x <= square_x2 + SPEED))
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

int main(int argc, char *argv[])
{
  Window win;
  
  bool redraw = true, sair = false;
  
  win = graphinit(RES_WIDTH, RES_HEIGHT);
  
  ALLEGRO_EVENT ev;
  ball_t ball;
  square_t square;
  mouse_t mouse;
  distance_t distance;

  int speed;
  mouse.button_state = false;

  ball.x = RES_WIDTH / 2;
  ball.y = RES_HEIGHT - BALL_SIZE;
  ball.ground = true;
  ball.shooted = false;

  square.x1 = RES_WIDTH/2-20;
  square.x2 = RES_WIDTH/2+20;
  square.y1 = RES_HEIGHT/2-20;
  square.y2 = RES_HEIGHT/2+20;

  while (!sair) 
  {
      al_wait_for_event(win.event_queue, &ev);

      switch (ev.type) 
      {

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

            if((ev.mouse.y <= RES_HEIGHT - 4 * BALL_SIZE))
              mouse.button_state = true;

            if(ball.ground && mouse.button_state)
            {
              mouse.x = ev.mouse.x;
              mouse.y = ev.mouse.y;

              // To do: melhorar isso.
              if((mouse.x <= win.disp_data.width * 0.25) || (mouse.x >= win.disp_data.width * 0.75))
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
                if(ball.x <= BALL_SIZE || ball.x >= win.disp_data.width - BALL_SIZE)
                {
                    ball.dx = -ball.dx;
                }
                /* Verifica o limite superior do Display */
                if(ball.y <= BALL_SIZE)
                  ball.dy = -ball.dy;

                /* Verifica o limite inferior do Display e reseta estrutura e a jogada */
                if(ball.y > win.disp_data.height - BALL_SIZE && abs(BALL_SIZE - RES_WIDTH) >= BALL_SIZE)
                {
                    ball.ground = true;
                    printf("ground: %d\n", ball.ground);
                    mouse.button_state = false;
                    ball.y = BALL_FLOOR;
                    ball.shooted = false;

                    if(ball.x <= BALL_SIZE)
                        ball.x = BALL_SIZE * 2;
                    if(ball.x >= RES_WIDTH - BALL_SIZE)
                        ball.x = RES_WIDTH - BALL_SIZE*2;
                }
                else
                {                    
                    printf("squareX1: %.2f, squareY1: %.2f, squareX2: %.2f, squareY2: %.2f, ballX: %.2f, ballY: %.2f\n", 
                    square.x1, square.y1, square.x2, square.y2, ball.x, ball.y);

                    if(collide_vertical(square.x1, square.y1, square.x2, square.y2, ball.x, ball.y))
                        ball.dy = -ball.dy; 
                    if(collide_lateral(square.x1, square.y1, square.x2, square.y2, ball.x, ball.y))
                        ball.dx = -ball.dx;
                    
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

    if(redraw && al_is_event_queue_empty(win.event_queue)) 
    {
        redraw = false;
        al_clear_to_color(PRETO);
        
        al_draw_filled_rectangle(square.x1, square.y1, square.x2, square.y2, al_map_rgb(255,255,0));
        al_draw_filled_circle(ball.x, ball.y, BALL_SIZE, al_map_rgb(255,255,255));

        al_flip_display();
    }
  } 

  graphdeinit(win);
}