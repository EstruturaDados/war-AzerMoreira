#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIOS 50
#define MAX_JOGADORES 6
#define MAX_NOME 30
#define MAX_COR 10

/* ==============================
   ESTRUTURAS E PROTÓTIPOS
   ============================== */

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

typedef struct {
    char descricao[120];
    int (*verificador)(Territorio*, int, const char*); // callback
} Missao;

/* Protótipos das funções */
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(const Territorio* mapa, int n);
void atacar(Territorio* mapa, int n, const char* corJogador);
void atribuirMissao(char* destino, Missao* missoes, int totalMissoes);
void exibirMissao(const char* missao);
int verificarMissao(const char* descricao, Territorio* mapa, int n);
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int totalJogadores);

/* Funções específicas de verificação de missões */
int missao_conquistar3(Territorio* mapa, int n, const char* cor);
int missao_eliminarVermelho(Territorio* mapa, int n, const char* cor);
int missao_controlarMetade(Territorio* mapa, int n, const char* cor);
int missao_controlarCor(Territorio* mapa, int n, const char* cor);
int missao_terFortes(Territorio* mapa, int n, const char* cor);

/* ==============================
   FUNÇÃO PRINCIPAL
   ============================== */

int main() {
    srand((unsigned)time(NULL));

    int nTerritorios;
    int totalJogadores;

    printf("=== WAR Estruturado — Versão Avançada ===\n\n");
    printf("Número de territórios (max %d): ", MAX_TERRITORIOS);
    scanf("%d", &nTerritorios);
    getchar();

    if (nTerritorios <= 0 || nTerritorios > MAX_TERRITORIOS) {
        printf("Número inválido.\n");
        return 1;
    }

    printf("Número de jogadores (2 a %d): ", MAX_JOGADORES);
    scanf("%d", &totalJogadores);
    getchar();

    if (totalJogadores < 2 || totalJogadores > MAX_JOGADORES) {
        printf("Número inválido de jogadores.\n");
        return 1;
    }

    /* Alocação dinâmica do mapa */
    Territorio* mapa = (Territorio*)calloc((size_t)nTerritorios, sizeof(Territorio));
    cadastrarTerritorios(mapa, nTerritorios);

    /* Criação das missões com callbacks */
    Missao missoesDisponiveis[] = {
        {"Conquistar 3 territorios seguidos.", missao_conquistar3},
        {"Eliminar todas as tropas vermelhas.", missao_eliminarVermelho},
        {"Controlar mais da metade do mapa.", missao_controlarMetade},
        {"Controlar todos os territorios da cor azul.", missao_controlarCor},
        {"Ter ao menos 2 territorios com mais de 10 tropas.", missao_terFortes}
    };
    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(Missao);

    /* Missões dos jogadores */
    char** missoesJogadores = (char**)malloc(sizeof(char*) * totalJogadores);
    char coresJogadores[MAX_JOGADORES][MAX_COR];

    for (int i = 0; i < totalJogadores; i++) {
        printf("Cor do jogador %d: ", i + 1);
        scanf(" %[^\n]", coresJogadores[i]);
        getchar();

        missoesJogadores[i] = (char*)malloc(120 * sizeof(char));
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, totalMissoes);
    }

    /* Exibe missões apenas uma vez */
    printf("\n--- Missões Sorteadas ---\n");
    for (int i = 0; i < totalJogadores; i++) {
        printf("Jogador %d (%s): %s\n", i + 1, coresJogadores[i], missoesJogadores[i]);
    }

    /* Loop principal do menu */
    int opcao;
    int jogadorAtual = 0;
    int vencedor = -1;

    do {
        printf("\n===== MENU =====\n");
        printf("Jogador atual: %d (%s)\n", jogadorAtual + 1, coresJogadores[jogadorAtual]);
        printf("1. Exibir mapa\n");
        printf("2. Atacar\n");
        printf("3. Mostrar missão\n");
        printf("4. Passar turno\n");
        printf("5. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                exibirMapa(mapa, nTerritorios);
                break;
            case 2:
                atacar(mapa, nTerritorios, coresJogadores[jogadorAtual]);
                break;
            case 3:
                printf("Missão: %s\n", missoesJogadores[jogadorAtual]);
                break;
            case 4:
                /* Verificação silenciosa da missão */
                if (verificarMissao(missoesJogadores[jogadorAtual], mapa, nTerritorios)) {
                    vencedor = jogadorAtual;
                    opcao = 5;
                } else {
                    jogadorAtual = (jogadorAtual + 1) % totalJogadores;
                }
                break;
            case 5:
                printf("Encerrando jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 5 && vencedor == -1);

    if (vencedor != -1) {
        printf("\n🏆 Jogador %d (%s) venceu com a missão: %s\n",
               vencedor + 1, coresJogadores[vencedor], missoesJogadores[vencedor]);
    }

    liberarMemoria(mapa, missoesJogadores, totalJogadores);
    return 0;
}

/* ==============================
   FUNÇÕES AUXILIARES
   ============================== */

void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        printf("Cor: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void exibirMapa(const Territorio* mapa, int n) {
    printf("\n--- Mapa Atual ---\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] %-15s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void atacar(Territorio* mapa, int n, const char* corJogador) {
    int a, d;
    printf("\nÍndice atacante: ");
    scanf("%d", &a);
    printf("Índice defensor: ");
    scanf("%d", &d);

    if (a < 0 || a >= n || d < 0 || d >= n) {
        printf("Índices inválidos.\n");
        return;
    }
    if (strcmp(mapa[a].cor, corJogador) != 0) {
        printf("Você só pode atacar de territórios da sua cor!\n");
        return;
    }
    if (strcmp(mapa[a].cor, mapa[d].cor) == 0) {
        printf("Não pode atacar territórios aliados!\n");
        return;
    }

    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        printf("Ataque bem-sucedido! Você conquista o território.\n");
        strcpy(mapa[d].cor, corJogador);
        mapa[d].tropas = mapa[a].tropas / 2;
        mapa[a].tropas -= mapa[d].tropas;
    } else {
        printf("Defesa bem-sucedida. Você perde uma tropa.\n");
        mapa[a].tropas = (mapa[a].tropas > 0) ? mapa[a].tropas - 1 : 0;
    }
}

/* ==============================
   SISTEMA DE MISSÕES COM CALLBACKS
   ============================== */

void atribuirMissao(char* destino, Missao* missoes, int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx].descricao);
}

/* Identifica qual callback usar com base no texto da missão */
int verificarMissao(const char* descricao, Territorio* mapa, int n) {
    if (strstr(descricao, "3 territorios")) return missao_conquistar3(mapa, n, "");
    if (strstr(descricao, "vermelh")) return missao_eliminarVermelho(mapa, n, "");
    if (strstr(descricao, "metade")) return missao_controlarMetade(mapa, n, "");
    if (strstr(descricao, "azul")) return missao_controlarCor(mapa, n, "azul");
    if (strstr(descricao, "10 tropas")) return missao_terFortes(mapa, n, "");
    return 0;
}

/* --- Funções específicas --- */
int missao_conquistar3(Territorio* mapa, int n, const char* cor) {
    for (int i = 0; i < n - 2; i++) {
        if (strcmp(mapa[i].cor, mapa[i+1].cor) == 0 &&
            strcmp(mapa[i].cor, mapa[i+2].cor) == 0)
            return 1;
    }
    return 0;
}

int missao_eliminarVermelho(Territorio* mapa, int n, const char* cor) {
    for (int i = 0; i < n; i++)
        if (strcasecmp(mapa[i].cor, "vermelho") == 0) return 0;
    return 1;
}

int missao_controlarMetade(Territorio* mapa, int n, const char* cor) {
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++)
            if (strcasecmp(mapa[i].cor, mapa[j].cor) == 0) count++;
        if (count > n / 2) return 1;
    }
    return 0;
}

int missao_controlarCor(Territorio* mapa, int n, const char* cor) {
    for (int i = 0; i < n; i++)
        if (strcasecmp(mapa[i].cor, cor) != 0) return 0;
    return 1;
}

int missao_terFortes(Territorio* mapa, int n, const char* cor) {
    int count = 0;
    for (int i = 0; i < n; i++)
        if (mapa[i].tropas > 10) count++;
    return (count >= 2);
}

/* ==============================
   LIBERAÇÃO DE MEMÓRIA
   ============================== */
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int totalJogadores) {
    free(mapa);
    for (int i = 0; i < totalJogadores; i++) free(missoesJogadores[i]);
    free(missoesJogadores);
}
