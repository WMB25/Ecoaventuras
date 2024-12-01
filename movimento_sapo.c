//#include <stdio.h>
//#include <stdlib.h>
//#include <allegro5/allegro.h>
//#include <allegro5/allegro_image.h>
//#include <allegro5/allegro_primitives.h>
//#include <allegro5/allegro_audio.h>
//#include <allegro5/allegro_acodec.h>
//#include <allegro5/keyboard.h>
//#include <allegro5/allegro_font.h>
//#include <allegro5/allegro_ttf.h>
//#include <time.h>
//#include <math.h>
//
//typedef struct {
//    ALLEGRO_BITMAP* background;
//    ALLEGRO_BITMAP* config_background;
//    ALLEGRO_BITMAP* escolher_mapas_background;
//    ALLEGRO_BITMAP* sprite_sapo;
//    ALLEGRO_BITMAP* img_pantanal;
//    ALLEGRO_BITMAP* img_pampa;
//    ALLEGRO_BITMAP* img_caatinga;
//    ALLEGRO_BITMAP* img_boi;
//    ALLEGRO_BITMAP* img_sobre_biomas;
//    ALLEGRO_BITMAP* img_amazonia;
//    ALLEGRO_BITMAP* img_sobre_caatinga;
//    ALLEGRO_BITMAP* img_sobre_pantanal;
//    ALLEGRO_BITMAP* img_sobre_pampa;
//    ALLEGRO_BITMAP* img_sobre_amazonia;
//    ALLEGRO_BITMAP* img_jacare;
//    ALLEGRO_BITMAP* img_fim;
//    ALLEGRO_BITMAP* fundo_pergunta;
//    ALLEGRO_BITMAP* SPRITE;
//    ALLEGRO_FONT* font;
//    ALLEGRO_TIMER* timer;
//    ALLEGRO_DISPLAY* display;
//    ALLEGRO_EVENT_QUEUE* event_queue;
//    ALLEGRO_SAMPLE* som_menu;
//    ALLEGRO_SAMPLE_INSTANCE* inst_som_menu;
//} AllegroRecursos;
//
//typedef struct {
//    int frame_largura;
//    int frame_altura;
//    int num_frames;
//    int frame;
//    int courrent_frame;
//    float x;
//    float y;
//    float velocidade_puloX;
//    float velocidade_puloY;
//    float gravidade;
//    bool pulo;
//}frog;
//frog sapo = { 64, 64, 4, 0.f, 64, 20, 400, -5.0f, -0.0f, 0.5f, false };
//
//void iniciar_sapo(ALLEGRO_EVENT event) {
//    // Processar entrada do jogador
//    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
//        if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
//            sapo->courrent_frame = 64;
//            sapo->x += 5;
//        }
//        if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
//            sapo->courrent_frame = 64;
//            sapo->x -= 5;
//        }
//        if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
//            sapo->courrent_frame = 64;
//            sapo->y += 5;
//        }
//        if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
//            sapo->courrent_frame = 64;
//            sapo->y -= 5;
//        }
//        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !sapo->pulo) {
//            sapo->pulo = true;
//            sapo->velocidade_puloY = sapo->velocidade_puloX; // Força inicial do pulo
//        }
//    }
//
//    // Atualizar pulo (se ativo)
//    if (sapo->pulo) {
//        sapo->y += sapo->velocidade_puloY;
//        sapo->velocidade_puloY += sapo->gravidade;
//        if (sapo->y >= 300) { // Chegou ao chão
//            sapo->y = 300;
//            sapo->velocidade_puloY = 0;
//            sapo->pulo = false;
//        }
//    }
//
//    // Garantir que o sapo fique dentro dos limites da tela
//    if (sapo->x < 0) {
//        sapo->x = 0;
//    }
//    if (sapo->x > 1280 - sapo->frame) {
//        sapo->x = 1280 - sapo->frame;
//    }
//}
