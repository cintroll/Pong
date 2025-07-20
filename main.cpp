/*
 * /Pong/main.cpp
 *
 *  Criado em: 08/06/2010
 *      Autor: Matheus Cintra & Lucas Topp
 */
/* Inicia as bibliotecas do sdl e as bibliotecas basicas */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

/* Declara as superficies do SDL, variaveis globais */
SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *background2 = NULL;
SDL_Surface *menu = NULL;
SDL_Surface *menu2 = NULL;
SDL_Surface *credito = NULL;
SDL_Surface *barra = NULL;
SDL_Surface *bola = NULL;
SDL_Surface *bloco = NULL;
SDL_Surface *placar = NULL;

/*Declara as variaveis para texto, fonte e cor do texto */
TTF_Font *font = NULL;
TTF_Font *font2 = NULL;
SDL_Color textColor = {255, 255, 255};
char texto[30];

/* Declara ponteiro de arquivo */
FILE *arq;

/* Define as estruturas do bloco e do placar */
typedef struct {
  float posx;
  float posy;
  bool estado;
} BLOCO;

typedef struct {
  char player1[30];
  char player2[30];
  int placar_p1;
  int placar_p2;
} PLACAR;

PLACAR score;

/* Declara as constantes do tamanho da tela e BPP */
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

/* Declara as variveis de controle de eventos de teclados, mouse e outros */
SDL_Event event;
const Uint8 *keystates = NULL;

/* Declara as variaveis globais necessarias para fisica do jogo */
float posx_bar_esq, posy_bar_esq;
float vely_bar_esq, acy_bar_esq;
float posx_bar_dir = 545, posy_bar_dir = 250;
float vely_bar_dir, acy_bar_dir;
float posx_bola = 285, posy_bola = 285;
float velx_bola = -4, vely_bola = 0;
int n_blocos, n_resto;
bool estado;
bool sair = false;
BLOCO blocos[10];

/* Variaveis de controle dos frames por segundo */
const int FRAMES_PER_SECOND = 50;
int frame = 0;

/* Funcao para carregar as imagens no SDL, recebe como parametro
 *  o caminho da imagem, funcao pronta e copiado do tutorial de SDL  */
SDL_Surface *load_image(const char *path) {

  // carregada
  SDL_Surface *loadedImage = NULL;

  // otimizada
  SDL_Surface *optimizedImage = NULL;

  loadedImage = IMG_Load(path);

  if (loadedImage != NULL) { // se deu tudo certo,
    Uint32 colorkey = SDL_MapRGB(loadedImage->format, 0xFF, 0, 0xFF);
    SDL_SetColorKey(loadedImage, SDL_TRUE, colorkey);

    optimizedImage =
        SDL_ConvertSurfaceFormat(loadedImage, screen->format->format, 0);
    SDL_FreeSurface(loadedImage);
  } else {
    printf("[ERRO]: ao tentar carregar \"%s\"\n", path);
  }
  return optimizedImage;
}

/* Funcao para carregar as imagens na tela do computador,
 *  recebe como parametro as posicoes x e y da imagem,
 *  a variavel da imagem e a variavel de onde vai ser
 *   carregada a imagem, funcao pronta e copiado do
 *    tutorial de SDL */
void apply_surface(float x, float y, SDL_Surface *source,
                   SDL_Surface *destination) {
  // Make a temporary rectangle to hold the offsets
  SDL_Rect offset;

  // Give the offsets to the rectangle
  offset.x = (int)x;
  offset.y = (int)y;

  // Blit the surface
  SDL_BlitSurface(source, NULL, destination, &offset);
}

/* ************ C L A S S E   T I M E R ***************************** */
/*
        Classe timer copiado do tutorial do SDL
        classe usada para controlar os frames
        do jogo.
*/
// The timer
class Timer {
private:
  // The clock time when the timer started
  int startTicks;

  // The ticks stored when the timer was paused
  int pausedTicks;

  // The timer status
  bool paused;
  bool started;

public:
  // Initializes variables
  Timer();

  // The various clock actions
  void start();
  void stop();
  void pause();
  void unpause();

  // Gets the timer's time
  int get_ticks();

  // Checks the status of the timer
  bool is_started();
  bool is_paused();
};

Timer::Timer() {
  // Initialize the variables
  startTicks = 0;
  pausedTicks = 0;
  paused = false;
  started = false;
}

void Timer::start() {
  // Start the timer
  started = true;

  // Unpause the timer
  paused = false;

  // Get the current clock time
  startTicks = SDL_GetTicks();
}

void Timer::stop() {
  // Stop the timer
  started = false;

  // Unpause the timer
  paused = false;
}

void Timer::pause() {
  // If the timer is running and isn't already paused
  if ((started == true) && (paused == false)) {
    // Pause the timer
    paused = true;

    // Calculate the paused ticks
    pausedTicks = SDL_GetTicks() - startTicks;
  }
}

void Timer::unpause() {
  // If the timer is paused
  if (paused == true) {
    // Unpause the timer
    paused = false;

    // Reset the starting ticks
    startTicks = SDL_GetTicks() - pausedTicks;

    // Reset the paused ticks
    pausedTicks = 0;
  }
}

int Timer::get_ticks() {
  // If the timer is running
  if (started == true) {
    // If the timer is paused
    if (paused == true) {
      // Return the number of ticks when the timer was paused
      return pausedTicks;
    } else {
      // Return the current time minus the start time
      return SDL_GetTicks() - startTicks;
    }
  }

  // If the timer isn't running
  return 0;
}

bool Timer::is_started() { return started; }

bool Timer::is_paused() { return paused; }
/* ****************** F I M   T I M E R ************************ */

/************************* CLASSE STRINGINPUT ******************/
/*
 * Classe StringInput  copiado do tutorial do SDL
 * classe usada para obter string, atraves
 * do teclado;
 */
// The key press interpreter
class StringInput {
private:
  // The storage string
  std::string str;

  // The text surface
  SDL_Surface *text;

public:
  // Initializes variables
  StringInput();

  // Does clean up
  ~StringInput();

  // Handles input
  void handle_input();

  // Shows the message on screen
  void show_centered();
};

StringInput::StringInput() {
  // Initialize the string
  str = "";

  // Initialize the surface
  text = NULL;

  // Enable Unicode
  // SDL_EnableUNICODE(SDL_ENABLE);
}

StringInput::~StringInput() {
  // Free text surface
  SDL_FreeSurface(text);

  // Disable Unicode
  // SDL_EnableUNICODE(SDL_DISABLE);
}

void StringInput::handle_input() {
  // If a key was pressed
  if (event.type == SDL_KEYDOWN) {
    // Keep a copy of the current version of the string
    std::string temp = str;

    // If the string less than maximum size
    if (str.length() <= 16) {
      // If the key is a space
      if (event.key.keysym.sym == SDLK_SPACE) {
        // Append the character
        str += (char)event.key.keysym.sym;
      }
      // If the key is a number
      else if ((event.key.keysym.sym >= (Uint16)'0') &&
               (event.key.keysym.sym <= (Uint16)'9')) {
        // Append the character
        str += (char)event.key.keysym.sym;
      }
      // If the key is a uppercase letter
      else if ((event.key.keysym.sym >= (Uint16)'A') &&
               (event.key.keysym.sym <= (Uint16)'Z')) {
        // Append the character
        str += (char)event.key.keysym.sym;
      }
      // If the key is a lowercase letter
      else if ((event.key.keysym.sym >= (Uint16)'a') &&
               (event.key.keysym.sym <= (Uint16)'z')) {
        // Append the character
        str += (char)event.key.keysym.sym;
      }
    }

    // If backspace was pressed and the string isn't blank
    if ((event.key.keysym.sym == SDLK_BACKSPACE) && (str.length() != 0)) {
      // Remove a character from the end
      str.erase(str.length() - 1);
    }

    // If the string was changed
    if (str != temp) {
      // Free the old surface
      SDL_FreeSurface(text);

      // Render a new text surface
      text = TTF_RenderText_Solid(font, str.c_str(), textColor);
      strcpy(texto, str.c_str());
    }
  }
}

void StringInput::show_centered() {
  // If the surface isn't blank
  if (text != NULL) {
    // Show the name
    apply_surface((SCREEN_WIDTH - text->w) / 2, (SCREEN_HEIGHT - text->h) / 2,
                  text, screen);
  }
}

/************** FIM STRINGINPUT *******************/

/* Funcao que gera posicoes x e y aleatorias
 *  para os blocos, recebe como parametro o
 *  indice do elementos do vetor blocos[]
 */
void pos_aleatoria(int i) {
  int x, y, j;

  x = rand() % 380 + 100;
  y = rand() % 450 + 50;
  for (j = 0; j < i; j++) {
    while (abs((int)blocos[j].posx - x) <= 40 &&
           abs((int)blocos[j].posy - y) <=
               80) // controle para evitar gerar um bloco sobre o outro
    {
      if (abs((int)blocos[j].posx - x) <=
          40) // caso a posicao x for igual ou estiver dentro de um intervalo
              // gera outra posicao
        x = rand() % 380 + 100;
      if (abs((int)blocos[j].posy - y) <=
          80) // caso a posicao y for igual ou estiver dentro de um intervalo
              // gera outra posicao
        y = rand() % 450 + 50;
    }
  }

  blocos[i].posx = x;
  blocos[i].posy = y;
}

/*
 * Funcao que carrega os arquivos nas
 * variaveis do SDL, retorna false
 * senao abrir um dos arquivos.
 */
bool carrega_arquivos() {
  background = load_image("background.png");
  background2 = load_image("background2.png");
  menu = load_image("menu1.png");
  menu2 = load_image("menu2.png");
  credito = load_image("credito.bmp");
  barra = load_image("barra.png");
  bola = load_image("bola.png");
  bloco = load_image("bloco.png");
  font = TTF_OpenFont("verdana.TTF", 18);
  font2 = TTF_OpenFont("verdana.TTF", 20);

  return (background && barra && bola && bloco && font && font2 &&
          background2 && menu && menu2 && credito);
}

/*
 * Funcao que inicia o SDL, a fonte e
 *  configura a tela do SDL, retorna falso
 *  se alguma dessas operacoes der erro
 */
bool inicia_tudo() {
  bool arquivos;

  srand(time(NULL)); // inicia a semente para gera�ao de numeros aleatorios

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    return false;
  }

  /* Inicia o suporte para uso de fonte */
  if (TTF_Init() == -1) {
    return false;
  }

  // configura a tela do SDL
  // screen =
  //    SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
  //    SDL_SWSURFACE);
  window = SDL_CreateWindow("Ping: the pong", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_SWSURFACE);
  if (window == NULL) {
    return false;
  }

  screen = SDL_GetWindowSurface(window);

  if (screen == NULL) {
    return false;
  }

  // Configura o nome da tela
  // SDL_WM_SetCaption("Ping : the Pong", NULL);

  arquivos = carrega_arquivos();
  if (!arquivos)
    printf("Erro a carregar os arquivos");

  return (arquivos);
}

/*
 * Funcao que desenha os objetos e textos do jogo
 */
void desenha() {
  int i;

  // Desenha os objetos na tela
  apply_surface(0, 0, background, screen);
  apply_surface(posx_bar_esq, posy_bar_esq, barra, screen);
  apply_surface(posx_bar_dir, posy_bar_dir, barra, screen);
  apply_surface(posx_bola, posy_bola, bola, screen);

  placar = TTF_RenderText_Solid(font2, "Placar:", textColor);
  apply_surface(650, 0, placar, screen);
  SDL_FreeSurface(placar);
  sprintf(texto, "Player 1-> %d", score.placar_p1);
  placar = TTF_RenderText_Solid(font, texto, textColor);
  apply_surface(650, 20, placar, screen);
  SDL_FreeSurface(placar);
  sprintf(texto, "Player 2-> %d", score.placar_p2);
  placar = TTF_RenderText_Solid(font, texto, textColor);
  apply_surface(650, 40, placar, screen);
  SDL_FreeSurface(placar);
  placar = TTF_RenderText_Solid(font, "Player 1 - W - S ", textColor);
  apply_surface(560, 475, placar, screen);
  SDL_FreeSurface(placar);
  placar = TTF_RenderText_Solid(font, "Player 2 - Cima - Baixo", textColor);
  apply_surface(560, 500, placar, screen);
  SDL_FreeSurface(placar);
  placar = TTF_RenderText_Solid(font, "Pausa - p", textColor);
  apply_surface(560, 525, placar, screen);
  SDL_FreeSurface(placar);
  placar = TTF_RenderText_Solid(font, "Voltar - ESC", textColor);
  apply_surface(560, 550, placar, screen);
  SDL_FreeSurface(placar);

  for (i = 0; i < n_blocos; i++) {
    if (blocos[i].estado) {
      apply_surface(blocos[i].posx, blocos[i].posy, bloco,
                    screen); // desenha o bloco caso este esteje abilitado e se
                             // tiver bloco para desenhar
    }
  }

  // Atualiza a tela
  // SDL_Flip(screen);
  SDL_UpdateWindowSurface(window);
}

/*
 * Funcao que limpa a memoria e
 * encerra o suporte a fonte e
 * fecha o SDL, funcao usada ao
 * fechar o programa
 *
 */
void clean_up() {
  // Libera a memoria limpando as superficies
  SDL_FreeSurface(background);
  SDL_FreeSurface(barra);
  SDL_FreeSurface(bola);
  SDL_FreeSurface(placar);
  SDL_FreeSurface(bloco);
  SDL_FreeSurface(background2);
  SDL_FreeSurface(menu);
  SDL_FreeSurface(menu2);
  SDL_FreeSurface(credito);
  SDL_FreeSurface(screen);
  // Fecha as fontes
  TTF_CloseFont(font);
  TTF_CloseFont(font2);

  // Fecha o suporte a fontes
  TTF_Quit();
  IMG_Quit();

  SDL_DestroyWindow(window);

  // Encerra o SDL
  SDL_Quit();
}

/*
 * Funcao que controla a dinamica da barra esquerda
 * como os movimentos da barra, sua velocida, aceleracao
 *
 */
void barra_esquerda() {
  keystates = SDL_GetKeyboardState(NULL);

  if (posy_bar_esq <= 450 &&
      posy_bar_esq >= 50) // limita o espaco que a barra pode percorrer
  {
    /* controle da teclas w e s para movimentar a barra e atribui uma velocida e
     * aceleracao a mesma */
    if ((keystates[SDL_SCANCODE_S] == 0 && keystates[SDL_SCANCODE_W] == 0) ||
        (keystates[SDL_SCANCODE_S] == 1 && keystates[SDL_SCANCODE_W] == 1))
      vely_bar_esq = 0;

    acy_bar_esq = (keystates[SDL_SCANCODE_S] - keystates[SDL_SCANCODE_W]) * 0.5;

    if (vely_bar_esq <= 5 && vely_bar_esq >= -5) {
      vely_bar_esq += acy_bar_esq;
    }

    posy_bar_esq += vely_bar_esq;
  }
  if (posy_bar_esq > 450) // impede que a barra ultrapasse o espaco que esta
                          // pode percorrer, parte inferior
  {
    posy_bar_esq = 450;
    vely_bar_esq = 0;
  }
  if (posy_bar_esq < 50) // impede que a barra ultrapasse o espaco que esta pode
                         // percorrer parte superior
  {
    posy_bar_esq = 50;
    vely_bar_esq = 0;
  }
}

/*
 * Funcao que controla a dinamica da barra direita
 * como os movimentos da barra, sua velocida, aceleracao
 *
 */
void barra_direita() {
  keystates = SDL_GetKeyboardState(NULL);

  if (posy_bar_dir <= 450 &&
      posy_bar_dir >= 50) // limita o espaco que a barra pode percorrer
  {
    /* controle da teclas w e s para movimentar a barra e atribui uma velocida e
     * aceleracao a mesma */
    if ((keystates[SDL_SCANCODE_DOWN] == 0 &&
         keystates[SDL_SCANCODE_UP] == 0) ||
        (keystates[SDL_SCANCODE_DOWN] == 1 && keystates[SDL_SCANCODE_UP] == 1))
      vely_bar_dir = 0;

    acy_bar_dir =
        (keystates[SDL_SCANCODE_DOWN] - keystates[SDL_SCANCODE_UP]) * 0.5;
    if (vely_bar_dir <= 5 && vely_bar_dir >= -5) {
      vely_bar_dir += acy_bar_dir;
    }
    posy_bar_dir += vely_bar_dir;
  }

  if (posy_bar_dir > 450) // impede que a barra ultrapasse o espaco que esta
                          // pode percorrer, parte inferior
  {
    posy_bar_dir = 450;
    vely_bar_dir = 0;
  }
  if (posy_bar_dir < 50) // impede que a barra ultrapasse o espaco que esta pode
                         // percorrer, parte superior
  {
    posy_bar_dir = 50;
    vely_bar_dir = 0;
  }
}

/*
 * Funcao que controla a dinamica da bola
 * como sua posicao na tela, velocidade,
 * aceleracao, espaco que pode percorrer
 * e choque com as barras
 *
 */
void BOLA() {
  posx_bola += velx_bola;
  posy_bola += vely_bola;

  if (posy_bola < 45) // impede que a bola ultrapasse o limite superior da tela
  {
    posy_bola = 45;
    vely_bola = vely_bola * -1; // inverte o movimento em y da bola
  }

  if (posy_bola > 525) // impede que a bola ultrapasse o limite inferior da tela
  {
    posy_bola = 525;
    vely_bola = vely_bola * -1; // inverte o movimento em y da bola
  }

  /* Controla o choque da bola com a barra esquerda */
  if (posx_bola < 50 &&
      ((posy_bola + 25) >= posy_bar_esq &&
       (posy_bola + 5) <= (posy_bar_esq + 100))) // checa se houve o choque
  {
    posx_bola = 50;
    if (velx_bola >= -6 && velx_bola <= 6)
      velx_bola = velx_bola * -1.09; // inverte a velocidade x e soma mais 9% ,
                                     // se a velocidade ainda nao for a maxima
    else
      velx_bola = velx_bola * -1;
    vely_bola +=
        vely_bar_esq; // soma a velocidade y da barra com a velocidade y da bola
    if (vely_bola <= -5 || vely_bola >= 5) // limita a velocidade y da bola
      vely_bola = (int)vely_bola / 5 * 5;
  }

  /* Controla o choque da bola com a barra direita */
  if (posx_bola > 520 &&
      ((posy_bola + 25) >= posy_bar_dir &&
       (posy_bola + 5) <= (posy_bar_dir + 100))) // checa se houve o choque
  {
    posx_bola = 520;
    if (velx_bola >= -6 && velx_bola <= 6)
      velx_bola = velx_bola * -1.09; // inverte a velocidade x e soma mais 9% ,
                                     // se a velocidade ainda nao for a maxima
    else
      velx_bola = velx_bola * -1;
    vely_bola +=
        vely_bar_dir; // soma a velocidade y da barra com a velocidade y da bola
    if (vely_bola <= -5 || vely_bola >= 5) // limita a velocidade y da bola
      vely_bola = (int)vely_bola / 5 * 5;
  }
}

/*
 * funcao que conta o placar do jogo,
 * recebe como parametro o placar maximo
 * para aquela partida, enquanto o placar
 * maximo nao for atingido a funcao retorna
 * true, quando atingido o placar maximo
 * a funcao retoran false
 */
bool pontuacao(int max) {
  if (posx_bola < 25) // verifica se o player 2 marcou ponto
  {
    /* retorna a bola e as barras na posicao inicial */
    posy_bola = 285;
    posx_bola = 285;
    posy_bar_dir = 250;
    posy_bar_esq = 250;
    SDL_Delay(1000);
    vely_bola = 0;
    velx_bola = 4;     // a bola retorna para o player 2
    score.placar_p2++; // acrescenta o placar do player 2
  }

  if (posx_bola > 545) // verifica se o player 1 marcou ponto
  {
    /* retorna a bola e as barras na posicao inicial */
    posy_bola = 285;
    posx_bola = 285;
    posy_bar_dir = 250;
    posy_bar_esq = 250;
    SDL_Delay(1000);
    vely_bola = 0;
    velx_bola = -4;    // a bola retorna para o player 1
    score.placar_p1++; // acrescenta o placar do player 1
  }

  if (score.placar_p1 == max ||
      score.placar_p2 == max) // verifica se a pontuacao maxima
    return false;
  else
    return true;
}

/*
 * funcao que gera os blocos aletorios,
 * e controla o choque deles com a bola
 */
void obstaculo() {
  int i;

  if (estado == true) // se nao ha blocos na tela, a funcao gera mais bloco
  {
    n_blocos = rand() % 10 + 1;
    n_resto = n_blocos;
    estado = false;
    for (i = 0; i < n_blocos; i++) {
      blocos[i].estado = true;
      pos_aleatoria(i);
    }
  }
  i = 0;
  while (i < n_blocos &&
         n_resto > 0) // se existir bloco na tela, verifica a colisao
  {
    /* verifica a colisao entre o bloco e a bola, se o bloco estiver abilitado
     */
    if (blocos[i].estado && (posx_bola + 5) <= (blocos[i].posx + 20) &&
        (posx_bola + 25) >= blocos[i].posx &&
        (posy_bola + 25) >= blocos[i].posy &&
        (posy_bola + 5) <= (blocos[i].posy + 50)) {
      if (posx_bola + 15 >= blocos[i].posx + 20) // parte a direita do bloco
      {
        velx_bola = velx_bola * -1;
      }
      if (posx_bola + 15 <= blocos[i].posx) // parte a esquerda do bloco
      {
        velx_bola = velx_bola * -1;
      }
      if (posy_bola + 15 <= blocos[i].posy) // parte superior do bloco
      {
        vely_bola = vely_bola * -1;
      }
      if (posy_bola + 15 >= blocos[i].posy + 50) // parte inferior do bloco
      {
        vely_bola = vely_bola * -1;
      }
      blocos[i].estado = false; // desabilita o bloco
      n_resto--;                // descrece o numero de bloco
    }
    i++;
  }

  if (n_resto ==
      0) // caso nao haja bloco na tela, abilita a funcao a desenhar mais blocos
    estado = true;
}

/* funcao que escreve no arquivo o placar da partida
 * e armazena os dez ultimos placares
 */
void escreve_arq() {
  PLACAR dados[9];
  int i = 0, j;

  arq = fopen("score.dad", "rb"); // abre o arquivo binario no modo leitura

  if (arq !=
      NULL) // se o arquivo existir, pega os ultimos nove ,ou menos, placares
  {
    while (i < 9 && fread(&dados[i], sizeof(PLACAR), 1, arq) == 1)
      i++;
  }
  fclose(arq);

  arq = fopen("score.dad", "wb"); // abre no modo escrita, excluindo o arquivo
                                  // antigo e criando um novo

  fwrite(&score, sizeof(PLACAR), 1, arq); // escreve o placar da ultima partida
  for (j = 0; j < i; j++)
    fwrite(&dados[j], sizeof(PLACAR), 1,
           arq); // escreve os nove ou menos, anteriores placares

  fclose(arq);
}

/* funcao usada para pegar o nome do player na tela
 * funcao copiada do tutorial do SDL, a funcao
 * recebe como parametro que jogador ela deve pegar o nome
 */
void pega_nome(int player) {
  char aux[30];
  int flag;

  // Keep track if whether or not the user has entered their name
  bool nameEntered = false;

  // The gets the user's name
  StringInput name;

  flag = player + 1;
  // While the user hasn't quit
  while (player != flag) {
    sprintf(aux, "Nome do player %d",
            player); // imprime da na tela o texto para o jogador, saber qual
                     // nome deve digitar
    // Set the message
    placar = TTF_RenderText_Solid(font, aux, textColor);

    // While there's events to handle
    while (SDL_PollEvent(&event)) {
      // If the user hasn't entered their name yet
      if (nameEntered == false) {
        // Get user input
        name.handle_input();

        // If the enter key was pressed
        if ((event.type == SDL_KEYDOWN) &&
            (event.key.keysym.sym == SDLK_RETURN)) {
          // Change the flag
          nameEntered = true;
          player++;
        }
      }
    }
    if (player == 1)
      strcpy(score.player1, texto); // grava o nome do player 1
    if (player == 2)
      strcpy(score.player2, texto); // grava o nome do player 2
    nameEntered = false;

    // Apply the background
    apply_surface(0, 0, background2, screen);

    // Show the message
    apply_surface((SCREEN_WIDTH - placar->w) / 2,
                  ((SCREEN_HEIGHT / 2) - placar->h) / 2, placar, screen);

    // Show the name on the screen
    name.show_centered();

    // Update the screen
    // SDL_Flip(screen);
    SDL_UpdateWindowSurface(window);
  }
}

/* funcao que inicia o jogo
 * chama as outras funcoes para iniciar o jogo
 * e controlar sua dinamica, recebe como parametro
 * o placar maximo da partida
 */
void jogo(int max) {
  bool pause = true;
  bool quit = false;
  Timer fps; // controle de FPS

  /* seta as condicoes inicias do jogo */
  score.placar_p1 = 0;
  score.placar_p2 = 0;
  posx_bar_esq = 45;
  posy_bar_esq = 250;
  vely_bar_esq = 0;
  acy_bar_esq = 0;
  posx_bar_dir = 545;
  posy_bar_dir = 250;
  vely_bar_dir = 0;
  acy_bar_dir = 0;
  posx_bola = 285;
  posy_bola = 285;
  velx_bola = -4;
  vely_bola = 0;
  estado = true;
  n_blocos = 0;

  keystates = SDL_GetKeyboardState(NULL);
  desenha(); // desenha os obejtos na tela
  SDL_Delay(2000);

  while (!quit && !sair) {

    fps.start();
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) // se clicar no botao de fechar
      {
        sair = true; // muda "sair" para true e, dai, sai do while
      }
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym ==
            SDLK_p) // controla o p, para saber se esta tecla foi pressiona, e
                    // assim poder ativar e desativar o pause
          pause = !pause;
        if (event.key.keysym.sym ==
            SDLK_ESCAPE) // se o ESC for pressionada, volta ao menu principal do
                         // jogo
          quit = true;
      }
    }
    if (pause) {
      barra_esquerda();    // chama a funcao de controle da barra esquerda
      barra_direita();     // chama a funcao de controle da barra esquerda
      BOLA();              // chama a funcao de controle da  bola
      if (!pontuacao(max)) // caso a pontuacao de um jogar chegue no max
      {
        pega_nome(1);        // pega o nome do player 1
        pega_nome(2);        // pega o nome do player 2
        escreve_arq();       // escreve o placar no arquivo
        score.placar_p1 = 0; // zera o placar
        score.placar_p2 = 0; // zera o placar
        quit = true;         // volta ao placar
      }
      obstaculo(); // chama a funcao que controla os blocos
    }

    desenha();
    frame++;
    if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
      SDL_Delay((1000 / FRAMES_PER_SECOND) -
                fps.get_ticks()); // controla os frames
    }
  }
}

/* Funcao que cria o menu do jogo,
 * monitora os cliques do mouse, para chamar
 * as funcoes apropriadas
 */
void menu_jogo() {
  bool quit = false;
  Timer fps;

  while (!quit && !sair) {
    fps.start();
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) // se clicar no botao de fechar
      {
        sair = true; // muda "sair" para true e, dai, sai do while
      }

      if (event.type == SDL_MOUSEBUTTONDOWN) { // se o evento for um botao de
                                               // mouse pressionado
        if (event.button.button == SDL_BUTTON_LEFT) // se o botao for o esquerdo
        {
          /* condicoes abaixo delimitando o botoes do botao, e monitoram o
           * clique posicao do mouse para chamar a funcao correta */
          if (event.button.x >= 215 && event.button.x <= 615 &&
              event.button.y >= 215 && event.button.y <= 266)
            quit = true;
          if (event.button.x >= 225 && event.button.x <= 605 &&
              event.button.y >= 272 && event.button.y <= 323)
            jogo(3);
          if (event.button.x >= 225 && event.button.x <= 605 &&
              event.button.y > 323 && event.button.y <= 374)
            jogo(5);
          if (event.button.x >= 225 && event.button.x <= 605 &&
              event.button.y > 374 && event.button.y <= 425)
            jogo(7);
          if (event.button.x >= 215 && event.button.x <= 615 &&
              event.button.y >= 476 && event.button.y <= 527)
            quit = true;
        }
      }
    }

    apply_surface(0, 0, menu2, screen);

    // SDL_Flip(screen);
    SDL_UpdateWindowSurface(window);
  }
}

/* Fun��o que gera os cr�ditos do Jogo */

void creditos() {
  bool quit;
  float pos = 600;
  Timer fps;

  while (!quit && !sair) {
    fps.start();
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) // se clicar no botao de fechar
        sair = true;              // muda "quit" para true e, dai, sai do while

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym ==
            SDLK_ESCAPE) // se o ESC for pressionada, volta ao menu principal do
                         // jogo
          quit = true;
      }
    }

    apply_surface(0, 0, credito, screen);

    /*escreve os creditos na tela */
    placar = TTF_RenderText_Solid(font2, "USP Sao Carlos", textColor);
    apply_surface(260, pos, placar, screen);
    SDL_FreeSurface(placar);

    placar = TTF_RenderText_Solid(font, "ESC para voltar!", textColor);
    apply_surface(550, 10, placar, screen);
    SDL_FreeSurface(placar);

    placar = TTF_RenderText_Solid(font2, "Engenharia de Computacao", textColor);
    apply_surface(200, pos + 20, placar, screen);
    SDL_FreeSurface(placar);

    placar = TTF_RenderText_Solid(font2, "Trabalho II de ICC I", textColor);
    apply_surface(240, pos + 60, placar, screen);
    SDL_FreeSurface(placar);

    placar = TTF_RenderText_Solid(font2, "Ping : the Pong", textColor);
    apply_surface(260, pos + 80, placar, screen);
    SDL_FreeSurface(placar);

    placar = TTF_RenderText_Solid(font2, "Programadores:", textColor);
    apply_surface(260, pos + 120, placar, screen);
    SDL_FreeSurface(placar);

    placar =
        TTF_RenderText_Solid(font2, "Lucas Eduardo Topp - 7126740", textColor);
    apply_surface(190, pos + 140, placar, screen);
    SDL_FreeSurface(placar);

    placar = TTF_RenderText_Solid(font2, "Matheus Cintra Goncalves - 7126611",
                                  textColor);
    apply_surface(160, pos + 160, placar, screen);
    SDL_FreeSurface(placar);

    pos--;              // faz os texto se mexerem na tela
    if (pos + 180 <= 0) // fazem os textos ficarem em loop
      pos = 600;

    // SDL_Flip(screen);
    frame++;
    if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
      SDL_Delay((1000 / FRAMES_PER_SECOND) -
                fps.get_ticks()); // controla os frames
    }
    SDL_UpdateWindowSurface(window);
  }
}

/* funcao que mostra os
 * dez ultimos, ou menos, placares
 */
void mostra_score() {
  bool quit = false;
  PLACAR dados[10];
  int i, j;
  char string[50];

  while (!quit && !sair) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) // se clicar no botao de fechar
        sair = true;              // muda "quit" para true e, dai, sai do while

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym ==
            SDLK_ESCAPE) // se o ESC for pressionada, volta ao menu principal do
                         // jogo
          quit = true;
      }
    }

    apply_surface(0, 0, credito, screen);

    placar = TTF_RenderText_Solid(font, "ESC para voltar!", textColor);
    apply_surface(550, 10, placar, screen);
    SDL_FreeSurface(placar);

    arq = fopen("score.dad", "rb"); // abre o arquivo no modo texto
    if (arq != NULL)                // se o arquivo existir
    {
      placar = TTF_RenderText_Solid(font2, "Ultimos 10 Scores", textColor);
      apply_surface(150, 20, placar, screen);
      SDL_FreeSurface(placar);

      i = 0;
      while (i < 10 && fread(&dados[i++], sizeof(PLACAR), 1, arq) == 1)
        ; // pega dez, ou menos, registros do arquivo
      for (j = 0; j < i; j++) {
        // mostra os placares na tela
        sprintf(string, "%s - %d X %d - %s", dados[j].player1,
                dados[j].placar_p1, dados[j].placar_p2, dados[j].player2);
        placar = TTF_RenderText_Solid(font2, string, textColor);
        apply_surface(150, 50 + j * 20, placar, screen);
        SDL_FreeSurface(placar);
      }

    } else // caso nao haja o arquivo
    {
      // imprime na tela uma mensagem de erro
      placar = TTF_RenderText_Solid(font2, "Nenhum registro encontrado!!",
                                    textColor);
      apply_surface(300, 150, placar, screen);
      SDL_FreeSurface(placar);
    }

    // SDL_Flip(screen);
    SDL_UpdateWindowSurface(window);
    fclose(arq);
  }
}

/* o main chama a funcao de iniciar o SDL
 * e cria o menu principal, e monitora
 * o mouse para chamar as corretas funcoes
 */
int main(int argc, char *args[]) {
  if (!inicia_tudo())                // inicia o SDL
    printf("Erro ao iniciar o SDL"); // mensagem de erro caso na inicie o SDL
  else                               // se o SDL Iniciar corretamente
  {
    while (!sair) {
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) // se clicar no botao de fechar
        {
          sair = true; // muda "sair" para true e, dai, sai do while
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) { // se o evento for um botao de
                                                 // mouse pressionado
          if (event.button.button ==
              SDL_BUTTON_LEFT) // se o botao for o esquerdo
          {
            /* condicoes abaixo delimitando o botoes do botao, e monitoram o
             * clique posicao do mouse para chamar a funcao correta */
            if (event.button.x >= 215 && event.button.x <= 615 &&
                event.button.y >= 215 && event.button.y <= 266)
              menu_jogo();
            if (event.button.x >= 215 && event.button.x <= 615 &&
                event.button.y >= 286 && event.button.y <= 337)
              mostra_score();
            if (event.button.x >= 215 && event.button.x <= 615 &&
                event.button.y >= 357 && event.button.y <= 408)
              creditos();
            if (event.button.x >= 215 && event.button.x <= 615 &&
                event.button.y >= 428 && event.button.y <= 479)
              sair = true;
          }
        }
      }

      apply_surface(0, 0, menu, screen);

      // SDL_Flip(screen);
      SDL_UpdateWindowSurface(window);
    }
  }
  clean_up();
  return 0;
}
