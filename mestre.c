#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
  WAR Estruturado — Missões Estratégicas (versão final)
  - Usa structs, ponteiros e alocação dinâmica.
  - Missões sorteadas e armazenadas dinamicamente (malloc).
  - Modularizado: cadastro, exibição, ataque, atribuição/verificação de missão, liberação.
*/

/* Estrutura que representa um território */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Protótipos */
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(const Territorio* mapa, int n);                // passagem por valor (dados constantes)
void atacar(Territorio* atacante, Territorio* defensor);       // usa ponteiros para modificar
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, Territorio* mapa, int n);
void exibirMissao(const char* missao);                         // exibe por valor (não modifica)
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int totalJogadores);

int main() {
    srand((unsigned) time(NULL));

    int nTerritorios;
    printf("=== WAR Estruturado — Missões Estratégicas ===\n\n");
    printf("Digite o número de territórios no mapa: ");
    if (scanf("%d", &nTerritorios) != 1 || nTerritorios <= 0) {
        printf("Entrada inválida. Encerrando.\n");
        return 1;
    }
    getchar(); // consome \n remanescente

    /* Aloca mapa dinamicamente */
    Territorio* mapa = (Territorio*) calloc((size_t)nTerritorios, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    /* Cadastro inicial */
    cadastrarTerritorios(mapa, nTerritorios);

    /* Vetor de missões (descrições constantes) */
    char* missoesDisponiveis[] = {
        "Conquistar 3 territorios seguidos.",
        "Eliminar todas as tropas da cor vermelha.",
        "Controlar mais da metade do mapa.",
        "Possuir ao menos 2 territorios com mais de 10 tropas.",
        "Conquistar um território inimigo com mais de 8 tropas."
    };
    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    /* Suporta 2 jogadores (expansível) */
    int totalJogadores = 2;
    char** missoesJogadores = (char**) malloc(sizeof(char*) * totalJogadores);
    if (!missoesJogadores) {
        printf("Erro ao alocar memória para missões dos jogadores.\n");
        free(mapa);
        return 1;
    }

    /* Aloca individualmente cada string de missão (armazenamento dinâmico) */
    for (int i = 0; i < totalJogadores; i++) {
        missoesJogadores[i] = (char*) malloc(128 * sizeof(char)); // 128 bytes por missão
        if (!missoesJogadores[i]) {
            printf("Erro ao alocar memória para missão do jogador %d.\n", i + 1);
            /* libera o que já foi alocado e sai */
            for (int j = 0; j < i; j++) free(missoesJogadores[j]);
            free(missoesJogadores);
            free(mapa);
            return 1;
        }
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, totalMissoes);
    }

    /* Exibe as missões apenas uma vez (início) */
    printf("\n--- Missões Sorteadas ---\n");
    for (int p = 0; p < totalJogadores; p++) {
        printf("Jogador %d: ", p + 1);
        exibirMissao(missoesJogadores[p]);
    }

    /* Loop de jogo: turnos simples; o jogador escolhe atacante/defensor */
    char continuar = 's';
    int turno = 1;
    while (continuar == 's' || continuar == 'S') {
        printf("\n===== TURNO %d =====\n", turno);
        exibirMapa(mapa, nTerritorios);

        int idxAtacante, idxDefensor;
        printf("\nEscolha o índice do território atacante (0 a %d): ", nTerritorios - 1);
        if (scanf("%d", &idxAtacante) != 1) break;
        printf("Escolha o índice do território defensor (0 a %d): ", nTerritorios - 1);
        if (scanf("%d", &idxDefensor) != 1) break;

        /* Valida índices */
        if (idxAtacante < 0 || idxAtacante >= nTerritorios ||
            idxDefensor < 0 || idxDefensor >= nTerritorios) {
            printf("Índices inválidos. Tente novamente.\n");
        } else if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
            printf("Não é permitido atacar um território da mesma cor (aliado).\n");
        } else if (mapa[idxAtacante].tropas <= 0) {
            printf("Território atacante não possui tropas suficientes para atacar.\n");
        } else {
            /* Realiza ataque usando ponteiros */
            atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
        }

        /* Verificação silenciosa das missões (após cada turno) */
        for (int p = 0; p < totalJogadores; p++) {
            if (verificarMissao(missoesJogadores[p], mapa, nTerritorios)) {
                printf("\n🏆 Jogador %d cumpriu sua missão e venceu o jogo!\n", p + 1);
                /* libera memória e encerra */
                liberarMemoria(mapa, missoesJogadores, totalJogadores);
                return 0;
            }
        }

        printf("\nDeseja continuar o jogo? (s/n): ");
        scanf(" %c", &continuar);
        turno++;
    }

    /* Ao final, libera memória */
    liberarMemoria(mapa, missoesJogadores, totalJogadores);
    printf("\nJogo encerrado. Memória liberada.\n");
    return 0;
}

/* Função: cadastra territórios (modifica o vetor via ponteiro) */
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);    // aceita espaços
        printf("Cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Quantidade de tropas: ");
        while (scanf("%d", &mapa[i].tropas) != 1) { // validação simples
            printf("Valor inválido. Digite um número inteiro para tropas: ");
            getchar();
        }
        getchar(); // consome \n remanescente
    }
}

/* Exibe o mapa (dados passados por valor como const para não modificar) */
void exibirMapa(const Territorio* mapa, int n) {
    printf("\n--- Mapa Atual ---\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] %-15s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* Simula ataque entre territórios usando ponteiros para modificar dados */
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n>> %s (%s, %d tropas) ataca %s (%s, %d tropas)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    int dadoA = (rand() % 6) + 1; // 1..6
    int dadoD = (rand() % 6) + 1; // 1..6
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        printf("Resultado: atacante venceu!\n");
        /* Transferência de controle: defensor assume cor do atacante.
           Transferimos metade das tropas do atacante para o defensor.
           Ajuste: atacante perde a metade transferida. */
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas <= 0) tropasTransferidas = 1; // ao menos 1
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("%s agora é da cor %s e possui %d tropas. %s ficou com %d tropas.\n",
               defensor->nome, defensor->cor, defensor->tropas,
               atacante->nome, atacante->tropas);
    } else {
        printf("Resultado: defensor resistiu. Atacante perde 1 tropa.\n");
        if (atacante->tropas > 0) atacante->tropas -= 1;
        printf("%s agora possui %d tropas.\n", atacante->nome, atacante->tropas);
    }
}

/* Sorteia uma missão e copia para destino (destino deve ter memória alocada) */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

/* Exibe missão (passagem por valor — não altera a missão) */
void exibirMissao(const char* missao) {
    printf("%s\n", missao);
}

/* Verifica se a missão foi cumprida — lógica simples baseada em palavras-chave */
int verificarMissao(const char* missao, Territorio* mapa, int n) {
    /* Verificações simples (padrões baseados nas descrições das missões) */
    int i, count = 0;

    /* Missão: Conquistar 3 territorios seguidos. */
    if (strstr(missao, "3 territorios") || strstr(missao, "3 territ")) {
        for (i = 0; i < n - 2; i++) {
            if (strcmp(mapa[i].cor, mapa[i+1].cor) == 0 &&
                strcmp(mapa[i].cor, mapa[i+2].cor) == 0 &&
                mapa[i].tropas > 0 && mapa[i+1].tropas > 0 && mapa[i+2].tropas > 0) {
                return 1;
            }
        }
    }

    /* Missão: Eliminar todas as tropas da cor vermelha. */
    if (strstr(missao, "vermelha") || strstr(missao, "vermelho")) {
        for (i = 0; i < n; i++) {
            if (strcasecmp(mapa[i].cor, "vermelho") == 0) {
                return 0; // ainda existe território vermelho
            }
        }
        return 1; // nenhum território vermelho encontrado
    }

    /* Missão: Controlar mais da metade do mapa. */
    if (strstr(missao, "metade") || strstr(missao, "metade do mapa")) {
        /* Tentamos detectar se alguma cor controla > n/2. */
        /* Simples: verifica se existe cor que aparece > n/2 vezes. */
        for (i = 0; i < n; i++) {
            int j, cnt = 0;
            for (j = 0; j < n; j++) {
                if (strcasecmp(mapa[i].cor, mapa[j].cor) == 0) cnt++;
            }
            if (cnt > n / 2) return 1;
        }
    }

    /* Missão: Possuir ao menos 2 territorios com mais de 10 tropas. */
    if (strstr(missao, "10 tropas") || strstr(missao, "mais de 10 tropas") ) {
        for (i = 0; i < n; i++) {
            if (mapa[i].tropas > 10) count++;
        }
        if (count >= 2) return 1;
    }

    /* Missão: Conquistar um território inimigo com mais de 8 tropas. */
    if (strstr(missao, "mais de 8 tropas") || strstr(missao, "8 tropas")) {
        for (i = 0; i < n; i++) {
            if (mapa[i].tropas > 8) return 1;
        }
    }

    /* Caso nenhuma condição específica atendida */
    return 0;
}

/* Libera toda a memória alocada dinamicamente para mapa e missões dos jogadores */
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int totalJogadores) {
    if (mapa) free(mapa);
    if (missoesJogadores) {
        for (int i = 0; i < totalJogadores; i++) {
            if (missoesJogadores[i]) free(missoesJogadores[i]);
        }
        free(missoesJogadores);
    }
}