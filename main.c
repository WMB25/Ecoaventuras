#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

int main(int argc, char** argv) {

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

    ALLEGRO_BITMAP* background = al_load_bitmap("menu.png");
    if (!background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
        return -1;
    }

    ALLEGRO_BITMAP* config_background = al_load_bitmap("config.jpg");
    if (!config_background) {
        fprintf(stderr, "Falha ao carregar a imagem de configuração!\n");
        al_destroy_bitmap(background);
        return -1;
    }

    ALLEGRO_BITMAP* choose_maps_background = al_load_bitmap("choose_maps.jpg");
    if (!choose_maps_background) {
        fprintf(stderr, "Falha ao carregar a imagem de mapas!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(config_background);
        return -1;
    }

    int image_width = al_get_bitmap_width(background);
    int image_height = al_get_bitmap_height(background);

    ALLEGRO_DISPLAY* display = al_create_display(image_width, image_height);
    if (!display) {
        fprintf(stderr, "Falha ao criar a janela!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(config_background);
        al_destroy_bitmap(choose_maps_background);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(config_background);
        al_destroy_bitmap(choose_maps_background);
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Evento de clique do mouse (Configurações) --> Tela de Configurações
    int area_config_x = 515;
    int area_config_y = 405;
    int area_config_width = 250;
    int area_config_height = 100;

    // Evento de clique do mouse (Jogar) --> Tela de Escolher Mapas
    int area_maps_x = 541; 
    int area_maps_y = 290; 
    int area_maps_width = 200; 
    int area_maps_height = 100; 

    bool running = true;
    int current_background = 0; // 0: menu, 1: config, 2: choose maps

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        // Isso é pra fechar a janela quando clicar no X
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        // Se o mouse foi clicado enttra no if
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Verifica se a area clicada é a area de configurações
            if (event.mouse.x >= area_config_x && event.mouse.x <= area_config_x + area_config_width &&
                event.mouse.y >= area_config_y && event.mouse.y <= area_config_y + area_config_height) {
                printf("Evento --> Clique Registrado na area configurações\n");
                current_background = 1;
            }

            // Verifica se a area clicada é a area de jogar
            if (event.mouse.x >= area_maps_x && event.mouse.x <= area_maps_x + area_maps_width &&
                event.mouse.y >= area_maps_y && event.mouse.y <= area_maps_y + area_maps_height) {
                printf("Evento --> Clique Registrado na area jogar\n");
                current_background = 2;
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Isso aqui vai desenhar o fundo atual segundo o if de cima
        if (current_background == 1) {
            al_draw_bitmap(config_background, 0, 0, 0);
        }
        else if (current_background == 2) {
            al_draw_bitmap(choose_maps_background, 0, 0, 0);
        }
        else {
            al_draw_bitmap(background, 0, 0, 0);
        }

        al_flip_display();
    }

    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(background);
    al_destroy_bitmap(config_background);
    al_destroy_bitmap(choose_maps_background);
    al_destroy_display(display);

    return 0;
}
