#include "../include/read_file.h"
#include "../include/create_tree.h"
#include "../include/compress.h"
#include "../include/decompress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

void print_menu() {
  printf("\n--------------------------\n");
  printf("ESCOLHA UMA OPÇÃO:\n[1] - COMPACTAR ARQUIVO\n[2] - DESCOMPACTAR ARQUIVO\n[3] - SAIR");
  printf("\n--------------------------\n");
}

void call_compress() {
  printf("Informe o caminho do arquivo: ");
  char path[PATH_MAX];
  scanf("%s", path);
  compress(path);
  printf("Arquivo comprimido com sucesso!\n");
}

void call_decompress() {
  printf("Informe o caminho do arquivo: ");
  char path[PATH_MAX];
  scanf("%s", path);
  decompress(path);
  printf("Arquivo descompactado com sucesso!\n");
}

int main() {
  while (1) {
    print_menu();
    int op;
    scanf("%d", &op);
    switch (op) {
      case 1:
        call_compress();
        continue;
      case 2:
        call_decompress();
        continue;
      case 3:
        exit(0);
      default:
        printf("Opção inválida\n");
    }
  }
}