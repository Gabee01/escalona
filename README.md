## Universidade Federal do Paraná (UFPR).
### Aluno: Gabriel Alcides Carraro
### Professor: Eduardo Cunha de Almeida.
### Disciplina: Banco de dados/Tópicos em bancos de dados.

# Escalonamento de transações

Nesse trabalho foram implementados dois algoritmos que decidem se duas transações podem ser executadas paralelamente.
As transações são lidas até que todas elas sejam commitadas. Quando esse evento aconte, um escalonamento novo e gerado para essas transações e analisado pelos dois algoritmos.

O primeio algoritmo se trata do teste de seriabilidade por conflito. 
Nele utilizamos um grafo, nomeado no código de 'scheduling'(escalonamento), onde cada transação desse escalonamento será um nó, e a adição de arestas se dá pelas seguintes regras: 

1. Aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti
1. Aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti
1. Aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti

Onde Ti e Tj são as duas transações que participam do escalonamento (nodos do grafo).

Após essa adição de arestas, podemos testar o grafo para seriabilidade por conflito. O teste diz que, se o grafo gerado possuir um ciclo, ele não é serial, e logo suas transações não garantem a consistencia dos dados se rodadas simultaneamente.

Para a detecção do ciclo no grafo, foram utilizados os algoritmos apresentados [nesse material sobre ciclos em grafos] do Paulo Feofiloff da USP.

Nesse material são apresentados vários algoritmos para a *detecção de um território aciclico*. Optei pela abordagem recursiva de Terr-Acicl, e o mesmo está como um comentario logo acima do método que o implementa no código (bool aciclicTerritory(Node pNode)), da mesma forma, o método (declarado bool isAciclic(Graph pGraph)) implementa a *detecção de um ciclo* de fato como apresentado na mesma página.

Com esse algoritmo de detecção de ciclos no grafo, obtemos a resposta do teste de seriabilidade por conflito.

O segundo algoritmo, se trata do teste de equivalência por visão. Nele, o objetivo é descobrir se, ao rodarmos duas transações como se tivessem chego serialmente, gerando um novo escalonamento onde imaginamos que as instruções para cada transação tivessem chego em ordem serial (instruções de T1 e depois instruções de T2).
Após a geração desse escalonamento serial, fazemos um teste para ver se o escalonamento gerado é equivalente ao escalonamento original, ou seja, são mantidas as seguintes propriedades:

1. Para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
1. Se o operador w(y) em Tk é a ultima escrita de y em S, então w(y) em Tk deve ser a última escrita em S'

Os métodos que participam desse teste, no código são definidos como:
- bool hasEquivalent(Graph scheduling):
Responsável por iterar e tentar as combinações das ordens das transação (funciona para duas tratando cada uma como primeira de cada vez).
- bool compareByVision(InstructionsList originalScheduling, InstructionsList serialScheduling):
Onde as listas de transações são analisadas para sabermos se obedecem as regras exigidas.

[nesse material sobre ciclos em grafos]:https://www.ime.usp.br/~pf/analise_de_algoritmos/aulas/cycles-and-dags.html

# Log de transações

Nessa segunda parte do trabalho, foram implementados logs para as transações sendo processadas.

Para que o código ficasse bem dividido, os métodos criados para essa segunda parte foram implementados no arquivo logs.c (com cabeçalho em logs.h), porém também foram feitas alterações melhorando o código entregue no primeiro trabalho.

O arquivo que antes se chamava grafo.c(.h) agora se chama dataStructures, visto que possui a implementação de várias estruturas de dados.

As instruções, que antes era tratada como um vetor, agora são armazenadas em uma estutura a parte.

Os logs seguem as regras da especificação, com 4 tipos de comandos:

1. < TS; Ti; start >
1. < TS; Ti; commit >
1. < TS; Ti; abort >
1. < TS; Ti; Xj; V1; V2 >

Transações que não fazem escrita não aparecem no log;
Os valores finais das variáveis não estão sendo salvos em nenhum lugar. Imagino que isso será utilizado para a parte de recuperação de dados, e é facilmente implementável na estrutura que possuo hoje, não a implementei pois fiquei em dúvida como ficaria a saída nesse caso.

Acho que é isso. Sem muitos detalhes para essa segunda parte.