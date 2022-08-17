#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  Window win;
  // ALLEGRO_BITMAP *image, *image1, *image2;

  bool redraw = true, sair = false;
  float bouncer_x, bouncer_y, bouncer_dx = GRID, bouncer_dy = -GRID;
  bool mouse_button_state, ground;

  win = graphinit(RES_WIDTH, RES_HEIGHT);

  // image1 = imagemArq("Bug_01.png", BOUNCER_SIZE, BOUNCER_SIZE,win);
  // image2 = imagemArq("Bug_02.png", BOUNCER_SIZE, BOUNCER_SIZE, win);
  // image = image1;

  // bouncer_x = win.disp_data.width / 2.0 - BOUNCER_SIZE / 2.0;
  // bouncer_y = win.disp_data.height / 2.0 - BOUNCER_SIZE / 2.0;

  bouncer_x = win.disp_data.width / 2;
  bouncer_y = win.disp_data.height - BOUNCER_SIZE;

  /* Enquanto  a tecla ESC  n�o for  pressionada, ir� executar
     o c�digo especificado
  */
  // int i = 0;
  while (!sair) 
  {
      ALLEGRO_EVENT ev;
      al_wait_for_event(win.event_queue, &ev);

      ground = false;
      // button_up = false;
      
      switch (ev.type) 
      {
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
          mouse_button_state = true;
          break;
    
        case ALLEGRO_EVENT_TIMER:
            
            if(mouse_button_state)
            {
              while(!ground)
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
                      bouncer_dx = GRID;
                      bouncer_dy = -GRID;
                      bouncer_y = win.disp_data.height - BOUNCER_SIZE;
                  }
                  else
                  {
                      bouncer_x += bouncer_dx;
                      bouncer_y += bouncer_dy;
                  }
                  
                  redraw = true;
                  break;
              }
            }
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE: 
            sair = true; 
            break;
    
        case ALLEGRO_EVENT_KEY_UP:
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) sair = true;
                break;

        // case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
        //   image1 = image;
        //   image = image2;
        //   image2 = image1;
        //   break;

        // case ALLEGRO_EVENT_MOUSE_AXES:
        // case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
        //     bouncer_x = ev.mouse.x;
        //     bouncer_y = ev.mouse.y;
        //     break;
    }

    if(redraw && al_is_event_queue_empty(win.event_queue)) 
    {
      redraw = false;
      al_clear_to_color(VERDE);
      // al_draw_bitmap(image, bouncer_x, bouncer_y, 0);
      al_draw_filled_circle(bouncer_x, bouncer_y, BOUNCER_SIZE, al_map_rgb(255,255,255));
      
      al_flip_display();
    }
  } 

  // al_destroy_bitmap(image)

  graphdeinit(win);
}