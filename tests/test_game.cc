// Unit tests for the game logic in the headers. ncurses is not linked: the one
// ncurses call the headers make, mvaddch, is replaced by a stub that records
// what was drawn, so the tests can compare "where it is drawn" with "where the
// collision code thinks it is".
#include <cstdio>
#include <cstdlib>
#include <set>
#include <utility>
#include <vector>

static std::set<std::pair<int, int> > drawn;   // (row, col) of every mvaddch
static int mvaddch(int y, int x, int) { drawn.insert(std::make_pair(y, x)); return 0; }

#include "../funciones.h"
#include "../serpiente.h"
#include "../bala.h"
#include "../tanke.h"
#include "../game.h"

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); ++failures; } } while (0)

// The screen cell (row, col) where the snake draws board index p.
static std::pair<int, int> cell(int p, int z) {
  pii c = conv(p, z);
  return std::make_pair(z - c.first, c.second);
}

// conv walks the board as a zigzag: index 0 is the top-left corner and every
// next index is a horizontal or vertical neighbour of the previous one.
static void test_conv_is_a_zigzag() {
  const int sizes[] = {5, 6, 19, 20};
  for (int s = 0; s < 4; ++s) {
    int z = sizes[s];
    CHECK(conv(0, z).first == z - 1);
    std::set<std::pair<int, int> > seen;
    for (int p = 0; p < z * z; ++p) {
      pii c = conv(p, z);
      CHECK(c.first >= 0 && c.first < z && c.second >= 0 && c.second < z);
      seen.insert(c);
      if (p > 0) {
        pii prev = conv(p - 1, z);
        CHECK(std::abs(c.first - prev.first) + std::abs(c.second - prev.second) == 1);
      }
    }
    CHECK((int)seen.size() == z * z);   // a bijection onto the board
  }
}

// A bullet must collide in the cell it is drawn in; otherwise shots that
// visibly hit the snake pass through it, and shots that miss split it.
static void test_bullet_collides_where_it_is_drawn() {
  const int sizes[] = {19, 20};
  for (int s = 0; s < 2; ++s) {
    int z = sizes[s];
    for (int y = 0; y < z; ++y) {
      bala b(y, 0);
      for (int step = 1; step < z; ++step) {
        drawn.clear();
        b.actualizar(z);
        CHECK(drawn.size() == 1 && *drawn.begin() == cell(b.coor(z), z));
      }
    }
  }
}

// Same for the cannon: the snake reaching it must mean reaching the drawn 'a'.
static void test_cannon_collides_where_it_is_drawn() {
  const int sizes[] = {19, 20};
  for (int s = 0; s < 2; ++s) {
    int z = sizes[s];
    tanke t;
    t.Crear(z, 0);
    for (int i = 0; i < z; ++i) {
      drawn.clear();
      t.actualizar(z, i == 0 ? 0 : -1);   // walk it from the middle to one edge
      CHECK(drawn.size() == 1 && *drawn.begin() == cell(t.coor(z), z));
    }
  }
}

// The cannon stays on the board however long a key is held.
static void test_cannon_stays_on_the_board() {
  int z = 10;
  tanke t;
  t.Crear(z, 0);
  for (int i = 0; i < 3 * z; ++i) t.actualizar(z, -1);
  CHECK(t.Get() == 0);
  for (int i = 0; i < 3 * z; ++i) t.actualizar(z, 1);
  CHECK(t.Get() == z - 1);
}

// The snake moves one index per tick and its head is its last segment.
static void test_snake_advances() {
  int Z[10 * 10] = {0};
  Serpiente S;
  S.agrSerpiente(0, 4, Z);
  CHECK(S.GetCabeza() == 3);
  S.actualizar(10);
  CHECK(S.Get(0) == std::make_pair(1, 4));
  CHECK(S.GetCabeza() == 4);
  CHECK(S.colision(10, 1) == 0 && S.colision(10, 4) == 0);
  CHECK(S.colision(10, 0) == -1 && S.colision(10, 5) == -1);
}

// A bullet on a body segment splits the snake into two pieces around it;
// a bullet on the head ends the game.
static void test_bullet_splits_body_and_kills_head() {
  int z = 20;
  std::vector<int> Z(z * z, 0);
  Game g(z, 8, &Z[0]);              // one piece covering indices 0..7
  g.B.agrBala(0, &Z[0]);
  // put the bullet on index 3 of the board, whatever row/column that is
  pii c = conv(3, z);
  g.B.B[0].x = c.first;
  g.B.B[0].y = c.second;
  CHECK(g.B.coor(0, z) == 3);
  CHECK(!g.colision_Bala());
  CHECK(g.C.size() == 2);
  if (g.C.size() != 2) return;
  CHECK(g.C.Get(0) == std::make_pair(4, 4));   // 4..7 keeps the head
  CHECK(g.C.Get(1) == std::make_pair(0, 3));   // 0..2 is the new tail piece
  CHECK(g.C.colision(z, 3) == -1);             // the hit cell is now empty

  pii h = conv(g.C.GetCabeza(), z);
  g.B.B[0].x = h.first;
  g.B.B[0].y = h.second;
  CHECK(g.colision_Bala());
}

// The game is over when the snake reaches the cannon's cell.
static void test_snake_reaching_cannon_ends_game() {
  int z = 6;
  std::vector<int> Z(z * z, 0);
  Game g(z, 3, &Z[0]);
  CHECK(!g.colision_Tanke());
  int ticks = 0;
  while (!g.colision_Tanke() && ticks < z * z) { g.actualizar(0, false); ++ticks; }
  CHECK(g.colision_Tanke());
  CHECK(g.C.GetCabeza() == g.T.coor(z));
}

int main() {
  test_conv_is_a_zigzag();
  test_bullet_collides_where_it_is_drawn();
  test_cannon_collides_where_it_is_drawn();
  test_cannon_stays_on_the_board();
  test_snake_advances();
  test_bullet_splits_body_and_kills_head();
  test_snake_reaching_cannon_ends_game();
  if (failures) { std::printf("%d check(s) failed\n", failures); return 1; }
  std::printf("all game tests passed\n");
  return 0;
}
