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

#define BOUNCER_SIZE 10
#define GRID         4.0
#define SPEED        20
#define SPEED2       (SPEED * 1.05)


struct ball
{
  float x,y;
  float dx,dy;
};
typedef struct ball ball_t;

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  Window win;
  
  bool redraw = true, sair = false;
  
  win = graphinit(RES_WIDTH, RES_HEIGHT);
  
  ALLEGRO_EVENT ev;
  float mouse_x, mouse_y;
  double direction;
  float bouncer_x, bouncer_y, bouncer_dx, bouncer_dy;
  int speed = SPEED;


  bouncer_x = win.disp_data.width / 2;
  bouncer_y = win.disp_data.height - BOUNCER_SIZE;
  bouncer_dx = SPEED;
  bouncer_dy = -SPEED;

  bool mouse_button_state, ground = true;
  ball_t balls_array[5];
  
  while (!sair) 
  {
      
      al_wait_for_event(win.event_queue, &ev);

      switch (ev.type) 
      {

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

            mouse_button_state = true;
            if(ground)
            {
              mouse_x = ev.mouse.x;
              mouse_y = ev.mouse.y;

              if((mouse_x <= win.disp_data.width * 0.25) || (mouse_x >= win.disp_data.width * 0.75))
                  speed = SPEED2;
              else 
                  speed = SPEED;

              direction = atan2(mouse_y - bouncer_y , mouse_x - bouncer_x);
              bouncer_dx = speed * cos(direction);
              bouncer_dy = speed * sin(direction);

              ground = false;
            }

            break;
    
        case ALLEGRO_EVENT_TIMER:
            
            if(mouse_button_state)
            {
                  if(bouncer_x <= BOUNCER_SIZE || bouncer_x >= win.disp_data.width - BOUNCER_SIZE)
                    bouncer_dx = -bouncer_dx;
                  
                  if(bouncer_y <= BOUNCER_SIZE)
                    bouncer_dy = -bouncer_dy;

                  if (bouncer_y > win.disp_data.height - BOUNCER_SIZE)
                  {
                      ground = true;
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