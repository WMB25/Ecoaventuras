#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

typedef struct {
    ALLEGRO_SAMPLE* som_menu;
    ALLEGRO_SAMPLE_INSTANCE* inst_som_menu;
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* config_background;
    ALLEGRO_BITMAP* escolher_mapas_background;
    ALLEGRO_BITMAP* bg_mapa_branco;
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



#define quantidade_bois 4
#define largura_tela 1350

typedef struct {
    ALLEGRO_BITMAP* img_boi;
    float boi_x, boi_y;
    float velocidade;
} boi;

void inicializar_bois(boi bois[], AllegroRecursos* recursos) {

    inicializar_componentes_allegro(recursos);
    srand(time(NULL));

    bois[0].img_boi = recursos->img_boi;
    bois[0].boi_x = -50;
    bois[0].boi_y = 280;
    bois[0].velocidade = 2.0f + (rand() % 4);

    bois[1].img_boi = recursos->img_boi;
    bois[1].boi_x = -150;
    bois[1].boi_y = 350;
    bois[1].velocidade = 2.0f + (rand() % 4);

    bois[2].img_boi = recursos->img_boi;
    bois[2].boi_x = -250;
    bois[2].boi_y = 490;
    bois[2].velocidade = 2.0f + (rand() % 4);

    bois[3].img_boi = recursos->img_boi;
    bois[3].boi_x = -350;
    bois[3].boi_y = 550;
    bois[3].velocidade = 2.0f + (rand() % 4);
}


void desenhar_bois_e_mover(AllegroRecursos* recursos, boi bois[]) {
    for (int i = 0; i < quantidade_bois; i++) {
        bois[i].boi_x += bois[i].velocidade;

        if (bois[i].boi_x > largura_tela) {
            bois[i].boi_x = -150;
            bois[i].velocidade = 2.0f + (rand() % 4);  
        }
        al_draw_bitmap(bois[i].img_boi, bois[i].boi_x, bois[i].boi_y, 0);
    }
}