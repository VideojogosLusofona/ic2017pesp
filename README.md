<!--
Projeto de Época Especial de Introdução à Computação 2017/2018 (c) by Nuno
Fachada

Projeto de Época Especial de Introdução à Computação 2017/2018 is licensed
under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International
License.

You should have received a copy of the license along with this
work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
-->

[![Enunciado: CC BY-NC-SA 4.0](https://img.shields.io/badge/Enunciado-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)
[![Código: GPLv3](https://img.shields.io/badge/Código-GPLv3-blue.svg)](http://www.gnu.org/licenses/gpl.html)

# Projeto de Época Especial de Introdução à Computação 2017/2018

## Descrição do problema

Os alunos devem implementar, em grupos de 1 a 3 elementos, um jogo _roguelike_
em C com níveis [gerados procedimentalmente](#procedural) em grelhas 8x8. O
jogador começa no lado esquerdo da grelha (1ª coluna), e o seu objetivo é
encontrar a saída do nível, que se encontra do lado direito dessa mesma grelha
(8ª coluna). Pelo meio o jogador pode encontrar NPCs (agressivos ou neutros),
encontrar itens (comida, armas, mapas), possivelmente apanhando-os, e cair em
armadilhas.

Os níveis vão ficando progressivamente mais difíceis, com mais monstros, mais
armadilhas e menos itens. O _score_ final do jogador é igual ao nível atingido,
existindo uma tabela dos top 10 _high scores_, que deve persistir quando o
programa termina e o PC é desligado.

No início de cada nível, o jogador só tem conhecimento da sua vizinhança (de
[Von Neumann][]). À medida que o jogador se desloca, o mapa vai-se revelando. O
jogador só pode deslocar-se na sua vizinhança de [Von Neumann][] usando as
teclas WASD.

### Modo de funcionamento

O jogo começa por apresentar o menu principal, que deve conter as seguintes
opções:

1.  New game
2.  High scores
3.  Credits
4.  Quit

Caso o utilizador selecione as opções 2 ou 3, é mostrada a informação
solicitada, após a qual o utilizador pressiona ENTER (ou qualquer tecla) para
voltar ao menu principal. A opção 4 termina o programa. Se for selecionada a
opção 1, começa um novo jogo.

#### Ações disponíveis no jogo

As ações disponíveis em cada _turn_ são as seguintes:

*   `WSAD` para movimento.
*   `F` para atacar um NPC no _tile_ atual.
*   `E` para apanhar um item no _tile_ atual (incluindo mapas).
*   `U` para usar um item (arma ou comida).
    *   No caso de uma arma, a mesma é equipada (selecionada para combate).
        Caso exista uma arma equipada anteriormente, a mesma passa para o
        inventário.
    *   No caso de comida, a mesma é consumida, aumentando o HP na quantidade
        especificada para a comida em questão, até um máximo de 100.
*   `V` para deixar cair um item (arma ou comida) no _tile_ atual.
*   `I` para mostrar informação acerca dos itens (armas e comida) e armadilhas
    disponíveis no jogo. Esta opção **não** consome uma _turn_.
*   `Q` para terminar o jogo.

As opções `F`, `E`, `U` e `V` devem ser seguidas de um número, indicando qual o
NPC a atacar ou o item a apanhar/usar/deixar cair. Deve ser permitido cancelar
a opção antes da indicação do número, sem que o jogador perca uma _turn_.

Em cada _turn_ é consumido automaticamente 1 HP do jogador, independentemente
da ação realizada.

#### O jogador

O jogador tem várias características, algumas definidas diretamente, outras
calculáveis a partir das restantes:

*   `HP` (_hit points_) - Vida do jogador, entre 0 e 100; quando chega a zero
    o jogador morre.
*   `SelectedWeapon` - A arma que o jogador usa em combate.
*   `Inventory` - Lista de itens que o jogador transporta, nomeadamente comida
    e armas.
*   `MaxWeight` - Constante que define o peso máximo que o jogador pode
    carregar.
*   `Weight` - Peso total de tudo o que o jogador transporta, nomeadamente
    itens no inventário (armas e comida) e a arma equipada. Não pode
    ultrapassar `MaxWeight`.

#### NPCs

Os NPCs têm as seguintes características:

*   `HP` (_hit points_) - Vida do NPC, semelhante à do jogador. Inicialmente os
    NPCs devem ter HPs relativamente pequenos, aumentando à medida que o jogo
    progride para níveis mais difíceis. O HP inicial dos NPCs é
    [aleatório](#procedural), mas nunca ultrapassando o valor 100.
*   `AttackPower` - O máximo de HP que o NPC pode retirar ao jogador em cada
    ataque. Inicialmente os NPCs devem ter um `AttackPower` relativamente
    pequeno, aumentando à medida que o jogo progride para níveis mais difíceis.
    O `AttackPower` de cada NPC é [aleatório](#procedural), mas nunca
    ultrapassando o valor 100.
*   `State` - Estado do NPC, um de dois estados possíveis:
    *   _Hostile_ - Ataca o jogador assim que o jogador se move para o seu
        _tile_.
    *   _Neutral_ - NPC ignora o jogador quando o jogador se move para o seu
        _tile_. Caso o jogador ataque um NPC neste estado, o estado do NPC é
        alterado para _Hostile_.

#### Itens

Todos os itens têm a seguinte característica:

*   `Weight` - Peso do item.

Existem os seguintes itens em concreto:

*   Comida - Podem existir diferentes tipos de comida, à escolha dos alunos.
    Cada tipo diferente de comida fornece ao jogador um HP pré-definido
    (`HPIncrease`), que não pode ultrapassar o valor 100, quando usado.
*   Armas - Podem existir diferentes tipos de armas, à escolha dos alunos. Cada
    tipo diferente de arma tem um `AttackPower` e `Durability` específicos. O
    primeiro, entre 1 e 100, representa o máximo de HP que o jogador pode
    retirar ao NPC quando o ataca. A `Durability`, entre 0 e 1, representa a
    [probabilidade](#procedural) da arma não se estragar quando usada num
    ataque. As armas são retiradas do jogo no momento em que se estragam.

Os itens podem existir em qualquer _tile_ do nível (exceto `EXIT!`) bem como
no inventário do jogador. No caso das armas, podem ainda ser equipadas pelo
jogador. Os itens podem também ser deixados cair pelos NPCs no _tile_ onde se
encontram após perderem um combate com o jogador.

#### Mapas

Existe um mapa por nível, colocado [aleatoriamente](#procedural) num _tile_
(excepto o _tile_ `EXIT!`). Caso o jogador apanhe o mapa, todas as partes
inexploradas do nível são reveladas.

#### Combate

Um NPC `Hostile` ataca o jogador quando este entra ou se mantém no _tile_ onde
o NPC está presente. A quantidade de HP que o jogador perde é igual a um valor
[aleatório](#procedural) entre 0 e o `AttackPower` do NPC.

O jogador pode atacar qualquer NPC presente no mesmo _tile_ selecionando a
opção `F` e especificando qual o NPC a atacar. A quantidade de HP que o jogador
retira ao NPC é igual a um valor [aleatório](#procedural) entre 0 e o
`AttackPower` da arma equipada. O jogador não pode atacar NPCs senão tiver uma
arma equipada.

Quando é realizado um ataque pelo jogador, existe uma
[probabilidade](#procedural) igual a `1 - Durability` da arma equipada se
partir. Neste caso a arma é removida das "mãos" do jogador e do jogo.

Caso o jogador não tenha uma arma equipada, pode gastar uma _turn_ a equipar
uma arma que tenha no inventário. O jogador pode ainda gastar uma _turn_ a
consumir comida (caso a tenha) se considerar que o seu HP está muito baixo. Em
ambos os casos o jogador será atacado se estiver no mesmo _tile_ que um NPC
hostil.

Caso o jogador vença o NPC (ou seja, caso o HP do NPC diminua até zero), o NPC
desaparece do jogo, deixando para trás zero ou mais itens
[aleatórios](#procedural), que o jogador pode ou não apanhar. O número e
qualidade dos itens deixados para trás pelo NPC **não** varia com a dificuldade
do nível.

Se o NPC vencer o jogador (ou seja, caso o HP do jogador chegue a zero), o jogo
termina.

#### Armadilhas

As armadilhas têm as seguintes características:

*   `MaxDamage` - Valor máximo de HP que jogador pode perder se cair na
    armadilha. É um valor entre 0 e 100.
*   `FallenInto` - Indica se o jogador já caiu na armadilha ou não.

Podem existir diferentes tipos de armadilha no jogo, cada uma com um valor
específico para `MaxDamage`. É possível inclusive existir mais do que uma
armadilha por _tile_.

Quando o jogador entra pela primeira vez num _tile_ com uma ou mais armadilhas,
cada armadilha provoca uma perda [aleatória](#procedural) de HP ao jogador
entre 0 e `MaxDamage`, e o respetivo estado `FallenInto` passa a `true`. Se o
jogador voltar a entrar ou se passar mais _turns_ nesse _tile_, as armadilhas
já não causam estragos.

#### Fim do jogo

O jogo pode terminar de duas formas:

1.  Quando o HP do jogador chega a zero devido a cansaço (pois o jogador perde
    1 HP por _turn_), devido a combate ou devido a armadilhas.
2.  Quando o jogador seleciona a opção `Q`.

Em qualquer dos casos, verifica-se se o nível atingido está entre os 10
melhores, e em caso afirmativo, solicita-se ao jogador o seu nome para o mesmo
figurar na tabela de _high scores_.

#### Níveis e dificuldade

À medida que o jogo avança, os níveis vão ficando mais difíceis. Mais
concretamente, à medida que o jogo avança:

*   Relativamente aos NPCs:
    *   Devem tendencialmente existir em maior número.
    *   A proporção _Hostiles_/_Neutrals_ deve ir aumentando.
    *   O `HP` e `AttackPower` devem ser cada vez maiores (mas nunca
        ultrapassando o máximo, 100).
*   Devem existir tendencialmente mais armadilhas.
*   Devem existir tendencialmente menos itens (comida e armas) disponíveis para
    o jogador apanhar.

Na secção [Geração procedimental e aleatoriedade](#procedural) são apresentadas
algumas sugestões de como gerar infinitamente níveis de dificuldade cada vez
maior.

<a name="procedural"></a>

### Geração procedimental e aleatoriedade

A [geração procedimental][GP] é uma peça fundamental na história dos Videojogos,
tanto antigos como atuais. A [geração procedimental][GP] consiste na criação
algorítmica e automática de dados, por oposição à criação manual dos mesmos. É
usada nos Videojogos para criar grandes quantidades de conteúdo, promovendo a
imprevisibilidade e a rejogabilidade dos jogos.

A biblioteca `stdlib.h` contém a função [rand()][] para geração de números
aleatórios. Para usarmos esta função é primeiro necessário inicializar a
semente do gerador com a função [srand()][rand()]. Uma forma comum de o fazer é
usar a hora atual em segundos, que pode ser obtida com a função [time()][] da
biblioteca `time.h`, como indicado [neste exemplo][randex].

Como se pode observar, a função [rand()][] retorna um inteiro entre 0 e
`RAND_MAX`. No entanto é por vezes útil obtermos inteiros entre 0 e determinado
valor. Para o fazer basta obter o resto da divisão inteira entre o valor gerado
e o valor máximo a obter mais um:

```c
/* Obter um numero aleatorio entre 0 e max */
int max = 100;
int a = rand() % (max + 1);
```

Também é tipicamente útil obtermos _doubles_ entre 0 e 1. Uma forma simples de
o fazer é criarmos uma função para o efeito. Por exemplo:

```c
/**
 * @brief
 * Returns random double between 0 and 1.
 *
 * @return Random double between 0 and 1.
 * */
double rand01() {
    return (double) rand() / (double) RAND_MAX;
}
```

Em alternativa podemos usar o gerador de números aleatórios fornecido na
biblioteca [Glib][GlibRand], como indicado [neste exemplo][glibrandex]. Esta
biblioteca contém inclusive funções que devolvem _doubles_ entre 0 e 1, bem
como inteiros e _doubles_ dentro de intervalos especificados pelo utilizador.
No entanto, os exemplos que se seguem usam o [rand()][] da biblioteca nativa
`stdlib.h`.

Usando a função `rand01()` é possível "atirar a moeda ao ar", por exemplo:

```c
if (rand01() < 0.25) {
    printf("Cara");
} else {
    printf("Coroa");
}
```

No exemplo anterior definimos a probabilidade de sair "Cara" em 25% (e
consequentemente, de sair "Coroa" em 75%). O mesmo tipo de código pode ser
usado para determinar se a arma do jogador se estragou durante um ataque:

```c
if (rand01() < 1 - weapon->durability)
{
    /* Arma estragou-se, fazer qualquer coisa sobre isso */
}
```

O HP a ser subtraído devido a ataque ou armadilhas também pode ser determinado
com a função `rand01()`, multiplicando o valor retornado pelo possível máximo
em questão. Por exemplo:

```c
/* Valor de HP a subtrair num ataque, será no máximo igual a AttackPower */
double damage = rand01() * weapon->attackPower;
```

```c
/* Valor de HP a subtrair devido a armadilha, será no máximo igual a
   MaxDamage */
double damage = rand01() * trap->maxDamage;
```

A função `rand01()` pode ainda ser usada para determinar o `HP` inicial,
o `AttackPower` e o `State` dos NPCs. No caso das duas primeiras
características, basta multiplicar o valor de retorno de `rand01()` pelo
máximo desejado (nunca superior a 100). No caso do `State`, que pode ter apenas
dois valores discretos, usamos um cálculo de probabilidade como fizemos no
exemplo "cara ou coroa".

O seguinte código exemplifica uma possível forma de criar os NPCs para um novo
nível (atenção que o código é meramente exemplificativo):

```c
/* Quantidade inicial de NPCs no nível */
int numberOfNPCs = rand() / (maxNPCsForThisLevel + 1);

/* Criar cada um dos NPCs */
for (int i = 0; i < numberOfNPCs; i++)
{
    /* Determinar HP inicial do NPC */
    double hp = rand01() * maxHPForThisLevel;
    /* Determinar AttackPower do NPC */
    double attackPower = rand01() * maxAPForThisLevel;
    /* Determinar State do NPC */
    enum NPCState state = rand01() < hostileProbabilityForThisLevel
        ? Hostile
        : Neutral;

    /* Determinar posição do NPC */
    int row = rand() % 8; /* Boa ideia se 8 for uma constante tipo levelRows */
    int col = rand() % 8; /* Boa ideia se 8 for uma constante tipo levelCols */

    /* Criar NPC */
    NPC *npc = npc_new(hp, attackPower, state);

    /* Adicionar NPC ao tile escolhido aleatoriamente */
    grid_add(grid, row, col, npc);
}
```

O código anterior assume que as variáveis `maxNPCsForThisLevel`,
`maxHPForThisLevel`, `maxAPForThisLevel` e `hostileProbabilityForThisLevel` já
existem. Estas variáveis devem ir aumentando de valor à medida que o jogador
vai passando os níveis. A forma mais simples consiste em usar uma
[função][funções] para relacionar a variável desejada (_y_) com o nível atual
do jogo (_x_). Algumas funções apropriadas para o efeito são a
[função linear][], a [função linear por troços][], a [função logística][] ou a
[função logarítmica][]. No site disponibilizado através deste [link][funções],
é possível manipular os diferentes parâmetros das várias funções de modo a
visualizar como as mesmas podem relacionar o nível atual (_x_) com o valor de
saída desejado (_y_). É também [disponibilizado um pequena
biblioteca](code) com as várias funções sugeridas.

<a name="visualize"></a>

### Visualização do jogo

A visualização do jogo pode ser feita na consola/terminal com simples recurso a
`printfs` e funções similares, ou com recurso a uma biblioteca gráfica ou de
jogos, como por exemplo:

*   [g2][] - Simples mas limitada.
*   [Ncurses][] - ASCII art (texto), ver referência [\[2\]](#ref2).
*   [Allegro5][] - Bom meio termo, com bons exemplos em C.
*   [Raylib][] - Muito interessante, mas instalação no Ubuntu não é trivial (ver
    instruções no Wiki da [página no GitHub][Raylib-gh]).
*   [SDL2][] - Muito versátil e abrangente, mas poucos exemplos em C.

O uso de uma biblioteca gráfica é opcional e terá uma bonificação na nota
final se corretamente implementado (ver [Fase Extra](#fase-extra)), mesmo que
a interação com o utilizador seja feita através do terminal (isto é, usando
funções tipo `scanf` ou `fgets`).

A visualização do jogo deve ser feita em modo de texto (consola). Os exemplos
que se seguem assumem uma implementação simples em consola/terminal ou com
[Ncurses][].

#### Ecrã principal

O ecrã principal do jogo deve mostrar o seguinte:

*   Mapa do jogo, distinguindo claramente a parte explorada da parte
    inexplorada.
*   Estatísticas do jogador: nível atual, _hit points_ (HP), arma selecionada e
    percentagem de ocupação do inventário.
*   Em cada _tile_ do mapa explorado devem ser diferenciáveis os vários
    elementos presentes (itens, NPCs, etc), até um máximo razoável.
*   Uma legenda, explicando o que é cada elemento no mapa.
*   Uma ou mais mensagens descrevendo o resultado das ações realizadas na
    _turn_ anterior por parte dos jogadores e dos _NPCs_ no _tile_ atual.
*   Descrição do que está no _tile_ atual, bem como nos _tiles_ na respetiva
    vizinhança de [Von Neumann][].
*   Indicação das ações realizáveis.

A [Figura 1](#fig1) mostra uma possível implementação da visualização do jogo
em modo de texto.

<a name="fig1"></a>

```
+++++++++++++++++++++++++++ IC Rogue : Level 009 +++++++++++++++++++++++++++

~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~    Player stats
~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~    ------------
                                                   HP        - 34.4
#.... !.... ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~    Weapon    - Rusty Sword
..... ..... ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~    Inventory - 91.7% full

..... ..... ..... ..... ..... ~~~~~ ~~~~~ ~~~~~
..... ..... ..... ..... ..... ~~~~~ ~~~~~ ~~~~~

..... ..... ..... ?.... ..... >#... ##=!. ~~~~~    Legend
..... ..... ..... ..... ..... ..... ..... ~~~~~    ------
                                                      P - Player
~~~~~ =.... ..... ..... +#... ..... ..... .....   EXIT! - Exit
~~~~~ ..... ..... ..... ..... ..... ..... .....       . - Empty
                                                      ~ - Unexplored
~~~~~ ~~~~~ !=+.. ..... ..... ..... P#>.. EXIT!       = - Neutral NPC
~~~~~ ~~~~~ ..... ..... ..... ..... ..... EXIT!       # - Hostile NPC
                                                      + - Food
~~~~~ ~~~~~ ~~~~~ ~~~~~ +!... ..... !M... ~~~~~       > - Weapon
~~~~~ ~~~~~ ~~~~~ ~~~~~ ..... ..... ..... ~~~~~       ! - Trap
                                                      M - Map
~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~
~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~ ~~~~~

Messages
--------
* You moved WEST
* You were attacked by an NPC and lost 5.3 HP

What do I see?
--------------
* NORTH : Empty
* EAST  : Exit
* WEST  : Empty
* SOUTH : Trap (Hell Pit), Map
* HERE  : NPC (Hostile, HP=14.2, AP= 8.5), Weapon (Shiny Sword)

Options
-------
(W) Move NORTH  (A) Move WEST    (S) Move SOUTH (D) Move EAST
(F) Attack NPC  (E) Pick up item (U) Use item   (V) Drop item
(I) Information (Q) Quit game

>
```

**Figura 1** - Possível implementação da visualização do jogo em modo de texto
(ecrã principal).

#### Ecrã de ataque (opção F)

A opção `F` pode ser utilizada quando existem NPCs no mesmo _tile_ do jogador.
Deve ser mostrada uma mensagem de erro quando a opção `F` é selecionada e não
existem NPCs no _tile_ onde o jogador se encontra. Caso existam NPCs no _tile_,
deve ser apresentado um menu semelhante ao indicado na [Figura 2](#fig2).

<a name="fig2"></a>

```
Select NPC to attack
--------------------

0. Go back
1. Hostile, HP=19.7, AP= 3.0
2. Neutral, HP=62.1, AP=22.8
3. Hostile, HP=31.9, AP= 9.3

>
```

**Figura 2** - Possível menu para seleção de NPC a atacar.

#### Ecrã de apanhar/usar/deixar cair item (opções E, U e V)

A opção `E` pode ser utilizada quando existem itens no mesmo _tile_ do jogador.
Deve ser mostrada uma mensagem de erro quando a opção `E` é selecionada e não
existem itens no _tile_ onde o jogador se encontra.

As opções `U` e `V` podem ser utilizadas quando existem itens no inventário do
jogador. Deve ser mostrada uma mensagem de erro quando uma destas opções é
selecionada e não existem itens no inventário do jogador.

Caso existam itens no inventário deve ser apresentado um menu semelhante ao
indicado na [Figura 3](#fig3).

<a name="fig3"></a>

```
Select item to XXXX
-------------------

0. Go back
1. Weapon (Cursed Dagger)
2. Food (Apple)

>
```

**Figura 3** - Possível menu para seleção de item. `XXXX` deve ser substituído
por `pick up`, `use` ou `drop`, dependendo da opção escolhida.

#### Ecrã de informação (opção I)

Este ecrã aparece quando é selecionada a opção `I`, mostrando informação sobre
os diferentes itens e armadilhas existentes no jogo. O jogador deve pressionar
ENTER ou qualquer tecla para voltar ao ecrã principal, que deve ser redesenhado.
O uso desta opção **não** gasta uma _turn_. A [Figura 4](#fig4) mostra um
possível ecrã de informação (os itens e armadilhas apresentadas são meramente
exemplificativos).

<a name="fig4"></a>

```
Food             HPIncrease      Weight
---------------------------------------
Apple                    +4         0.5
Eggs                     +5         0.6
Fish                    +10         1.0
Meat                    +10         1.2
Water                    +2         0.8

Weapon          AttackPower      Weight     Durability
------------------------------------------------------
Shiny Sword            10.0         3.0           0.90
Rusty Sword            10.0         3.0           0.60
Shiny Dagger            5.0         1.0           0.95
Cursed Dagger          12.0         1.0           0.20
Power Axe              18.0         8.0           0.92
Heavy Mace             16.0         7.0           0.96
Chainsaw               40.0        20.0           0.50

Trap              MaxDamage
---------------------------
Hell Pit                -10
Sharp Spikes            -15
Banana Peel              -3
Bear Trap                -8
Bottomless Chasm        -30
```

**Figura 4** - Possível ecrã de informação (os itens e armadilhas apresentadas
são meramente exemplificativos).

#### Ecrã de terminação do jogo (opção Q)

Quando a opção `Q` é selecionada deve ser apresentada uma pergunta de
confirmação do género `Do you really want to quit? (y/n)`.

## Implementação

<a name="orgproj"></a>

### Organização do projeto e estrutura de ficheiros

O projeto deve estar devidamente organizado, fazendo uso de múltiplas funções
espalhadas de forma lógica em vários ficheiros. Os alunos devem criar os seus
próprios tipos com `typedef`, suportados em enumerações e `structs`. Por
exemplo, o tipo `PLAYER` deve ser definido como um apontador para uma
`struct` com os campos que definem um _player_, e todas as funções que operam
sobre esse tipo (por exemplo `player_new` ou `player_remove_hp()`) devem estar
no mesmo par de ficheiros `.c`/`.h`.

A compição do projeto deve ser realizada com o comando [`make`][], pelo que o
projeto deve conter uma _Makefile_ configurada para compilação C99 (_flags_
`-std=c99 -Wall -Wextra -Wpedantic`) em Linux, de preferência Ubuntu.

<a name="fases"></a>

### Fases da implementação

O jogo deve ser implementado incrementalmente em várias fases. Os projetos
precisam de implementar pelo menos a Fase 1 para serem avaliados, mesmo que
tenham implementado a [Fase Extra](#fase-extra). Atenção que a
[geração procedimental/aleatória](#procedural) dos elementos do jogo, bem como
a [visualização](#visualize), são **obrigatórias** em todas as fases de
implementação.

Para fins de avaliação, a fase tida em conta é a anterior à fase mais baixa que
ficou por implementar. Por exemplo, se o grupo implementar tudo até à fase 5
(inclusive), bem como as fases 7 e 9, a fase tida em conta para avaliação é a
fase 5. Ou seja, é vantajoso seguir a ordem sugerida para as fases de
implementação e não "saltar" fases. A única excepção é a
[Fase Extra](#fase-extra), que pode ser implementada e será avaliada de forma
separada das restantes fases.

#### Fase 1

Na fase 1 devem ser implementados os seguintes pontos:

*   Menu principal, com todas as opções a funcionar excepto _High Scores_.
*   Jogo:
    *   Grelha do jogo contém jogador e _Exit_, colocados
        [aleatoriamente](#procedural) na 1ª e 8ª colunas da grelha,
        respetivamente.
    *   Jogador inicia jogo com HP igual a 100.
    *   Jogador controlável com as teclas WASD, quando chega à _Exit_ termina o
        nível atual, começando um novo nível.
    *   Jogador perde 1 HP por cada _turn_.
    *   Jogador morre quando HP chega a zero.

A implementação completa desta fase equivale a 50% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 5).

#### Fase 2

Na fase 2 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Implementação das partes exploradas e inexploradas do mapa. As partes
    inexploradas devem ser claramente distinguíveis das partes exploradas.

A implementação completa desta fase equivale a 60% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 6).

#### Fase 3

Na fase 3 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Implementação dos mapas e da funcionalidade `(E) Pick up item` apenas para
    mapas. Quando apanhados, os mapas revelam o nível na sua totalidade. Os
    mapas não são guardados no inventário, desaparecendo do nível quando
    apanhados.

A implementação completa desta fase equivale a 65% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 6.5).

#### Fase 4

Na fase 4 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Implementação de armadilhas: quando o jogador se move pela primeira vez
    para um _tile_ que contém uma armadilha, perde HP entre 0 e o valor de
    `MaxDamage` da armadilha em questão.
*   Implementação da opção `(I) Information`, que apresenta informação acerca
    dos diferentes tipos de armadilha no jogo.

A implementação completa desta fase equivale a 70% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 7).

#### Fase 5

Na fase 5 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Implementação dos _high scores_ usando ficheiros:
    *   Opção _High Scores_ do menu principal permite visualizar os 10 melhores
        _scores_.
    *   Quando jogador morre ou seleciona a opção `Q`, _score_ é guardado caso
        esteja entre os 10 melhores.

A implementação completa desta fase equivale a 75% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 7.5).

#### Fase 6

Na fase 6 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Jogador tem inventário que permite guardar itens até um peso máximo
    pré-determinado.
*   Implementação das funcionalidades `(E) Pick up item` e `(V) Drop item` para
    comida e armas. Quando este tipo de itens (comida e armas) são apanhados,
    são guardados no inventário do jogador, caso o mesmo ainda suporte o peso.
*   Atualização da opção `(I) Information` de modo a mostrar informação acerca
    dos diferentes itens existentes no jogo.

A implementação completa desta fase equivale a 80% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 8).

#### Fase 7

Na fase 7 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Implementação da funcionalidade `(U) Use item`, nomeadamente:
    *   O jogador pode consumir itens de comida presentes no seu inventário, e
        o seu `HP` deve aumentar de acordo com a comida consumida, até ao
        máximo de 100.
    *   O jogador pode equipar uma das armas que tem no seu inventário. A arma
        equipada continua a contar para o peso total do inventário. A arma
        anteriormente equipada é movida para o inventário.

A implementação completa desta fase equivale a 85% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 8.5).

#### Fase 8

Na fase 8 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Implementação de NPCs com as características pedidas. Os NPCs existem no
    jogo e aparecem na visualização, mas não interferem, não atacam o jogador e
    não podem ser atacados.

A implementação completa desta fase equivale a 90% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 9).

#### Fase 9

Na fase 9 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Combate passivo: o jogador é atacado por NPCs hostis quando se move para
    _tile_ onde os mesmos se encontrem.

A implementação completa desta fase equivale a 95% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 9.5).

#### Fase 10

Na fase 10 devem ser implementados os seguintes pontos (além dos pontos
indicados nas fases anteriores):

*   Combate ativo: implementação da opção `(F) Attack NPC`.

A implementação completa desta fase equivale a 100% de cumprimento do
[objetivo **O1**](#objetivos) (nota máxima 10).

#### Fase extra

A fase extra oferece um bónus extra até 3 valores, sendo avaliada de forma
separada das restantes fases. A sua correta implementação permite compensar
eventuais problemas e/ou limitações noutras partes do código e/ou do projeto. A
fase extra consiste em:

*   Uso de biblioteca gráfica para implementação da visualização do jogo, tal
    como descrito na secção [Visualização do jogo](#visualize).

<a name="objetivos"></a>

## Objetivos e critério de avaliação

Este projeto tem os seguintes objetivos:

*   **O1** - Jogo deve funcionar como especificado (ver [fases](#fases) de
    implementação, obrigatório implementar pelo menos a Fase 1).
*   **O2** - Projeto e código bem organizados, nomeadamente: a) programa deve
    estar organizado em vários ficheiros `.c` e `.h` com uso de _Makefile_ (ver
    secção [Organização do projeto e estrutura de ficheiros](#orgproj)); b)
    código devidamente comentado e indentado; c) inexistência de código
    "morto", que não faz nada, como por exemplo variáveis ou funções nunca
    usadas; d) soluções [simples][KISS] e eficientes; e, e) projeto compila e
    executa sem erros no compilador [GCC][] e/ou [Clang][] com as opções
    `-std=c99 -Wall -Wextra -Wpedantic` no sistema operativo Ubuntu Linux.
*   **O3** - Projeto adequadamente comentado e documentado. Documentação deve
    ser feita e gerada com o [Doxygen][]. A documentação deve ser gerada em
    HTML e deve ser incluída no ZIP do projeto (mas não integrada no
    repositório Git).
*   **O4** - Repositório Git deve refletir boa utilização do mesmo, com
    _commits_ de todos os elementos do grupo e mensagens de _commit_ que sigam
    as melhores práticas para o efeito (como indicado
    [aqui](https://chris.beams.io/posts/git-commit/),
    [aqui](https://gist.github.com/robertpainsi/b632364184e70900af4ab688decf6f53),
    [aqui](https://github.com/erlang/otp/wiki/writing-good-commit-messages) e
    [aqui](https://stackoverflow.com/questions/2290016/git-commit-messages-50-72-formatting)).
*   **O5** - Relatório em formato [Markdown][] (ficheiro `README.md`),
    organizado da seguinte forma:
    *   Título do projeto.
    *   Nome dos autores (primeiro e último) e respetivos números de aluno.
    *   Indicação do repositório público Git utilizado. Esta indicação é
        opcional, pois podem preferir desenvolver o projeto num repositório
        privado.
    *   Informação de quem fez o quê no projeto. Esta informação é
        **obrigatória** e deve refletir os _commits_ feitos no Git.
    *   Descrição da solução:
        *   Fase implementada (1 a 10, e/ou extra).
        *   Arquitetura da solução, com breve explicação de como o programa foi
            organizado e indicação das estruturas de dados usadas (para o
            inventário e para a grelha de jogo, por exemplo), bem como os
            algoritmos implementados (para desenhar o mapa e para geração
            procedimental, por exemplo).
        *   Um fluxograma mostrando o funcionamento geral do jogo.
    *   Conclusões e matéria aprendida.
    *   Referências:
        *   Incluindo trocas de ideias com colegas, código aberto reutilizado e
            bibliotecas de terceiros utilizadas. Devem ser o mais detalhados
            possível.
    *   **Nota:** o relatório deve ser simples e breve, com informação mínima e
        suficiente para que seja possível ter uma boa ideia do que foi feito.
        Atenção aos erros ortográficos, pois serão tidos em conta na nota final.

O projeto tem um peso de 10 valores na nota final da disciplina e será avaliado
de forma qualitativa. Isto significa que todos os objetivos têm de ser
parcialmente ou totalmente cumpridos. Ou seja, se os alunos ignorarem
completamente um dos objetivos, a nota final será zero.

A nota individual de cada aluno é limitada superiormente pela nota do exame.
Os alunos podem solicitar discussão/defesa do projeto caso pretendam
ultrapassar esta limitação. A discussão é considerada uma avaliação oral e pode
aumentar ou baixar a nota atribuída sem qualquer limite se for considerado que
a nota do projeto e exame não reflecte o conhecimento da matéria.

A nota mínima neste projeto para aprovação na componente prática de IC é de
4.5 valores.

## Entrega

O projeto deve ser entregue via Moodle até às 23h de 16 de setembro de 2018.
Deve ser submetido um ficheiro `zip` com os seguintes conteúdos:

*   Solução completa do projeto, contendo adicionalmente e obrigatoriamente:
    *   Pasta escondida `.git` com o repositório Git local do projeto.
    *   Documentação HTML gerada com [Doxygen][] (não deve fazer parte do
        repositório Git).
    *   Ficheiro `README.md` contendo o relatório do projeto em formato
        [Markdown][].
    *   Ficheiro de imagem contendo o fluxograma.

Notas adicionais para entrega:

*   Os grupos podem ter entre 1 a 3 elementos.
*   Todos os ficheiros C e Markdown devem ser gravados em codificação [UTF-8][].

## Sugestões para o desenvolvimento do projeto

1.  Ler e reler o enunciado até deixarem de existir dúvidas sobre o mesmo. Se
    as mesmas persistirem, entrem em contato com o docente para esclarecimentos
    adicionais.
2.  Fazer um plano de desenvolvimento do projeto, com um objetivo concreto das
    fases a implementar.
3.  Dividir bem o trabalho entre os diferentes elementos do grupo.
4.  Organizar as estruturas e funções em ficheiros separados em volta de um
    conceito comum: NPC, grelha, _player_, etc. Por exemplo, no caso da
    grelha podem desenvolver um módulo (e.g. `grid.c` e `grid.h`), onde
    definem o tipo `GRID` para grelhas com vizinhança de [Von Neumann][], e
    todas as funções que operam sobre variáveis desse tipo (e.g. criar grelha,
    destruir grelha, adicionar item a _tile_ da grelha, etc).
5.  As funções devem ser pequenas e com responsabilidades bem definidas. Se uma
    função começar a ficar muito grande, devem considerar dividir a função em
    várias funções.
6.  Existem uma série de ferramentas úteis que podem facilitar bastante o
    desenvolvimento do projeto, como por exemplo:
    *   [cppcheck][] para verificação estática do código fonte. O [cppcheck][]
        (e outras ferramentas similares) fazem uma verificação mais
        aprofundada do código, detetando possíveis problemas como operações
        entre tipos diferentes, ficheiros não devidamente fechados ou acesso
        potencial a zonas inválidas da memória. Para o código deste projeto, o
        [cppcheck][] pode ser invocado na pasta do projeto da seguinte
        forma: `cppcheck --enable=all --language=c --platform=unix64
        --std=c99 *.c`.
    *   [Valgrind][] para verificação dinâmica do programa. Ao contrário do
        [cppcheck][], o [Valgrind][] tenta detetar _bugs_ no programa enquanto
        o mesmo está a ser executado. É especialmente útil para descobrir erros
        de acesso à memória e verificar se toda a memória alocada foi
        devidamente libertada. Caso o executável do projeto se chame `rogue`, o
        [Valgrind][] pode ser usado para verificar o programa da seguinte
        forma: `valgrind --leak-check=full ./rogue`.
    *   [GDB][] para execução passo a passo e _debugging_ do programa. Tal como
        discutido na aula 4, o [GDB][] permite executar programas passo a passo
        (desde que tenham sido compilados com a opção `-g`). Muitas vezes é a
        única forma de se perceber o que o código está realmente a fazer e
        corrigir _bugs_ complicados. Caso o executável do projeto se chame
        `rogue`, o [GDB][] pode ser usado para executar o programa da seguinte
        forma: `gdb ./rogue`.

## Honestidade académica

Nesta disciplina, espera-se que cada aluno siga os mais altos padrões de
honestidade académica. Isto significa que cada ideia que não seja do aluno deve
ser claramente indicada, com devida referência ao respetivo autor. O não
cumprimento desta regra constitui plágio.

O plágio inclui a utilização de ideias, código ou conjuntos de soluções de
outros alunos ou indivíduos, ou quaisquer outras fontes para além dos textos de
apoio à disciplina, sem dar o respetivo crédito a essas fontes. Os alunos são
encorajados a discutir os problemas com outros alunos e devem mencionar essa
discussão quando submetem os projetos. Essa menção **não** influenciará a nota.
Os alunos não deverão, no entanto, copiar códigos, documentação e relatórios de
outros alunos, ou dar os seus próprios códigos, documentação e relatórios a
outros em qualquer circunstância. De facto, não devem sequer deixar códigos,
documentação e relatórios em computadores de uso partilhado.

Nesta disciplina, a desonestidade académica é considerada fraude, com todas as
consequências legais que daí advêm. Qualquer fraude terá como consequência
imediata a anulação dos projetos de todos os alunos envolvidos (incluindo os
que possibilitaram a ocorrência). Qualquer suspeita de desonestidade académica
será relatada aos órgãos superiores da escola para possível instauração de um
processo disciplinar. Este poderá resultar em reprovação à disciplina,
reprovação de ano ou mesmo suspensão temporária ou definitiva da ULHT.

_Texto adaptado da disciplina de [Algoritmos e
Estruturas de Dados][aed] do [Instituto Superior Técnico][ist]_

## Referências

*   <a name="ref1">\[1\]</a> Pereira, A. (2017). C e Algoritmos, 2ª edição.
    Sílabo.
*   <a name="ref2">\[2\]</a> Procedural generation. (2018). Retrived May 25,
    2018 from <https://en.wikipedia.org/wiki/Procedural_generation>.
*   <a name="ref2">\[2\]</a> Reagan, P. (2014). [Game Programming in C with the
    Ncurses
    Library](https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library),
    Viget Labs.
*   <a name="ref3">\[3\]</a> Marshall, D. (1999).
    [Writing Larger Programs](http://users.cs.cf.ac.uk/Dave.Marshall/C/node35.html),
    Cardiff School of Computer Science and Informatics.
*   <a name="ref4">\[4\]</a> Sizer, B. (2013). [Organizing Code Files in C and
    C++](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/organizing-code-files-in-c-and-c-r3173), GameDev.net.
*   <a name="ref5">\[5\]</a> Kieras, D. (2012). [C Header File
    Guidelines](http://umich.edu/~eecs381/handouts/CHeaderFileGuidelines.pdf),
    EECS Department, University of Michigan.
*   <a name="ref6">\[6\]</a> Ekstrand, J. (2013). [Header file best
    practices](http://www.jlekstrand.net/math/teaching/programming-course/unit-2/header-file-best-practice/),
    Math Department, Iowa State University.
*   <a name="ref7">\[7\]</a> Magnes, M. et al. (2012). [What should and what
    shouldn't be in a header
    file?](https://softwareengineering.stackexchange.com/questions/167723/what-should-and-what-shouldnt-be-in-a-header-file),
    Software Engineering, StackExchange.com.
*   <a name="ref8">\[8\]</a> Backus, J. et al. (2009). [Code organization style
    for C?](https://stackoverflow.com/questions/1479574/code-organization-style-for-c),
    StackOverflow.com.
*   <a name="ref9">\[9\]</a> Cronin, K. et al. [Organization of C
    files](https://stackoverflow.com/questions/47919/organization-of-c-files),
    StackOverflow.com.
*   <a name="ref10">\[10\]</a> "horseyguy" et al. [Good way to organize C
    source files?](https://stackoverflow.com/questions/1263480/good-way-to-organize-c-source-files),
    StackOverflow.com.
*   <a name="ref11">\[11\]</a> Allain, A. (2017). [Compiling and
    Linking](https://www.cprogramming.com/compilingandlinking.html),
    CProgramming.com.

## Licenças

Todo o código neste repositório é disponibilizado através da licença [GPLv3][].
O enunciado e restante documentação são disponibilizados através da licença
[CC BY-NC-SA 4.0][].

## Metadados

*   Autor: [Nuno Fachada][]
*   Curso: [Licenciatura em Videojogos][lamv]
*   Instituição: [Universidade Lusófona de Humanidades e Tecnologias][ULHT]

[GPLv3]:https://www.gnu.org/licenses/gpl-3.0.en.html
[CC BY-NC-SA 4.0]:https://creativecommons.org/licenses/by-nc-sa/4.0/
[lamv]:https://www.ulusofona.pt/licenciatura/videojogos
[Nuno Fachada]:https://github.com/fakenmc
[ULHT]:https://www.ulusofona.pt/
[aed]:https://fenix.tecnico.ulisboa.pt/disciplinas/AED-2/2009-2010/2-semestre/honestidade-academica
[ist]:https://tecnico.ulisboa.pt/pt/
[Markdown]:https://guides.github.com/features/mastering-markdown/
[GCC]:https://gcc.gnu.org/
[Clang]:https://clang.llvm.org/
[Doxygen]:http://www.stack.nl/~dimitri/doxygen/
[Ncurses]:https://www.gnu.org/software/ncurses/ncurses.html
[g2]:http://www.ncbr.muni.cz/~martinp/g2/index.html
[Allegro5]:http://liballeg.org/
[SDL2]:https://www.libsdl.org/
[raylib]:http://www.raylib.com/
[raylib-gh]:https://github.com/raysan5/raylib
[GDB]:https://www.gnu.org/software/gdb/
[cppcheck]:http://cppcheck.sourceforge.net/
[Valgrind]:http://valgrind.org/
[`make`]:https://www.gnu.org/software/make/manual/make.html
[KISS]:https://en.wikipedia.org/wiki/KISS_principle
[Von Neumann]:https://en.wikipedia.org/wiki/Von_Neumann_neighborhood
[GP]:https://en.wikipedia.org/wiki/Procedural_generation
[rand()]:http://man7.org/linux/man-pages/man3/rand.3.html
[time()]:http://man7.org/linux/man-pages/man2/time.2.html
[randex]:https://github.com/VideojogosLusofona/ic2017aulas/blob/master/aula05/random.c
[glibrandex]:https://github.com/VideojogosLusofona/ic2017aulas/blob/master/aula05/terceiros.c
[GlibRand]:https://developer.gnome.org/glib/stable/glib-Random-Numbers.html
[função logística]:https://en.wikipedia.org/wiki/Logistic_function
[função linear por troços]:https://en.wikipedia.org/wiki/Piecewise_linear_function
[função logarítmica]:https://en.wikipedia.org/wiki/Logarithm#Logarithmic_function
[função linear]:https://en.wikipedia.org/wiki/Linear_function_(calculus)
[funções]:https://www.desmos.com/calculator/x5nmemnwsu
[UTF-8]:https://en.wikipedia.org/wiki/UTF-8
