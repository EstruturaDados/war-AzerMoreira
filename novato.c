#include <stdio.h>
#include <string.h>

// Definição da estrutura que representa um território
// Agrupa informações relacionadas: nome, cor do exército e quantidade de tropas
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    struct Territorio territorios[5]; // Vetor para armazenar 5 territórios
    int i;

    printf("=== Cadastro de Territórios ===\n\n");

    // Entrada de dados dos 5 territórios
    for (i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);

        // Leitura do nome do território
        printf("Digite o nome do território: ");
        scanf(" %[^\n]", territorios[i].nome);  // Lê string com espaços

        // Leitura da cor do exército
        printf("Digite a cor do exército: ");
        scanf(" %[^\n]", territorios[i].cor);

        // Leitura da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n");
    }

    // Exibição dos dados cadastrados
    printf("\n=== Territórios Cadastrados ===\n\n");
    for (i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
        printf("------------------------------\n");
    }

    return 0;
}