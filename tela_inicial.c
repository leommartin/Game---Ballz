#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define WIDTH 640
#define HEIGHT 480

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    must_init(display, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_primitives_addon(), "primitives");

    // Inicializa addons de fonte
    must_init(al_init_font_addon(), "font addon");
    must_init(al_init_ttf_addon(), "ttf addon");

    // Cria fonte
    ALLEGRO_FONT* displayFont = al_load_font("desenho.ttf", 100, 0);
    ALLEGRO_FONT* fontGame = al_load_font("game_over.ttf", 30, 0);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

   al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // game logic goes here.
                redraw = true;
                break;

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
            
            // Desenha texto com a fonte criada
            al_draw_text(displayFont, al_map_rgb(255, 20, 147), WIDTH/2-80, HEIGHT/4, ALLEGRO_ALIGN_CENTER, "B");
            al_draw_text(displayFont, al_map_rgb(255, 165, 0), WIDTH/2-20, HEIGHT/4, ALLEGRO_ALIGN_CENTER, "a");
            al_draw_text(displayFont, al_map_rgb(0, 0, 205), WIDTH/2+20, HEIGHT/4, ALLEGRO_ALIGN_CENTER, "l");
            al_draw_text(displayFont, al_map_rgb(0, 255, 255), WIDTH/2+50, HEIGHT/4, ALLEGRO_ALIGN_CENTER, "l");
            al_draw_text(displayFont, al_map_rgb(124,252,0), WIDTH/2+90, HEIGHT/4, ALLEGRO_ALIGN_CENTER, "z");

            al_draw_filled_rounded_rectangle(WIDTH/2-95, HEIGHT/2, WIDTH/2+100, HEIGHT/2+40, 20, 20, al_map_rgb(255, 20, 147));
            al_draw_text(fontGame, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT/2+10, ALLEGRO_ALIGN_CENTER, "PLAY");

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_font(displayFont);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

// deeppink 255, 20, 147
// mediumvioletred 199,21,133
// fuchsia 255,0,255