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
+++++++++++++++++++++++++++ LP1 Rogue : Level 009 +++++++++++++++++++++++++++

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
exemplo, o tipo `Player` deve ser definido como um apontador para uma
`struct player`, e todas as funções que operam sobre esse tipo (por exemplo
`player_new` ou `player_remove_hp()`) devem estar no mesmo par de ficheiros
`.c`/`.h`.

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
*   **O2** - Projeto e código bem organizados, nomeadamente: a) estrutura de
    classes bem pensada (ver secção [Organização do projeto e estrutura de
    ficheiros](#orgproj)); b) código devidamente comentado e indentado; c)
    inexistência de código "morto", que não faz nada, como por exemplo
    variáveis ou funções nunca usadas; d) soluções [simples][KISS] e
    eficientes; e, e) projeto compila e executa sem erros e/ou  _warnings_.
*   **O3** - Projeto adequadamente comentado e documentado. Documentação deve
    ser feita e gerada com o [Doxygen][]. A documentação gerada deve estar
    incluída no ZIP do projeto (mas não integrada no repositório Git).
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
      organizado e indicação das estruturas de dados usadas (para o inventário
      e para a grelha de jogo, por exemplo), bem como os algoritmos
      implementados (para desenhar o mapa e para geração procedimental, por
      exemplo).
    * Um diagrama UML mostrando as relações entre as classes e tipos
      desenvolvidos no jogo. Não é necessário indicar os conteúdos das classes
      (variáveis, propriedades, métodos, etc).
    * Um fluxograma mostrando o _game loop_.
  * Conclusões e matéria aprendida.
  * Referências:
    * Incluindo trocas de ideias com colegas, código aberto reutilizado e
      bibliotecas de terceiros utilizadas. Devem ser o mais detalhados
      possível.
  * **Nota:** o relatório deve ser simples e breve, com informação mínima e
    suficiente para que seja possível ter uma boa ideia do que foi feito.
    Atenção aos erros ortográficos, pois serão tidos em conta na nota final.

O projeto tem um peso de 5 valores na nota final da disciplina e será avaliado
de forma qualitativa. Isto significa que todos os objetivos têm de ser
parcialmente ou totalmente cumpridos. Ou seja, se os alunos ignorarem
completamente um dos objetivos, a nota final será zero.

A nota individual de cada aluno será atribuída com base na avaliação do
projeto, na percentagem de trabalho realizada (indicada no relatório e
verificada através dos _commits_) e na discussão do projeto. Se o aluno tiver
realizado uma percentagem equitativa do projeto e se souber explicar o que fez
durante a discussão, então a nota individual deverá ser muito semelhante à
avaliação do projeto.

A nota mínima neste projeto para aprovação na componente prática de LP1 é de
4.5 valores.


* Jogo deve funcionar como especificado.
* Código deve compilar sem erros no compilador [GCC] e/ou [Clang] com as opções
  `-std=c99 -Wall -Wextra -Wpedantic`.
* Código deve estar devidamente comentado e indentado.
* Tamanho da grelha e número de agentes não deve ser fixado no código, mas sim
  especificado no ficheiro de configuração.
* Documentação do projeto deve ser feita com [Doxygen].
* Programa deve estar organizado em vários ficheiros `.c` e `.h` com uso de
  _Makefile_.
* Visualização do jogo deve ser feita com recurso a uma biblioteca gráfica ou
  de jogos (ver secções [Visualização do jogo](#visualize) e
  [Código exemplo](#examplecode)).













### Entrega

O projeto deve ser entregue via Moodle até às 23h de 18 de fevereiro de 2018.
Deve ser submetido um ficheiro `zip` com os seguintes conteúdos:

* Ficheiros `.c` e `.h`, ficheiro `Makefile`, ficheiro `Doxyfile` e ficheiro
  `.gitignore`.
* Pasta escondida `.git` contendo o repositório Git local do projeto.
* Ficheiro `README.md` contendo o relatório do projeto em formato [Markdown]
  organizado da seguinte forma:
  * Título do projeto.
  * Nome dos autores (primeiro e último) e respetivos números de aluno.
  * Descrição da solução:
    * Arquitetura da solução, com breve explicação de como o programa foi
      estruturado.
      * Um fluxograma simples ou gráficos semelhantes à Figura [1](#figura1)
        são bem vindos.
    * Estruturas de dados: grelha de simulação, agentes, outras estruturas
      auxiliares relevantes.
    * Algoritmos: procura de agente mais próximo, cálculo de direção na grelha,
      _shuffling_ (embaralhamento) dos agentes antes de cada _turn_, outros
      algoritmos relevantes.
  * Manual de utilizador:
    * Como compilar: qual o comando ou comandos gerar uma _build_ do jogo.
    * Como jogar: que teclas pressionar e/ou onde clicar para mover agentes
      (modo interativo); tecla e/ou botão para passar para a próxima _turn_
      (modo automático); outras funcionalidades importantes que o utilizador
      possa controlar.
  * Conclusões e matéria aprendida.
  * Referências:
    * Incluindo trocas de ideias com colegas, código aberto reutilizado e
      bibliotecas de terceiros utilizadas. Devem ser o mais detalhados possível.
  * **Nota:** o relatório deve ser simples e breve, com informação mínima e
    suficiente para que seja possível ter uma boa ideia do que foi feito.

**Atenção**: Todos os ficheiros C e Markdown devem ser gravados em codificação
UTF-8<sup>[5](#fn5)</sup>.

<a name="extensoesop"></a>

## Notas adicionais e material didático para desenvolvimento do projeto

Esta secção apresenta algumas notas adicionais, bem como algum material
didático para auxiliar no desenvolvimento do projeto.

### Sugestões para o desenvolvimento do projeto

1. Devem começar o projeto a partir do código disponibilizado na pasta
   [code](code). Se usarem [Git], o primeiro _commit_ do projeto pode conter
   exatamente os ficheiros que compõem este exemplo<sup>[11](#fn11)</sup>.
2. Ler e reler o enunciado até deixarem de existir dúvidas sobre o mesmo. Se as
   mesmas persistirem, entrem em contato com o docente para esclarecimentos
   adicionais.
3. Fazer um plano de desenvolvimento do projeto. As seguintes limitações são
   aceitáveis numa primeira fase: 1) fixar no código (usando constantes) o
   tamanho do mundo de jogo, o número de agentes, e restantes parâmetros de
   configuração do jogo; e, 2) usar as funções no ficheiro
   [showworld_simple.c](code/showworld_simple.c) (ver secção
   [Visualização do jogo](#visualize)) para visualização do mundo de jogo. Numa
   segunda fase, quando os básicos estiverem todos a funcionar, podem então: 1)
   implementar o tratamento das opções de linha de comandos; 2) usar
   alocação/libertação de memória para terem tamanhos variáveis do mundo e
   número de agentes; e, 3) implementar visualização com uma biblioteca gráfica
   externa.
4. Dividir bem o trabalho entre os diferentes elementos do grupo.
5. Organizar as estruturas e funções em ficheiros separados em volta de um
   conceito comum: coordenada, agente, grelha, etc. Por exemplo, no caso das
   coordenadas podem desenvolver um módulo (e.g. `coordenadas.c` e
   `coordenadas.h`), onde definem o tipo `COORD` para coordenadas 2D em grelha
   toroidal com vizinhança de Von Neumman, e todas as funções que operam sobre
   variáveis desse tipo (e.g. deslocamento, comparação de coordenadas,
   distância, direção entre uma coordenada e outra, etc).
6. As funções devem ser pequenas e com responsabilidades bem definidas. Se uma
   função começar a ficar muito grande, devem considerar dividir a função em
   várias funções.
7. Existem uma série de ferramentas úteis que podem facilitar bastante o
   desenvolvimento do projeto, como por exemplo:
    * [Git] para colaboração e controlo de versões do código fonte.
      * Tendo em conta a complexidade do projeto, que requer a experimentação
        de diferentes abordagens e uma colaboração de facto entre todos os
        membros do grupo, o uso de [Git] pode facilitar bastante o
        desenvolvimento do projeto<sup>[7](#fn7),[11](#fn11)</sup>.
    * [cppcheck] para verificação estática do código fonte.
      * O [cppcheck] (e outras ferramentas similares) fazem uma verificação
        mais aprofundada do código, detetando possíveis problemas como
        operações entre tipos diferentes, ficheiros não devidamente fechados ou
        acesso potencial a zonas inválidas da memória. Para o código deste
        projeto, o [cppcheck] pode ser invocado na pasta do projeto da seguinte
        forma: `cppcheck --enable=all --language=c --platform=unix64
        --std=c99 *.c`.
    * [Valgrind] para verificação dinâmica do programa.
      * Ao contrário do [cppcheck], o [Valgrind] tenta detetar _bugs_ no
        programa enquanto o mesmo está a ser executado. É especialmente útil
        para descobrir erros de acesso à memória e verificar se toda a memória
        alocada foi devidamente libertada. Caso o executável do projeto se
        chame `zombies`, o [Valgrind] pode ser usado para verificar o programa
        da seguinte forma: `valgrind --leak-check=full ./zombies`.
    * [GDB] para execução passo a passo e _debugging_ do programa.
      * Tal como discutido na aula 4, o [GDB] permite executar programas passo
        a passo (desde que tenham sido compilados com a opção `-g`). Muitas
        vezes é a única forma de se perceber o que o código está realmente a
        fazer e corrigir _bugs_ complicados. Caso o executável do projeto se
        chame `zombies`, o [GDB] pode ser usado para executar o programa da
        seguinte forma: `gdb ./zombies`.

<a name="examplecode"></a>

### Código exemplo

A pasta [code](code) contém codigo auxiliar para desenhar o mundo do jogo. A
[Figura 1](#figura1) mostra como o código está organizado.

<a name="figura1"></a>

![arquitectura](https://user-images.githubusercontent.com/3018963/34787896-c2f7e734-f630-11e7-8947-2ac796bad33d.png)

**Figura 1** - Organização do código auxiliar para desenhar o mundo do jogo no
ecrã.

O ficheiro [showworld.h](code/showworld.h) declara seis tipos e/ou funções que
devem ser usados para o desenvolvimento da parte de visualização do projeto,
nomeadamente:

* [`AGENT_TYPE`](code/showworld.h#L34) - Enumeração que define os diferentes
  tipos de agente, tal como indicado na [Tabela 3](#tabela3).
* [`SHOWWORLD`](code/showworld.h#L45) - Tipo abstrato que representa o
  _display_ ou visualização do mundo de simulação.
* [`get_agent_info_at`](code/showworld.h#L52) - Tipo de função que retorna
  informação sobre um agente em dado local do mundo de jogo.
* [`showworld_new`](code/showworld.h#L71) - Função para criar um _display_ ou
  visualização do mundo de jogo (objeto do tipo `SHOWWORLD`).
* [`showworld_destroy`](code/showworld.h#L84) - Função para libertar memória
  associada a um _display_ ou visualização do mundo de jogo (objeto do tipo
  `SHOWWORLD`).
* [`showworld_update`](code/showworld.h#L91) - Função que atualiza o _display_
  / visualização do mundo de jogo.

O ficheiro [showworld_simple.c](code/showworld_simple.c) define uma
implementação concreta dos tipos e/ou funções declaradas de forma abstrata na
interface [showworld.h](code/showworld.h), podendo ser utilizado numa primeira
fase de desenvolvimento do projeto. Quando o projeto estiver a funcionar
adequadamente, os alunos devem criar uma implementação concreta da visualização
do jogo que faça uso de uma biblioteca gráfica, tal como indicado
[anteriormente](#visualize). Para o efeito devem substituir o ficheiro
[showworld_simple.c](code/showworld_simple.c) por outra implementação, mas que
continue a obedecer à interface definida em [showworld.h](code/showworld.h).

Por sua vez, o ficheiro [example.c](code/example.c) contém um exemplo de como
usar a interface definida em [showworld.h](code/showworld.h).

As próximas subsecções descrevem em detalhe a implementação
[showworld_simple.c](code/showworld_simple.c), e como os alunos podem criar a
sua própria implementação da visualização.

#### Exemplo de implementação: showworld_simple.c

A implementação de visualização definida no ficheiro
[showworld_simple.c](code/showworld_simple.c) mostra o mundo do jogo no
terminal, indicando se o agente é zombie (`z`) ou humano (`h`), o ID do agente
em hexadecimal (por exemplo, `z0A`), e diferenciando com `Z` ou `H` maiúsculo
caso o agente em questão seja controlado por um jogador (por exemplo, `H19`).
Caso não exista um agente na célula em questão, é impresso um ponto (`.`). Para
um mundo 5x5 com 4 zombies e 1 humano, com um dos zombies controlado por um
jogador, a implementação mostra algo parecido com a [Figura 2](#figura2).

<a name="figura2"></a>

```
.   .  Z02  .   .

.  z00  .   .   .

.   .   .   .   .

.   .  z03  .   .

.  z01  .  h04  .
```

**Figura 2** - Exemplo do mundo de jogo com dimensões 5x5, tal como mostrado
na implementação de visualização definida em
[showworld_simple.c](code/showworld_simple.c).

##### Implementação do objeto SHOWWORLD para visualização do mundo de simulação

O tipo concreto `SHOWWORLD` está definido como uma [estrutura com três
campos](code/showworld_simple.c#L36): `xdim`, `ydim` e `aginfo_func`. Os dois
primeiros campos representam o tamanho horizontal e vertical do mundo de jogo,
respetivamente. O terceito campo é do tipo
[`get_agent_info_at`](code/showworld.h#L52), ou seja, representa um apontador
para uma função que retorna informação sobre um agente em dado local do mundo
de jogo.

Não é preciso nada de mais, uma vez que este tipo de visualização é muito
simples.

##### Implementação da função showworld_new()

A [implementação](code/showworld_simple.c#L50) desta função aloca memória para
um objeto do tipo [`SHOWWORLD`](code/showworld_simple.c#L36), guardando nos
campos dessa estrutura o tamanho do mundo e o apontador para a função que
retorna informação sobre um agente em dado local do mundo de jogo, do tipo
[`get_agent_info_at`](code/showworld.h#L52).

##### Implementação da função showworld_destroy()

Uma vez que apenas foi efetuada uma
[alocação de memória](code/showworld_simple.c#L60) para criação do objeto
`SHOWWORLD`, a função [showworld_destroy()](code/showworld_simple.c#L68)
simplesmente liberta essa memória usando a função `free()`.

##### Implementação da função showworld_update()

Esta função vai mostrar o mundo do jogo tal como aparece na
[Figura 2](#figura2). A função não devolve nada (`void`), mas aceita dois
argumentos. O primeiro, de nome `sw`, é um objeto do tipo `SHOWWORLD` que
contém informação sobre o _display_ / visualização do jogo. O segundo, de nome
`w`, é do tipo `void *` (apontador genérico), e aponta para a estrutura de
dados que contém o mundo do jogo.

Como é possível observar no [código](code/showworld_simple.c#L76), esta
implementação da função `showworld_update()` percorre todas as células da
grelha de simulação, por linha e por coluna, obtém informação sobre o agente em
cada posição (usando o apontador para função guardado num dos campos da
variável `sw`), e imprime no ecrã, de forma formatada, a informação obtida.
Esta função não precisa de saber nada sobre o mundo de simulação, apontado pela
variável `w`, pois este é passado como argumento e interpretado pela função
apontada por `sw->aginfo_func`.

#### Apontador para função do tipo get_agent_info_at

Existe ainda uma questão crucial e não esclarecida. Onde está definida a
estrutura de dados que contém o mundo de simulação, bem como a função que a
sabe interpretar? A reposta é a seguinte: tanto a estrutura de dados, bem como
a função que a interpreta, devem ser desenvolvidas no código do projeto. Uma
vez que o mundo de simulação vai ser definido de forma específica por cada
grupo, faz então sentido que a função que obtém informação sobre um agente em
determinada localização no mundo seja também definida pelo grupo. Esta função
deve obedecer ao tipo `get_agent_info_at`, definido na interface
[showworld.h](code/showworld.h#L52). No caso do código exemplo, a função está
definida no ficheiro [example.c](code/example.c#L207).

##### Como funcionam as funções do tipo get_agent_info_at?

As funções do tipo `get_agent_info_at` devem devolver um `unsigned int`
contendo informação sobre um agente, recebendo como argumentos a variável `w`
(apontador genérico para o mundo do jogo), bem como as coordenadas `(x,y)` da
posição sobre a qual se pretende obter informação. O `unsigned int` com
informação sobre um agente está organizado internamente como indicado na
[Tabela 2](#tabela2).

<a name="tabela2"></a>

**Tabela 2** - Informação sobre um agente tal como devolvida por funções do
tipo `get_agent_info_at`.

| Bits            | _31–19_ | _18–3_       | _2_             | _1–0_          |
|-----------------|---------|--------------|-----------------|----------------|
| **Significado** | Livre   | ID do agente | Agente jogável? | Tipo de agente |


Os dois bits menos significativos, nas posições 0 e 1, representam o tipo de
agente. O bit na posição 2 indica se o agente é controlado por um jogador (`1`)
ou pela IA (`0`). Os bits entre as posições 3 e 18 contêm o ID do agente.
Finalmente, os bits mais significativos (posições 19 a 31) estão livres para
uso do aluno, caso assim o entenda.

Os possíveis tipos de agente (posições 0 e 1) estão definidos numa enumeração
de nome `AGENT_TYPE` no ficheiro [showworld.h](code/showworld.h#L34), tal como
indicado na [Tabela 3](#tabela3). O tipo `Unknown` nunca deve ocorrer. Se tal
acontecer, significa que o jogo tem um _bug_.

<a name="tabela3"></a>

**Tabela 3** - Tipos de agentes definidos na enumeração
[`AGENT_TYPE`](code/showworld.h#L34).

| Tipo      | Significado            | Código (dec.)  | Código (bin.)  |
|-----------|------------------------|----------------|----------------|
| `None`    | Nenhum agente presente | 0              | 00             |
| `Human`   | Agente humano          | 1              | 01             |
| `Zombie`  | Agente zombie          | 2              | 10             |
| `Unknown` | Agente desconhecido    | 3              | 11             |


#### Exemplo de uso

Um exemplo desta abordagem está disponível no ficheiro
[example.c](code/example.c). Este programa cria uma grelha de simulação de
dimensões 20x20, na qual os agentes são colocados aleatoriamente, invocando
depois a função `showworld_update()` para mostrar o mundo aleatoriamente
gerado. A grelha de simulação (mundo do jogo) é definida como um _array_
bidimensional de apontadores para agente, onde cada posição `[i][j]` do
_array_, correspondente a uma coordenada `(x,y)` no mundo do jogo, contém
um apontador para agente. Por sua vez, os agentes são definidos como uma
[estrutura de nome `AGENT` com três campos](code/example.c#L54): tipo de agente
(`AGENT_TYPE`), agente jogável (`unsigned char`, 0 ou 1), e ID do agente
(`unsigned short`). A função [`example_get_ag_info()`](code/example.c#L207),
que obedece ao tipo `get_agent_info_at`, sabe interpretar a variável `w` como
_array_ de apontadores para agente, sabendo também como obter informação sobre
um agente em determinada posição do _array_. Esta função é dada a conhecer ao
código de visualização durante a criação do objeto `SHOWWORLD`, sendo passada
como [último argumento da função `showworld_new()`](code/example.c#L94).

Convém referir que as estruturas de dados usadas neste exemplo poderão não ser
adequadas ou suficientes para o desenvolvimento do projeto.

## Honestidade académica

Nesta disciplina, espera-se que cada aluno siga os mais altos padrões de
honestidade académica. Isto significa que cada ideia que não seja do
aluno deve ser claramente indicada, com devida referência ao respectivo
autor. O não cumprimento desta regra constitui plágio.

O plágio inclui a utilização de ideias, código ou conjuntos de soluções
de outros alunos ou indivíduos, ou quaisquer outras fontes para além
dos textos de apoio à disciplina, sem dar o respectivo crédito a essas
fontes. Os alunos são encorajados a discutir os problemas com outros
alunos e devem mencionar essa discussão quando submetem os projetos.
Essa menção **não** influenciará a nota. Os alunos não deverão, no
entanto, copiar códigos, documentação e relatórios de outros alunos, ou dar os
seus próprios códigos, documentação e relatórios a outros em qualquer
circunstância. De facto, não devem sequer deixar códigos, documentação e
relatórios em computadores de uso partilhado.

Nesta disciplina, a desonestidade académica é considerada fraude, com
todas as consequências legais que daí advêm. Qualquer fraude terá como
consequência imediata a anulação dos projetos de todos os alunos envolvidos
(incluindo os que possibilitaram a ocorrência). Qualquer suspeita de
desonestidade académica será relatada aos órgãos superiores da escola
para possível instauração de um processo disciplinar. Este poderá
resultar em reprovação à disciplina, reprovação de ano ou mesmo
suspensão temporária ou definitiva da ULHT<sup>[12](#fn12)</sup>.

## Notas

<sup><a name="fn2">2</a></sup> Numa grelha 2D, a
[vizinhança de Von Neumman](https://en.wikipedia.org/wiki/Von_Neumann_neighborhood)
é composta pela célula central, pelas células do lado direito e esquerdo e
pelas células em cima e em baixo.

<sup><a name="fn5">5</a></sup> Este pormenor é especialmente importante em
Windows pois regra geral a codificação UTF-8 não é usada por omissão. Em todo o
caso, e dependendo do editor usado, a codificação UTF-8 pode ser selecionada na
janela de "Save as"/"Guardar como", ou então nas preferências do editor.

<sup><a name="fn6">6</a></sup> A qualidade do código e das soluções inclui
vários aspetos, como por exemplo: 1) código bem indentado; 2) evitar código
"morto", que não faz nada, tal como variáveis ou funções nunca usadas; 3) as
soluções desenvolvidas são [simples][KISS] e/ou eficientes; 4) código compila
sem erros e _warnings_; 5) código devidamente organizado e dividido em funções
e ficheiros de forma lógica e bem estruturada; 6) código não acede a zonas não
alocadas da memória, como por exemplo índices fora dos limites de um _array_;
ou, 7) toda a memória alocada com as funções `malloc` e `calloc` é devidamente
libertada com a função `free`.

<sup><a name="fn7">7</a></sup> Neste projeto não é necessário fazer _fork_
deste repositório. Caso usem Git, os alunos podem inicializar um repositório
local vazio ou com os conteúdos da pasta [code](code) e desenvolver o projeto a
partir desse ponto. Dito isto, para um projeto desta dimensão e com grupos de
até 3 alunos, o uso de Git não é apenas recomendado para uma colaboração
eficiente: é absolutamente essencial. Caso usem um ou mais repositórios remotos
para colaboração devem indicar esse facto no relatório.

<sup><a name="fn8">8</a></sup>
[Application Programming Interface](https://en.wikipedia.org/wiki/Application_programming_interface).

<sup><a name="fn9">9</a></sup> Mais especificamente a ferramenta
[GNU Make][`make`], uma vez que existem várias variantes desta [abordagem de
automatização][Make] de _builds_, nomeadamente o programa [NMake] incluido com
o Microsoft Visual Studio. Apesar de ser orientada ao C e C++, a ferramenta
[Make] pode ser usada em projetos desenvolvidos em qualquer linguagem de
programação.

<sup><a name="fn10">10</a></sup> O Doxygen está disponível para _download_
[aqui](https://www.stack.nl/~dimitri/doxygen/download.html) (Linux/Windows/Mac).
Pode também ser instalado em Ubuntu com o comando `sudo apt install doxygen`,
ou através dos gestores de pacotes [Homebrew](https://brew.sh/) (macOS) e
[Chocolatey](https://chocolatey.org/) (Windows).

<sup><a name="fn11">11</a></sup> É também boa ideia criarem um ficheiro
[`.gitignore`](https://stackoverflow.com/questions/27850222/what-is-gitignore-exactly/27850270)
para evitar a inclusão de ficheiros binários no repositório (como por exemplo
ficheiros objeto ou o executável do projeto). O ficheiro `.gitignore` [incluido
neste repositório](https://github.com/VideojogosLusofona/ic2017p2/blob/master/.gitignore)
é um bom ponto de partida.

<sup><a name="fn12">12</a></sup> Texto adaptado da disciplina de [Algoritmos e
Estruturas de Dados][aed] do [Instituto Superior Técnico][ist].

## Referências

* <a name="ref1">\[1\]</a> Pereira, A. (2017). C e Algoritmos, 2ª edição. Sílabo.
* <a name="ref2">\[2\]</a> Reagan, P. (2014). [Game Programming in C with the
Ncurses Library](https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library),
Viget Labs.
* <a name="ref3">\[3\]</a> Marshall, D. (1999).
[Writing Larger Programs](http://users.cs.cf.ac.uk/Dave.Marshall/C/node35.html),
Cardiff School of Computer Science and Informatics.
* <a name="ref4">\[4\]</a> Sizer, B. (2013). [Organizing Code Files in C and C++](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/organizing-code-files-in-c-and-c-r3173), GameDev.net.
* <a name="ref5">\[5\]</a> Kieras, D. (2012). [C Header File Guidelines](http://umich.edu/~eecs381/handouts/CHeaderFileGuidelines.pdf),
EECS Department, University of Michigan.
* <a name="ref6">\[6\]</a> Ekstrand, J. (2013). [Header file best practices](http://www.jlekstrand.net/math/teaching/programming-course/unit-2/header-file-best-practice/),
Math Department, Iowa State University.
* <a name="ref7">\[7\]</a> Magnes, M. et al. (2012). [What should and what shouldn't be in a header file?](https://softwareengineering.stackexchange.com/questions/167723/what-should-and-what-shouldnt-be-in-a-header-file),
Software Engineering, StackExchange.com.
* <a name="ref8">\[8\]</a> Backus, J. et al. (2009). [Code organization style for C?](https://stackoverflow.com/questions/1479574/code-organization-style-for-c),
StackOverflow.com.
* <a name="ref9">\[9\]</a> Cronin, K. et al. [Organization of C files](https://stackoverflow.com/questions/47919/organization-of-c-files),
StackOverflow.com.
* <a name="ref10">\[10\]</a> "horseyguy" et al. [Good way to organize C source files?](https://stackoverflow.com/questions/1263480/good-way-to-organize-c-source-files),
StackOverflow.com.
* <a name="ref11">\[11\]</a> Allain, A. (2017). [Compiling and Linking](https://www.cprogramming.com/compilingandlinking.html),
CProgramming.com.

## Licenças

Todo o código neste repositório é disponibilizado através da licença [GPLv3].
O enunciado e restante documentação são disponibilizados através da licença
[CC BY-NC-SA 4.0].

## Metadados

* Autor: [Nuno Fachada]
* Curso:  [Licenciatura em Aplicações Multimédia e Videojogos][lamv]
* Instituição: [Universidade Lusófona de Humanidades e Tecnologias][ULHT]

[GPLv3]:https://www.gnu.org/licenses/gpl-3.0.en.html
[CC BY-NC-SA 4.0]:https://creativecommons.org/licenses/by-nc-sa/4.0/
[lamv]:https://www.ulusofona.pt/licenciatura/aplicacoes-multimedia-e-videojogos
[Nuno Fachada]:https://github.com/fakenmc
[ULHT]:https://www.ulusofona.pt/
[aed]:https://fenix.tecnico.ulisboa.pt/disciplinas/AED-2/2009-2010/2-semestre/honestidade-academica
[ist]:https://tecnico.ulisboa.pt/pt/
[Markdown]:https://guides.github.com/features/mastering-markdown/
[GCC]:https://gcc.gnu.org/
[Clang]:https://clang.llvm.org/
[Gedit]:https://wiki.gnome.org/Apps/Gedit
[Geany]:https://www.geany.org/
[Code::Blocks]:http://www.codeblocks.org/
[Atom]:https://atom.io/
[XCode]:https://developer.apple.com/xcode/
[Notepad++]:https://notepad-plus-plus.org/
[Remarkable]:https://remarkableapp.github.io/
[Git]:https://git-scm.com/
[repositório]:https://github.com/VideojogosLusofona/ic2017p1
[GitHub]:https://github.com/
[git-tutorial]:https://try.github.io/levels/1/challenges/1
[Moodle]:https://secure.grupolusofona.pt/ulht/moodle/
[GitLab]:https://gitlab.com/
[BitBucket]:https://bitbucket.org/
[NotABug]:https://notabug.org/
[StackEdit]:https://stackedit.io/editor
[(GitHub-Flavored) Markdown Editor]:https://jbt.github.io/markdown-editor/
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
[Make]:https://en.wikipedia.org/wiki/Make_(software)
[NMake]:https://docs.microsoft.com/pt-pt/cpp/build/nmake-reference
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
