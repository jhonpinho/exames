#include "exames.h"
#include <stdio.h>
#include <unistd.h>
#define NUMERO_MEDICOS 3

// Variável para representar o número total de médico
int numeroTotalMedicos = NUMERO_MEDICOS;

// Array para rastrear o status de cada médico (0 representa médico livre, 1
// representa médico ocupado)
int medicosOcupados[NUMERO_MEDICOS] = {0};

// Função para verificar se há algum médico livre
int existeMedicoDisponivel() {
  for (int i = 0; i < NUMERO_MEDICOS; ++i) {
    if (!medicosOcupados[i]) {
      // Se encontrar um médico livre, retorna 1
      return 1;
    }
  }

  // Se nenhum médico estiver livre, retorna 0
  return 0;
}

// Função que gera um CPF aleatório no formato padrão nacional
void gerarCpfAleatorio(char *cpf) {
  sprintf(cpf, "%03d.%03d.%03d-%02d", rand() % 1000, rand() % 1000,
          rand() % 1000, rand() % 100);
}

// Função para inicializar a fila.
Fila *inicializarFila(int capacidade) {
  Fila *fila = (Fila *)malloc(sizeof(Fila));
  fila->inicio = NULL;
  fila->fim = NULL;
  fila->tamanho = 0;
  fila->capacidade = capacidade;
  return fila;
}

// Função que calcula e mostra as Métricas.
void calcularEMostrarMetricas(Fila *filaLaudo, int tempo) {
  int totalLaudos = filaLaudo->tamanho;
  int laudosConcluidos = 0;
  int tempoEsperaTotal = 0;

  Fila *filaTemporaria = inicializarFila(filaLaudo->capacidade);

  while (!filaVazia(filaLaudo)) {

    int idPaciente = filaLaudo->inicio->dado;
    int condicao = filaLaudo->inicio->condicao;
    int instanteTempo = filaLaudo->inicio->instante_tempo;
    int tempoEspera = filaLaudo->inicio->tempo_espera;

    // Atualizando métricas 
    if (existeMedicoDisponivel()) {
      laudosConcluidos++;
    }
   tempoEsperaTotal += tempoEspera;
    // Desenfileirar apenas uma vez no loop
    desenfileirar(filaLaudo);

    enfileirarLaudo(filaTemporaria, idPaciente, condicao, instanteTempo,
                    tempoEspera);
  }

  // Restaura a fila original
  while (!filaVazia(filaTemporaria)) {
    enfileirarLaudo(filaLaudo, filaTemporaria->inicio->dado,
                    filaTemporaria->inicio->condicao,
                    filaTemporaria->inicio->instante_tempo,
                    filaTemporaria->inicio->tempo_espera);

    // Desenfileirar apenas uma vez no loop
    desenfileirar(filaTemporaria);
  }

  liberarFila(filaTemporaria);

  printf("Total de laudos concluídos até o momento: %d\n", laudosConcluidos);
  if (laudosConcluidos > 0) {
    printf("Tempo médio de espera para laudo: %.2f unidades de tempo\n",
           (float)tempoEsperaTotal / laudosConcluidos);
  } else {
    printf("Ainda não há laudos concluídos.\n");
  }
  printf("Percentual de laudos concluídos: %.2f%%\n",
         (float)laudosConcluidos / totalLaudos * 100);
}

int filaVazia(Fila *fila) { return (fila->tamanho == 0); }

int filaCheia(Fila *fila) { return (fila->tamanho == fila->capacidade); }

void enfileirar(Fila *fila, int dado) {
  if (filaCheia(fila)) {
    printf("Erro: a fila está cheia.\n");
    exit(EXIT_FAILURE);
  }
  No *novoNo = (No *)malloc(sizeof(No));
  novoNo->dado = dado;
  novoNo->proximo = NULL;

  if (filaVazia(fila)) {
    fila->inicio = novoNo;
    fila->fim = novoNo;
  } else {
    fila->fim->proximo = novoNo;
    fila->fim = novoNo;
  }

  fila->tamanho++;
}

int desenfileirar(Fila *fila) {
  if (filaVazia(fila)) {
    printf("Erro: a fila está vazia.\n");
    exit(EXIT_FAILURE);
  }

  No *noRemovido = fila->inicio;
  int dado = noRemovido->dado;

  fila->inicio = fila->inicio->proximo;
  free(noRemovido);

  fila->tamanho--;

  return dado;
}

void liberarFila(Fila *fila) {
  No *atual = fila->inicio;
  while (atual != NULL) {
    No *proximo = atual->proximo;
    free(atual);
    atual = proximo;
  }
  free(fila);
}

void enfileirarLaudo(Fila *fila, int idPaciente, int condicao,
                     int instanteTempo, int tempoEspera) {
  if (filaCheia(fila)) {
    printf("Erro: a fila de laudos está cheia.\n");
    exit(EXIT_FAILURE);
  }

  No *novoNo = (No *)malloc(sizeof(No));
  novoNo->dado = idPaciente;
  novoNo->condicao = condicao;
  novoNo->instante_tempo = instanteTempo;
  novoNo->tempo_espera = tempoEspera;
  novoNo->proximo = NULL;

  if (filaVazia(fila)) {
    fila->inicio = novoNo;
    fila->fim = novoNo;
  } else {
    fila->fim->proximo = novoNo;
    fila->fim = novoNo;
  }

  fila->tamanho++;
}
