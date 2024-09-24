#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

// Adicione as declara��es das vari�veis de som aqui
ALLEGRO_SAMPLE* som_menu = NULL;
ALLEGRO_SAMPLE_INSTANCE* inst_som_menu = NULL;

int inicializar_componentes_allegro(ALLEGRO_BITMAP** background, ALLEGRO_BITMAP** config_background,
    ALLEGRO_BITMAP** choose_maps_background, ALLEGRO_DISPLAY** display,
    ALLEGRO_EVENT_QUEUE** event_queue) {

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

    if (!al_install_mouse()) {
        fprintf(stderr, "Falha ao inicializar o mouse!\n");
        return -1;
    }

    if (!al_install_audio()) {
        fprintf(stderr, "Falha ao inicializar o �udio!\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar o codec de �udio!\n");
        return -1;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Falha ao reservar �udio!\n");
        return -1;
    }

    *background = al_load_bitmap("menu.png");
    if (!*background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
        return -1;
    }

    *config_background = al_load_bitmap("config.jpg");
    if (!*config_background) {
        fprintf(stderr, "Falha ao carregar a imagem de configura��o!\n");
        al_destroy_bitmap(*background);
        return -1;
    }

    *choose_maps_background = al_load_bitmap("choose_maps.jpg");
    if (!*choose_maps_background) {
        fprintf(stderr, "Falha ao carregar a imagem de mapas!\n");
        al_destroy_bitmap(*background);
        al_destroy_bitmap(*config_background);
        return -1;
    }

    // CARREGA SONS
    som_menu = al_load_sample("menu.ogg");
    if (!som_menu) {
        fprintf(stderr, "Falha ao carregar o arquivo de �udio!\n");
        al_destroy_bitmap(*background);
        al_destroy_bitmap(*config_background);
        al_destroy_bitmap(*choose_maps_background);
        return -1;
    }

    // CRIA A INSTANCIA DO SONS
    inst_som_menu = al_create_sample_instance(som_menu);
    if (!inst_som_menu) {
        fprintf(stderr, "Falha ao criar a inst�ncia de �udio!\n");
        al_destroy_sample(som_menu);
        al_destroy_bitmap(*background);
        al_destroy_bitmap(*config_background);
        al_destroy_bitmap(*choose_maps_background);
        return -1;
    }

    int largura_da_imagem = al_get_bitmap_width(*background);
    int altura_da_imagem = al_get_bitmap_height(*background);
    *display = al_create_display(largura_da_imagem, altura_da_imagem);
    if (!*display) {
        fprintf(stderr, "Falha ao criar a janela!\n");
        al_destroy_sample(som_menu);
        al_destroy_sample_instance(inst_som_menu);
        al_destroy_bitmap(*background);
        al_destroy_bitmap(*config_background);
        al_destroy_bitmap(*choose_maps_background);
        return -1;
    }

    *event_queue = al_create_event_queue();
    if (!*event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos!\n");
        al_destroy_sample(som_menu);
        al_destroy_sample_instance(inst_som_menu);
        al_destroy_bitmap(*background);
        al_destroy_bitmap(*config_background);
        al_destroy_bitmap(*choose_maps_background);
        al_destroy_display(*display);
        return -1;
    }

    return 0;
}
