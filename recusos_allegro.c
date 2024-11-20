#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/keyboard.h>
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
    ALLEGRO_BITMAP* pergunta1;
    ALLEGRO_BITMAP * pergunta2;
    ALLEGRO_BITMAP * pergunta3;
    ALLEGRO_BITMAP * pergunta4;
    ALLEGRO_BITMAP * pergunta5;
    ALLEGRO_BITMAP * pergunta6;
    ALLEGRO_BITMAP * pergunta7;
    ALLEGRO_BITMAP * pergunta8;
    ALLEGRO_BITMAP * pergunta9;
    ALLEGRO_BITMAP * pergunta10;
} AllegroRecursos;

int inicializar_componentes_allegro(AllegroRecursos* recursos) {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro!\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar o add-on de imagem!\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar o add-on de primitivas!\n");
        return -1;
    }

    if (!al_init_font_addon()) {
        fprintf(stderr, "Falha ao inicializar o add-on de fontes!\n");
        return -1;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "Falha ao inicializar o mouse!\n");
        return -1;
    }

    if (!al_install_audio()) {
        fprintf(stderr, "Falha ao inicializar o audio!\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar o codec de audio!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        printf("Erro ao instalar o teclado!\n");
        return -1;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Falha ao reservar audio!\n");
        return -1;
    }
    if (!al_init_font_addon()) {
        printf("Erro ao iniciar Leitor de Fonte\n");
        return -1;
    }
    if (!al_init_ttf_addon()) {
        printf("Erro ao iniciar Leitor De fontes Externa!\n");
    }

    recursos->background = al_load_bitmap("menu.jpeg");
    if (!recursos->background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
        return -1;
    }
    recursos->img_fim = al_load_bitmap("img_fim.png");
    if (!recursos->img_fim) {
        fprintf(stderr, "Falha ao carregar a imagem do fim!\n");
        return -1;
    }

    recursos->img_amazonia = al_load_bitmap("img_amazonia.jpg");
    if (!recursos->img_amazonia) {
        fprintf(stderr, "Falha ao carregar a imagem da amazonia!\n");
        return -1;
    }

    recursos->img_sobre_amazonia = al_load_bitmap("img_sobre_amazonia.png");
    if (!recursos->img_sobre_amazonia) {
        fprintf(stderr, "Falha ao carregar a imagem sobre amazonia!\n");
        return -1;
    }

    recursos->img_sobre_pampa = al_load_bitmap("img_sobre_pampa.png");
    if (!recursos->img_sobre_pampa) {
        fprintf(stderr, "Falha ao carregar a imagem sobre pampa!\n");
        return -1;
    }

    recursos->img_sobre_pantanal = al_load_bitmap("img_sobre_pantanal.png");
    if (!recursos->img_sobre_pantanal) {
        fprintf(stderr, "Falha ao carregar a imagem sobre pantanal!\n");
        return -1;
    }

    recursos->img_sobre_caatinga = al_load_bitmap("img_sobre_caatinga.png");
    if (!recursos->img_sobre_caatinga) {
        fprintf(stderr, "Falha ao carregar a imagem sobre caatinga!\n");
        return -1;
    }

    recursos->img_jacare = al_load_bitmap("img_jacare.png");
    if (!recursos->img_jacare) {
        fprintf(stderr, "Falha ao carregar a imagem do jacare!\n");
        return -1;
    }

    recursos->img_boi = al_load_bitmap("img_boi.png");
    if (!recursos->img_boi) {
        fprintf(stderr, "Falha ao carregar a imagem do boi!\n");
        return -1;
    }

    recursos->img_sobre_biomas = al_load_bitmap("sobreBiomas.jpg");
    if (!recursos->img_sobre_biomas) {
        fprintf(stderr, "Falha ao carregar a imagem sobre os biomas!\n");
        return -1;
    }

    recursos->config_background = al_load_bitmap("config.jpg");
    if (!recursos->config_background) {
        fprintf(stderr, "Falha ao carregar a imagem de configura��o!\n");
        return -1;
    }

    recursos->img_pampa = al_load_bitmap("img_pampa.png");
    if (!recursos->img_pampa) {
        fprintf(stderr, "Falha ao carregar a imagem do pampa!\n");
        return -1;
    }

    recursos->img_caatinga = al_load_bitmap("img_caatinga.png");
    if (!recursos->img_caatinga) {
        fprintf(stderr, "Falha ao carregar a imagem da caatinga!\n");
        return -1;
    }

    recursos->grama_amazonia = al_load_bitmap("grama.png");
    if (!recursos->grama_amazonia) {
        fprintf(stderr, "Falha ao carregar a imagem da grama!\n");
        return -1;
    }

    recursos->sprite_sapo = al_load_bitmap("froggy.png");
    if (!recursos->sprite_sapo) {
        printf("Erro ao carregar Sapo\n");
        return -1;
    }

    recursos->font = al_load_ttf_font("Poppins-Medium.ttf", 24, 0);
    if (!recursos->font) {
        printf("Erro ao Carregar Fontes!\n");
    }

    recursos->escolher_mapas_background = al_load_bitmap("escolher_mapas.jpg");
    if (!recursos->escolher_mapas_background) {
        fprintf(stderr, "Falha ao carregar a imagem de escolher mapas!\n");
        return -1;
    }

    recursos->img_pantanal = al_load_bitmap("img_pantanal.jpg");
    if (!recursos->img_pantanal) {
        fprintf(stderr, "Falha ao carregar a imagem do pantanal!\n");
        return -1;
    }

    recursos->som_menu = al_load_sample("menu.ogg");
    if (!recursos->som_menu) {
        fprintf(stderr, "Falha ao carregar o som do menu!\n");
        return -1;
    }

    recursos->SPRITE = al_load_bitmap("personagem.png");
    if (!recursos->SPRITE) {
        fprintf(stderr, "Falha ao carregar o personagem!\n");
        return -1;
    }

    recursos->inst_som_menu = al_create_sample_instance(recursos->som_menu);
    if (!recursos->inst_som_menu) {
        fprintf(stderr, "Falha ao criar a instancia do som!\n");
        return -1;
    }

    recursos->pergunta1 = al_load_bitmap("1_pergunta.png");
    if (!recursos->pergunta1) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 01!\n");
        return -1;
    }

    recursos->pergunta2 = al_load_bitmap("2_pergunta.png");
    if (!recursos->pergunta1) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 02!\n");
        return -1;
    }


    recursos->pergunta3 = al_load_bitmap("3_pergunta.png");
    if (!recursos->pergunta3) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 03!\n");
        return -1;
    }

    recursos->pergunta4 = al_load_bitmap("4_pergunta.png");
    if (!recursos->pergunta4) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 04!\n");
        return -1;
    }

    recursos->pergunta5 = al_load_bitmap("5_pergunta.png");
    if (!recursos->pergunta5) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 05!\n");
        return -1;
    }

    recursos->pergunta6 = al_load_bitmap("6_pergunta.png");
    if (!recursos->pergunta6) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 06!\n");
        return -1;
    }

    recursos->pergunta7 = al_load_bitmap("7_pergunta.png");
    if (!recursos->pergunta7) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 07!\n");
        return -1;
    }

    recursos->pergunta8 = al_load_bitmap("8_pergunta.png");
    if (!recursos->pergunta3) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 08!\n");
        return -1;
    }

    recursos->pergunta9 = al_load_bitmap("9_pergunta.png");
    if (!recursos->pergunta3) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 09!\n");
        return -1;
    }

    recursos->pergunta10 = al_load_bitmap("10_pergunta.png");
    if (!recursos->pergunta10) {
        fprintf(stderr, "Falha ao carregar a imagem pergunta 10!\n");
        return -1;
    }

    recursos->display = al_create_display(1280, 800);
    if (!recursos->display) {
        fprintf(stderr, "Falha ao criar a tela!\n");
        return -1;
    }

    recursos->event_queue = al_create_event_queue();
    if (!recursos->event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos!\n");
        al_destroy_display(recursos->display);
        return -1;
    }

    return 0;
}
