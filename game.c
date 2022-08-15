#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define BUFFER_WIDTH    320
#define BUFFER_HEIGHT   240

#define DISPLAY_SCALE   2
#define DISPLAY_WIDTH   (BUFFER_WIDTH    * DISPLAY_SCALE)
#define DISPLAY_HEIGHT  (BUFFER_HEIGHT   * DISPLAY_SCALE)

enum BOUNCER_TYPE 
{
    BT_CIRCLE,
    BT_N
};

typedef struct BOUNCER
{
    float x, y;
    float dx, dy;
    int type;
} BOUNCER;

void must_init(bool test, const char *description)
{
    if(test) 
        return;
    
    printf("Erro: Inicialização de %s falhou.\n", description);
    exit(1);
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    // Suaviza formas do display
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    ALLEGRO_BITMAP* buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    
    must_init(display, "display");
    must_init(buffer, "buffer");

    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_install_mouse(), "mouse");
    
    // must_init(al_init_image_addon(), "image addon");
    // ALLEGRO_BITMAP *ball = al_load_bitmap("ball.png");
    // // must_init(ball, "ball");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;

    BOUNCER obj[BT_N];
    for(int i = 0; i < BT_N; i++)
    {
        BOUNCER* b = &obj[i];
        b->x = rand() % 640;
        b->y = rand() % 480;
        b->dx = ((((float)rand()) / RAND_MAX) - 0.5) * 2 * 4;
        b->dy = ((((float)rand()) / RAND_MAX) - 0.5) * 2 * 4;
        b->type = i;
        // we get one of every BOUNCER_TYPE
    }

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                
                for(int i = 0; i < BT_N; i++)
                {
                    BOUNCER* b = &obj[i];
                    b->x += b->dx;
                    b->y += b->dy;

                    if(b->x < 0)
                    {
                        b->x  *= -1;
                        b->dx *= -1;
                    }
                    if(b->x > 640)
                    {
                        b->x -= (b->x - 640);
                        b->dx *= -1;
                    }
                    if(b->y < 0)
                    {
                        b->y  *= -1;
                        b->dy *= -1;
                    }
                    if(b->y > 480)
                    {
                        b->x -= (b->y - 480);
                        b->dy *= -1;
                    }
                }

                redraw = true;
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                al_get_mouse_state(&mouse_state);

                // if(click_play_button(&mouse_state)) 
                // {
                //     fprintf(stderr,"Botão play foi acionado.");
                // }
                // break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if(event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
                    break;      

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0,0,0));

            for(int i = 0; i < BT_N; i++)
            {
                BOUNCER* b = &obj[i];
                switch(b->type)
                {
                    case BT_CIRCLE:
                        al_draw_filled_circle(b->x, b->y, 8, al_map_rgb(255,255,255));
                        break;
                }
            } 
            
            al_flip_display();

            redraw = false;
        }
    }

    // al_destroy_bitmap(ball);    
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}




   