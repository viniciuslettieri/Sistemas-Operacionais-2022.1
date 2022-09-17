# Simulador de Memória - LRU - Least Recently Used

Esse é um simulador do algoritmo de LRU para memória da disciplina de Sistemas Operacionais 1.

O projeto foi desenvolvido para sistema Windows e suporta cliques no terminal quando em MODO 1. O modo, assim como outras variáveis, podem ser alteradas no 'Source/Estruturas.h'. A execução do projeto pode ser realizada com o comando 'make' no diretório 'Trabalho 2'.

## Introdução

Neste trabalho objetivamos desenvolver um **simulador de memória** utilizando a política de **remoção de páginas pelo algoritmo de *Least Recently Used (LRU)***. 

Ou seja, os frames da memória são marcados em ordem de uso, através de uma fila modificada que permite mover um frame da própria fila para o final dela (no caso de uso recente). Quando a memória não possui mais capacidade o frame escolhido para ser removido, através do processo de swap out, será o último da fila.

Também estabelecemos que cada processo possui um limite de frames que pode ter na memória, chamado **Working Set Limit**. Seu funcionamento é semelhante com o do LRU, removendo o menos recentemente utilizado. Sendo assim, utilizamos a própria implementação do LRU para cada um dos processos. Isso faz com que seja necessário sincronizar a inserção e remoção de frames entre o LRU de cada processo e o Geral.

## Constantes de Ambiente

- **Working Set Limit**: Define a quantidade máxima de páginas que um processo pode ter na memória principal. Ao ultrapassar esse limite, a página com uso mais antigo desse processo deverá ter seu frame removido da Memória Principal.
    
    ```c
    #define WORK_SET_LIMIT 4
    ```
    
- **Quantidade de frames na memória**: Define a quantidade máxima de frames que a Memória Principal suporta. Ao ultrapassar esse limite, a página de uso mais antigo de algum dos processos terá seu frame removido da Memória Principal.
    
    ```c
    #define NUM_FRAMES 64
    ```
    
- **Número de páginas de um processo:** Define a quantidade de páginas que cada processo tem.
    
    ```c
    #define NUM_PAGINAS_PROCESSO 50
    ```
    
- **Número de processos criados:** Define máxima de processos criados na simulação.
    
    ```c
    #define NUM_PROCESSOS 20
    ```
    
- **Tamanho do Swap:** Define a quantidade de páginas que o swap suporta.
    
    ```c
    #define TAM_SWAP 500
    ```
    
- **Definimos que nosso swap remove uma página por vez, ao invés de remover um processo inteiro e suas páginas da memória.**
- **Modos de Apresentação:** existem dois modos de apresentação, definidos pelo `MODO` do arquivo `Estruturas.h`. O **modo 1** permite que seja mostrada cada requisição de página por vez, onde o usuário clica na tela para passar para a próxima. No **modo 2** temos a apresentação padrão, em que a cada 3 segundos todos os processos ativos solicitam uma página.
    
    ```c
    #define MODO 2
    ```
    
- **Solicitação de Páginas:** para tornar a simulação mais dinâmica, a cada 3 segundos todos os processos solicitam página, mas não na mesma ordem. Fazemos um shuffle permutativo dos processos atuais para que a ordem dos processos seja aleatória.

## Funcionamento do Simulador

Dado todo esse contexto e definida toda a estrutura de dados, o simulador fica em um loop infinito (basta dar Ctrl + C para parar), efetuando as seguintes etapas:

1. **Criação e aleatoriedade de alocação dos processos:**
    
    Seguimos a regra solicitada de que a cada 3 segundos todos os processos, mas ao invés de seguir a ordem numérica de cada processo, fazemos uma permutaçã para permitir aleatoriedade de ordem.
    
2. **Loop entre os processos ativos para alocação de páginas:**
    
    Inicialmente, geramos um id de página que não está na memória principal para aquele processo.
    
    Com o id da página, podemos alocá-la na memória principal. No entanto, antes de alocá-la, checamos se ela está na área de swap. Se estiver, buscamos de lá e adicionamos na principal.
    
    Fazemos a remoção seguida de criação pois reaproveitamos a estrutura e os métodos do LRU para a nossa área de swap. Idealmente, teríamos a área de swap sendo um hashmap, com o pair (paginaID, PID) e um ponteiro para a struct da página. No entanto, C não disponibiliza tal estrutura, e optamos por não implementar o hashmap por fugir do foco do trabalho. Dessa forma, temos a busca e retorno do elemento da área de swap menos eficiente do que poderia ser.
    
3. **Ainda no loop de alocação de páginas, temos dois fluxos em seguida: 
Quando o processo atingiu ou não seu work set limit.**
    1. **Não atingiu o work set limit**: Como o processo não possui o número máximo de páginas, podemos inserir a página na memória principal respeitando o LRU normalmente. Caso na inserção alguma outra página tenha sido removida, precisamos atualizar o LRU **(do processo)** e a tabela de páginas do processo que sofreu a remoção de página, assim como colocar a página removida na área de swap. Então, por fim, atualizamos o LRU **(do processo)** e a tabela de páginas do processo que alocou a página no loop mais interno.
        
        
    2. **Atingiu o work set limit**: Neste caso, o processo atingiu seu número máximo de páginas, e só poderá remover páginas que ele mesmo alocou na memória. Para descobrir a página a ser removida, ele utiliza a estrutura do seu próprio LRU com as páginas alocadas por ele na memória principal. Dessa forma, ele remove a página mais antiga que alocou que ainda está ativa e põe a nova página alocada no LRU do processo.
        
        Com a página em mãos, ele acessa a tabela de páginas e pega o ponteiro para a página na memória principal. Então, com o método auxiliar removeDaMemoriaPrincipal, ele faz uso do LRU ser duplamente encadeado para fazer a remoção acessando diretamente a memória principal e alterando os ponteiros. Em seguida, ele remove também da tabela de páginas a página removida e a insere na área de swap. Por fim, insere na memória principal e na tabela de páginas a nova página alocada.

## Execução do Simulador

Como falado anteriormente, subdividimos as tarefas em diversos arquivos auxiliares, o que requer que o arquivo do `Principal.c` seja compilado em conjunto com os demais arquivos, da seguinte maneira:

`gcc -o simulador Estruturas.c Fila.c Interface.c Principal.c`

## Saída do Simulador

Primeiro, vamos denotar as responsabilidades de cada área da interface gráfica, para em seguida demonstrar a execução do programa. Então, temos as seguintes áreas:

- **Próxima Página Requisitada:** Refere-se à pagina alocada pelo processo corrente. Usando o modo de execução em clique é fácil de acompanhar as alocações. Agora, se usamos o modo de execução normal, a alocação de páginas é muito rápida e fica difícil de acompanhar em tempo real.
- **Frames em Memória:** Indica quais frames estão atualmente alocados por páginas na memória principal. Normalmente, será um valor entre 0 a n, com valor máximo de n sendo 63 em nossas execuções.
- **Sequência LRU:** Refere-se ao LRU principal, a representação da memória principal. Nele, indicamos o frame e a página alocada nele, com o ID dela e o PID. Em nossas execuções, terá tamanho limite de 64 frames.
- **Tabela de Páginas:** Indicamos as páginas alocadas por cada um dos processos. Se a página está alocada pelo processo na memória principal, ela terá um valor em nossa tabela. Caso contrário, ficará vazio. Temos então, P processos como nossas linhas, e M páginas como nossas colunas da tabela.
- **Área de Swap:** Possui o mesmo formato do LRU da memória principal, tirando o fato de que não indicamos o frame da área de swap. Dessa forma, nossa área de Swap é um LRU com um limite bem grande, que apenas armazena a página caso ela seja requisitada novamente pelo processo solicitante.

**Tendo em mente cada área da interface gráfica, vejamos em detalhes o programa em execução:**

**OBS:** Temos dois modos de execução. O primeiro modo é o descrito nos requisitos do trabalho, com o intervalo de 3 segundos entre a execução de todos os processos. O segundo modo (utilizado para demonstração) aguarda o clique do usuário na tela para fazer a próxima alocação de página.