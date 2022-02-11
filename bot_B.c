#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 50

// definindo o struct dos bots
typedef struct
{
  char id[50];
  int x;
  int y;
} Bot;
Bot *bots;
int qtdBots = 0; 

// definindo o struct dos peixes
typedef struct
{
  int x;
  int y;
  int quantidade;
  int valor;
} Peixe;
Peixe *peixes;
int qtdPeixes = 0; 

// definindo o struct dos portos
typedef struct
{
  int x;
  int y;
} Porto;
Porto *portos;
int qtdPortos = 0; 

// definindo a capacidade do bot e sua posicao
int capacidade = 0;
int by, bx;

// ler os dados
void readData(int h, int w)
{
  char id[MAX_STR];
  int v, n, x, y;
  // contadores de pontos de pesca e portos
  int contPeixe = 0;
  int contPorto = 0;

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      scanf("%i", &v);
      // verifica se o ponto é um porto
      // caso seja, realoca o array de portos e 
      // adiciona o porto ao array
      if (v == 1)
      {
        portos = realloc(portos, sizeof(Porto) * (contPorto + 1));
        portos[contPorto].x = j;
        portos[contPorto].y = i;
        contPorto += 1;
      } // caso nao seja um porto
      else if (v > 1)
      {
        // verifica a unidade e a dezena da posicao
        int dezena = ((int)(v / 10.0)) * 10;
        int unidade = v - dezena;

        // realoca o array dos pontos de pesca
        // e adiciona o ponto de pesca ao array,
        // passando suas coordenadas, preço e quantidade
        if (dezena > 0 && unidade > 1)
        {
          peixes = realloc(peixes, sizeof(Peixe) * (contPeixe + 1));
          peixes[contPeixe].x = j;
          peixes[contPeixe].y = i;
          peixes[contPeixe].quantidade = unidade;
          int valor = 0;
          if (dezena == 10)
            valor = 100;
          else if (dezena == 20)
            valor = 200;
          else if (dezena == 30)
            valor = 150;
          peixes[contPeixe].valor = valor;
          contPeixe += 1;
        }
      }
    }
  }

  // adiciona às variaveis globais qtdPeixes e qtdPortos
  // a quantidade, respectivamente, de peixes e portos no
  // mapa
  qtdPeixes = contPeixe;
  qtdPortos = contPorto;

  scanf(" BOTS %i", &n);
  qtdBots = n;
  
  // define o tamanho do array de bot
  bots = malloc(sizeof(Bot) * n);
  for (int i = 0; i < n; i++)
  {
    // adiciona ao array de bots as posições e o id do bot digitado
    scanf("%s %i %i", bots[i].id, &bots[i].y, &bots[i].x);
  }
}

// verifica se há algum bot na posicao digitada
int checkBotIn (int x, int y){
  // verifica se a posicao digitada é um porto
  for (int j = 0; j < qtdPortos; j++){
    if(portos[j].x == x && portos[j].y == y){
      return 0;
    }
  }
  // verifica se há o bot na posicao
  for (int i = 0; i < qtdBots ; i++){
    if(bots[i].x == x && bots[i].y == y)
      return 1;
  }
  return 0;
}

// checa se o bot pode ir para a posicao digitada
int checkValidPosition(int w, int h, int x, int y, char direcao[]) {
  if (strcmp(direcao, "LEFT") == 0){
    if(x == 0 || checkBotIn (x-1, y) == 1)
      return 0;
  }
  else if (strcmp(direcao, "UP") == 0){
    if(y == 0 || checkBotIn (x, y-1) == 1)
      return 0;
  }
  else if (strcmp(direcao, "RIGHT") == 0 ){
    if(x == w-1 || checkBotIn (x+1, y) == 1)
      return 0;
  }
  else if (strcmp(direcao, "DOWN") == 0 ){
    if(y == h-1 || checkBotIn (x, y+1) == 1)
      return 0;
  }
  return 1; 
}

// verifica se o bot pode pescar na posicao
// informada
int canFish (int bx, int by){
  for (int i = 0; i < qtdPeixes; i++) {
    if(bx == peixes[i].x && by == peixes[i].y && capacidade < 10) {
      return 1;
    }
  }
  return 0;
}

// verifica se o bot pode vender na
// posicao informada
int canSell (int bx, int by){
  for (int i = 0; i < qtdPortos; i++) {
    if(bx == portos[i].x && by == portos[i].y && capacidade > 0) {
      return 1;
    }
  }
  return 0;
}

// verifica se o bot deve se mover
int canMove (int f, int s){
  if (f == 0 && s ==0)
    return 1;
  return 0;
}

// pega a posicao do porto mais proximo
int * getCloserHarbor(int x, int y) {
  static int response[2];
  int mod = 1000;
  for (int i = 0; i < qtdPortos; i++) {
    int porX = portos[i].x;
    int porY = portos[i].y;

    int difX = porX - x;
    int difY = porY - y;

    if ( difX < 0 )
      difX *= -1;

    if ( difY < 0 )
      difY *= -1;

    int sum = difX + difY;

    if ( sum < mod ) {
      mod = sum;
      response[0] = porY;
      response[1] = porX;
    }

  }
  return response;
}

// pega a posição do ponto de pesca mais proximo
int * getCloserFish(int x, int y) {
  static int response[2];
  int mod = 1000;
  for (int i = 0; i < qtdPeixes; i++) {
    int fishX = peixes[i].x;
    int fishY = peixes[i].y;

    int difX = fishX - x;
    int difY = fishY - y;

    if ( difX < 0 )
      difX *= -1;

    if ( difY < 0 )
      difY *= -1;

    int sum = difX + difY;

    if ( sum < mod && checkBotIn(fishX, fishY) == 0) {
      mod = sum;
      response[0] = fishY;
      response[1] = fishX;
    }

  }
  return response;
}

int main() 
{
  char line[MAX_STR];
  char myId[MAX_STR];

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  int h, w;

  scanf("AREA %i %i", &h, &w);
  scanf(" ID %s", myId);

  while (1)
  {
    readData(h, w);

    // pega as informacoes do bot
    for (int i = 0; i < qtdBots ; i++){
      if(strcmp(myId, bots[i].id) == 0){
        bx = bots[i].x, by = bots[i].y;  
      }
    }

    // verifica se o bot pode se mover
    // caso ele tenha peixes para vender ainda,
    // ou para pescar, ele não se mexe
    if (canMove(canFish(bx, by), canSell(bx, by)) == 1){
      // MOVIMENTAçÃO
      int *objetivo;
      if (capacidade == 10){
        objetivo = getCloserHarbor(bx, by);
      } else {
        objetivo = getCloserFish(bx, by);
      }

      if(bx > objetivo[1] && checkValidPosition(w,h,bx,by,"LEFT") == 1){
        printf("LEFT\n");
      } else if(bx < objetivo[1] && checkValidPosition(w,h,bx,by,"RIGHT") == 1){
        printf("RIGHT\n");
      } else if(by > objetivo[0] && checkValidPosition(w,h,bx,by,"UP") == 1){
        printf("UP\n");
      } else if(by < objetivo[0] && checkValidPosition(w,h,bx,by,"DOWN") == 1){
        printf("DOWN\n");
      } else {
        printf("\n");
      }

    } else {
      if(canFish(bx, by) == 1) {
        capacidade++;
        printf("FISH\n");
      }
      if(canSell(bx, by) == 1) {
        capacidade = 0;
        printf("SELL\n");
      }
    }

    scanf("%s", line);
  }

  return 0;
}