#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Detective Quest - mapa da mansão como 
 * árvore binária de salas (nós)
 *
 * Funções principais:
 *  - criarSala(nome)   : aloca dinamicamete uma sala e copia o nome
 *  - explorarSalas(root): permite navegação interativa pelo mapa
 *  - liberarSalas(root) : libera memória da árvore
 *
 * Regras de navegação interativa:
 *  - 'e' = ir para esquerda
 *  - 'd' = ir para direita
 *  - 's' = sair do jogo
 *
 * O programa monta um mapa fixo (na main) e permite ao jogador
 * explorar a partir do Hall de Entrada até atingir uma sala-folha.
 */

/* Definição da estrutura Sala */
typedef struct Sala {
    char *nome;
    struct Sala *esq; /* caminho para a esquerda */
    struct Sala *dir; /* caminho para a direita */
} Sala;

/* criarSala: cria dinamicamente uma sala com o nome informado.
 * Retorna ponteiro para a nova Sala (ou NULL em caso de erro).
 */
Sala *criarSala(const char *nome) {
    if (nome == NULL) return NULL;
    Sala *nova = (Sala *) malloc(sizeof(Sala));
    if (!nova) {
        fprintf(stderr, "Erro: falha na aloca\xE7\xE3o de mem\xF3ria para Sala.\n");
        return NULL;
    }
    size_t len = strlen(nome);
    nova->nome = (char *) malloc(len + 1);
    if (!nova->nome) {
        fprintf(stderr, "Erro: falha na aloca\xE7\xE3o de mem\xF3ria para nome.\n");
        free(nova);
        return NULL;
    }
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

/* liberarSalas: libera recursivamente toda a árvore de salas. */
void liberarSalas(Sala *root) {
    if (!root) return;
    liberarSalas(root->esq);
    liberarSalas(root->dir);
    free(root->nome);
    free(root);
}

/* lerOpcao: le uma opcao do usuário e retorna o primeiro caractere significativo.
 * Normaliza letras maiúsculas para minúsculas.
 */
char lerOpcao(void) {
    char buffer[100];
    if (!fgets(buffer, sizeof(buffer), stdin)) return '\0';
    /* procurar o primeiro caractere não branco */
    for (size_t i = 0; buffer[i] != '\0'; ++i) {
        if (buffer[i] == '\n' || buffer[i] == '\r') continue;
        if (buffer[i] == ' ' || buffer[i] == '\t') continue;
        char c = buffer[i];
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        return c;
    }
    return '\0';
}

/* explorarSalas: permite a navegação do jogador a partir do nó raiz.
 * Exibe a sala atual, as opções disponíveis e registra o caminho percorrido.
 * A exploração termina quando o jogador atingir uma sala-folha ou escolher 's' para sair.
 */
void explorarSalas(Sala *root) {
    if (!root) {
        printf("Mapa vazio. Nada para explorar.\n");
        return;
    }

    /* Guardar o caminho percorrido (lista simples usando array dinâmico) */
    size_t capacidade = 8;
    size_t tam = 0;
    char **caminho = (char **) malloc(sizeof(char *) * capacidade);
    if (!caminho) {
        fprintf(stderr, "Erro: falha na aloca\xE7\xE3o de mem\xF3ria para caminho.\n");
        return;
    }

    Sala *atual = root;
    while (atual) {
        /* registrar a sala atual no caminho */
        if (tam == capacidade) {
            capacidade *= 2;
            char **tmp = (char **) realloc(caminho, sizeof(char *) * capacidade);
            if (!tmp) {
                fprintf(stderr, "Erro: falha no realloc do caminho.\n");
                break;
            }
            caminho = tmp;
        }
        caminho[tam++] = atual->nome;

        /* exibir informações ao jogador */
        printf("\nVoce esta na sala: %s\n", atual->nome);

        /* verificar se é folha (sem caminhos) */
        int temEsq = (atual->esq != NULL);
        int temDir = (atual->dir != NULL);
        if (!temEsq && !temDir) {
            printf("Esta sala nao possui caminhos. Voce alcancou um comodo final.\n");
            break; /* fim da exploração */
        }

        /* mostrar opções de navegação */
        printf("Escolhas disponiveis:\n");
        if (temEsq) printf("  (e) - Ir para a esquerda -> %s\n", atual->esq->nome);
        if (temDir) printf("  (d) - Ir para a direita  -> %s\n", atual->dir->nome);
        printf("  (s) - Sair da exploracao\n");
        printf("Digite sua escolha (e/d/s): ");

        char op = lerOpcao();
        if (op == '\0') {
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }
        if (op == 's') {
            printf("Exploracao encerrada pelo jogador.\n");
            break;
        }
        if (op == 'e') {
            if (!temEsq) {
                printf("Nao ha caminho para a esquerda desta sala. Tente outra opcao.\n");
                continue;
            }
            atual = atual->esq;
            continue;
        }
        if (op == 'd') {
            if (!temDir) {
                printf("Nao ha caminho para a direita desta sala. Tente outra opcao.\n");
                continue;
            }
            atual = atual->dir;
            continue;
        }

        printf("Opcao desconhecida. Use 'e', 'd' ou 's'.\n");
    }

    /* exibir o caminho percorrido */
    if (tam > 0) {
        printf("\n--- Caminho percorrido ---\n");
        for (size_t i = 0; i < tam; ++i) {
            printf("%zu. %s\n", i + 1, caminho[i]);
        }
        printf("-------------------------\n");
    }

    free(caminho);
}

/* main: monta o mapa (arvore fixa) e inicia a exploracao */
int main(void) {
    /* Montagem manual do mapa da mansao */
    Sala *hall = criarSala("Hall de Entrada");

    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *escritorio = criarSala("Escritorio");
    Sala *quarto = criarSala("Quarto Principal");
    Sala *porao = criarSala("Porao");
    Sala *terraço = criarSala("Terra\xE7o");

    /* conexoes: construir a arvore
     *                Hall
     *               /    \
     *         Sala de Estar  Biblioteca
     *         /      \         /   \
     *     Cozinha  Jardim Escritorio Quarto
     *     /   
     *  Porao
     *
     *  (Terraço sera filho da Biblioteca -> direita direita)
     */
    hall->esq = salaEstar;
    hall->dir = biblioteca;

    salaEstar->esq = cozinha;
    salaEstar->dir = jardim;

    cozinha->esq = porao; /* porao e uma sala-folha */
    cozinha->dir = NULL;

    jardim->esq = NULL;
    jardim->dir = NULL; /* jardim eh folha */

    biblioteca->esq = escritorio;
    biblioteca->dir = quarto;

    escritorio->esq = NULL;
    escritorio->dir = NULL; /* escritorio eh folha */

    quarto->esq = NULL;
    quarto->dir = terraço; /* quarto -> terraço (folha) */

    terraço->esq = NULL;
    terraço->dir = NULL;

    /* iniciar exploracao interativa */
    printf("Bem-vindo(a) a Detective Quest - exploracao da mansao!\n");
    printf("Voce comeca no Hall de Entrada. Escolha caminhos e tente encontrar o culpado...\n");

    explorarSalas(hall);

    /* liberar memoria antes de encerrar */
    liberarSalas(hall);

    printf("Obrigado por jogar!\n");
    return 0;
}