#include "exames.h"
#include <time.h>
#include <unistd.h>

void simularChegadaPacientes(Fila *filaPacientes, int *idPaciente, int tempo) {
  if (rand() % 100 < 20) {
    Paciente novoPaciente;
    gerarCpfAleatorio(
        novoPaciente.cpf); 
    sprintf(novoPaciente.nome, "Paciente%d", *idPaciente);
    novoPaciente.idade = rand() % 100;
    novoPaciente.id = *idPaciente;

    enfileirar(filaPacientes, *idPaciente);
    (*idPaciente)++;

    printf("Tempo %d: Chegou um novo paciente. Nome: %s, CPF: %s, Idade: %d\n",
           tempo, novoPaciente.nome, novoPaciente.cpf, novoPaciente.idade);
  }
}

void simularExameRaioX(Fila *filaPacientes, Fila *filaExames, Fila *filaLaudo,
                       int *aparelhosDisponiveis, int tempo) {
  if (!filaVazia(filaPacientes) && *aparelhosDisponiveis > 0) {
    int idPaciente = desenfileirar(filaPacientes);
    int tempoExame = rand() % 6 + 5;

    Exame novoExame;
    novoExame.id_paciente = idPaciente;
    novoExame.tempo_exame = tempoExame;
    novoExame.instante_tempo = tempo;

    enfileirar(filaExames, idPaciente);

    printf("Tempo %d: Iniciando exame para Paciente %d. Tempo estimado: %d\n",
           tempo, idPaciente, tempoExame);

    // Adicionando o exame à fila de laudo
    enfileirarLaudo(
        filaLaudo, novoExame.id_paciente, novoExame.instante_tempo, 0,
        0); 
  }
}

void simularLaudos(Fila *filaExames, Fila *filaLaudo,
                   int *radiologistasDisponiveis, int tempo) {
  if (!filaVazia(filaExames) && *radiologistasDisponiveis > 0) {
    int idPaciente = desenfileirar(filaExames);
    int tempoPreparoLaudo = rand() % 21 + 10;

    RegistroLaudo novoLaudo;
    novoLaudo.id_paciente = idPaciente;
    novoLaudo.condicao = rand() % 5;
    novoLaudo.instante_tempo = tempo + tempoPreparoLaudo;
    novoLaudo.tempo_espera = tempo - filaLaudo->inicio->instante_tempo;

    printf("Tempo %d: Laudo para Paciente %d concluído em %d unidades de "
           "tempo. Condição: %d.\n",
           tempo + tempoPreparoLaudo, idPaciente, tempoPreparoLaudo,
           novoLaudo.condicao);

    // Atualizando o tempo de espera para os exames na fila de laudo
    No *atual = filaLaudo->inicio;
    while (atual != NULL) {
      atual->tempo_espera = tempo - atual->instante_tempo;
      atual = atual->proximo;
    }

    // Adicionando o laudo à fila
    enfileirarLaudo(filaLaudo, idPaciente, novoLaudo.condicao,
                    novoLaudo.instante_tempo, novoLaudo.tempo_espera);
    (*radiologistasDisponiveis)++;
  }
}

int main() {
  srand(time(NULL));

  Fila *filaPacientes = inicializarFila(100);
  Fila *filaExames = inicializarFila(100);
  Fila *filaLaudo = inicializarFila(100);

  int idPaciente = 1;
  int aparelhosDisponiveis = 5;
  int radiologistasDisponiveis = 3;

  for (int tempo = 1; tempo <= 43200; tempo++) {
    simularChegadaPacientes(filaPacientes, &idPaciente, tempo);

    if (tempo % 10 == 0) {
      printf("\n--- Relatório de Métricas (Tempo %d) ---\n", tempo);
      calcularEMostrarMetricas(filaLaudo, tempo);
    }

    simularExameRaioX(filaPacientes, filaExames, filaLaudo,
                      &aparelhosDisponiveis, tempo);
    simularLaudos(filaExames, filaLaudo, &radiologistasDisponiveis, tempo);

    sleep(1);
  }

  liberarFila(filaPacientes);
  liberarFila(filaExames);
  liberarFila(filaLaudo);

  return 0;
}
