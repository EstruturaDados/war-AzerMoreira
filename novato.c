#include <stdio.h>
#include <string.h>

/*
 * Estrutura que representa um território no sistema.
 * Cada território possui:
 * - nome: nome do território
 * - cor: cor do exército que o controla
 * - tropas: quantidade de tropas presentes
 */
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    // Vetor que armazenará 5 territórios
    struct Territorio territorios[5];

    printf("=== Cadastro de Territórios ===\n\n");

    // Laço para entrada dos dados de 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("Digite os dados do território %d:\n", i + 1);

        // Limpa o buffer de entrada (evita problemas com fgets)
        fflush(stdin);

        printf("Nome do território: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        // Remove o '\n' do final, se existir
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n");
    }

    // Exibição dos dados cadastrados
    printf("=== Territórios Cadastrados ===\n\n");
    for (int i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do Exército: %s\n", territorios[i].cor);
        printf("Quantidade de Tropas: %d\n", territorios[i].tropas);
        printf("---------------------------\n");
    }

    return 0;
}