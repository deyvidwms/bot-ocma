#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 50

// Estrutura de um bot.
typedef struct
{
  char id[50];
  int x;
  int y;
} Bot;

// Variaveis referentes ao armazenamento dos bots, lista e quantidade.
Bot *bots;
int qtdBots = 0; 

// Estrutura de um peixe.
typedef struct
{
  int x;
  int y;
  int quantidade;
  int valor;
} Peixe;

// Variaveis referentes ao armazenamento dos peixes, lista e quantidade.
Peixe *peixes;
int qtdPeixes = 0; 

// Estrutura de um porto.
typedef struct
{
  int x;
  int y;
} Porto;

// Variaveis referentes ao armazenamento dos portos, lista e quantidade.
Porto *portos;
int qtdPortos = 0; 

// Dados do bot, capacidade e cordenadas atuais.
int capacidade = 0;
int by, bx;


// Movimentação padrão inicial.
char movHorizontal[10] = "LEFT";
char movVertical[10] = "UP";


// -- FUNÇÕES --
// Ler os dados fornecidos ao bot e os armazena em variavies.
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



// Checa se a cordenada indicada está ocupada por um outro bot, caso a cordenada seja um porto mesmo que tenha outro bot lá ele retorna 0, retorna 1 caso a posição esteja ocupada.
int checkBotIn (int x, int y){
  for (int j = 0; j < qtdPortos; j++){
    if(portos[j].x == x && portos[j].y == y){
      return 0;
    }
  }
  for (int i = 0; i < qtdBots ; i++){
    if(bots[i].x == x && bots[i].y == y)
      return 1;
  }
  return 0;
}

// Tenta movimentar horizontalmente, caso não seja possivel tenta movimnetar verticalmente, retorna 1 caso consiga movimentar.
int tryMove(int w, int h, int x, int y, char mH[], char mV[]){
  if (checkValidPosition(w,h,x,y,mH) == 1){
    printf("%s\n",mH);
    return 1;
  } else if (checkValidPosition(w,h,x,y,mV) == 1) {
    printf("%s\n", mV);
    return 1;
  } else 
    return 0;
}

// Checa se a movimentação indicada é valida, retorna 1 caso seja.
int checkValidPosition(int w, int h, int x, int y, char direcao[]) {
  if (strcmp(direcao, "LEFT") == 0){
    if(x == 0 || checkBotIn (x-1, y) == 1){
      strcpy(movHorizontal,"RIGHT");
      return 0;
    }
  }
  else if (strcmp(direcao, "UP") == 0){
    if(y == 0 || checkBotIn (x, y-1) == 1){
      strcpy(movVertical,"DOWN");
      return 0;
    }
  }
  else if (strcmp(direcao, "RIGHT") == 0 ){
    if(x == w-1 || checkBotIn (x+1, y) == 1){
      strcpy(movHorizontal,"LEFT");
      return 0;
    }
  }
  else if (strcmp(direcao, "DOWN") == 0 ){
    if(y == h-1 || checkBotIn (x, y+1) == 1){
      strcpy(movVertical,"UP");
      return 0;
    }
  }
  return 1; 
}

// Verifica se deve pescar, retorna 1 se estiver sobre um ponto de pesca valido, e a capacidade do barco estiver menor que 10.
int canFish (int bx, int by){
  for (int i = 0; i < qtdPeixes; i++) {
    if(bx == peixes[i].x && by == peixes[i].y && capacidade < 10) {
      return 1;
    }
  }
  return 0;
}

// Verifica se deve vender, retorna 1 se estiver sobre um porte e a capacidade do barco for maior que 0, indicando que existe algum peixe guardado.
int canSell (int bx, int by){
  for (int i = 0; i < qtdPortos; i++) {
    if(bx == portos[i].x && by == portos[i].y && capacidade > 0) {
      return 1;
    }
  }
  return 0;
}

// Verifica se deve andar, retorna 1 se não puder pescar ou vender.
int canMove (int f, int s){
  if (f == 0 && s ==0)
    return 1;
  return 0;
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

    // Indentifica o bot pelo myID e guarda as cordenadas.
    for (int i = 0; i < qtdBots ; i++){
      if(strcmp(myId, bots[i].id) == 0){
        bx = bots[i].x, by = bots[i].y;  
      }
    }
    

    // Verifica se deve andar fazer outra ação.
    if (canMove(canFish(bx, by), canSell(bx, by)) == 1){

      // Logica de movimentação do bot burro: Se movimenta na horizontal, caso encontre algum obstaculo inverte o sentido do movimento horizontal e se movimenta verticalmente, caso encontre um obstaculo inverte o sentido da movimentação vertical e se movimenta horizontalmente.
      if(tryMove(w,h,bx,by,movHorizontal,movVertical) == 0){
        tryMove(w,h,bx,by,movHorizontal,movVertical);
      }

    } else {

      // Logica de execução de outra ação qualquer, no caso se for possivel pescar ele pesca e se caso for possivel vender ele vende.
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