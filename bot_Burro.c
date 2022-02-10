/********************************************************************
  Bot-exemplo

  Após receber as informações iniciais do jogo, a cada rodada esse
  bot irá se movimentar para esquerda.
  Cabe a você agora aprimorar sua estratégia!!!
 ********************************************************************/

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

/* ADAPTAR EM FUNÇÃO DE COMO OS DADOS SERÃO ARMAZENADOS NO SEU BOT */
void readData(int h, int w)
{
  char id[MAX_STR];
  int v, n, x, y;
  int contPeixe = 0;
  int contPorto = 0;

  // lê os dados da área de pesca
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      scanf("%i", &v);
      if (v == 1)
      {
        // adiciona os portos aos dados
        portos = realloc(portos, sizeof(Porto) * (contPorto + 1));
        portos[contPorto].x = j;
        portos[contPorto].y = i;
        // printf("portos[%d, %d]", portos[contPorto].x, portos[contPorto].y);
        contPorto += 1;
      }
      else if (v > 1)
      {
        // adiciona os peixes aos dados
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

  // lê os dados dos bots
  scanf(" BOTS %i", &n);
  qtdBots = n;
  // o " " antes de BOTS é necessário para ler o '\n' da linha anterior
  bots = malloc(sizeof(Bot) * n);
  for (int i = 0; i < n; i++)
  {
    // adiciona os bots aos dados
    scanf("%s %i %i", bots[i].id, &bots[i].y, &bots[i].x);
  }
}

char movHorizontal[10] = "LEFT";
char movVertical[10] = "UP";

int checkBotIn (int x, int y){
  for (int i = 0; i < qtdBots ; i++){
    if(bots[i].x == x && bots[i].y == y)
      return 1;
  }
  return 0;
}

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

int main() 
{
  char line[MAX_STR]; // dados temporários
  char myId[MAX_STR]; // identificador do bot em questão

  setbuf(stdin, NULL);  // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL); // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);

  // === INÍCIO DA PARTIDA ===
  int h, w;

  scanf("AREA %i %i", &h, &w); // lê a dimensão da área de pesca: altura (h) x largura (w)
  scanf(" ID %s", myId);       // ...e o id do bot
  // obs: o " " antes de ID é necessário para ler o '\n' da linha anterior

  // char nomeArquivo[60] = "";
  // strcat(nomeArquivo, myId);
  // strcat(nomeArquivo, ".txt");

  // FILE *arquivo = fopen(nomeArquivo, "w+"); // abre arquivo

  // if (arquivo == NULL)
  // {
  //   printf("Erro na abertura do arquivo");
  // }
  // else
  // {
  //   fprintf(arquivo, "%d", 10000); // Escreve uma linha no arquivo
  //   fclose(arquivo); // fecha o arquivo
  // }

  // Para "debugar", é possível enviar dados para a saída de erro padrão (stderr).
  // Esse dado não será enviado para o simulador, apenas para o terminal.
  // A linha seguinte é um exemplo. Pode removê-la se desejar.
  // fprintf(stderr, "Meu id = %s\n", myId);

  // === PARTIDA ===
  // O bot entra num laço infinito, mas não se preocupe porque o simulador irá matar
  // o processo quando o jogo terminar.
  while (1)
  {
    readData(h, w);

    // Printa informações



      // Portos
    // fprintf(stderr, "Portos:\n");
    // for (int i = 0; i < qtdPortos; i++) {
    //   fprintf(stderr, "[X: %d,Y: %d]\n", portos[i].x, portos[i].y);
    // }

    // Peixes disponives
    // fprintf(stderr, "Peixes:\n");
    // for (int i = 0; i < qtdPeixes; i++) {
    //   if(peixes[i].y == by)
    //     fprintf(stderr, "[X: %d,Y: %d,Quantidade: %d\n", peixes[i].x,peixes[i].y, peixes[i].quantidade);
    // }
    
      // Informações do bot
    // fprintf(stderr, "Capacidade do bot: %d\n", capacidade);
    for (int i = 0; i < qtdBots ; i++){
      if(strcmp(myId, bots[i].id) == 0){
        // fprintf(stderr, "Cordenadas do Bot:\n");
        bx = bots[i].x, by = bots[i].y;  
        // fprintf(stderr, "[X: %d,Y: %d]\n", bx, by);
      }
    }

    // Ação tomado

    if (canMove(canFish(bx, by), canSell(bx, by)) == 1){
      // LOGICA DE MOVIMENTAÇÃO

      if(tryMove(w,h,bx,by,movHorizontal,movVertical) == 0){
        tryMove(w,h,bx,by,movHorizontal,movVertical);
      }

      //FIM da LOGICA DE MOVIMENTAÇÃO
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

    

    // lê qual foi o resultado da ação (e eventualmente atualiza os dados do bot).
    scanf("%s", line);
  }

  return 0;
}