#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 50

typedef struct
{
  char id[50];
  int x;
  int y;
} Bot;
Bot *bots;
int qtdBots = 0; 

typedef struct
{
  int x;
  int y;
  int quantidade;
  int valor;
} Peixe;
Peixe *peixes;
int qtdPeixes = 0; 

typedef struct
{
  int x;
  int y;
} Porto;
Porto *portos;
int qtdPortos = 0; 

int capacidade = 0;
int by, bx;

void readData(int h, int w)
{
  char id[MAX_STR];
  int v, n, x, y;
  int contPeixe = 0;
  int contPorto = 0;

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      scanf("%i", &v);
      if (v == 1)
      {
        portos = realloc(portos, sizeof(Porto) * (contPorto + 1));
        portos[contPorto].x = j;
        portos[contPorto].y = i;
        contPorto += 1;
      }
      else if (v > 1)
      {
        int dezena = ((int)(v / 10.0)) * 10;
        int unidade = v - dezena;

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

  qtdPeixes = contPeixe;
  qtdPortos = contPorto;

  scanf(" BOTS %i", &n);
  qtdBots = n;
  bots = malloc(sizeof(Bot) * n);
  for (int i = 0; i < n; i++)
  {
    scanf("%s %i %i", bots[i].id, &bots[i].y, &bots[i].x);
  }
}

int checkBotIn (int x, int y){
  for (int i = 0; i < qtdBots ; i++){
    if(bots[i].x == x && bots[i].y == y)
      return 1;
  }
  return 0;
}

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

int canFish (int bx, int by){
  for (int i = 0; i < qtdPeixes; i++) {
    if(bx == peixes[i].x && by == peixes[i].y && capacidade < 10) {
      return 1;
    }
  }
  return 0;
}

int canSell (int bx, int by){
  for (int i = 0; i < qtdPortos; i++) {
    if(bx == portos[i].x && by == portos[i].y && capacidade > 0) {
      return 1;
    }
  }
  return 0;
}

int canMove (int f, int s){
  if (f == 0 && s ==0)
    return 1;
  return 0;
}

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

int * getCloserFish(int x, int y) {
  static int response[2];
  int mod = 0;
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
    float valorPasso = (peixes[i].valor*peixes[i].quantidade)/sum;

    if ( valorPasso > mod && checkBotIn(fishX, fishY) == 0) {
      mod = valorPasso;
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

    for (int i = 0; i < qtdBots ; i++){
      if(strcmp(myId, bots[i].id) == 0){
        bx = bots[i].x, by = bots[i].y;  
      }
    }


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