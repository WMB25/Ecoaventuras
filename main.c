#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/keyboard.h>

// Struct para agrupar os ponteiros 
typedef struct {
    ALLEGRO_SAMPLE* som_menu;
    ALLEGRO_SAMPLE_INSTANCE* inst_som_menu;
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* config_background;
    ALLEGRO_BITMAP* escolher_mapas_background;
    ALLEGRO_BITMAP* bg_mapa_branco;
    ALLEGRO_BITMAP* imagem_grama;
    ALLEGRO_BITMAP* grama_amazonia;
    ALLEGRO_BITMAP* SPRITE;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
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

    if (!al_install_keyboard()) {
        printf("Erro ao instalar o teclado!\n");
        return -1;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Falha ao reservar �udio!\n");
        return -1;
    }

    recursos->background = al_load_bitmap("menu.png");
    if (!recursos->background) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo!\n");
        return -1;
    }

    recursos->config_background = al_load_bitmap("config.jpg");
    if (!recursos->config_background) {
        fprintf(stderr, "Falha ao carregar a imagem de configura��o!\n");
        return -1;
    }

    recursos->imagem_grama = al_load_bitmap("grama.png");
    if (!recursos->imagem_grama) {
        fprintf(stderr, "Falha ao carregar a imagem da grama!\n");
        return -1;
    }

    recursos->grama_amazonia = al_load_bitmap("grama.png");
    if (!recursos->imagem_grama) {
        fprintf(stderr, "Falha ao carregar a imagem da grama!\n");
        return -1;
    }

    recursos->bg_mapa_branco = al_load_bitmap("bg_mapa_branco.png");
    if (!recursos->bg_mapa_branco) {
        fprintf(stderr, "Falha ao carregar a imagem de fundo branco!\n");
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

    recursos->SPRITE = al_load_bitmap("dragon.png");
    if (!recursos->SPRITE) {
        fprintf(stderr, "Falha ao carregar O PERSONAGEM!\n");
        return -1;
    }

    recursos->inst_som_menu = al_create_sample_instance(recursos->som_menu);
    if (!recursos->inst_som_menu) {
        fprintf(stderr, "Falha ao criar a inst�ncia do som!\n");
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

// FUN��O DESENHAR FUNDO COM BASE NA MATRIZ

void draw_grama(int map[][13], ALLEGRO_BITMAP* imagem, int tamanho) {

    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            if (map[i][j] == 0) {
                al_draw_bitmap(imagem, j * tamanho, i * tamanho, 0);
            }

        }
    }
}

int main(int argc, char** argv) {
    AllegroRecursos recursos;

    if (inicializar_componentes_allegro(&recursos) != 0) {
        return -1;
    }


    // AREA --> VARIAVEIS DE CONTROLE
    int largura_da_imagem = 0;
    int altura_da_imagem = 0;
    int imagem_fundo = 0; // 0: Imagem Menu, 1: Imagem Config, 2: Imagem escolher_mapas,  100: Fundo Branco
    bool jogo_rodando = true;
    bool tocar_som_menu = false;

    typedef struct {
        float frame;
        int pos_x;
        int pos_y;
        int current_frame_y;
    }mov_p;
    mov_p mov_prs = { 0.f, 0, 0, 161 };

    // STRUCTS PARA �REAS CLIC�VEIS
    typedef struct  {
        int x;
        int y;
        int largura;
        int altura;
    } area;

    //STRUCTS DE MAPAS
    typedef struct {
        int colunas_mapa;
        int tamanho_mapa;
        int tamanho_titulo;
    } mapa;
    mapa areaMapa = { 13, 100, 128 };

    // Structs para areas clic�veis
    area area_config = { 515, 405, 250, 100 };
    area area_jogar = { 541, 290, 200, 100 };
    area area_desligar_som = { 320, 600, 220, 70 };
    area area_ligar_som = { 740, 600, 220, 70 };
    area area_voltar = { 165, 40, 200, 90 };
    //endere�o de tela 

    int Amazonia = 100;
    area escolha_amazonia = { 30, 45, 350, 350 };
    int map_Amazonia[10][13] = {
       {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0 , 0, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
    };

    int Pampa = 110;
    area escolha_pampa = { 470, 430, 350, 350 };
    int map[10][13] = {
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
    };

    int Pantanal = 120;
    area escolha_pantanal = { 900, 430, 350, 350 }; 
    int map_Pantanal[10][13] = {
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
    };

    int Catinga = 130;
    area escolha_caatinga = { 900, 45, 350, 350 }; 
    int map_Catinga[10][13] = {
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0},
    };

    // struct area escolha_cerrado = { 470, 45, 350, 350 };
    // struct area escola_mataAtlantica = { 30, 430, 350, 350 };

    al_set_sample_instance_playmode(recursos.inst_som_menu, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(recursos.inst_som_menu, al_get_default_mixer());

    if (imagem_fundo == 0 && tocar_som_menu) {
        al_play_sample_instance(recursos.inst_som_menu);
    }

    al_register_event_source(recursos.event_queue, al_get_display_event_source(recursos.display));
    al_register_event_source(recursos.event_queue, al_get_mouse_event_source());
    al_register_event_source(recursos.event_queue, al_get_keyboard_event_source());

    while (jogo_rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(recursos.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogo_rodando = false;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

            // Verifica se clicou na �rea de configura��es 
            if (event.mouse.x >= area_config.x && event.mouse.x <= area_config.x + area_config.largura &&
                event.mouse.y >= area_config.y && event.mouse.y <= area_config.y + area_config.altura) {
                printf("Evento --> Clique Registrado na �rea configura��es\n");
                imagem_fundo = 1;
                if (tocar_som_menu) {
                    al_stop_sample_instance(recursos.inst_som_menu);
                    tocar_som_menu = false;
                }
            }

            // Verifica se clicou na �rea de jogar
            if (event.mouse.x >= area_jogar.x && event.mouse.x <= area_jogar.x + area_jogar.largura &&
                event.mouse.y >= area_jogar.y && event.mouse.y <= area_jogar.y + area_jogar.altura) {
                printf("Evento --> Clique Registrado na �rea jogar\n");
                imagem_fundo = 2;
                if (tocar_som_menu) {
                    al_stop_sample_instance(recursos.inst_som_menu);
                    tocar_som_menu = false;
                }
            }

            // Verifica se clicou na �rea de voltar, Config.
            if (imagem_fundo == 1 && event.mouse.x >= area_voltar.x && event.mouse.x <= area_voltar.x + area_voltar.largura &&
                event.mouse.y >= area_voltar.y && event.mouse.y <= area_voltar.y + area_voltar.altura) {
                printf("Evento --> Clique Registrado na �rea voltar\n");
                imagem_fundo = 0;
                if (tocar_som_menu) {
                    al_play_sample_instance(recursos.inst_som_menu);
                }
            }

            // Verifica se clicou na �rea de desligar som, Config.
            if (imagem_fundo == 1 && event.mouse.x >= area_desligar_som.x && event.mouse.x <= area_desligar_som.x + area_desligar_som.largura &&
                event.mouse.y >= area_desligar_som.y && event.mouse.y <= area_desligar_som.y + area_desligar_som.altura) {
                printf("Evento --> Clique Registrado na �rea desligar som\n");
                al_stop_sample_instance(recursos.inst_som_menu);
                tocar_som_menu = false;
            }

            // Verifica se clicou na �rea de ligar som, Config
            if (imagem_fundo == 1 && event.mouse.x >= area_ligar_som.x && event.mouse.x <= area_ligar_som.x + area_ligar_som.largura &&
                event.mouse.y >= area_ligar_som.y && event.mouse.y <= area_ligar_som.y + area_ligar_som.altura) {
                printf("Evento --> Clique Registrado na �rea ligar som\n");
                al_play_sample_instance(recursos.inst_som_menu);
                tocar_som_menu = true;
            }

            // Verifica Area de Clique --> Amazonia
            if (event.mouse.x >= escolha_amazonia.x && event.mouse.x <= escolha_amazonia.x + escolha_amazonia.largura &&
                event.mouse.y >= escolha_amazonia.y && event.mouse.y <= escolha_amazonia.y + escolha_amazonia.altura) {
                printf("Evento --> Clique Registrado na area Amazonia\n");
                imagem_fundo = Amazonia;
                tocar_som_menu = true;
            }

            // Verifica Area de Clique --> Catinga
            if (event.mouse.x >= escolha_caatinga.x && event.mouse.x <= escolha_caatinga.x + escolha_caatinga.largura &&
                event.mouse.y >= escolha_caatinga.y && event.mouse.y <= escolha_caatinga.y + escolha_caatinga.altura) {
                printf("Evento --> Clique Registrado na area catinga\n");
                imagem_fundo = Catinga;
                tocar_som_menu = true;
            }

            // Verifica Area de Clique --> Pampa
            if (event.mouse.x >= escolha_pampa.x && event.mouse.x <= escolha_pampa.x + escolha_pampa.largura &&
                event.mouse.y >= escolha_pampa.y && event.mouse.y <= escolha_pampa.y + escolha_pampa.altura) {
                printf("Evento --> Clique Registrado na area Pampa\n");
                imagem_fundo = Pampa;
                tocar_som_menu = true;
            }

            // Verifica Area de Clique --> Pantanal
            if (event.mouse.x >= escolha_pantanal.x && event.mouse.x <= escolha_pantanal.x + escolha_pantanal.largura &&
                event.mouse.y >= escolha_pantanal.y && event.mouse.y <= escolha_pantanal.y + escolha_pantanal.altura) {
                printf("Evento --> Clique Registrado na area Pantanal\n");
                imagem_fundo = Pantanal;
                tocar_som_menu = true;
            }

        }

        // Desenha Imagem de Fundo com base em sua numera��o
        if (imagem_fundo == 0) {
            al_draw_bitmap(recursos.background, 0, 0, 0);
        }
        else if (imagem_fundo == 1) {
            al_draw_bitmap(recursos.config_background, 0, 0, 0);
        }
        else if (imagem_fundo == 2) {
            al_draw_bitmap(recursos.escolher_mapas_background, 0, 0, 0);
        }
        else if (imagem_fundo == Amazonia) {
            al_draw_bitmap(recursos.bg_mapa_branco, 0, 0, 0);
            draw_grama(map_Amazonia, recursos.imagem_grama, areaMapa.tamanho_mapa);

            //Movimenta��o
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                mov_prs.current_frame_y = 161;
                mov_prs.pos_x += 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                mov_prs.current_frame_y = 161 * 3;
                mov_prs.pos_x -= 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                mov_prs.current_frame_y = 161 * 2;
                mov_prs.pos_y = 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                mov_prs.current_frame_y = 0;
                mov_prs.pos_y -= 20;
            }
            mov_prs.frame += 0.3f;
            if (mov_prs.frame > 3) {
                mov_prs.frame -= 3;
            }
            al_draw_bitmap_region(recursos.SPRITE, 191 * (int)mov_prs.frame, mov_prs.current_frame_y, 191, 161, mov_prs.pos_x, mov_prs.pos_y, 0);

        }
        else if (imagem_fundo == Catinga) {
            al_draw_bitmap(recursos.bg_mapa_branco, 0, 0, 0);
            draw_grama(map_Catinga, recursos.imagem_grama, areaMapa.tamanho_mapa);

            //Movimenta��o
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                mov_prs.current_frame_y = 161;
                mov_prs.pos_x += 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                mov_prs.current_frame_y = 161 * 3;
                mov_prs.pos_x -= 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                mov_prs.current_frame_y = 161 * 2;
                mov_prs.pos_y = 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                mov_prs.current_frame_y = 0;
                mov_prs.pos_y -= 20;
            }
            mov_prs.frame += 0.3f;
            if (mov_prs.frame > 3) {
                mov_prs.frame -= 3;
            }
            al_draw_bitmap_region(recursos.SPRITE, 191 * (int)mov_prs.frame, mov_prs.current_frame_y, 191, 161, mov_prs.pos_x, mov_prs.pos_y, 0);
        }
        else if (imagem_fundo == Pampa) {
            al_draw_bitmap(recursos.bg_mapa_branco, 0, 0, 0);
            draw_grama(map_Catinga, recursos.imagem_grama, areaMapa.tamanho_mapa);

            //Movimenta��o
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                mov_prs.current_frame_y = 161;
                mov_prs.pos_x += 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                mov_prs.current_frame_y = 161 * 3;
                mov_prs.pos_x -= 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                mov_prs.current_frame_y = 161 * 2;
                mov_prs.pos_y = 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                mov_prs.current_frame_y = 0;
                mov_prs.pos_y -= 20;
            }
            mov_prs.frame += 0.3f;
            if (mov_prs.frame > 3) {
                mov_prs.frame -= 3;
            }
            al_draw_bitmap_region(recursos.SPRITE, 191 * (int)mov_prs.frame, mov_prs.current_frame_y, 191, 161, mov_prs.pos_x, mov_prs.pos_y, 0);
        }
        else if (imagem_fundo == Pantanal) {
            al_draw_bitmap(recursos.bg_mapa_branco, 0, 0, 0);
            draw_grama(map_Pantanal, recursos.imagem_grama, areaMapa.tamanho_mapa);

            //Movimenta��o
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                mov_prs.current_frame_y = 161;
                mov_prs.pos_x += 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                mov_prs.current_frame_y = 161 * 3;
                mov_prs.pos_x -= 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                mov_prs.current_frame_y = 161 * 2;
                mov_prs.pos_y = 20;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                mov_prs.current_frame_y = 0;
                mov_prs.pos_y -= 20;
            }
            mov_prs.frame += 0.3f;
            if (mov_prs.frame > 3) {
                mov_prs.frame -= 3;
            }
            al_draw_bitmap_region(recursos.SPRITE, 191 * (int)mov_prs.frame, mov_prs.current_frame_y, 191, 161, mov_prs.pos_x, mov_prs.pos_y, 0);
        }
        al_flip_display();
    }

    // LIMPA --> DESTROI TUDO DA MEMORIA
    al_destroy_bitmap(recursos.background);
    al_destroy_bitmap(recursos.config_background);
    al_destroy_bitmap(recursos.imagem_grama);
    al_destroy_bitmap(recursos.SPRITE);
    al_destroy_bitmap(recursos.bg_mapa_branco);
    al_destroy_bitmap(recursos.escolher_mapas_background);
    al_destroy_sample(recursos.som_menu);
    al_destroy_sample_instance(recursos.inst_som_menu);
    al_destroy_display(recursos.display);
    al_destroy_event_queue(recursos.event_queue);

    return 0;
}