#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

// Struct para agrupar os ponteiros 
struct AllegroRecursos {
    ALLEGRO_SAMPLE* som_menu;
    ALLEGRO_SAMPLE_INSTANCE* inst_som_menu;
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* config_background;
    ALLEGRO_BITMAP* escolher_mapas_background;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
};

int inicializar_componentes_allegro(struct AllegroRecursos* recursos) {
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
        fprintf(stderr, "Falha ao inicializar o áudio!\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar o codec de áudio!\n");
        return -1;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Falha ao reservar áudio!\n");
        return -1;
    }

    recursos->background = al_load_bitmap("menu.png");
    if (!recursos->background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
        return -1;
    }

    recursos->config_background = al_load_bitmap("config.jpg");
    if (!recursos->config_background) {
        fprintf(stderr, "Falha ao carregar a imagem de configuração!\n");
        return -1;
    }

    recursos->escolher_mapas_background = al_load_bitmap("escolher_mapas.jpg");
    if (!recursos->escolher_mapas_background) {
        fprintf(stderr, "Falha ao carregar a imagem de escolher mapas!\n");
        return -1;
    }

    recursos->som_menu = al_load_sample("menu.ogg");
    if (!recursos->som_menu) {
        fprintf(stderr, "Falha ao carregar o som do menu!\n");
        return -1;
    }

    recursos->inst_som_menu = al_create_sample_instance(recursos->som_menu);
    if (!recursos->inst_som_menu) {
        fprintf(stderr, "Falha ao criar a instância do som!\n");
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

int main(int argc, char** argv) {

    struct AllegroRecursos recursos;

    if (inicializar_componentes_allegro(&recursos) != 0) {
        return -1;
    }

    // AREA --> VARIAVEIS DE CONTROLE
    int largura_da_imagem = 0;
    int altura_da_imagem = 0;
    int imagem_fundo = 0; // 0: Imagem Menu, 1: Imagem Config, 2: Imagem escolher_mapas
    bool jogo_rodando = true;
    bool tocar_som_menu = false;

    // STRUCTS PARA ÁREAS CLICÁVEIS
    struct area {
        int x;
        int y;
        int largura;
        int altura;
    };

    // Structs para áreas clicáveis
    struct area area_config = { 515, 405, 250, 100 };
    struct area area_mapa = { 541, 290, 200, 100 };
    struct area area_desligar_som = { 320, 600, 220, 70 };
    struct area area_ligar_som = { 740, 600, 220, 70 };
    struct area area_voltar = { 165, 40, 200, 90 };

    // Areas abaixo não foram vinculadas a nada ainda apenas criada
    struct area escolha_amazonia = { 30, 45, 350, 350 };
    struct area escolha_cerrado = { 470, 45, 350, 350 };
    struct area escolha_caatinga = { 900, 45, 350, 350 };
    struct area escola_mataAtlantica = { 30, 430, 350, 350 };
    struct area escolha_pampa = { 470, 430, 350, 350 };
    struct area escolha_pantanal = { 900, 430, 350, 350 };

    al_set_sample_instance_playmode(recursos.inst_som_menu, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(recursos.inst_som_menu, al_get_default_mixer());

    // Toca o som apenas no menu (quando o fundo for 0 == Imagem menu)
    if (imagem_fundo == 0 && tocar_som_menu) {
        al_play_sample_instance(recursos.inst_som_menu);
    }

    // Registra os eventos de mouse e de fechar a janela no (X)
    al_register_event_source(recursos.event_queue, al_get_display_event_source(recursos.display));
    al_register_event_source(recursos.event_queue, al_get_mouse_event_source());

    // Loop principal de eventos
    while (jogo_rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(recursos.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogo_rodando = false;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Verifica se clicou na área de configurações 
            if (event.mouse.x >= area_config.x && event.mouse.x <= area_config.x + area_config.largura &&
                event.mouse.y >= area_config.y && event.mouse.y <= area_config.y + area_config.altura) {
                printf("Evento --> Clique Registrado na área configurações\n");
                imagem_fundo = 1;
                if (tocar_som_menu) {
                    al_stop_sample_instance(recursos.inst_som_menu);
                    tocar_som_menu = false;
                }
            }

            // Verifica se clicou na área de jogar
            if (event.mouse.x >= area_mapa.x && event.mouse.x <= area_mapa.x + area_mapa.largura &&
                event.mouse.y >= area_mapa.y && event.mouse.y <= area_mapa.y + area_mapa.altura) {
                printf("Evento --> Clique Registrado na área jogar\n");
                imagem_fundo = 2;
                if (tocar_som_menu) {
                    al_stop_sample_instance(recursos.inst_som_menu);
                    tocar_som_menu = false;
                }
            }

            // Verifica se clicou na área de voltar
            if (imagem_fundo == 1 && event.mouse.x >= area_voltar.x && event.mouse.x <= area_voltar.x + area_voltar.largura &&
                event.mouse.y >= area_voltar.y && event.mouse.y <= area_voltar.y + area_voltar.altura) {
                printf("Evento --> Clique Registrado na área voltar\n");
                imagem_fundo = 0;
                if (tocar_som_menu) {
                    al_play_sample_instance(recursos.inst_som_menu);
                }
            }

            // Verifica se clicou na área de desligar som
            if (imagem_fundo == 1 && event.mouse.x >= area_desligar_som.x && event.mouse.x <= area_desligar_som.x + area_desligar_som.largura &&
                event.mouse.y >= area_desligar_som.y && event.mouse.y <= area_desligar_som.y + area_desligar_som.altura) {
                printf("Evento --> Clique Registrado na área desligar som\n");
                al_stop_sample_instance(recursos.inst_som_menu);
                tocar_som_menu = false;
            }

            // Verifica se clicou na área de ligar som
            if (imagem_fundo == 1 && event.mouse.x >= area_ligar_som.x && event.mouse.x <= area_ligar_som.x + area_ligar_som.largura &&
                event.mouse.y >= area_ligar_som.y && event.mouse.y <= area_ligar_som.y + area_ligar_som.altura) {
                printf("Evento --> Clique Registrado na área ligar som\n");
                al_play_sample_instance(recursos.inst_som_menu);
                tocar_som_menu = true;
            }
        }

        // Desenhar a imagem de fundo de acordo com a variável imagem_fundo
        if (imagem_fundo == 0) {
            al_draw_bitmap(recursos.background, 0, 0, 0);
        } else if (imagem_fundo == 1) {
            al_draw_bitmap(recursos.config_background, 0, 0, 0);
        } else if (imagem_fundo == 2) {
            al_draw_bitmap(recursos.escolher_mapas_background, 0, 0, 0);
        }

        al_flip_display();
    }

    // Libera recursos
    al_destroy_sample_instance(recursos.inst_som_menu);
    al_destroy_sample(recursos.som_menu);
    al_destroy_bitmap(recursos.background);
    al_destroy_bitmap(recursos.config_background);
    al_destroy_bitmap(recursos.escolher_mapas_background);
    al_destroy_event_queue(recursos.event_queue);
    al_destroy_display(recursos.display);

    return 0;
}
