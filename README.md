# centipede-ncurses

[![CI](https://github.com/Wil12Fred/centipede-ncurses/actions/workflows/ci.yml/badge.svg)](https://github.com/Wil12Fred/centipede-ncurses/actions/workflows/ci.yml) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A small terminal shooter in the style of *Centipede*, written in C++ with ncurses. The
snake moves along a zigzag path that is just a single integer index mapped onto the board,
which keeps collisions and splitting down to integer interval arithmetic.

```

         ooo
         ooo



    *

    *



    a
```
*(a real frame from a 12×12 game, captured with `tmux capture-pane` after two shots)*

## Features

- A snake (`o`) winds down the screen row by row in a zigzag.
- You move a cannon (`a`) along the bottom row and fire bullets (`*`) upwards.
- A bullet that hits the body splits the snake into two independent pieces.
- The game ends when a bullet hits the head, or when the snake reaches the cannon.
- Any board size and initial snake length, read from standard input.

## Requirements

| | Debian / Ubuntu | Arch Linux |
|---|---|---|
| compiler, make | `g++ make` | `gcc make` |
| ncurses | `libncurses-dev` | `ncurses` |
| tests (pseudo-terminal run) | `bsdutils` (provides `script`, installed by default) | `util-linux` |

## Build

```sh
make          # builds ./snake and ./pr
```

## Run

The game first reads two numbers from standard input — the board size and the initial
snake length — and then switches to the ncurses screen.

```sh
./snake
12 6
```

| Key | Action |
|---|---|
| ← / → | move the cannon |
| Space | fire |
| q | quit |

The game advances one step every 0.8 s. After game over, press any key and it prints:

```
Game Over
```

`pr` is an unrelated ncurses "what is your name" test program from the same folder.

## Tests

```sh
make test
```

- `tests/test_game.cc` — unit tests of the game logic in the headers, compiled without
  ncurses (the one drawing call, `mvaddch`, is replaced by a stub that records cells):
  the zigzag mapping `conv` is a bijection whose consecutive indices are neighbours;
  bullets and the cannon collide in exactly the cell where they are drawn, on even and
  odd board sizes; the cannon cannot leave the board; the snake advances one index per
  tick; a hit on the body splits it into the two expected pieces; a hit on the head and
  the snake reaching the cannon both end the game.
- `tests/smoke.sh` — runs the real binary twice: with stdin at EOF the snake must reach
  the cannon and print `Game Over`; through a pseudo-terminal (`script`), pressing `q`
  must end a game that would otherwise last about five minutes.

## Project layout

| File | Contents |
|---|---|
| `main.cc` | input loop and ncurses setup |
| `game.h` | `Game`: collision checks and the update tick |
| `serpiente.h` | `serpiente` (one segment run) and `Serpiente` (the list of pieces) |
| `tanke.h` | the cannon |
| `bala.h` | bullets |
| `funciones.h` | index → zigzag board coordinate conversion |
| `tests/` | unit tests and the headless smoke run |

## Known limitations

- Bullets are never removed: a bullet that splits the snake keeps flying, and bullets
  that leave the top of the board are still updated every tick.
- The board is a variable-length array on the stack (`int Z[z][z]`), so very large sizes
  overflow the stack; the input is not validated.
- No score, no levels, and the snake does not speed up.
- Fixed in this pass (see the tests): bullets and the cannon were drawn in the mirrored
  column of the one used for collisions (so visible hits passed through the snake), the
  collision column was also wrong on odd board sizes, the cannon could leave the board,
  and there was no quit key other than Ctrl+C.

## Background

Written in 2016 for an objects and data abstraction course at university.

## License

MIT — see [LICENSE](LICENSE).
