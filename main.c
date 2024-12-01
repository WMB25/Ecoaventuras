#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include <math.h>

typedef struct {
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* config_background;
    ALLEGRO_BITMAP* escolher_mapas_background;
    ALLEGRO_BITMAP* sprite_sapo;
    ALLEGRO_BITMAP* img_pantanal;
    ALLEGRO_BITMAP* img_pampa;
    ALLEGRO_BITMAP* img_caatinga;
    ALLEGRO_BITMAP* img_boi;
    ALLEGRO_BITMAP* img_sobre_biomas;
    ALLEGRO_BITMAP* img_amazonia;
    ALLEGRO_BITMAP* img_sobre_caatinga;
    ALLEGRO_BITMAP* img_sobre_pantanal;
    ALLEGRO_BITMAP* img_sobre_pampa;
    ALLEGRO_BITMAP* img_sobre_amazonia;
    ALLEGRO_BITMAP* img_jacare;
    ALLEGRO_BITMAP* img_fim;
    ALLEGRO_BITMAP* fundo_pergunta;
    ALLEGRO_BITMAP* SPRITE;
    ALLEGRO_BITMAP* logo;
    ALLEGRO_FONT* font;
    ALLEGRO_TIMER* timer;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_SAMPLE* som_menu;
    ALLEGRO_SAMPLE_INSTANCE* inst_som_menu;
} AllegroRecursos;

// ISSO SERVE PRA DEFINIR CONSTANTES/VALORES -> DEIXA MAAIS FACIL DE MEXER NO CODIGO
#define quantidade_bois 4
#define quantidade_jacare 2
#define largura_tela 1350   

typedef struct {
    ALLEGRO_BITMAP* img_boi;
    float boi_x, boi_y;
    float velocidade;
} boi;

void inicializar_bois(boi bois[], AllegroRecursos* recursos);
desenhar_bois_e_mover(bois);

typedef struct {
    ALLEGRO_BITMAP* img_jacare;
    float jacare_x, jacare_y;
    float velocidade;
} jacare;

void inicializar_jacare(jacare jacare[], AllegroRecursos* recursos);
void jacare_mover(AllegroRecursos* recursos, jacare jacare[]);

// ----------------------------------- FUNÇÃO PERGUNTAS ----------------------------------- \\

typedef struct {
    int x, y;
    char Questoes[100];
    int resposta_certa;
    char opcoesDasQuestoes[5][100];
    bool respondido;
}pergunta;
void impressao_pergunta(pergunta* questoes, ALLEGRO_FONT* font, ALLEGRO_BITMAP* fundo ) {
    const int larguraT = 1280; 
    const int alturaT = 800;  

    const int larguraFundo = 1000;
    const int alturaFundo = 400;

    const int larguraAlt = 300;
    const int alturaAlt = 50;
    const int espacoEntreAlt = 20;

    int pos_x_fundo = (larguraT - larguraFundo) / 2;
    int pos_y_fundo = (alturaT - alturaFundo) / 4;
    
    al_draw_scaled_bitmap(fundo,0, 0,al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), pos_x_fundo, pos_y_fundo, larguraFundo, alturaFundo, 0);

    int larguraPergunta = al_get_text_width(font, questoes->Questoes);
    int alturaTexto = al_get_font_line_height(font);

    int pos_x_texto = pos_x_fundo + (larguraFundo - larguraPergunta + 500) / 12;
    int pos_y_texto = pos_y_fundo + (alturaTexto / 4) + 50;

    al_draw_text(font, al_map_rgb(255, 255, 255), pos_x_texto, pos_y_texto, 0, questoes->Questoes);

    int pos_x_alt = pos_x_fundo + (larguraFundo - 2 * larguraAlt - espacoEntreAlt) / 2;
    int pos_y_alt = pos_y_fundo + alturaFundo / 2;

    
    for (int i = 0; i < 4; i++) {
        int coluna = i % 2;  
        int linha = i / 2;   

        int x_alt = pos_x_alt + coluna * (larguraAlt + espacoEntreAlt);
        int y_alt = pos_y_alt + linha * (alturaAlt + espacoEntreAlt);

        
        int larguraTextoAlt = al_get_text_width(font, questoes->opcoesDasQuestoes[i]);
        int alturaTextoAlt = al_get_font_line_height(font);

        int x_texto = x_alt + (larguraAlt - larguraTextoAlt) / 2;
        int y_texto = y_alt + (alturaAlt - alturaTextoAlt) / 2;

        al_draw_text(font, al_map_rgb(255, 255, 255), x_texto, y_texto, 0, questoes->opcoesDasQuestoes[i]);
    }
}
bool proximidade_pergunta(int pos_x, int pos_y, pergunta* posicao) {
    int distanciaX = abs(pos_x - posicao->x);
    int distanciaY = abs(pos_y - posicao->y);
    return (distanciaX < 50 && distanciaY < 50);
}
bool verificar_resposta(pergunta* questao, int resposta) {
    return (resposta == questao->resposta_certa);
}

// ----------------------------------- FUNÇÃO DO SAPO ----------------------------------- \\

typedef struct {
    int frame_largura;
    int frame_altura;
    int num_frames;
    int frame;
    int courrent_frame;
    float x;
    float y;
    float velocidade_puloX;
    float velocidade_puloY;
    float gravidade;
    bool pulo;
}frog;
frog sapo = { 64, 64, 4, 0.f, 64, 20, 400, -5.0f, -0.0f, 0.5f, false };

// ----------------------------------- AREA BLOQUEADA NOS MAPAS ----------------------------------- \\

typedef struct {
    int x, y, largura, altura;
} area_limite;
bool esta_em_area_bloqueada(int pos_x, int pos_y, area_limite* area_bloqueada) {
    if (pos_x >= area_bloqueada->x && pos_x <= area_bloqueada->x + area_bloqueada->largura &&
        pos_y >= area_bloqueada->y && pos_y <= area_bloqueada->y + area_bloqueada->altura) {
        return true;
    }
    return false;
}

// ----------------------------------- VERIFICAÇÃO DE CLIQUE NO BOTÃO PRÓXIMO ----------------------------------- \\

bool verificar_clique_na_area(ALLEGRO_EVENT event, area_limite* area) {
    if (event.mouse.x >= area->x && event.mouse.x <= area->x + area->largura &&
        event.mouse.y >= area->y && event.mouse.y <= area->y + area->altura) {
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv) {
    AllegroRecursos recursos;
    inicializar_componentes_allegro(&recursos);

    // AREA --> VARIAVEIS DE CONTROLE
    int tela = 0;
    bool jogo_rodando = true;
    bool tocar_som_menu = false;

    boi bois[quantidade_bois]; // Array de bois
    inicializar_bois(bois, &recursos);

    jacare jacare[quantidade_jacare];
    inicializar_jacare(jacare, &recursos);

    typedef struct {
        float frame;
        int pos_x;
        int pos_y;
        int current_frame_y;
    } mov_p;

    // Declaramos uma variável externa, estamos avisando o compilador que esta variável esta em outro arquivo
    extern mov_p mov_prs;
    void movimento_personagem(ALLEGRO_EVENT event); // Chamando a função de movimentar o personagem

    pergunta QuestoesA[5] = {
          {85, 260, "Qual a proporção do território brasileiro ocupada pela Amazônia?", 1, {"1) 25%", "2) 49%", "3) 60%", "4) 75%"}, false},
          {470, 110, "Qual animal é conhecido como o maior felino da Amazônia?", 1, {"1) Tamanduá-bandeira", "2) Onça-pintada", "3) Macaco-aranha", "4) Arara-azul"}, false},
          {510, 650, "Qual desses é um recurso natural da Amazônia?", 2, {"1) Petróleo", "2) Madeira", "3) Gás natural", "4) Ferro"}, false},
          {820, 360, "Qual é uma das principais ameaças ao bioma amazônico?", 2, {"1) Plantação de soja", "2) Turismo sustentável", "3) Desmatamento", "4) Energia solar."}, false},
          {1110, 330, "Quem são os povos originarios?", 2, {"1) Os maori", "2) Os aborígenes", "3) Os indíginas", "4) Os toltecas"}, false}
    };
    pergunta QuestoesP[5] = {
      {210, 410, "Qual o maior animal terrestre do Pantanal", 0, {"1) Onça-pintada", "2) Capivara", "3) Jacaré", "4) Arara-azul"}, false},
      {210, 410, "Qual é o bioma mais extenso do mundo em áreas alagadas?", 2, {"1) Amazônia", "2) Cerrado", "3) Pantanal", "4) Caatinga"}, false},
      {210, 410, "Em quais países o Pantanal está localizado?", 2, {"1) Brasil, Argentina e Uruguai", "2) Brasil, Bolívia e Paraguai", "3) Brasil, Peru e Colômbia", "4) Brasil, Chile e Venezuela"}, false},
      {210, 410, "Qual animal é mencionado como habitante do Pantanal?", 2, {"1) Onça-pintada", "2) Urso polar", "3) Canguru", "4) Leão"}, false},
      {210, 410, "Qual a estação mais chuvosa no Pantanal?", 1, {"1) Primavera", "2) Verão", "3) Outono", "4) Inverno"}, false}
    };
    int quantidade_perguntaA = sizeof(QuestoesA) / sizeof(QuestoesA[0]);
    int quantidade_perguntaP = sizeof(QuestoesP) / sizeof(QuestoesP[0]);
    pergunta* pergunta_atual = NULL;
    bool exibindo_perguntas = false;

    // STRUCTS PARA AREAS CLICAVEIS
    typedef struct  {
        int x, y, largura, altura;
    } area;

    // Structs para areas clicáveis principal
    area area_config = { 420, 420, 435, 180 };
    area area_jogar = { 425, 220, 435, 180 };
    area area_desligar_som = { 320, 600, 220, 70 };
    area area_ligar_som = { 740, 600, 220, 70 };
    area area_voltar = { 165, 40, 200, 90 };
    area_limite  proximo_biomas = { 965, 653, 300, 100 };

    area escolha_amazonia = { 5, 5, 630, 400 };
    area escolha_pampa = { 645, 5, 630, 400 };
    area escolha_pantanal = { 5, 418, 630, 400 };
    area escolha_caatinga = { 645, 418, 630, 400 };

    area proxima_fase_amazonia = { 1230, 200, 100, 300};
    area proxima_fase_pampa = { 1184, 95, 30, 60 };
    area proxima_fase_pantanal = { 1230, 250, 55, 350 };
    area proxima_fase_caatinga = { 880, 420, 200, 200 };

    // Limitação de areas
    area_limite limite_pampa_01 = { 0, 660, 515, 150 };
    area_limite limite_pampa_02 = { 680, 660, 600, 150 };
    area_limite limite_pampa_03 = { 0, 460, 115, 70 };
    area_limite limite_pampa_04 = { 250, 460, 1000, 70 };
    area_limite limite_pampa_05 = { 0, 0, 750, 300 };
    area_limite limite_pampa_06 = { 950, 260, 350, 50 };

    area_limite limite_caatinga_01 = { 0, 0, 280, 800 };
    area_limite limite_caatinga_02 = { 0, 650, 1280, 300 };
    area_limite limite_caatinga_03 = { 550, 0, 1280, 350 };
    area_limite limite_caatinga_04 = { 550, 350, 300, 150 };

    area_limite limite_amazonia_01 = { 0, 450, 500, 400 };
    area_limite limite_amazonia_02 = { 700, 0, 1300, 200 };
    area_limite limite_amazonia_03 = { 700, 190, 250, 130 };
    area_limite limite_amazonia_04 = { 730, 450, 500, 400 };

    area_limite limite_pantanal_01 = { 0, 700, 1300, 250};
    area_limite limite_pantanal_02 = { 210, 288, 100, 100 };
    area_limite limite_pantanal_03 = { 210, 530, 100, 100 };
    area_limite limite_pantanal_04 = { 1150, 0, 150, 150 };
    area_limite limite_pantanal_05 = { 600, 95, 120, 150 }; 
    //area_limite limite_pantanal_06 = { 210, 410, 100, 100 }; AREA FLOR DO PANTANAL

    al_set_sample_instance_playmode(recursos.inst_som_menu, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(recursos.inst_som_menu, al_get_default_mixer());

    if (tela == 0 && tocar_som_menu) {
        al_play_sample_instance(recursos.inst_som_menu);
    }

    al_register_event_source(recursos.event_queue, al_get_timer_event_source(recursos.timer));
    al_register_event_source(recursos.event_queue, al_get_display_event_source(recursos.display));
    al_register_event_source(recursos.event_queue, al_get_mouse_event_source());
    al_register_event_source(recursos.event_queue, al_get_keyboard_event_source());

    al_start_timer(recursos.timer);
    al_set_window_title(recursos.display, "Ecoaventuras");
    al_set_display_icon(recursos.display, recursos.logo);

    while (jogo_rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(recursos.event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogo_rodando = false;
        }       

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (tela == 0) {
                // Verifica area de Clique --> Iniciar
                if (event.mouse.x >= area_jogar.x && event.mouse.x <= area_jogar.x + area_jogar.largura &&
                    event.mouse.y >= area_jogar.y && event.mouse.y <= area_jogar.y + area_jogar.altura) {
                    printf("Evento --> Clique Registrado na area jogar\n");
                    tela = 2;
                    if (tocar_som_menu) {
                        al_stop_sample_instance(recursos.inst_som_menu);
                        tocar_som_menu = false;
                    }
                }

                // area de Configurações
                if (event.mouse.x >= area_config.x && event.mouse.x <= area_config.x + area_config.largura &&
                    event.mouse.y >= area_config.y && event.mouse.y <= area_config.y + area_config.altura) {
                    printf("Evento --> Clique Registrado na area configurações\n");
                    tela = 1;
                    if (tocar_som_menu) {
                        al_stop_sample_instance(recursos.inst_som_menu);
                        tocar_som_menu = false;
                    }
                }
            }
            else if (tela == 1) {
                // // Verifica area de Clique --> Voltar
                if (event.mouse.x >= area_voltar.x && event.mouse.x <= area_voltar.x + area_voltar.largura &&
                    event.mouse.y >= area_voltar.y && event.mouse.y <= area_voltar.y + area_voltar.altura) {
                    printf("Evento --> Clique Registrado na area voltar\n");
                    tela = 0; 

                    if (tocar_som_menu) {
                        al_play_sample_instance(recursos.inst_som_menu);
                    }
                }

                // // Verifica area de Clique --> Desligar Som
                if (event.mouse.x >= area_desligar_som.x && event.mouse.x <= area_desligar_som.x + area_desligar_som.largura &&
                    event.mouse.y >= area_desligar_som.y && event.mouse.y <= area_desligar_som.y + area_desligar_som.altura) {
                    printf("Evento --> Clique Registrado na area desligar som\n");
                    al_stop_sample_instance(recursos.inst_som_menu);
                    tocar_som_menu = false;
                }

                // Verifica area de Clique --> Ligar Som
                if (event.mouse.x >= area_ligar_som.x && event.mouse.x <= area_ligar_som.x + area_ligar_som.largura &&
                    event.mouse.y >= area_ligar_som.y && event.mouse.y <= area_ligar_som.y + area_ligar_som.altura) {
                    printf("Evento --> Clique Registrado na area ligar som\n");
                    al_play_sample_instance(recursos.inst_som_menu);
                    tocar_som_menu = true;
                }
            }
            else if (tela == 100) {
                // Verifica area de Clique --> Amazonia
                if (event.mouse.x >= escolha_amazonia.x && event.mouse.x <= escolha_amazonia.x + escolha_amazonia.largura &&
                    event.mouse.y >= escolha_amazonia.y && event.mouse.y <= escolha_amazonia.y + escolha_amazonia.altura) {
                    printf("Evento --> Clique Registrado na area Amazonia\n");
                    tela = 5;
                    tocar_som_menu = true;
                    
                }

                // Verifica area de Clique --> Catinga
                if (event.mouse.x >= escolha_caatinga.x && event.mouse.x <= escolha_caatinga.x + escolha_caatinga.largura &&
                    event.mouse.y >= escolha_caatinga.y && event.mouse.y <= escolha_caatinga.y + escolha_caatinga.altura) {
                    printf("Evento --> Clique Registrado na area catinga\n");
                    tela = 6;
                    tocar_som_menu = true;
                    
                }

                // Verifica area de Clique --> Pampa
                if (event.mouse.x >= escolha_pampa.x && event.mouse.x <= escolha_pampa.x + escolha_pampa.largura &&
                    event.mouse.y >= escolha_pampa.y && event.mouse.y <= escolha_pampa.y + escolha_pampa.altura) {
                    printf("Evento --> Clique Registrado na area Pampa\n");
                    tela = 3;
                    tocar_som_menu = true;
                    
                }

                // Verifica area de Clique --> Pantanal
                if (event.mouse.x >= escolha_pantanal.x && event.mouse.x <= escolha_pantanal.x + escolha_pantanal.largura &&
                    event.mouse.y >= escolha_pantanal.y && event.mouse.y <= escolha_pantanal.y + escolha_pantanal.altura) {
                    printf("Evento --> Clique Registrado na area Pantanal\n");
                    tela = 4;
                    tocar_som_menu = true;
                    
                }
            }
        }

        if (tela == 0) { // INICIO
            al_draw_bitmap(recursos.background, 0, 0, 0);

            //ALLEGRO_COLOR cor_area = al_map_rgb(255, 0, 0);
            //al_draw_filled_rectangle(area_config.x, area_config.y, area_config.x + area_config.largura, area_config.y + area_config.altura, cor_area);
        }

        else if (tela == 1) { // CONFIGURAÇÕES
            al_draw_bitmap(recursos.config_background, 0, 0, 0);
        }
        
        if (tela == 2) { // BIOMAS
            al_draw_bitmap(recursos.img_sobre_amazonia, 0, 0, 0);

            if (verificar_clique_na_area(event, &proximo_biomas)) {
                printf("Evento --> Clique Registrado na area proximo\n");
                tela = 200;
            }
        }

        else if (tela == 200) { // Sobre Pampa
            al_draw_bitmap(recursos.img_sobre_pampa, 0, 0, 0);

            if (verificar_clique_na_area(event, &proximo_biomas)) {
                printf("Evento --> Clique Registrado na area proximo\n");
                tela = 300;
            }
        }

        else if (tela == 300) { // Sobre Pantanal
            al_draw_bitmap(recursos.img_sobre_pantanal, 0, 0, 0);

            if (verificar_clique_na_area(event, &proximo_biomas)) {
                printf("Evento --> Clique Registrado na area proximo\n");
                tela = 400;
            }
        }

        else if (tela == 400) { // Sobre Caatinga
            al_draw_bitmap(recursos.img_sobre_caatinga, 0, 0, 0);

            if (verificar_clique_na_area(event, &proximo_biomas)) {
                printf("Evento --> Clique Registrado na area proximo\n");
                tela = 100;
            }
        }

        else if (tela == 100) { // ESCOLHER MAPA
            al_draw_bitmap(recursos.escolher_mapas_background, 0, 0, 0);
        }

        else if (tela == 5) { // AMAZONIA
            al_draw_bitmap(recursos.img_amazonia, 0, 0, 0);
            movimento_personagem(event);
            al_draw_bitmap_region(recursos.SPRITE, 41.33 * (int)mov_prs.frame, mov_prs.current_frame_y, 41.33, 36, mov_prs.pos_x, mov_prs.pos_y, 0);

            if (esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_amazonia_01) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_amazonia_02) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_amazonia_03) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_amazonia_04)) {

                mov_prs.pos_x = 15;
                mov_prs.pos_y = 100;
                printf("Evento --> Area Bloqueada, Voltando ao inicio.\n");
            }
                       
            if (!exibindo_perguntas) {
                pergunta_atual = NULL;
                for (int i = 0; i < quantidade_perguntaA; i++) {
                    if (!QuestoesA[i].respondido && proximidade_pergunta(mov_prs.pos_x, mov_prs.pos_y, &QuestoesA[i])) {
                        pergunta_atual = &QuestoesA[i];
                        exibindo_perguntas = true;
                        break;
                    }
                }
            }
            if (exibindo_perguntas && pergunta_atual != NULL) {  
                    impressao_pergunta(pergunta_atual, recursos.font, recursos.fundo_pergunta);

                    int resposta = event.keyboard.keycode - ALLEGRO_KEY_1;
                    if (resposta >= 0 && resposta < 4) {
                        if (verificar_resposta(pergunta_atual, resposta)) {
                            printf("Resposta esta Correto!\n");
                           /* exibindo_perguntas = true;*/
                        }
                        else {
                            printf("Resposta Esta errada\n");
                        }
                        pergunta_atual->respondido = true;
                        exibindo_perguntas = false;
                        pergunta_atual = NULL;
                    }
             
            }
            // SISTEMA -> MUDANÇA DE MAPA
            int aprox = 100.0f;
            if (mov_prs.pos_x >= proxima_fase_amazonia.x - aprox && mov_prs.pos_x <= proxima_fase_amazonia.x + aprox &&
                mov_prs.pos_y >= proxima_fase_amazonia.y - aprox && mov_prs.pos_y <= proxima_fase_amazonia.y + aprox) {
                printf("Evento --> Mudando de Tela Pampa\n");
                tela = 3;
            }
        }

        else if (tela == 3) { // PAMPA
            al_draw_bitmap(recursos.img_pampa, 0, 0, 0);
            movimento_personagem(event);

            if (esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_pampa_01) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_pampa_02) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_pampa_03) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_pampa_04) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_pampa_05) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_pampa_06)) {

                mov_prs.pos_x = 500;
                mov_prs.pos_y = 650;
                printf("Evento --> Area Bloqueada, Voltando ao inicio.\n");
            }

            
            al_draw_bitmap_region(recursos.SPRITE, 41.33 * (int)mov_prs.frame, mov_prs.current_frame_y, 41.33, 36, mov_prs.pos_x, mov_prs.pos_y, 0);

            // SISTEMA -> MUDANÇA DE MAPA
            int aprox = 100.0f;
            if (mov_prs.pos_x >= proxima_fase_pampa.x - aprox && mov_prs.pos_x <= proxima_fase_pampa.x + aprox &&
                mov_prs.pos_y >= proxima_fase_pampa.y - aprox && mov_prs.pos_y <= proxima_fase_pampa.y + aprox) {
                printf("Evento --> Mudando de Tela Pantanal\n");
                tela = 4;
                exibindo_perguntas = false;
                pergunta_atual = NULL;
            }

            // Colisão dos bois
            for (int i = 0; i < quantidade_bois; i++) {
                if (mov_prs.pos_x >= bois[i].boi_x - 5 && mov_prs.pos_x <= bois[i].boi_x + al_get_bitmap_width(bois[i].img_boi) + 5
                    && mov_prs.pos_y >= bois[i].boi_y - 5 && mov_prs.pos_y <= bois[i].boi_y + al_get_bitmap_height(bois[i].img_boi) + 5) {
                    mov_prs.pos_x = 500;
                    mov_prs.pos_y = 650;
                    printf("Evento --> Colisão com o boi, retornando à posição inicial.\n");
                }
            }
        }
        if (event.type == ALLEGRO_EVENT_TIMER && tela == 3) {
            desenhar_bois_e_mover(&recursos, bois);
        }

        else if (tela == 4) {// PANTANAL
            al_stop_timer(recursos.timer);
            al_draw_bitmap(recursos.img_pantanal, 0, 0, 0);
          
            if (!exibindo_perguntas) {
                for (int i = 0; i < quantidade_perguntaP; i++) {
                    if (!QuestoesP[i].respondido && proximidade_pergunta(sapo.x, sapo.y, &QuestoesP[i])) {
                        pergunta_atual = &QuestoesP[i];
                        exibindo_perguntas = true;
                        break;
                    }
                }
            }
            if (exibindo_perguntas && pergunta_atual != NULL) {
                impressao_pergunta(pergunta_atual, recursos.font, recursos.fundo_pergunta);

                int resposta = event.keyboard.keycode - ALLEGRO_KEY_1;
                if (resposta >= 0 && resposta < 4) {
                    if (verificar_resposta(pergunta_atual, resposta)) {
                        printf("Resposta esta Correto!\n");
                        sapo.x+= 100;
                    }
                    else {
                        printf("Resposta Esta Errada\n");
                    }
                    pergunta_atual->respondido = true;
                    exibindo_perguntas = false;
                    pergunta_atual = NULL;
                }
            }

            if (sapo.pulo) {
                sapo.y += sapo.velocidade_puloY;
                sapo.velocidade_puloY += sapo.gravidade;
                if (sapo.y >= 300) {
                    sapo.y = 300;
                    sapo.velocidade_puloY = 0;
                    sapo.pulo = false;
                }
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                sapo.courrent_frame = 64;
                sapo.x += 5;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                sapo.courrent_frame = 64;
                sapo.x -= 5;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                sapo.courrent_frame = 64;
                sapo.y += 5;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                sapo.courrent_frame = 64;
                sapo.y -= 5;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !sapo.pulo) {
                sapo.pulo = true;
                sapo.velocidade_puloY = sapo.velocidade_puloX;
            }
            if (sapo.x < 0) {
                sapo.x = 0;
            }
            else if (sapo.x > 1280 - sapo.frame) {
                sapo.x = 1280 - sapo.frame;
            }
            if (tela == 4 && ALLEGRO_EVENT_TIMER) {
                al_start_timer(recursos.timer);
                jacare_mover(&recursos, jacare);
            }
            /*extern frog sapo;
            iniciar_sapo(&recursos);*/

            al_draw_bitmap_region(recursos.sprite_sapo, (int)(sapo.frame % sapo.num_frames) * sapo.frame_largura, 0, sapo.frame_largura, sapo.frame_altura, sapo.x, sapo.y, 0);

            if (esta_em_area_bloqueada(sapo.x, sapo.y, &limite_pantanal_01) ||
                esta_em_area_bloqueada(sapo.x, sapo.y, &limite_pantanal_02) ||
                esta_em_area_bloqueada(sapo.x, sapo.y, &limite_pantanal_03) ||
                esta_em_area_bloqueada(sapo.x, sapo.y, &limite_pantanal_04) ||
                esta_em_area_bloqueada(sapo.x, sapo.y, &limite_pantanal_05)) {

                sapo.x = 20;
                sapo.y = 400;
                printf("Evento --> Area Bloqueada, Voltando ao inicio.\n");
            }

            // Colisão dos jacarés
            for (int i = 0; i < quantidade_jacare; i++) {
                if (sapo.x >= jacare[i].jacare_x - 5 && sapo.x <= jacare[i].jacare_x + al_get_bitmap_width(jacare[i].img_jacare) + 5
                    && sapo.y >= jacare[i].jacare_y - 5 && sapo.y <= jacare[i].jacare_y + al_get_bitmap_height(jacare[i].img_jacare) + 5) {
                    sapo.x = 20;
                    sapo.y = 400;
                    printf("Evento --> Colisão com o jacare, retornando à posição inicial.\n");
                }
            }

            // SISTEMA -> MUDANÇA DE MAPA
            int aprox = 150.0f; 
            if (mov_prs.pos_x >= proxima_fase_pantanal.x - aprox && mov_prs.pos_x <= proxima_fase_pantanal.x + aprox &&
                mov_prs.pos_y >= proxima_fase_pantanal.y - aprox && mov_prs.pos_y <= proxima_fase_pantanal.y + aprox) {
                printf("Evento --> Mudando de Tela Caatinga\n");
                tela = 6;
            }
        }
     \
      /*  if (tela == 4) {
            al_stop_timer(recursos.timer);
                if (sapo.pulo) {
                    sapo.y += sapo.velocidade_puloY;
                    sapo.velocidade_puloY += sapo.gravidade;
                    if (sapo.y >= 300) {
                        sapo.y = 300;
                        sapo.velocidade_puloY = 0;
                        sapo.pulo = false;
                    }
                }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                sapo.courrent_frame = 64;
                sapo.x += 5;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                sapo.courrent_frame = 64;
                sapo.x -= 5;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                sapo.courrent_frame = 64;
                sapo.y += 5;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                sapo.courrent_frame = 64;
                sapo.y -= 5;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !sapo.pulo) {
                sapo.pulo = true;
                sapo.velocidade_puloY = sapo.velocidade_puloX;
            }
            if (sapo.x < 0) {
                sapo.x = 0;
            }
            else if (sapo.x > 1280 - sapo.frame) {
                sapo.x = 1280 - sapo.frame;
            }
           
        }*/
        
        else if (tela == 6) { // CAATINGA
            al_draw_bitmap(recursos.img_caatinga, 0, 0, 0);
            movimento_personagem(event);
            al_draw_bitmap_region(recursos.SPRITE, 41.33 * (int)mov_prs.frame, mov_prs.current_frame_y, 41.33, 36, mov_prs.pos_x, mov_prs.pos_y, 0);

            if (esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_caatinga_01) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_caatinga_02) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_caatinga_03) ||
                esta_em_area_bloqueada(mov_prs.pos_x, mov_prs.pos_y, &limite_caatinga_04)) {

                mov_prs.pos_x = 300;
                mov_prs.pos_y = 10;
                printf("Evento --> Area Bloqueada, Voltando ao inicio.\n");
            }

            // SISTEMA -> MUDANÇA DE MAPA
            int aprox = 20.0f;
            if (mov_prs.pos_x >= proxima_fase_caatinga.x - aprox && mov_prs.pos_x <= proxima_fase_caatinga.x + aprox &&
                mov_prs.pos_y >= proxima_fase_caatinga.y - aprox && mov_prs.pos_y <= proxima_fase_caatinga.y + aprox) {
                printf("Evento --> Mudando de Tela Fim\n");
                tela = 999;
            }
        }
        
        else if (tela == 999) {
            al_draw_bitmap(recursos.img_fim, 0, 0, 0);

            if (event.mouse.x >= proximo_biomas.x && event.mouse.x <= proximo_biomas.x + proximo_biomas.largura &&
                event.mouse.y >= proximo_biomas.y && event.mouse.y <= proximo_biomas.y + proximo_biomas.altura) {
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    printf("Evento --> Clique Registrado na area menu\n");
                    tela = 0;
                }
            }
        }

        al_flip_display();
    }

    // LIMPA --> DESTROI TUDO DA MEMORIA
    limpar_recursos(&recursos);

    return 0;
}
