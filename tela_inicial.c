#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define BUFFER_WIDTH    320
#define BUFFER_HEIGHT   240

#define DISPLAY_SCALE   2
#define DISPLAY_WIDTH   (BUFFER_WIDTH    * DISPLAY_SCALE)
#define DISPLAY_HEIGHT  (BUFFER_HEIGHT   * DISPLAY_SCALE)
#define LETTER_HEIGHT   (BUFFER_HEIGHT/4 * DISPLAY_SCALE)

#define PLAY_BUTTON_POS_X1 (DISPLAY_WIDTH/2-100)
#define PLAY_BUTTON_POS_Y1 (DISPLAY_HEIGHT/2)
#define PLAY_BUTTON_POS_X2 (DISPLAY_WIDTH/2+100)
#define PLAY_BUTTON_POS_Y2 (DISPLAY_HEIGHT/2+40)
#define ROUNDED_SIZE       20

#define ROSA               234, 35, 94

void must_init(bool test, const char *description)
{
    if(test) 
        return;
    
    printf("Erro: Inicialização de %s falhou.\n", description);
    exit(1);
}

bool collide(int p1_x1, int p1_y1, int p1_x2, int p1_y2, int p2_x1, int p2_y1, int p2_x2, int p2_y2)
{
    if(p1_x1 > p2_x2)
        return false;
    if(p1_x2 < p2_x1)
        return false;
    if(p1_y1 > p2_y2)
        return false;
    if(p1_y2 < p2_y1)
        return false;

    return true;
}

bool click_play_button(ALLEGRO_MOUSE_STATE *mouse_state)
{
    return collide
    (
        mouse_state->x,
        mouse_state->y,
        mouse_state->x,
        mouse_state->y,
        PLAY_BUTTON_POS_X1,
        PLAY_BUTTON_POS_Y1,
        PLAY_BUTTON_POS_X2,
        PLAY_BUTTON_POS_Y2
    );
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

    must_init(al_init_font_addon(), "font addon");
    must_init(al_init_ttf_addon(), "ttf addon");

    // Cria fonte 
    ALLEGRO_FONT* logoFont = al_load_font("desenho.ttf", 100, 0);
    ALLEGRO_FONT* fontGame = al_load_font("game_over.ttf", 30, 0);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;

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

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                al_get_mouse_state(&mouse_state);

                if(click_play_button(&mouse_state)) 
                {
                    fprintf(stderr,"Botão play foi acionado.");
                }
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
            
            // Desenha texto com a fonte logo ttf
            al_draw_text(logoFont, al_map_rgb(234, 35, 94), DISPLAY_WIDTH/2-80, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "B");
            al_draw_text(logoFont, al_map_rgb(255, 165, 0), DISPLAY_WIDTH/2-20, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "a");
            al_draw_text(logoFont, al_map_rgb(0, 0, 205), DISPLAY_WIDTH/2+20, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "l");
            al_draw_text(logoFont, al_map_rgb(0, 255, 255), DISPLAY_WIDTH/2+50, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "l");
            al_draw_text(logoFont, al_map_rgb(124,252,0), DISPLAY_WIDTH/2+90, LETTER_HEIGHT, ALLEGRO_ALIGN_CENTER, "z");

            // Desenha botão de PLAY
            al_draw_filled_rounded_rectangle(PLAY_BUTTON_POS_X1, PLAY_BUTTON_POS_Y1, PLAY_BUTTON_POS_X2, PLAY_BUTTON_POS_Y2, ROUNDED_SIZE, ROUNDED_SIZE, al_map_rgb(ROSA));
            
            // Desenha palavra PLAY usando o retangulo como referência
            al_draw_text
            (
                fontGame, 
                al_map_rgb(255, 255, 255),
                 (PLAY_BUTTON_POS_X1 + (PLAY_BUTTON_POS_X2 - PLAY_BUTTON_POS_X1)/2), 
                 (PLAY_BUTTON_POS_Y1 + (PLAY_BUTTON_POS_Y2 - PLAY_BUTTON_POS_Y1)/4),
                 ALLEGRO_ALIGN_CENTER, 
                 "PLAY"
            );

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(fontGame);
    al_destroy_font(logoFont);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

// button collor of Ballz (234, 35, 94)

// deeppink 255, 20, 147
// mediumvioletred 199,21,133
// fuchsia 255,0,255