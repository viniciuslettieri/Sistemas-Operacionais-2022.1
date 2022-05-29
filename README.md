# Sistemas-Operacionais-2022.1


Repositório para a tarefa de simulação de escalonamento de processos usando a estratégia de Round Robin com Feedback, da disciplina de Sistemas Operacionais.

Neste trabalho objetivamos desenvolver um **simulador de escalonador de processos** utilizando a estratégia de **Round Robin com Feedback**. Ou seja, os processos serão executados em fatias de tempo e se revezarão para o processamento. Essa ordem é determinada pelo feedback, que consiste em diferentes filas com prioridades, onde os processos são adicionados de acordo com seu estado atual.

Decidimos iniciar o simulador através da **geração de uma tabela de definições de processos**, contendo os tempos de entrada, serviço e os possíveis IOs, assim como vimos durante as aulas teóricas.

A partir dessa tabela, gerada no início do código usando as constantes definidas, fazemos a **simulação do escalonador a cada unidade de tempo**, mostrando as filas e processos ativos.

Para unificar todas as constantes do trabalho, definimo-as no arquivo `Util.h`


## Estrutura Geral do Código

Decidimos organizar o código em arquivos biblioteca separados com suas funções específicas.

Temos nosso **arquivo principal**, chamado `Simulador`, que é responsável por todo o código responsável pelo funcionamento do escalonador e suas operações de print.

Para as bibliotecas auxiliares, temos:

- `Util.h` e `Util.c` → Possui as constantes e enums que servem como premissas para o nosso simulador.
    
- `Processo.h` e `Processo.c` → Definição das estruturas de `Processo`, que será como uma instância de um processo guardando seu PCB, e a `DefinicaoProcesso`, que é como uma linha da tabela de definição dos processos, com as informações de início e fim. Além disso, implementa o gerador automático de uma definição de processo, utilizada para construir nossa tabela.
 
- `Queue.h` e `Queue.h`→ Implementação de fila para as filas de IO(disco, fita, impressora) e as filas de alta/baixa prioridade de processos. Nesse arquivo temos as operações básicas como remover o primeiro elemento da fila e inserir ao fim dela. Além deles, temos outras operações auxiliares e necessárias para o funcionamento do Simulador.
   
- `ListaIO.h` e `ListaIO.h` → Implementação de uma lista encadeada para acompanhar o uso dos IO. Por ser uma lista encadeada, podemos remover elementos no meio e não nos importamos com a ordem deles.


## Funcionamento do Escalonador

Dado todo esse contexto e definida toda a estrutura de dados, o simulador consiste em **iterar entre as seguintes operações** até que todos os processos tenham sido finalizados:

1. **Tratamento de Novos Processos para a Fila de Alta Prioridade.**
    
    Onde iteramos pela tabela de definicao de processos buscando alguém com tempo de entrada igual ao tempo presente.
    
2. **Tratamento dos Processos que saem de IO, para a Fila de Baixa (Disco) ou Alta Prioridade (Fita e Impressora).**
    
    Iteramos pela estrutura de Lista de IO procurando se algum processo saiu de IO. Para isso, comparamos o tempo de saída das operações de IO com o tempo corrente de CPU. Quando forem iguais, removemos o elemento da lista e colocamos o processo na fila correta de acordo com o tipo de IO.
    
3. **Tratamento da Entrada em Fila de IO para o Processo Corrente [caso necessário].**
    
    Insere o processo na fila correspondente de IO e modifica o status dele para bloqueado, já que está ocupado com a operação. Em seguida, remove o processo corrente e a definição de processo corrente, para dar lugar a um novo processo executar (caso haja).
    
4. **Tratamento de Fim de Processo para o Corrente [caso necessário].**
    
    Verifica se o processo em execução está no seu tempo de finalização, determinado na tabela de definição de processos. Lembrando que o “tempo de finalização” é não-inclusivo para o valor final, mas indicamos sua saida nesse tempo inicial da unidade de tempo.
    
5. **Tratamento de Preempção para o Corrente [caso necessário].**
    
    Verifica se o instante atual é o último de execução no quantum atual para o processo corrente, caso positivo envia o processo para a fila de baixa prioridade.
    
6. **Tratamento de Ativação de Novo Processo, caso não tenha nenhum ativo.**
    
    Verifica se existe processo pronto para execução caso não tenha nenhum ativo.
    
7. **Tratamento de Entrada em IO, caso tenha liberado nesse instante e exista outro processo aguardando em fila.**
    
    Ao inicio do instante de tempo pode ter havido alguma liberação de IO, que será testada e poderá pegar um outro processo em fila.


## Execução do Simulador

Como falado anteriormente, subdividimos as tarefas em diversos arquivos e bibliotecas estáticas, o que requer que o arquivo do `Simulador.c` seja compilado em conjunto com os demais arquivos, da seguinte maneira:

`gcc -o simulador Util.c Processo.c ListaIO.c Queue.c Simulador.c`


## Saída do Simulador

Iniciamos nossa saída com a **tabela das definições dos processos**, com seus devidos tempos.

Em seguida mostramos todas as operações por cada unidade de tempo até a conclusão de todos os processos.

E finalizamos com as métricas de turnaround dos processos.