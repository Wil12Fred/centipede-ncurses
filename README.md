# centipede-ncurses

A small terminal shooter in the style of *Centipede*, written in C++ with ncurses for the
Objects and Data Abstraction course (Objetos y Abstracción de Datos, UCSP, 2016).

A snake (`o`) winds down the screen row by row, like a zigzag. You control a cannon (`a`) on
the bottom row and fire bullets (`*`) up at it. A bullet that hits the body splits the snake
into two pieces. The game ends when a bullet hits the head or the snake reaches the cannon.

```
         oooooooo





        *

        *

        *

        *

        *

        a
```
*(a real frame from a 20×20 game, captured with `tmux capture-pane`)*

## Build

Needs a C++ compiler and ncurses (`sudo apt install libncurses-dev`).

```sh
make
```

## Run

The game first reads two numbers from standard input: the board size and the
initial snake length. It then switches to the ncurses screen.

```sh
./snake
20 8
```

| Key | Action |
|---|---|
| ← / → | move the cannon |
| Space | fire |
| Ctrl+C | quit (there is no quit key) |

The game advances one step every 0.8 s. After game over, press any key and it prints
`Game Over`.

`pr` is an unrelated ncurses "what is your name" test program from the same folder.

## Layout

| File | Contents |
|---|---|
| `main.cc` | input loop and ncurses setup |
| `game.h` | `Game`: collision checks and the update tick |
| `serpiente.h` | `serpiente` (one segment run) and `Serpiente` (the list of pieces) |
| `tanke.h` | the cannon |
| `bala.h` | bullets |
| `funciones.h` | index → zigzag board coordinate conversion |
