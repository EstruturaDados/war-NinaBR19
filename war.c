// war_structurado.c
// PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO (Nível Aventureiro)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define NUM_TERRITORIOS 5
#define MAX_STR 64

typedef struct {
    char nome[MAX_STR];
    char cor[MAX_STR];    // cor do exército que domina o território
    int tropas;
} Territorio;

/* Protótipos das funções */

// Setup e gerenciamento de memória
Territorio* alocarMapa(size_t n);
void liberarMemoria(Territorio* mapa);

// Inicialização e exibição
void inicializarTerritorios(Territorio* mapa, size_t n);
void exibirMapa(const Territorio* mapa, size_t n, const char* corJogador);
void exibirMissao(int idMissao);

// Menu e input
void exibirMenuPrincipal(void);
void limparBufferEntrada(void);

// Fase de jogo / lógica
void faseDeAtaque(Territorio* mapa, size_t n, const char* corJogador);
void simularAtaque(Territorio* mapa, int idxOrigem, int idxDestino, const char* corJogador);
int sortearMissao(void);
int verificarVitoria(const Territorio* mapa, size_t n, const char* corJogador, int idMissao);

/* Implementação */

Territorio* alocarMapa(size_t n) {
    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    return mapa;
}

void liberarMemoria(Territorio* mapa) {
    if (mapa) free(mapa);
}

void inicializarTerritorios(Territorio* mapa, size_t n) {
    // Inicialização automática com nomes e configurações básicas
    const char* nomes[NUM_TERRITORIOS] = {
        "Amazonas", "Sertao", "Pantanal", "Mata Atlantica", "Cerrado"
    };
    // Cores possíveis que aparecerão no mapa (exceto a do jogador, que será atribuída depois)
    const char* coresPossiveis[] = {"Verde", "Azul", "Amarelo", "Vermelho"};
    for (size_t i = 0; i < n; ++i) {
        strncpy(mapa[i].nome, nomes[i % (sizeof(nomes)/sizeof(nomes[0]))], MAX_STR-1);
        // sortear cor e tropas iniciais
        const char* cor = coresPossiveis[rand() % (sizeof(coresPossiveis)/sizeof(coresPossiveis[0]))];
        strncpy(mapa[i].cor, cor, MAX_STR-1);
        // tropas entre 1 e 5
        mapa[i].tropas = 1 + rand() % 5;
    }
}

void exibirMapa(const Territorio* mapa, size_t n, const char* corJogador) {
    printf("\n=== MAPA ATUAL ===\n");
    printf("%-3s | %-18s | %-10s | %-6s\n", "ID", "TERRITÓRIO", "COR", "TROPAS");
    printf("----+--------------------+------------+-------\n");
    for (size_t i = 0; i < n; ++i) {
        // Indica com '*' se é do jogador
        char donoMarcador = (strcmp(mapa[i].cor, corJogador) == 0) ? '*' : ' ';
        printf("%2zu%c | %-18s | %-10s | %6d\n", i+1, donoMarcador, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("(Territórios marcados com '*' pertencem a você — cor: %s)\n", corJogador);
}

void exibirMissao(int idMissao) {
    printf("\n=== MISSÃO SECRETA ===\n");
    if (idMissao == 1) {
        printf("Destruir o exército Verde (eliminar todos os territórios com cor 'Verde').\n");
    } else if (idMissao == 2) {
        printf("Conquistar 3 territórios (ter 3 ou mais territórios sob sua cor).\n");
    } else {
        printf("Missão desconhecida.\n");
    }
}

void exibirMenuPrincipal(void) {
    printf("\n--- MENU ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* limpa */ }
}

void faseDeAtaque(Territorio* mapa, size_t n, const char* corJogador) {
    int origem = 0, destino = 0;
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Informe o ID do território atacante (pertencente a você): ");
    if (scanf("%d", &origem) != 1) { limparBufferEntrada(); printf("Entrada inválida.\n"); return; }
    limparBufferEntrada();
    printf("Informe o ID do território defensor (qualquer ID): ");
    if (scanf("%d", &destino) != 1) { limparBufferEntrada(); printf("Entrada inválida.\n"); return; }
    limparBufferEntrada();

    if (origem < 1 || origem > (int)n || destino < 1 || destino > (int)n) {
        printf("IDs devem estar entre 1 e %d.\n", (int)n);
        return;
    }
    if (origem == destino) {
        printf("Origem e destino não podem ser o mesmo território.\n");
        return;
    }

    int idxOrigem = origem - 1;
    int idxDestino = destino - 1;

    if (strcmp(mapa[idxOrigem].cor, corJogador) != 0) {
        printf("O território de origem não pertence a você.\n");
        return;
    }
    if (mapa[idxOrigem].tropas < 2) {
        printf("É necessário ter pelo menos 2 tropas no território atacante para iniciar um ataque.\n");
        return;
    }

    simularAtaque(mapa, idxOrigem, idxDestino, corJogador);
}

int numDadosAtaque(int tropasAtacante) {
    // Em regra similar ao Risk: atacante pode rolar até 3 dados, mas nunca mais do que tropas-1
    int max = tropasAtacante - 1;
    if (max >= 3) return 3;
    if (max >= 1) return max;
    return 0;
}

int numDadosDefesa(int tropasDefensor) {
    // defensor pode rolar até 2 dados, mas nunca mais do que tropas
    if (tropasDefensor >= 2) return 2;
    if (tropasDefensor >= 1) return 1;
    return 0;
}

int comparadorDecrescente(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ib - ia; // decrescente
}

void simularAtaque(Territorio* mapa, int idxOrigem, int idxDestino, const char* corJogador) {
    int tropasA = mapa[idxOrigem].tropas;
    int tropasD = mapa[idxDestino].tropas;

    int dadosA = numDadosAtaque(tropasA);
    int dadosD = numDadosDefesa(tropasD);

    if (dadosA <= 0) {
        printf("Ataque inválido: atacante não tem tropas suficientes para rolar dados.\n");
        return;
    }
    if (dadosD <= 0) {
        printf("Defensor não tem tropas — conquista automática.\n");
        // conquista direta
        mapa[idxDestino].cor[0] = '\0';
        strncpy(mapa[idxDestino].cor, corJogador, MAX_STR-1);
        mapa[idxOrigem].tropas -= 1;
        mapa[idxDestino].tropas = 1;
        printf("Território %s conquistado por %s!\n", mapa[idxDestino].nome, corJogador);
        return;
    }

    int maxDados = (dadosA > 3 ? dadosA : 3);
    int rollsA[3] = {0,0,0};
    int rollsD[2] = {0,0};

    // rolar dados
    for (int i = 0; i < dadosA; ++i) {
        rollsA[i] = 1 + rand() % 6;
    }
    for (int i = 0; i < dadosD; ++i) {
        rollsD[i] = 1 + rand() % 6;
    }

    // ordenar decrescente
    qsort(rollsA, dadosA, sizeof(int), comparadorDecrescente);
    qsort(rollsD, dadosD, sizeof(int), comparadorDecrescente);

    // mostrar resultados
    printf("\nResultado dos dados:\n");
    printf("Atacante (%s) - %d dados: ", mapa[idxOrigem].cor, dadosA);
    for (int i = 0; i < dadosA; ++i) printf("%d ", rollsA[i]);
    printf("\nDefensor (%s) - %d dados:  ", mapa[idxDestino].cor, dadosD);
    for (int i = 0; i < dadosD; ++i) printf("%d ", rollsD[i]);
    printf("\n");

    // comparar pares (até min(dadosA, dadosD))
    int pares = (dadosA < dadosD) ? dadosA : dadosD;
    int perdasA = 0, perdasD = 0;
    for (int i = 0; i < pares; ++i) {
        if (rollsA[i] > rollsD[i]) {
            // defensor perde 1 tropa
            perdasD++;
        } else {
            // empate ou defensor maior -> atacante perde 1 tropa (empates favorecem defensor por regra clássica,
            // mas no enunciado "Empates favorecem o atacante" foi mencionado em níveis — aqui vamos manter empate favorece atacante)
            // Para se alinhar ao enunciado, considerar que empate favorece atacante:
            if (rollsA[i] == rollsD[i]) {
                // empate favorece atacante -> defensor perde
                perdasD++;
            } else {
                perdasA++;
            }
        }
    }

    mapa[idxOrigem].tropas -= perdasA;
    mapa[idxDestino].tropas -= perdasD;

    printf("Perdas: Atacante perdeu %d tropa(s). Defensor perdeu %d tropa(s).\n", perdasA, perdasD);

    // verificar se defensor perdeu todas as tropas -> conquista
    if (mapa[idxDestino].tropas <= 0) {
        printf("Território %s foi conquistado por %s!\n", mapa[idxDestino].nome, corJogador);
        // transferir propriedade
        strncpy(mapa[idxDestino].cor, corJogador, MAX_STR-1);
        // mover 1 tropa do atacante para o território conquistado (se possível)
        if (mapa[idxOrigem].tropas >= 1) {
            mapa[idxOrigem].tropas -= 1;
            mapa[idxDestino].tropas = 1;
        } else {
            // caso improvável se atacante ficou sem tropas, colocar 1 tropa
            mapa[idxDestino].tropas = 1;
        }
    } else {
        printf("Território %s permanece com %d tropa(s).\n", mapa[idxDestino].nome, mapa[idxDestino].tropas);
    }

    // validar que tropas não fiquem negativas
    if (mapa[idxOrigem].tropas < 0) mapa[idxOrigem].tropas = 0;
    if (mapa[idxDestino].tropas < 0) mapa[idxDestino].tropas = 0;
}

int sortearMissao(void) {
    // 2 missões disponíveis: 1 - destruir Verde, 2 - conquistar 3 territórios
    return 1 + rand() % 2;
}

int verificarVitoria(const Territorio* mapa, size_t n, const char* corJogador, int idMissao) {
    if (idMissao == 1) {
        // verificar se existe algum território com cor "Verde"
        for (size_t i = 0; i < n; ++i) {
            if (strcmp(mapa[i].cor, "Verde") == 0) return 0; // ainda existe verde
        }
        return 1; // não encontrou "Verde" -> missão cumprida
    } else if (idMissao == 2) {
        int cont = 0;
        for (size_t i = 0; i < n; ++i) {
            if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
        }
        return (cont >= 3) ? 1 : 0;
    }
    return 0;
}

/* Função principal */
int main(void) {
    setlocale(LC_ALL, ""); // configura locale para português (se suportado)
    srand((unsigned int) time(NULL));

    // 1. Setup
    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (!mapa) {
        fprintf(stderr, "Falha na alocaçao do mapa.\n");
        return 1;
    }
    inicializarTerritorios(mapa, NUM_TERRITORIOS);

    // Escolher cor do jogador (fixa ou sorteada)
    const char* opcoesCores[] = {"Vermelho", "Verde", "Azul", "Amarelo"};
    const char* corJogador = opcoesCores[rand() % (sizeof(opcoesCores)/sizeof(opcoesCores[0]))];
    int idMissao = sortearMissao();

    printf("Bem-vindo ao Desafio WAR Estruturado!\n");
    printf("Sua cor: %s\n", corJogador);
    exibirMissao(idMissao);

    // Garantir que ao menos um território pertença ao jogador no início:
    // atribuir o primeiro território encontrado que não seja da mesma cor
    int pertence = 0;
    for (int i = 0; i < NUM_TERRITORIOS; ++i) {
        if (strcmp(mapa[i].cor, corJogador) == 0) { pertence = 1; break; }
    }
    if (!pertence) {
        // força posse do primeiro território
        strncpy(mapa[0].cor, corJogador, MAX_STR-1);
    }

    // 2. Loop principal
    int opcao = -1;
    do {
        exibirMapa(mapa, NUM_TERRITORIOS, corJogador);
        exibirMissao(idMissao);
        exibirMenuPrincipal();
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }
        limparBufferEntrada();
        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS, corJogador);
                break;
            case 2: {
                int v = verificarVitoria(mapa, NUM_TERRITORIOS, corJogador, idMissao);
                if (v) {
                    printf("\nParabéns! Você cumpriu sua missão!\n");
                    exibirMapa(mapa, NUM_TERRITORIOS, corJogador);
                    // encerrar o jogo com vitória
                    opcao = 0;
                } else {
                    printf("\nMissão ainda não cumprida. Continue jogando.\n");
                }
                break;
            }
            case 0:
                printf("Saindo do jogo. Até a próxima!\n");
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }

    } while (opcao != 0);

    // 3. Limpeza
    liberarMemoria(mapa);

    return 0;
    printf("Novo Commit\n");
}
