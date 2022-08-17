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

#define RES_WIDTH 800
#define RES_HEIGHT 600

#define BOUNCER_SIZE 10
#define GRID 4.0
#define SPEED 20

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  Window win;
  // ALLEGRO_BITMAP *image, *image1, *image2;

  bool redraw = true, sair = false;
  float bouncer_x, bouncer_y, bouncer_dx, bouncer_dy;
  int mouse_button_state, ground;

  win = graphinit(RES_WIDTH, RES_HEIGHT);

  // image1 = imagemArq("Bug_01.png", BOUNCER_SIZE, BOUNCER_SIZE,win);
  // image2 = imagemArq("Bug_02.png", BOUNCER_SIZE, BOUNCER_SIZE, win);
  // image = image1;

  // bouncer_x = win.disp_data.width / 2.0 - BOUNCER_SIZE / 2.0;
  // bouncer_y = win.disp_data.height / 2.0 - BOUNCER_SIZE / 2.0;

  bouncer_x = win.disp_data.width / 2;
  bouncer_y = win.disp_data.height - BOUNCER_SIZE;
  bouncer_dx = SPEED;
  bouncer_dy = -SPEED;
  

  /* Enquanto  a tecla ESC  n�o for  pressionada, ir� executar
     o c�digo especificado
  */
  // int i = 0;
  ALLEGRO_EVENT ev;
  float mouse_x, mouse_y;
  double direction;
//   mouse_button_state = 2;
  int speed = SPEED;

  while (!sair) 
  {
      
      al_wait_for_event(win.event_queue, &ev);

      ground = false;
      // button_up = false;
      
      switch (ev.type) 
      {
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            
            mouse_button_state = true;
            mouse_x = ev.mouse.x;
            mouse_y = ev.mouse.y;

            if((mouse_x <= win.disp_data.width * 0.25) || (mouse_x >= win.disp_data.width * 0.75))
                speed = SPEED * 1.05;
            else 
                speed = SPEED;

            direction = atan2(mouse_y - bouncer_y , mouse_x - bouncer_x);
            bouncer_dx = speed * cos(direction);
            bouncer_dy = speed * sin(direction);

            break;
    
        case ALLEGRO_EVENT_TIMER:
            
            if(mouse_button_state)
            {
                  if(bouncer_x < BOUNCER_SIZE || bouncer_x > win.disp_data.width - BOUNCER_SIZE)
                    bouncer_dx = -bouncer_dx;
                  
                  if(bouncer_y < BOUNCER_SIZE)
                    bouncer_dy = -bouncer_dy;

                  // bouncer_y > 590
                  if (bouncer_y > win.disp_data.height - BOUNCER_SIZE)
                  {
                      ground = 1;
                      printf("ground: %d\n", ground);
                      mouse_button_state = false;
                      bouncer_y = win.disp_data.height - BOUNCER_SIZE;
                  }
                  else
                  { 
                    bouncer_x += bouncer_dx;
                    bouncer_y += bouncer_dy;
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
      al_draw_filled_circle(bouncer_x, bouncer_y, BOUNCER_SIZE, al_map_rgb(255,255,255));
      
      al_flip_display();
    }
  } 

  graphdeinit(win);
}