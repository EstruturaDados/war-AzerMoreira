#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ----------------------
// Definição da Struct
// ----------------------
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ----------------------
// Protótipos das Funções
// ----------------------
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirTerritorios(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

// ----------------------
// Função principal
// ----------------------
int main() {
    srand(time(NULL)); // Garante aleatoriedade nos ataques

    int n;
    printf("=== WAR Estruturado ===\n\n");
    printf("Digite o número de territórios a cadastrar: ");
    scanf("%d", &n);
    getchar(); // limpa o buffer do teclado

    // Alocação dinâmica do vetor de territórios
    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Cadastro inicial
    cadastrarTerritorios(mapa, n);

    // Exibição inicial
    printf("\n=== Territórios cadastrados ===\n");
    exibirTerritorios(mapa, n);

    // Simulação de ataque
    int iAtacante, iDefensor;
    char continuar = 's';

    while (continuar == 's' || continuar == 'S') {
        printf("\nEscolha o território atacante (0 a %d): ", n - 1);
        scanf("%d", &iAtacante);
        printf("Escolha o território defensor (0 a %d): ", n - 1);
        scanf("%d", &iDefensor);

        // Validação
        if (iAtacante < 0 || iAtacante >= n || iDefensor < 0 || iDefensor >= n) {
            printf("❌ Índice inválido!\n");
        } else if (strcmp(mapa[iAtacante].cor, mapa[iDefensor].cor) == 0) {
            printf("⚠️ Não é possível atacar um território da mesma cor!\n");
        } else {
            atacar(&mapa[iAtacante], &mapa[iDefensor]);
        }

        printf("\n=== Situação Atual ===\n");
        exibirTerritorios(mapa, n);

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
    }

    liberarMemoria(mapa);
    printf("\nMemória liberada. Encerrando o jogo.\n");

    return 0;
}

// ----------------------
// Função de Cadastro
// ----------------------
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i);

        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);

        printf("Cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// ----------------------
// Função de Exibição
// ----------------------
void exibirTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %-15s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// ----------------------
// Função de Ataque
// ----------------------
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n🎲 Iniciando ataque de %s (%s) contra %s (%s)...\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Simula dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    // Resultado do ataque
    if (dadoAtacante > dadoDefensor) {
        printf("💥 O atacante venceu a batalha!\n");

        // Transferência de domínio e tropas
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;

    } else {
        printf("🛡️ O defensor resistiu! O atacante perde uma tropa.\n");
        if (atacante->tropas > 0)
            atacante->tropas -= 1;
    }
}

// ----------------------
// Função de Liberação de Memória
// ----------------------
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}
