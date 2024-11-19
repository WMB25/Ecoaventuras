#include <stdio.h>
#include <math.h> 
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

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



#define quantidade_jacare 2
#define largura_tela 1350

typedef struct {
    ALLEGRO_BITMAP* img_jacare;
    float jacare_x, jacare_y;
    float velocidade;
} jacare;

void inicializar_jacare(jacare jacare[], AllegroRecursos* recursos) {
    inicializar_componentes_allegro(recursos);
    srand(time(NULL));

    jacare[0].img_jacare = recursos->img_jacare;
    jacare[0].jacare_x = 1300; 
    jacare[0].jacare_y = 280;
    jacare[0].velocidade = 2.0f + (rand() % 4);

    jacare[1].img_jacare = recursos->img_jacare;
    jacare[1].jacare_x = 1300;  
    jacare[1].jacare_y = 450;
    jacare[1].velocidade = 3.0f + (rand() % 5);
}



void jacare_mover(AllegroRecursos* recursos, jacare jacare[]) {
    for (int i = 0; i < quantidade_jacare; i++) {
  
        jacare[i].jacare_x += jacare[i].velocidade;  

        if (jacare[i].jacare_x <= 300) {
            jacare[i].velocidade = fabs(jacare[i].velocidade);
            al_draw_bitmap(jacare[i].img_jacare, jacare[i].jacare_x, jacare[i].jacare_y, ALLEGRO_FLIP_HORIZONTAL);
        }
        else if (jacare[i].jacare_x >= 1300) {
            jacare[i].velocidade = -fabs(jacare[i].velocidade);  
            al_draw_bitmap(jacare[i].img_jacare, jacare[i].jacare_x, jacare[i].jacare_y, 0);
        } else {
            if (jacare[i].velocidade < 0) {
                al_draw_bitmap(jacare[i].img_jacare, jacare[i].jacare_x, jacare[i].jacare_y, 0);
            } else {
                al_draw_bitmap(jacare[i].img_jacare, jacare[i].jacare_x, jacare[i].jacare_y, ALLEGRO_FLIP_HORIZONTAL);
            }
        }
    }
}