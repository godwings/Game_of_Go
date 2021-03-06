#include "stdafx.h"
#include "Game.h"


Game::Game() { }

Game::Game(const int &_num_of_player, const short &_width_of_board) {
  board = new Board(_width_of_board);
  num_of_player = _num_of_player;
  vector<char> _player_icon;
  players.clear();
  for (int i = 0; i < num_of_player; i++) {
    players.push_back(new Player(i));
    _player_icon.push_back('k' + i);
  }
  printer = new Printer('.', num_of_player, players, _player_icon);\
}

Game::~Game() { }

bool Game::ReadCommand(Player * const _now_player, int &x, int &y) {
  string ope;
  cout << "Type your(" << printer->GetIcon(_now_player) << ") command:" << endl;
  getline(cin, ope);
  const int len = ope.length();
  switch (ope[0]) {
    case 'p':
    case 'P':
      if (len > 2 && isupper(ope[2])) {
        x = ope[2] - 'A';
      } else if (len > 2 && islower(ope[2])) {
        x = ope[2] - 'a';
      } else {
        cls();
        cout << "Undefined command" << endl;
        printer->Print(board);
        return ReadCommand(_now_player, x, y);
      }
      if (len > 3 && !isdigit(ope[3])) {
        cls();
        cout << "Undefined command" << endl;
        printer->Print(board);
        return ReadCommand(_now_player, x, y);
      }
      y = ope[3] & 0xf;
      if (len > 4 && isdigit(ope[4]))
        y = y * 10 + (ope[4] & 0xf);
      y--;
      break;
    default:
      return 0;
  }
  return 1;
}

Player* Game::Play() {
  int x, y;
  int time_index = 0;
  int now_player = -1;
  while (true) {
    cls();
    printer->Print(board);

    now_player++;
    time_index++;
    if (now_player == num_of_player)
      now_player = 0;

    AskForMove:
    if (!ReadCommand(players[now_player], x, y))
      break;
    if (!board->PlayAt(time_index, y, x, players[now_player])) {
      cls();
      cout << "Your movement is illegal." << endl;
      printer->Print(board);
      goto AskForMove;
    }
  }
  return players[now_player];
}