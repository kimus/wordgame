#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordboard.h"

const char alpha[] = "ABCDEFGHILMNOPQRSTUVWXYZ";

int main() {
  int linhas, colunas, mode;
  
  ask_game_mode(&mode);
  ask_dimensions(&linhas, &colunas);
  //mode = 1;
  //linhas = 11;
  //colunas = 11;
  
  char tabuleiro[linhas][colunas];
  char words[linhas][colunas];

  init_board(linhas, colunas, tabuleiro);
  print_board(linhas, colunas, tabuleiro);

  ask_position_and_word_help();
  int ok = 0;
  do {
    // pedir a posição e palavra
    int coluna; int linha; char dir; char word[100];
    ok = ask_position_and_word(linhas, colunas, &linha, &coluna, &dir, word);

    // posição e palavra colocada, inserir a palavra e re-desenhar o tabuleiro
    if (ok == 0) {
      add_word_to_board(linhas, colunas, tabuleiro, words, linha, coluna, dir, word);
      print_board(linhas, colunas, tabuleiro);
    } 
    
    // fim
    else if (ok == 2) {
      // TODO: ...
      printf("Total de pontos: %d\n", 0);
    }
  } while (ok == 0);  

  printf("bye!\n\n");
  return EXIT_SUCCESS;
}

void ask_game_mode(int* m) {
  int ok = 1;
  do {
    printf("Escolha o modo do jogo.\n");
    printf("\t1 - jogador (livre)\n");
    printf("\t2 - computador (livre)\n");
    printf("\t3 - computador (restrição)\n");
    printf("\t4 - computador (restrição e pontuação)\n");

    printf("indique o modo de jogo que quer jogar: ");
    scanf("%d", m);

    int modo = *m;

    if (modo >= 1 && modo <= 4) {
      ok = 0;
    } else {
      printf("Modo inválido. Digite valores entre 1 e 4 e tente novamente!\n");
    }
  }
  while (ok == 1);
}

void ask_dimensions(int* l, int* c) {
  int ok = 1;
  do {
    printf("indique a dimensao do tabuleiro que desejar: ");
    scanf("%dx%d", l, c);

    int linhas = *l;
    int colunas = *c;

    if ((linhas >= 7 && linhas <= 15) && (colunas >= 7 && colunas <= 15)) {
      ok = 0;
    } else {
      printf("Dimensão inválida. Digite valores entre 7 e 15 e tente novamente!\n");
    }
  }
  while (ok == 1);
}

void init_board(int linhas, int colunas, char tabuleiro[linhas][colunas]) {
  for (int linha = 0; linha < linhas; linha++) {
    for (int coluna = 0; coluna < colunas; coluna++) {
      // casas que valem o dobro (cantos do tabuleiro)
      if ((linha == 0 || linha == linhas - 1) && (coluna == 0 || coluna == colunas - 1)) {
        tabuleiro[linha][coluna] = '$';
      }

      // casas que valem o triplo (meio de cada aresta)
      else if (
        ((linha == 0) && (coluna == colunas / 2)) || 
        ((coluna == 0) && (linha == linhas / 2)) || 
        ((coluna == colunas - 1) && (linha == linhas / 2)) || 
        ((linha == linhas - 1) && (coluna == colunas / 2))
      ) {
        tabuleiro[linha][coluna] = '3';
      }

      // casas que valem o dobro (diagonais)
      else if (linha == coluna || coluna == linhas - linha - 1) {
        tabuleiro[linha][coluna] = '2';
      }

      // casa proíbidas
      else if (
        ((linha == 1) && (coluna == ((colunas / 2)-1) || (coluna == (colunas / 2)+1))) ||
        ((coluna == 1) && (linha == ((linhas / 2)-1) || (linha == (linhas / 2) + 1))) ||
        ((linha == linhas - 2) && (coluna == (( colunas / 2) - 1) || (coluna == (colunas / 2) + 1))) ||
        ((coluna == colunas - 2) && (linha == (( linhas / 2) - 1)|| (linha == (linhas / 2) + 1)))
      ) {
        tabuleiro[linha][coluna] = '#';
      }

      // casa vazia
      else {
        tabuleiro[linha][coluna] = '.';
      }
    }
  }
}

void print_board(int linhas, int colunas, char tabuleiro[linhas][colunas]) { 
  for (int linha = 0; linha <= linhas; linha++) {
    printf("\n");

    // imprimir informação da linha
    if (linha != linhas) {
      printf("%2d", linha + 1);
    } else {
      // ultima linha, informação das colunas, imprimir vazio
      printf("   ");
    }

    for (int coluna = 0; coluna < colunas; coluna++) {
      // ultima linha, imprimir informação das colunas
      if (linha == linhas) {
        printf("%c ", alpha[coluna]);
      } else {
        printf("%2c", tabuleiro[linha][coluna]);
      }
    }
  }

  printf("\n\n");
}

void add_word_to_board(int linhas, int colunas, char tabuleiro[linhas][colunas], char words[linhas][colunas], int linha, int coluna, char dir, char word[]) {
  printf("linha: %d\n", linha);
  printf("coluna: %d\n", coluna);
  printf("dir: %c\n", dir);
  printf("palavra: %s\n", word);
}

void ask_position_and_word_help() {
  printf("Deve inserir as palavras da seguinta forma:\n");
  printf("\t1. letra da coluna\n");
  printf("\t2. número da linha\n");
  printf("\t3. H para jogar na horizontal (ou 'V' para jogar na vertical)\n");
  printf("\t4. espaço e palavra.\n");
  printf("Por exemplo:\n");
  printf("\tC5H program\n\n");
}

int ask_position_and_word(int linhas, int colunas, int* linha, int* coluna, char* dir, char* word) {  
  int ok = 1;
  do {
    
    // limpa o buffer de input
    fflush(stdin);

    char input[255];
    printf("Insira as palavaras da forma anteriormente apresentada: ");
    scanf("%[^\n]", input);

    
    int c, l;
    char d;

    // sair do jogo se a palavra for igual a 'fim'
    const char end[] = "fim";
    if (strcmp(input, end) == 0) {
      ok = 2;
      break;
    }

    // a coluna está na primeira posição
    // converter o caracter para um valor numérico indo buscar a posição onde se encontra
    c = (strchr(alpha, input[0]) - alpha) + 1;
    // validar se o resultado está entre os valores possíveis
    if (c <= 0 || c > colunas) {
      printf("\nColuna inválida! Tem de ser um número entre 1 e %d. Por favor tente novamente.\n\n", colunas);
      continue;
    }

    // a linha está na segunda posição
    // validar se o resultado está entre os valores possíveis
    l = input[1] - '0';
    if (l <= 0 || l > linhas) {
      printf("\nLinha inválida! Tem de ser um número entre 1 e %d. Por favor tente novamente.\n\n", linhas);
      continue;
    }

    // a direção está na terceira posição
    // validar se está dentro dos dois valores possíveis
    d = input[2];
    if (d != 'H' && d != 'V') {
      printf("\nDirecção inválida! Tem de ser H ou V. Por favor tente novamente.\n\n");
      continue;
    }

    // o resto do input (retirando um espaço) será a palavra
    strncpy(word, input + 4, strlen(input + 4));

    // copiar valores para os apontadores
    *linha = l;
    *coluna = c;
    *dir = d;

    // TODO: validar se a palavra é válida de acordo com o diconário
    
    ok = 0;
  }
  while (ok == 1);

  return ok;
}
