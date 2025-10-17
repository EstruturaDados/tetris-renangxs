#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

// Representa uma peça do Tetris
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único crescente
} Peca;

// Fila circular de peças futuras
typedef struct {
    Peca itens[TAMANHO_FILA];
    int frente;     // índice do elemento da frente
    int tras;       // índice do último elemento
    int quantidade; // quantos elementos estão na fila
} Fila;

// Pilha de reserva (LIFO)
typedef struct {
    Peca itens[TAMANHO_PILHA];
    int topo; // índice do topo (-1 quando vazia)
} Pilha;

/* ----------- Funções da Fila (fila circular) ----------- */

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->quantidade == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// Enfileira uma peça no final (enqueue)
int enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return 0; // falha
    f->tras = (f->tras + 1) % TAMANHO_FILA;
    f->itens[f->tras] = p;
    f->quantidade++;
    return 1; // sucesso
}

// Desenfileira a peça da frente (dequeue). Retorna peça com id -1 se vazia.
Peca desenfileirar(Fila *f) {
    Peca vazio = {'-', -1};
    if (filaVazia(f)) return vazio;
    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAMANHO_FILA;
    f->quantidade--;
    return removida;
}

/* ----------- Funções da Pilha ----------- */

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == (TAMANHO_PILHA - 1);
}

// Empilha (push). Retorna 1 se sucesso, 0 se cheia.
int empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return 0;
    p->itens[++p->topo] = nova;
    return 1;
}

// Desempilha (pop). Se vazia, retorna peça com id -1.
Peca desempilhar(Pilha *p) {
    Peca vazio = {'-', -1};
    if (pilhaVazia(p)) return vazio;
    return p->itens[p->topo--];
}

/* ----------- Geração automática de peças ----------- */

// Gera peça com tipo aleatório e id sequencial (id passado como argumento)
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

/* ----------- Operações especiais (trocas) ----------- */

// Troca a peça da frente da fila com o topo da pilha.
// Retorna 1 se troca realizada, 0 caso contrário (fila/pilha vazias).
int trocarTopoFilaPilha(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) return 0;
    int idxFrente = f->frente;
    Peca aux = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[p->topo];
    p->itens[p->topo] = aux;
    return 1;
}

// Troca múltipla: troca os 3 primeiros elementos da fila com as 3 peças da pilha.
// Requisito: fila deve conter pelo menos 3 elementos e pilha deve ter 3 elementos.
// Retorna 1 se troca realizada, 0 caso contrário.
int trocaMultipla(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) return 0; // precisa ter ao menos 3 na fila e 3 na pilha

    // Para i = 0..2, trocar fila[posFila(i)] <-> pilha[topo - i]
    for (int i = 0; i < 3; i++) {
        int posFila = (f->frente + i) % TAMANHO_FILA;
        int posPilha = p->topo - i;
        Peca aux = f->itens[posFila];
        f->itens[posFila] = p->itens[posPilha];
        p->itens[posPilha] = aux;
    }
    return 1;
}

/* ----------- Exibição do estado (usabilidade) ----------- */

// Exibe a fila e a pilha de forma clara
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n----------------------------------------\n");
    printf("Estado atual:\n");

    // Fila
    printf("Fila de pecas:\t");
    if (filaVazia(f)) {
        printf("[vazia]");
    } else {
        for (int i = 0; i < f->quantidade; i++) {
            int idx = (f->frente + i) % TAMANHO_FILA;
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
    }
    printf("\n");

    // Pilha (Topo -> Base)
    printf("Pilha reserva:\t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n----------------------------------------\n");
}

/* ----------- Programa principal (menu) ----------- */

int main() {
    srand((unsigned) time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int proximoId = 0;
    int opcao;

    // Inicializa a fila com TAMANHO_FILA peças para começar
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(proximoId++));
    }

    do {
        exibirEstado(&fila, &pilha);

        // Menu
        printf("Opcoes disponiveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            // entrada inválida: limpar stdin e continuar
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada invalida. Tente novamente.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1: { // Jogar peça (dequeue)
                if (filaVazia(&fila)) {
                    printf("Nao ha pecas na fila para jogar.\n");
                } else {
                    Peca jogada = desenfileirar(&fila);
                    printf("Jogando peca [%c %d]\n", jogada.nome, jogada.id);
                    // Após remoção, gerar nova peça e enfileirar para manter fila cheia (quando possível)
                    if (!filaCheia(&fila)) {
                        Peca nova = gerarPeca(proximoId++);
                        enfileirar(&fila, nova);
                        printf("Nova peca gerada e adicionada ao final da fila: [%c %d]\n", nova.nome, nova.id);
                    }
                }
                break;
            }

            case 2: { // Reservar peça (mover da frente da fila para topo da pilha)
                if (filaVazia(&fila)) {
                    printf("Fila vazia: nenhuma peca para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("Pilha de reserva cheia: nao e possivel reservar.\n");
                } else {
                    // Remove da fila e empilha na pilha
                    Peca reservada = desenfileirar(&fila);
                    if (!empilhar(&pilha, reservada)) {
                        // caso improvável porque checamos pilhaCheia, mas por segurança:
                        printf("Erro ao empilhar a peca.\n");
                    } else {
                        printf("Peca reservada [%c %d] movida para o topo da pilha.\n", reservada.nome, reservada.id);
                        // Gera nova peça para manter fila cheia
                        if (!filaCheia(&fila)) {
                            Peca nova = gerarPeca(proximoId++);
                            enfileirar(&fila, nova);
                            printf("Nova peca gerada e adicionada ao final da fila: [%c %d]\n", nova.nome, nova.id);
                        }
                    }
                }
                break;
            }

            case 3: { // Usar peça reservada (pop da pilha)
                if (pilhaVazia(&pilha)) {
                    printf("Pilha vazia: nenhuma peca reservada para usar.\n");
                } else {
                    Peca usada = desempilhar(&pilha);
                    printf("Usando peca reservada [%c %d]\n", usada.nome, usada.id);
                    // Após remoção da pilha (que é uma remoção), gerar nova peça e enfileirar para manter fila cheia
                    if (!filaCheia(&fila)) {
                        Peca nova = gerarPeca(proximoId++);
                        enfileirar(&fila, nova);
                        printf("Nova peca gerada e adicionada ao final da fila: [%c %d]\n", nova.nome, nova.id);
                    }
                }
                break;
            }

            case 4: { // Trocar peça da frente da fila com o topo da pilha
                if (filaVazia(&fila)) {
                    printf("Nao e possivel trocar: fila vazia.\n");
                } else if (pilhaVazia(&pilha)) {
                    printf("Nao e possivel trocar: pilha vazia.\n");
                } else {
                    if (trocarTopoFilaPilha(&fila, &pilha)) {
                        printf("Troca realizada entre a frente da fila e o topo da pilha.\n");
                    } else {
                        printf("Falha na troca.\n");
                    }
                    // Trocas não removem elementos, portanto NENHUMA peça é gerada aqui.
                }
                break;
            }

            case 5: { // Troca múltipla: os 3 primeiros da fila <-> 3 da pilha
                if (fila.quantidade < 3) {
