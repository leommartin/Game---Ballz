/* Bibliotecas Padrão */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Allegro */
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "graphicStructure.h"

void must_init(bool test, const char *description)
{
    if(test) 
        return;
    
    printf("Erro: Inicialização de %s falhou.\n", description);
    exit(1);
}

gameWindow gameGraphicInit(int res_width, int res_height)
{
    gameWindow window = { NULL, NULL, NULL, NULL};
    
    /* Inicializa a Allegro */
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");

    /* Suaviza formas do display */
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_font_addon(), "font");
    must_init(al_init_ttf_addon(), "ttf");
    
    /* Cria e define tamanho do Display e Buffer */
    al_set_new_display_flags(ALLEGRO_WINDOWED); 
    
    window.display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    must_init(window.display, "display");

    // window.buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    // must_init(window.buffer, "buffer");

    /* Instala o handler de timer do Allegro */
    window.timer = al_create_timer( _60FPS_ );
    must_init(window.timer, "timer" );
    
    /* Cria uma fila de eventos */
    window.event_queue = al_create_event_queue();
    must_init(window.event_queue, "queue");
    
    /* Registra os possíveis eventos */
    al_register_event_source(window.event_queue, al_get_mouse_event_source());
    al_register_event_source(window.event_queue, al_get_keyboard_event_source());
    al_register_event_source(window.event_queue, al_get_display_event_source(window.display));
    al_register_event_source(window.event_queue, al_get_timer_event_source(window.timer));
    
    /* Insere Background Preto e inicia o timer */
    al_clear_to_color(PRETO);
    al_flip_display();
    al_start_timer(window.timer);
    
    return (window);
}

void gameGraphicDeinit(gameWindow win)
{
  al_destroy_timer(win.timer);
  al_destroy_event_queue(win.event_queue);
  al_destroy_display(win.display);
}