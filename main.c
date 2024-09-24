#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//Incluir Funções de outros arquivos
#include "inicializar.c"

// AREA --> VARIAVEIS - PONTEIROS
ALLEGRO_SAMPLE* som_menu = NULL;
ALLEGRO_SAMPLE_INSTANCE* inst_som_menu = NULL;
ALLEGRO_BITMAP* background = NULL;
ALLEGRO_BITMAP* config_background = NULL;
ALLEGRO_BITMAP* choose_maps_background = NULL;
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;

int largura_da_imagem = 0;
int altura_da_imagem = 0;
int imagem_fundo = 0; // 0: Imagem Menu, 1: Imagem Config, 2: Imagem Choose_maps
bool jogo_rodando = true; //Mantem o programa em execução ate alguém fecha-lo
bool play_som_menu = false;

// STRUCT DE AREA CLICAVEL COM O MOUSE
struct area {
    int x;
    int y;
    int largura;
    int altura;
};

int main(int argc, char** argv) {

    //FUNÇÃO DO INICIALIZAR.C
    if (verificador_inicializasao_allegro(&background, &config_background, &choose_maps_background, &display, &event_queue) != 0) {
        return -1;
    }

    // STRUCTS
    struct area area_config = { 515, 405, 250, 100 };
    struct area area_mapa = { 541, 290, 200, 100 };
    struct area area_desligar_som = { 320, 600, 220, 70 };
    struct area area_ligar_som = { 740, 600, 220, 70 };
    struct area area_voltar = { 165, 40, 200, 90 };

    // CARREGA SONS
    som_menu = al_load_sample("menu.ogg");
    if (!som_menu) {
        fprintf(stderr, "Falha ao carregar o arquivo de áudio!\n");
        return -1;
    }

    // CRIA A INSTANCIA DO SONS
    inst_som_menu = al_create_sample_instance(som_menu);
    if (!inst_som_menu) {
        fprintf(stderr, "Falha ao criar a instância de áudio!\n");
        return -1;
    }

    al_set_sample_instance_playmode(inst_som_menu, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(inst_som_menu, al_get_default_mixer());

    // TOCA O SOM SO NO MENU
    if (imagem_fundo == 0 && play_som_menu) {
        al_play_sample_instance(inst_som_menu);
    }

    // AREA EVENTOS --> CLIQUE DO MOUSE
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Evento de clique do mouse
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {  // Fecha a janela quando clicar no X
            running = false;
        }

        // VERIFICA SE O MOUSE FOI CLICADO
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE CONFIGURAÇÕES
            if (event.mouse.x >= area_config.x && event.mouse.x <= area_config.x + area_config.largura &&
                event.mouse.y >= area_config.y && event.mouse.y <= area_config.y + area_config.altura) {
                printf("Evento --> Clique Registrado na área configurações\n");
                imagem_fundo = 1;
                if (play_som_menu) {
                    al_stop_sample_instance(inst_som_menu);
                    play_som_menu = false;
                }
            }

            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE JOGAR
            if (event.mouse.x >= area_mapa.x && event.mouse.x <= area_mapa.x + area_mapa.largura &&
                event.mouse.y >= area_mapa.y && event.mouse.y <= area_mapa.y + area_mapa.altura) {
                printf("Evento --> Clique Registrado na área jogar\n");
                imagem_fundo = 2;
                if (play_som_menu) {
                    al_stop_sample_instance(inst_som_menu);
                    play_som_menu = false;
                }
            }

            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE VOLTAR PARA O MENU
            if (imagem_fundo == 1 && event.mouse.x >= area_voltar.x && event.mouse.x <= area_voltar.x + area_voltar.largura &&
                event.mouse.y >= area_voltar.y && event.mouse.y <= area_voltar.y + area_voltar.altura) {
                printf("Evento --> Clique Registrado na área voltar\n");
                imagem_fundo = 0;
                if (play_som_menu) {
                    al_play_sample_instance(inst_som_menu);
                }
            }

            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE DESLIGAR SOM
            if (imagem_fundo == 1 && event.mouse.x >= area_desligar_som.x && event.mouse.x <= area_desligar_som.x + area_desligar_som.largura &&
                event.mouse.y >= area_desligar_som.y && event.mouse.y <= area_desligar_som.y + area_desligar_som.altura) {
                printf("Evento --> Clique Registrado na área desligar som\n");
                al_stop_sample_instance(inst_som_menu);
                play_som_menu = false;
            }

            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE LIGAR SOM
            if (imagem_fundo == 1 && event.mouse.x >= area_ligar_som.x && event.mouse.x <= area_ligar_som.x + area_ligar_som.largura &&
                event.mouse.y >= area_ligar_som.y && event.mouse.y <= area_ligar_som.y + area_ligar_som.altura) {
                printf("Evento --> Clique Registrado na área ligar som\n");
                if (!play_som_menu) {
                    al_play_sample_instance(inst_som_menu);
                    play_som_menu = true;
                }
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0)); 
        if (imagem_fundo == 0) {
            al_draw_bitmap(background, 0, 0, 0);
        }
        else if (imagem_fundo == 1) {
            al_draw_bitmap(config_background, 0, 0, 0);
        }
        else if (imagem_fundo == 2) {
            al_draw_bitmap(choose_maps_background, 0, 0, 0);
        }

        al_flip_display();
    }

    // DESTROI DA MEMORIA APOS FECHAR
    al_destroy_sample(som_menu);
    al_destroy_sample_instance(inst_som_menu);
    al_destroy_bitmap(background);
    al_destroy_bitmap(config_background);
    al_destroy_bitmap(choose_maps_background);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
