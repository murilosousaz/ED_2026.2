# ED2026_2_EQ01 — Sistema de Gerenciamento de Inscrições em Eventos Acadêmicos

Trabalho prático de Estrutura de Dados (UECE, 2026.2) — TADs, Árvore Binária de
Busca, Listas e Filas Encadeadas.

## Compilação e execução

```sh
make clean
make
./bin/eventos < testes/t01.in
```

Também é possível salvar a saída para comparação:

```sh
./bin/eventos < testes/t01.in > resultado.out
diff resultado.out testes/t01.out
```

## Estrutura

- `src/` — TADs (atividade, ABB, lista, fila, histórico) e `main.c`
- `testes/` — pares `.in` / `.out` (t01 a t05)
- `bin/` — gerado pelo Makefile (`bin/eventos`)

## Como testar todos os casos

```sh
for i in 01 02 03 04 05; do
  ./bin/eventos < testes/t$i.in > /tmp/out$i
  diff testes/t$i.out /tmp/out$i && echo "t$i OK"
done
```

## Limitações conhecidas

- **Título e nome são uma palavra única** (sem espaços; `_` permitido), conforme
  o contrato da Seção 6.
- **Remoção de atividades da ABB não é implementada** — a especificação a declara
  opcional (funcionalidade extra).
- **ABB não é balanceada**: em sequência de códigos crescentes/decrescentes a
  árvore degenera e busca/inserção passam a O(A) no pior caso (altura = A − 1).
- **Linhas de entrada limitadas a 4095 caracteres**; os comandos do contrato são
  bem menores que isso.
- **Contadores e capacidade em `int`** (limite ~2,1 bilhões), acima do exigido
  (mín. 300 atividades / 5.000 solicitações).
- Consultas `ARVORE` recalculam contagem e altura por percurso (O(A)); não há
  cache desses valores.

## Ajustes antes da entrega

- `manifesto.json` e a tabela de participação do `relatorio.pdf` usam valores
  provisórios (equipe `EQ01`, matrícula de exemplo) — substituir pelos dados reais.
