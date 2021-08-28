
#ifndef CHESS_MOVE_ACCEPTABLE_H
#define CHESS_MOVE_ACCEPTABLE_H

#include "../Header-Program-Folder/global-include-header.h"
#include "../Header-Program-Folder/pieces-moves-valid.h"
#include "../Header-Program-Folder/chess-game-functions.h"

bool pawn_move_acceptable(Board, Move, Info);

bool knight_move_acceptable(Board, Move, Info);

bool queen_move_acceptable(Board, Move, Info);

bool king_move_acceptable(Board, Move, Info);

bool check_after_kingSwitch(Board, Move, Info);

bool rook_switch_able(Board, Move, Info);

bool rook_move_acceptable(Board, Move, Info);

bool check_check_situation(Board, Move, Info);

bool bishop_move_acceptable(Board, Move, Info);

bool diagonal_move_acceptable(Board, Move, Info);

bool straight_move_acceptable(Board, Move, Info);

bool simulate_check_move(Board, Point, Point);

bool piece_move_acceptable(Board, Move, Info);

#endif