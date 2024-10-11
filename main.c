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
    ALLEGRO_BITMAP* bg_mapa_branco;
    ALLEGRO_BITMAP* imagem_grama;
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

    recursos->imagem_grama = al_load_bitmap("grama.png");
    if (!recursos->imagem_grama) {
        fprintf(stderr, "Falha ao carregar a imagem da grama!\n");
        return -1;
    }

    recursos->bg_mapa_branco = al_load_bitmap("Bg_Mapas_Branco.png");
    if (!recursos->bg_mapa_branco) {
        fprintf(stderr, "Falha ao carregar a imagem de escolher mapas (Branco)!\n");
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


int main() {

    struct AllegroRecursos recursos;

    if (inicializar_componentes_allegro(&recursos) != 0) {
        return -1;
    }

    // AREA --> VARIAVEIS DE CONTROLE
    int largura_da_imagem = 0;
    int altura_da_imagem = 0;
    int imagem_fundo = 0; // 0: Imagem Menu, 1: Imagem Config, 2: Imagem escolher_mapas,  100: Fundo Branco (Caso Precise - Possível remoção)
    bool jogo_rodando = true;
    bool tocar_som_menu = false;


    // AREA --> VARIAVEIS DA MATRIZ
    int xOff = 0;
    int yOff = 0;
    int colunas_mapa = 10;
    int tamanho_mapa = 100;
    int tamanho_titulo = 128;

    int map[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    // STRUCTS PARA areaS CLICÁVEIS
    struct area {
        int x;
        int y;
        int largura;
        int altura;
    };

    // Structs para areas clicáveis
    struct area area_config = { 515, 405, 250, 100 };
    struct area area_jogar = { 541, 290, 200, 100 };
    struct area area_desligar_som = { 320, 600, 220, 70 };
    struct area area_ligar_som = { 740, 600, 220, 70 };
    struct area area_voltar = { 165, 40, 200, 90 };
    struct area escolha_pampa = { 470, 430, 350, 350 };

    // Areas abaixo não foram vinculadas a nada ainda apenas criada
    struct area escolha_amazonia = { 30, 45, 350, 350 };
    struct area escolha_pantanal = { 900, 430, 350, 350 };
    struct area escolha_caatinga = { 900, 45, 350, 350 };
 // struct area escolha_cerrado = { 470, 45, 350, 350 };
 // struct area escola_mataAtlantica = { 30, 430, 350, 350 };

    al_set_sample_instance_playmode(recursos.inst_som_menu, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(recursos.inst_som_menu, al_get_default_mixer());

    if (imagem_fundo == 0 && tocar_som_menu) {
        al_play_sample_instance(recursos.inst_som_menu);
    }

    al_register_event_source(recursos.event_queue, al_get_display_event_source(recursos.display));
    al_register_event_source(recursos.event_queue, al_get_mouse_event_source());

    while (jogo_rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(recursos.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogo_rodando = false;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Verifica Area de Clique --> Configurações
            if (event.mouse.x >= area_config.x && event.mouse.x <= area_config.x + area_config.largura &&
                event.mouse.y >= area_config.y && event.mouse.y <= area_config.y + area_config.altura) {
                printf("Evento --> Clique Registrado na area configurações\n");
                imagem_fundo = 1;
                if (tocar_som_menu) {
                    al_stop_sample_instance(recursos.inst_som_menu);
                    tocar_som_menu = false;
                }
            }

            // Verifica Area de Clique --> Area Jogar
            if (event.mouse.x >= area_jogar.x && event.mouse.x <= area_jogar.x + area_jogar.largura &&
                event.mouse.y >= area_jogar.y && event.mouse.y <= area_jogar.y + area_jogar.altura) {
                printf("Evento --> Clique Registrado na area jogar\n");
                imagem_fundo = 2;
                if (tocar_som_menu) {
                    al_stop_sample_instance(recursos.inst_som_menu);
                    tocar_som_menu = false;
                }
            }

            // Verifica Area de Clique --> Voltar Botão Configurações
            if (imagem_fundo == 1 && event.mouse.x >= area_voltar.x && event.mouse.x <= area_voltar.x + area_voltar.largura &&
                event.mouse.y >= area_voltar.y && event.mouse.y <= area_voltar.y + area_voltar.altura) {
                printf("Evento --> Clique Registrado na area voltar\n");
                imagem_fundo = 0;
                if (tocar_som_menu) {
                    al_play_sample_instance(recursos.inst_som_menu);
                }
            }

            // Verifica Area de Clique --> Desligar Som
            if (imagem_fundo == 1 && event.mouse.x >= area_desligar_som.x && event.mouse.x <= area_desligar_som.x + area_desligar_som.largura &&
                event.mouse.y >= area_desligar_som.y && event.mouse.y <= area_desligar_som.y + area_desligar_som.altura) {
                printf("Evento --> Clique Registrado na area desligar som\n");
                al_stop_sample_instance(recursos.inst_som_menu);
                tocar_som_menu = false;
            }

            // Verifica Area de Clique --> Ligar Som
            if (imagem_fundo == 1 && event.mouse.x >= area_ligar_som.x && event.mouse.x <= area_ligar_som.x + area_ligar_som.largura &&
                event.mouse.y >= area_ligar_som.y && event.mouse.y <= area_ligar_som.y + area_ligar_som.altura) {
                printf("Evento --> Clique Registrado na area ligar som\n");
                al_play_sample_instance(recursos.inst_som_menu);
                tocar_som_menu = true;
            }

            // Verifica Area de Clique --> Pampa
            if (event.mouse.x >= escolha_pampa.x && event.mouse.x <= escolha_pampa.x + escolha_pampa.largura &&
                event.mouse.y >= escolha_pampa.y && event.mouse.y <= escolha_pampa.y + escolha_pampa.altura) {
                printf("Evento --> Clique Registrado na area Pampa\n");
                imagem_fundo = 100;
                tocar_som_menu = true;
            }
        }

        // Desenha Imagem de Fundo com base em sua numeração
        if (imagem_fundo == 0) {
            al_draw_bitmap(recursos.background, 0, 0, 0);
        }
        else if (imagem_fundo == 1) {
            al_draw_bitmap(recursos.config_background, 0, 0, 0);
        }
        else if (imagem_fundo == 2) {
            al_draw_bitmap(recursos.escolher_mapas_background, 0, 0, 0);
        }
        else if (imagem_fundo == 100) {
            al_draw_bitmap(recursos.bg_mapa_branco, 0, 0, 0);

            // Desenha Grama Quando for 0 na matriz
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    if (map[i][j] == 0) {
                        al_draw_bitmap(recursos.imagem_grama, j * tamanho_mapa, i * tamanho_mapa, 0);
                    }
                }
            }
        }

        al_flip_display();
    }


    // LIMPA --> DESTROI TUDO DA MEMORIA
    al_destroy_sample_instance(recursos.inst_som_menu);
    al_destroy_sample(recursos.som_menu);
    al_destroy_bitmap(recursos.background);
    al_destroy_bitmap(recursos.config_background);
    al_destroy_bitmap(recursos.escolher_mapas_background);
    al_destroy_bitmap(recursos.imagem_grama);
    al_destroy_bitmap(recursos.bg_mapa_branco);
    al_destroy_event_queue(recursos.event_queue);
    al_destroy_display(recursos.display);

    return 0;
}