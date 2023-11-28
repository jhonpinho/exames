#ifndef EXAMES_H
#define EXAMES_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char nome[50];
  char cpf[15];
  int idade;
  int id;
} Paciente;

typedef struct {
  int id_paciente;
  int tempo_exame;
  int instante_tempo;
  int condicao; 
} Exame;

typedef struct {
  int id_paciente;
  int instante_tempo;
  int condicao;     
  int tempo_espera; 
} RegistroLaudo;

typedef struct No {
  int dado;
  int condicao;
  int instante_tempo;
  int tempo_espera;
  struct No *proximo;
} No;

typedef struct {
  No *inicio;
  No *fim;
  int tamanho;
  int capacidade;
} Fila;

void gerarCpfAleatorio(char *cpf);
Fila *inicializarFila(int capacidade);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
void enfileirar(Fila *fila, int dado);
int desenfileirar(Fila *fila);
void liberarFila(Fila *fila);
void enfileirarLaudo(Fila *fila, int idPaciente, int condicao,
                     int instanteTempo, int tempoEspera);
void calcularEMostrarMetricas(Fila *filaLaudo, int tempo);
int existeMedicoDisponivel();
#endif
