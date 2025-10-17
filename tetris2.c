#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

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

// Estrutura da pilha
typedef struct {
    Peca itens[TAMANHO_PILHA];
    int topo;
} Pilha;

// ---------- Funções da Fila ----------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == TAMANHO_FILA;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAMANHO_FILA;
    f->itens[f->tras] = p;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (!filaVazia(f)) {
        removida = f->itens[f->frente];
        f->frente = (f->frente + 1) % TAMANHO_FILA;
        f->quantidade--;
    }
    return removida;
}

// ---------- Funções da Pilha ----------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAMANHO_PILHA - 1;
}

void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Nao e possivel reservar mais pecas.\n");
        return;
    }
    p->itens[++p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (!pilhaVazia(p)) {
        removida = p->itens[p->topo--];
    } else {
        printf("Pilha vazia! Nenhuma peca reservada.\n");
    }
    return removida;
}

// ---------- Funções Gerais ----------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Exibir o estado atual da fila e pilha
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n=========================\n");
    printf("Estado atual:\n");

    // Exibe a fila
    printf("Fila de pecas: ");
    if (filaVazia(f)) {
        printf("[vazia]\n");
    } else {
        for (int i = 0; i < f->quantidade; i++) {
            int indice = (f->frente + i) % TAMANHO_FILA;
            printf("[%c %d] ", f->itens[indice].nome, f->itens[indice].id);
        }
        printf("\n");
    }

    // Exibe a pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]\n");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
        printf("\n");
    }

    printf("=========================\n");
}

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int proximoId = 0;
    int opcao;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(proximoId++));
    }

    do {
        exibirEstado(&fila, &pilha);
        printf("Opcoes de acao:\n");
        printf("1 - Jogar peca (remover da frente)\n");
        printf("2 - Reservar peca (mover para pilha)\n");
        printf("3 - Usar peca reservada (remover do topo)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("Jogando peca [%c %d]\n", jogada.nome, jogada.id);
                    // Após jogar, gera nova peça
                    enfileirar(&fila, gerarPeca(proximoId++));
                } else {
                    printf("Fila vazia! Nao ha peca para jogar.\n");
                }
                break;
            }

            case 2: {
                if (!filaVazia(&fila)) {
                    if (!pilhaCheia(&pilha)) {
                        Peca reservada = desenfileirar(&fila);
                        printf("Reservando peca [%c %d]\n", reservada.nome, reservada.id);
                        empilhar(&pilha, reservada);
                        // Gera nova peça após reserva
                        enfileirar(&fila, gerarPeca(proximoId++));
                    } else {
                        printf("A pilha de reserva esta cheia!\n");
                    }
                } else {
                    printf("A fila esta vazia!\n");
                }
                break;
            }

            case 3: {
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1) {
                    printf("Usando peca reservada [%c %d]\n", usada.nome, usada.id);
                }
                // Após usar, gera nova peça
                enfileirar(&fila, gerarPeca(proximoId++));
                break;
            }

            case 0:
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
