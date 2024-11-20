#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

typedef struct {
    ALLEGRO_SAMPLE* som_menu;
    ALLEGRO_SAMPLE_INSTANCE* inst_som_menu;
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* config_background;
    ALLEGRO_BITMAP* escolher_mapas_background;
    ALLEGRO_BITMAP* imagem_grama;
    ALLEGRO_BITMAP* sprite_sapo;
    ALLEGRO_BITMAP* grama_amazonia;
    ALLEGRO_BITMAP* img_pantanal;
    ALLEGRO_BITMAP* img_pampa;
    ALLEGRO_BITMAP* img_caatinga;
    ALLEGRO_BITMAP* img_boi;
    ALLEGRO_BITMAP* img_sobre_biomas;
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* img_amazonia;
    ALLEGRO_BITMAP* SPRITE;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_BITMAP* img_sobre_caatinga;
    ALLEGRO_BITMAP* img_sobre_pantanal;
    ALLEGRO_BITMAP* img_sobre_pampa;
    ALLEGRO_BITMAP* img_sobre_amazonia;
    ALLEGRO_BITMAP* img_jacare;
    ALLEGRO_BITMAP* img_fim;
} AllegroRecursos;



void limpar_recursos(AllegroRecursos* recursos) {
    al_destroy_bitmap(recursos->background);
    al_destroy_bitmap(recursos->config_background);
    al_destroy_bitmap(recursos->SPRITE);
    al_destroy_bitmap(recursos->img_pampa);
    al_destroy_bitmap(recursos->img_pantanal);
    al_destroy_bitmap(recursos->img_caatinga);
    al_destroy_bitmap(recursos->img_boi);
    al_destroy_bitmap(recursos->img_sobre_biomas);
    al_destroy_bitmap(recursos->escolher_mapas_background);
    al_destroy_bitmap(recursos->sprite_sapo);
    al_destroy_bitmap(recursos->img_amazonia);
    al_destroy_bitmap(recursos->img_sobre_caatinga);
    al_destroy_bitmap(recursos->img_sobre_pantanal);
    al_destroy_bitmap(recursos->img_sobre_pampa);
    al_destroy_bitmap(recursos->img_sobre_amazonia);
    al_destroy_bitmap(recursos->img_jacare);
    al_destroy_bitmap(recursos->img_fim);
    al_destroy_font(recursos->font);
    al_destroy_sample(recursos->som_menu);
    al_destroy_sample_instance(recursos->inst_som_menu);
    al_destroy_display(recursos->display);
    al_destroy_event_queue(recursos->event_queue);
}
