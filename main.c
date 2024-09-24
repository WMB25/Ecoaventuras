#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "inicializar.c"

// AREA --> VARIAVEIS
ALLEGRO_SAMPLE* som_menu = NULL;
ALLEGRO_SAMPLE_INSTANCE* inst_som_menu = NULL;
ALLEGRO_BITMAP* background = NULL;
ALLEGRO_BITMAP* config_background = NULL;
ALLEGRO_BITMAP* choose_maps_background = NULL;
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;

int largura_da_imagem = 0;
int altura_da_imagem = 0;
int current_background = 0; // 0: Imagem Menu, 1: Imagem Config, 2: Imagem Choose_maps
bool running = true;
bool play_som_menu = false; // o audio começa desligado

// AREA --> Posições e dimensões das áreas clicáveis
int area_config_x = 515;
int area_config_y = 405;
int area_config_width = 250;
int area_config_height = 100;

int area_mapa_x = 541;
int area_mapa_y = 290;
int area_mapa_largura = 200;
int area_mapa_altura = 100;

int area_desligar_som_x = 320;
int area_desligar_som_y = 600;
int area_desligar_som_largura = 220;
int area_desligar_som_altura = 70;

int area_ligar_som_x = 740;
int area_ligar_som_y = 600;
int area_ligar_som_largura = 220;
int area_ligar_som_altura = 70;

int area_voltar_x = 165;
int area_voltar_y = 40;
int area_voltar_largura = 200;
int area_voltar_altura = 90;

int main(int argc, char** argv) {

    if (verificador_inicializasao_allegro(&background, &config_background, &choose_maps_background, &display, &event_queue) != 0) { //FUNÇÃO DO INICIALIZAR.C
        return -1;
    }

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
    if (current_background == 0 && play_som_menu) {
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

        // Se o mouse foi clicado, entra no if
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE CONFIGURAÇÕES
            if (event.mouse.x >= area_config_x && event.mouse.x <= area_config_x + area_config_width &&
                event.mouse.y >= area_config_y && event.mouse.y <= area_config_y + area_config_height) {
                printf("Evento --> Clique Registrado na área configurações\n");
                current_background = 1;
                if (play_som_menu) {
                    al_stop_sample_instance(inst_som_menu);
                    play_som_menu = false;
                }
            }

            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE JOGAR
            if (event.mouse.x >= area_mapa_x && event.mouse.x <= area_mapa_x + area_mapa_largura &&
                event.mouse.y >= area_mapa_y && event.mouse.y <= area_mapa_y + area_mapa_altura) {
                printf("Evento --> Clique Registrado na área jogar\n");
                current_background = 2;
                if (play_som_menu) {
                    al_stop_sample_instance(inst_som_menu);
                    play_som_menu = false;
                }
            }

            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE VOLTAR PARA O MENU
            if (current_background == 1 && event.mouse.x >= area_voltar_x && event.mouse.x <= area_voltar_x + area_voltar_largura &&
                event.mouse.y >= area_voltar_y && event.mouse.y <= area_voltar_y + area_voltar_altura) {
                printf("Evento --> Clique Registrado na área voltar\n");
                current_background = 0;
                if (play_som_menu) {
                    al_play_sample_instance(inst_som_menu);
                }
            }

            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE DESLIGAR SOM
            if (current_background == 1 && event.mouse.x >= area_desligar_som_x && event.mouse.x <= area_desligar_som_x + area_desligar_som_largura &&
                event.mouse.y >= area_desligar_som_y && event.mouse.y <= area_desligar_som_y + area_desligar_som_altura) {
                printf("Evento --> Clique Registrado na área desligar som\n");
                al_stop_sample_instance(inst_som_menu); // Para o som
                play_som_menu = false; // Atualiza a flag do som
            }

            // VAI VERIFICAR SE A AREA CLICADA FOI A AREA DE LIGAR SOM
            if (current_background == 1 && event.mouse.x >= area_ligar_som_x && event.mouse.x <= area_ligar_som_x + area_ligar_som_largura &&
                event.mouse.y >= area_ligar_som_y && event.mouse.y <= area_ligar_som_y + area_ligar_som_altura) {
                printf("Evento --> Clique Registrado na área ligar som\n");
                if (!play_som_menu) {
                    al_play_sample_instance(inst_som_menu); // Reproduz o som
                    play_som_menu = true; // Atualiza a flag do som
                }
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0)); 
        if (current_background == 0) {
            al_draw_bitmap(background, 0, 0, 0);
        }
        else if (current_background == 1) {
            al_draw_bitmap(config_background, 0, 0, 0);
        }
        else if (current_background == 2) {
            al_draw_bitmap(choose_maps_background, 0, 0, 0);
        }

        al_flip_display();
    }

    al_destroy_sample(som_menu);
    al_destroy_sample_instance(inst_som_menu);
    al_destroy_bitmap(background);
    al_destroy_bitmap(config_background);
    al_destroy_bitmap(choose_maps_background);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
