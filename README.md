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

# Projeto de Época Especial de Introdução à Computação 2017/2018

## Descrição do problema

Os alunos devem implementar o jogo [Simplexity][] para dois jogadores.
Deve ser usada a linguagem C e os grupos podem ter entre 1 a 2 elementos.

### O jogo Simplexity

O jogo [Simplexity][] é semelhante ao [4-em-linha][], mas com uma variação:
além de **cor**, as peças têm também **forma**. O [Simplexity][] é jogado em
turnos, e em cada turno o jogador coloca uma peça numa das 7 colunas do
tabuleiro de jogo. A peça cai na vertical até atingir a base da coluna ou uma
peça já colocada na coluna. Um jogador vence quando 4 peças da sua **cor** ou
da sua **forma** são colocadas em linha (na vertical, horizontal ou diagonal).
Se ocorrer uma situação em que existam 4 peças em linha da mesma cor e 4 peças
em linha da mesma forma, a forma sobrepõem-se à cor para efeitos de vitória. O
jogo termina num empate após todas as peças terem sido colocadas em jogo sem
que exista uma situação de vitória.

A [Tabela 1](#tab1) mostra as condições de vitória para cada jogador, bem como
as peças alocadas inicialmente a cada um. Em caso de dúvidas podes consultar as
[regras do Simplexity][] ou entrar em contacto com o docente via Moodle.

<a name="tab1"></a>

**Tabela 1** - Condições de vitória (cor e forma) e peças alocadas inicialmente
a cada jogador.

| Jogador | Vitória de cor | Vitória de forma | Peças iniciais                             |
|---------|----------------|------------------|--------------------------------------------|
| 1       | Branco         | Cilindro         | 11 cubos brancos, 10 cilindros brancos     |
| 2       | Vermelho       | Cubo             | 11 cubos vermelhos, 10 cilindros vermelhos |

### Modo de funcionamento

O jogo começa automaticamente, entrando no seguinte ciclo (_game loop_):

1.  Mostrar tabuleiro (ver secção [Visualização do jogo](#visualize))
2.  Solicitar jogada ao jogador atual (jogador 1 é o primeiro a jogar)
3.  Verificar se existe uma condição de vitória
    *   Em caso afirmativo, terminar o jogo e indicar o vencedor
    *   Em caso negativo:
        *   Se ainda existirem peças para jogar, voltar ao ponto 1, alternando
            o jogador
        *   Caso contrário, terminar jogo com empate

No ponto 2, é solicitado ao jogador: a) a peça a jogar (cubo ou cilindro); e,
b) a coluna onde colocar a peça. Antes de solicitar a jogada, deve ser dada
indicação de quantas peças de cada tipo o jogador ainda tem para jogar. Após o
jogador ter inserido uma jogada, o jogo deve verificar se: a) o jogador ainda
tem peças do tipo indicado; e, b) a coluna do tabuleiro ainda tem espaço para
mais peças (máximo de 7 peças por coluna). Se alguma destas verificações
falhar, o jogo deve solicitar ao jogador a repetição da jogada.

No ponto 3, o tabuleiro deve ser analisado de modo a determinar se existe uma
das condições de vitória descritas na secção anterior.

### Extensões opcionais

Os alunos podem opcionalmente desenvolver um modo _single-player_, onde o
jogador adversário é implementado com inteligência artificial. Para o efeito
podem usar o algoritmo [Minimax][], com cortes [Alfa-Beta][] para acelerar o
cálculo, além de ser uma excelente oportunidade para o uso de recursão.

Esta extensão permite melhorar, até ao máximo de 10 valores, a nota do projeto.
Atenção que esta extensão não é simples e os conceitos envolvidos não fazem
parte da matéria da disciplina. Só devem abordar este problema quando o projeto
base estiver devidamente concluído. Se abordarem o problema devem fazê-lo num
ramo Git separado, para poderem facilmente voltar à versão base caso não tenham
sucesso na implementação da extensão.

<a name="visualize"></a>

### Visualização do jogo

A visualização do jogo deve ser feita com recurso a uma biblioteca gráfica ou
de jogos, como por exemplo:

*   [g2][] - Simples mas limitada.
*   [Ncurses][] - ASCII art (texto), ver referência [\[2\]](#ref2).
*   [Allegro5][] - Bom meio termo, com bons exemplos em C.
*   [Raylib][] - Muito interessante, mas instalação no Ubuntu não é trivial (ver
    instruções no Wiki da [página no GitHub][Raylib-gh]).
*   [SDL2][] - Muito versátil e abrangente, mas poucos exemplos em C.

O uso de uma biblioteca gráfica é obrigatório, embora se aceite que a leitura
de _inputs_ dos jogadores seja feita através do terminal (isto é, usando
funções tipo `scanf` ou `fgets`).

## Implementação

<a name="orgproj"></a>

### Organização do projeto e estrutura de ficheiros

O projeto deve estar devidamente organizado, fazendo uso de múltiplas funções
espalhadas de forma lógica em vários ficheiros. Os alunos devem criar os seus
próprios tipos com `typedef`, suportados em `structs` e enumerações. Por
exemplo, um possível tipo `PLAYER` deve ser definido como um apontador para uma
`struct` contendo os campos que definem um _player_. Adicionalmente, todas as
funções que operam sobre esse tipo (por exemplo `player_new` ou
`player_play()`) devem ser declaradas e definidas no mesmo par de ficheiros
`.c`/`.h`.

A compição do projeto deve ser realizada com o comando [`make`][], pelo que o
projeto deve conter uma _Makefile_ configurada para compilação C99 (_flags_
`-std=c99 -Wall -Wextra -Wpedantic`) em Linux, de preferência Ubuntu.

<a name="objetivos"></a>

## Objetivos e critério de avaliação

Este projeto tem os seguintes objetivos:

*   **O1** - Jogo deve funcionar como especificado.
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
        *   Arquitetura da solução, com breve explicação de como o programa foi
            organizado e indicação das estruturas de dados (para o tabuleiro de
            jogo, por exemplo) e algoritmos (para verificação da situação de
            vitória, por exemplo) utilizados.
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

A nota individual de cada aluno é limitada superiormente pela nota da
componente teórica. Os alunos podem solicitar discussão/defesa do projeto caso
pretendam ultrapassar esta limitação. A discussão é considerada uma avaliação
oral e pode aumentar ou baixar a nota atribuída sem qualquer limite se for
considerado que a nota do projeto e exame não reflecte o conhecimento da
matéria.

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

*   Os grupos podem ter entre 1 a 2 elementos.
*   Todos os ficheiros C e Markdown devem ser gravados em codificação [UTF-8][].

## Sugestões para o desenvolvimento do projeto

1.  Ler e reler o enunciado até deixarem de existir dúvidas sobre o mesmo. Se
    as mesmas persistirem, entrem em contato com o docente para esclarecimentos
    adicionais.
2.  Fazer um plano de desenvolvimento do projeto, com objetivos concretos e
    fases de implementação bem definidas.
3.  Dividir bem o trabalho entre os elementos do grupo.
4.  Organizar as estruturas e funções em ficheiros separados em volta de um
    conceito comum: _piece_, _board_, _player_, etc. Por exemplo, no caso do
    tabuleiro de jogo podem desenvolver um módulo (e.g. `board.c` e `board.h`),
    onde definem o tipo `BOARD`, bem como todas as funções que operam sobre
    variáveis desse tipo (e.g. criar tabuleiro, destruir tabuleiro, colocar
    peça em determinada posição do tabuleiro, etc).
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
        devidamente libertada. Caso o executável do projeto se chame
        `simplexity`, o [Valgrind][] pode ser usado para verificar o programa
        da seguinte forma: `valgrind --leak-check=full ./simplexity`.
    *   [GDB][] para execução passo a passo e _debugging_ do programa. Tal como
        discutido na aula 4, o [GDB][] permite executar programas passo a passo
        (desde que tenham sido compilados com a opção `-g`). Muitas vezes é a
        única forma de se perceber o que o código está realmente a fazer e
        corrigir _bugs_ complicados. Caso o executável do projeto se chame
        `simplexity`, o [GDB][] pode ser usado para executar o programa da
        seguinte forma: `gdb ./simplexity`.

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
*   <a name="ref12">\[12\]</a> Brain Bender Games (2009). Simplexity Game Rules.
    Discovery Bay Games.
*   <a name="ref13">\[13\]</a> Simplexity. (2009) Retrieved from
    <https://boardgamegeek.com/boardgame/55810/simplexity>

## Licenças

O enunciado é disponibilizado através da licença [CC BY-NC-SA 4.0][].

## Metadados

*   Autor: [Nuno Fachada][]
*   Curso: [Licenciatura em Videojogos][lamv]
*   Instituição: [Universidade Lusófona de Humanidades e Tecnologias][ULHT]

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
[regras do Simplexity]:https://john.cs.olemiss.edu/~dwilkins/CSCI531/fall12/Simplexity_rules.pdf
[Simplexity]:https://boardgamegeek.com/boardgame/55810/simplexity
[4-em-linha]:https://en.wikipedia.org/wiki/Connect_Four
[UTF-8]:https://en.wikipedia.org/wiki/UTF-8
[Minimax]:https://en.wikipedia.org/wiki/Minimax
[Alfa-Beta]:https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
