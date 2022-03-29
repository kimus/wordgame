#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "wordboard.h"

const char alpha[] = "ABCDEFGHILMNOPQRSTUVWXYZ";
const char points[] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10 };
int dic_lines = 0; // numero de palavras no dicionário
char** dic_words; // apontador para o dicionário de palavras

int main() {
  int linhas, colunas, mode;

  read_dict_file("/usr/share/dict/words");
  
  ask_game_mode(&mode);
  ask_dimensions(&linhas, &colunas);
  //mode = 1;
  //linhas = 11;
  //colunas = 11;
  
  char tabuleiro[linhas][colunas];
  char words[linhas][colunas];

  // 'limpar' as posições das palabras do tabuleiro
  for (int i = 0; i < linhas; i++) {
    memset(words[i], 0, colunas);
  }

  init_board(linhas, colunas, tabuleiro);
  print_board(linhas, colunas, tabuleiro, words);

  ask_position_and_word_help();
  int ok = 0;
  do {
    // pedir a posição e palavra
    int coluna; int linha; char dir; char word[100] = {0};
    ok = ask_position_and_word(linhas, colunas, &linha, &coluna, &dir, word);

    // posição e palavra colocada, inserir a palavra e re-desenhar o tabuleiro
    if (ok == 0) {
      add_word_to_board(linhas, colunas, tabuleiro, words, linha, coluna, dir, word);
      print_board(linhas, colunas, tabuleiro, words);
    } 
    
    // fim
    else if (ok == 2) {
      print_board_points(linhas, colunas, tabuleiro, words);
    }
  } while (ok == 0);  

  printf("bye!\n\n");
  return EXIT_SUCCESS;
}

void read_dict_file(char* file) {
  // abrir o ficheiro para leitura
  FILE *f= fopen(file, "r");

  if (f == NULL) {
    // perror() imprime o motivo do erro em vez de usar printf()
    perror("Não foi possível ler o dicionário de palavras");
    exit(EXIT_FAILURE);
  }

  // descobrir o numero de linhas do ficheiro
  char ch;
  dic_lines = 0;
  do {
    ch = fgetc(f);
    if (ch == '\n') dic_lines++;
  } while (ch != EOF);

  // voltar à primeira linha do ficheiro
  rewind(f);

  // alocar em memória
  dic_words = (char **) malloc(dic_lines * sizeof(char *));
  if (dic_words == NULL) {
    perror("Não foi possível alocar memóri para o dicionário de palavras");
    exit(EXIT_FAILURE);
  }

  // ler o ficheiro linha a linha e copiar para a memória
  for (int i = 0; i < dic_lines; i++) {
    dic_words[i] = NULL;
    size_t n = 0;
    size_t len = getline(&dic_words[i], &n, f);
    if (ferror(f)) {
      perror("Erro ao ler o dicionário");
      exit(EXIT_FAILURE);
    }

    dic_words[i][len -1] = '\0'; // remover a quebra de linha
  }

  printf("Foram encontradas %d palavras no dictionario.\n", dic_lines);

  // fechar o ficheiro
  fclose(f);
}

int is_word_valid(char word[]) {
  for (int i = 0; i < dic_lines; i++) {
    if (strcmp(dic_words[i], word) == 0) {
      return 0;
    }
  }
  return 1;
}

void ask_game_mode(int* m) {
  int ok = 1;
  do {
    fflush(stdin);

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
    fflush(stdin);

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
        ((linha == 1) && (coluna == ((colunas / 2) - 1) || (coluna == (colunas / 2) + 1))) ||
        ((coluna == 1) && (linha == ((linhas / 2) - 1) || (linha == (linhas / 2) + 1))) ||
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

void print_board(int linhas, int colunas, char tabuleiro[linhas][colunas], char words[linhas][colunas]) { 
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
        // se existir palavras já inseridas colocálas
        char c = words[linha][coluna];
        if (c && c != '\0') {
          printf("%2c", c);  
        } else {
          printf("%2c", tabuleiro[linha][coluna]);
        }
      }
    }
  }

  printf("\n\n");
}

void print_board_points(int linhas, int colunas, char tabuleiro[linhas][colunas], char words[linhas][colunas]) {
  int pontos = 0;

  for (int linha = 0; linha < linhas; linha++) {
    for (int coluna = 0; coluna < colunas; coluna++) {
      char p = tabuleiro[linha][coluna];
      char c = words[linha][coluna];
      if (c && c != '\0') {
        // buscar o index da letra e buscar os pontos da mesma
        int idx = (strchr(alpha, toupper(c)) - alpha) + 1;
        int pnt = points[idx];

        // aplicar os pontos
        if (p == '3') {
          pontos += pnt * 3;
        } else if (p == '2' || p == '$') {
          pontos += pnt * 2;
        } else {
          pontos += pnt;
        }
      }
    }
  }

  printf("\nTotal de pontos: %d\n", pontos);
}

void add_word_to_board(int linhas, int colunas, char tabuleiro[linhas][colunas], char words[linhas][colunas], int linha, int coluna, char dir, char word[]) {
  int l = linha - 1;
  int c = coluna - 1;
  int len = strlen(word);

  /*
  printf("linha: %d de %d (%d)\n", linha, linhas, c + len - 1);
  printf("coluna: %d de %d\n", coluna, colunas);
  printf("dir: %c\n", dir);
  printf("palavra: %s\n", word);
  */

  // valida se a palavra é válida de acordo com o diconário
  if (is_word_valid(word) == 1) {
    printf("\n\tPalavra inválida! Não existe no dicionário!\n");
    return;
  }


  // valida se a palavra cabe no tabuleiro
  if (
    (dir == 'H' && (c + len - 1) > colunas) ||
    (dir == 'V' && (l + len - 1) > linhas)
  ) {
    printf("\n\tA palavra não poder ser aplicada porque passa fora no tabuleiro!\n");
    return;
  }

  // valida se a palavra pode ser colocana tabuleiro (se intercecta alguma casa proíbida)
  for (int i = 0; i < len; i++) {
    
    // posição do tabuleiro
    int ll = l, cc = c;
    if (dir == 'H') {
      cc = c + i;
    } else {
      ll = l + i;
    }

    // buscar a casa da posição calculada
    char letra = tabuleiro[ll][cc];

    // validar a casa
    if (letra == '#') {
      printf("\n\tA palavra não pode ser aplicada porque a posição %c%d é uma casa proíbida!\n", alpha[cc], linha);
      return;
    }
  }

  // a palavra pode ser colocada sem problemas no tabuleiro
  // inserir a palavra na posição e direcção pretendidas
  for (int i = 0; i <= len; i++) {
    // posição do tabuleiro
    int ll = l, cc = c;
    if (dir == 'H') {
      cc = c + i;
    } else {
      ll = l + i;
    }
    
    // buscar a letra da palavra
    char letra = tolower(word[i]);
    
    // inserir a letra na posição do tabuleiro
    words[ll][cc] = letra;
  }
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
    c = (strchr(alpha, toupper(input[0])) - alpha) + 1;
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
    d = toupper(input[2]);
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
    
    ok = 0;
  }
  while (ok == 1);

  return ok;
}
