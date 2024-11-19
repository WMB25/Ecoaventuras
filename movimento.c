#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#define LARGURA_TELA 1280
#define ALTURA_TELA 800
#define LARGURA_PERSONAGEM 191
#define ALTURA_PERSONAGEM 161

typedef struct {
    float frame;
    int pos_x, pos_y, current_frame_y;
} mov_p;

mov_p mov_prs = { 0.f, 300, 10, 161 };

void movimento_personagem(ALLEGRO_EVENT event) {

    if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        mov_prs.current_frame_y = 128;
        mov_prs.pos_x += 20;
    }

    else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        mov_prs.current_frame_y = 128 * 3;
        mov_prs.pos_x -= 20;
    }

    else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        mov_prs.current_frame_y = 128 * 2;
        mov_prs.pos_y += 20;
    }

    else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
        mov_prs.current_frame_y = 0;
        mov_prs.pos_y -= 20;
    }

    if (mov_prs.pos_x < 0) {
        mov_prs.pos_x = 0; 
    }
    if (mov_prs.pos_x + LARGURA_PERSONAGEM > LARGURA_TELA) {
        mov_prs.pos_x = LARGURA_TELA - LARGURA_PERSONAGEM;  
    }
    if (mov_prs.pos_y < 0) {
        mov_prs.pos_y = 0;  
    }
    if (mov_prs.pos_y + ALTURA_PERSONAGEM > ALTURA_TELA) {
        mov_prs.pos_y = ALTURA_TELA - ALTURA_PERSONAGEM;  
    }

    mov_prs.frame += 0.3f;
    if (mov_prs.frame > 3) {
        mov_prs.frame -= 3;
    }
}
