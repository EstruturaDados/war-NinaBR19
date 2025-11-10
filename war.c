// ============================================================================
// Programa: Cadastro de Territórios
// Descrição: Este programa cadastra e exibe informações de 5 territórios,
// utilizando uma struct para armazenar nome, cor do exército e número de tropas.
// ============================================================================

#include <stdio.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Definição da estrutura (struct) que representa um território
// ---------------------------------------------------------------------------
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    // Vetor que armazena até 5 territórios
    struct Territorio territorios[5];

    // Loop para entrada dos dados dos 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("\n=== Cadastro do Território %d ===\n", i + 1);

        // Lendo o nome do território
        printf("Digite o nome do território: ");
        scanf("%s", territorios[i].nome);

        // Lendo a cor do exército
        printf("Digite a cor do exército: ");
        scanf("%s", territorios[i].cor);

        // Lendo a quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição dos dados cadastrados
    printf("\n=============================================\n");
    printf("     LISTA DE TERRITÓRIOS CADASTRADOS\n");
    printf("=============================================\n");

    for (int i = 0; i < 5; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do Exército: %s\n", territorios[i].cor);
        printf("Quantidade de Tropas: %d\n", territorios[i].tropas);
    }

    printf("\n=============================================\n");
    printf("Fim do cadastro! Dados exibidos com sucesso.\n");
    printf("=============================================\n");

    return 0;
    printf("Novo commit\n");
}
