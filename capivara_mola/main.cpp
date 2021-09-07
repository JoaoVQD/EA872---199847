#include <memory>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

/*
Para executar este c�digo no linux alterar o cabe�alho das bibliotecas:
#include <SDL.h> --> #include <SDL2/SDL.h>
#include <SDL_image.h> --> #include <SDL2/SDL_image.h>

*/

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;




class Model
{
  private:
   float data;
   float m;
   float k;
   float b;
   float x0;
   float v0;
   float dt;
   float a, f, x_atual, v_atual, x_anterior;


public:
    Model();
    void set_posicao(float new_data);
    void set_velocidade(float new_data2);
    float get_posicao();
    float get_velocidade();
    void set_forca(float new_data3);
    float get_forca();
    float get_k();
    float get_m();
    float get_b();
    float get_dt();
    float get_a();
    void set_a(float new_data4);

};

Model::Model() {

    m=10;
    k=100;
    b=2;
    x0=150;
    v0=0;
    dt=0.1;
    x_atual=x0;
    v_atual=v0;
    }
void Model::set_posicao(float new_data) { x_atual = new_data; }
void Model::set_velocidade(float new_data2) { v_atual = new_data2; }
float Model::get_posicao() { return x_atual; }
float Model::get_velocidade() { return v_atual; }
void Model::set_forca(float new_data3) { f = new_data3; }
float Model::get_forca() { return f; }
float Model::get_k() { return k; }
float Model::get_m() { return m; }
float Model::get_b() { return b; }
float Model::get_dt() { return dt; }
float Model::get_a() { return a; }
void Model::set_a(float new_data4){a=new_data4;}

class View
{
  private:
    Model &model;

  public:
    View(Model &model);
    void render();
};

View::View(Model &model):
  model(model)
{  }

void View::render() {
  std::cout << "Posicao: " << model.get_posicao() << std::endl;
}

class Controller
{
  private:
    Model &model;
    View &view;

  public:
    Controller(Model &model, View &view);
    void update();
};

Controller::Controller(Model &model, View &view) :
  // Lista de inicialiacao (initialization list)
  model(model),
  view(view)
{



}

void Controller::update() {

model.set_forca(-model.get_posicao()*model.get_k() -model.get_b()*model.get_velocidade());
model.set_a((model.get_forca())/(model.get_m()));
model.set_velocidade(model.get_velocidade() +model.get_a()*model.get_dt());
model.set_posicao(model.get_posicao() +model.get_velocidade()*model.get_dt());



}

int main(int argc, char *argv[])
{
  Model model = Model();
  View view = View(model);
  Controller controller = Controller(model, view);

  // Inicializando o subsistema de video do SDL
  if ( SDL_Init (SDL_INIT_VIDEO) < 0 ) {
    std::cout << SDL_GetError();
    return 1;
  }

  // Criando uma janela
  SDL_Window* window = nullptr;
  window = SDL_CreateWindow("Demonstracao do SDL2",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
  if (window==nullptr) { // Em caso de erro...
    std::cout << SDL_GetError();
    SDL_Quit();
    return 1;
  }



  // Inicializando o renderizador
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer==nullptr) { // Em caso de erro...
    SDL_DestroyWindow(window);
    std::cout << SDL_GetError();
    SDL_Quit();
    return 1;
  }

  // Carregando texturas
  // personagem
  SDL_Texture *texture = IMG_LoadTexture(renderer, "./capi.png");
  // fundo
  SDL_Texture *texture2 = IMG_LoadTexture(renderer, "./park.jpeg");

  // Quadrado onde a textura sera desenhada
  SDL_Rect target;
  target.x = 0;
  target.y = 0;
  SDL_QueryTexture(texture, nullptr, nullptr, &target.w, &target.h);


  // Controlador:
  bool rodando = true;

  // Variaveis para verificar eventos
  SDL_Event evento; // eventos discretos
  const Uint8* state = SDL_GetKeyboardState(nullptr); // estado do teclado

  // Laco principal do jogo
  target.y=150;
  target.x=150;
  while(rodando) {

        controller.update();
target.x=150+model.get_posicao();



    // Polling de eventos
    SDL_PumpEvents();
     // atualiza estado do teclado
    if (state[SDL_SCANCODE_LEFT]) target.x -= 100;
    if (state[SDL_SCANCODE_RIGHT]) target.x += 100;
    if (state[SDL_SCANCODE_UP]) target.y -= 10;
    if (state[SDL_SCANCODE_DOWN]) target.y += 10;




    while (SDL_PollEvent(&evento)) {
      if (evento.type == SDL_QUIT) {
        rodando = false;
      }
      // Exemplos de outros eventos.
      // Que outros eventos poderiamos ter e que sao uteis?
      //if (evento.type == SDL_KEYDOWN) {
      //}
      //if (evento.type == SDL_MOUSEBUTTONDOWN) {
      //}
    }

    // Desenhar a cena
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture2, nullptr, nullptr);
    SDL_RenderCopy(renderer, texture, nullptr, &target);
    SDL_RenderPresent(renderer);

    // Delay para diminuir o framerate
    SDL_Delay(10);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();








  return 0;
}
