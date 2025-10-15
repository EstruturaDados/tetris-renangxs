#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAMANHO_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Função para inicializar a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

// Função para verificar se a fila está cheia
int filaCheia(Fila *f) {
    return f->quantidade == TAMANHO_FILA;
}

// Função para verificar se a fila está vazia
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// Gera automaticamente uma nova peça com tipo aleatório e id sequencial
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Insere uma nova peça no final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia! Nao e possivel inserir nova peca.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAMANHO_FILA;
    f->itens[f->tras] = p;
    f->quantidade++;
}

// Remove a peça da frente da fila (dequeue)
void desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia! Nenhuma peca para jogar.\n");
        return;
    }
    printf("Jogando peca [%c %d]\n", f->itens[f->frente].nome, f->itens[f->frente].id);
    f->frente = (f->frente + 1) % TAMANHO_FILA;
    f->quantidade--;
}

// Exibe todas as peças da fila
void exibirFila(Fila *f) {
    printf("\nFila de pecas:\n[");
    for (int i = 0; i < f->quantidade; i++) {
        int indice = (f->frente + i) % TAMANHO_FILA;
        printf("%c %d", f->itens[indice].nome, f->itens[indice].id);
        if (i < f->quantidade - 1) printf("] [");
    }
    printf("]\n");
}

int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));

    int opcao;
    int proximoId = 0;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(proximoId++));
    }

    do {
        exibirFila(&fila);
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                break;
            case 2:
                enfileirar(&fila, gerarPeca(proximoId++));
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
