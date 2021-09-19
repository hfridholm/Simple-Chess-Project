
#ifndef EXECUTE_BOARD_MOVES_H
#define EXECUTE_BOARD_MOVES_H

void execute_piece_move(Board, Move, Info*);

void execute_pawn_move(Board, Move);

void execute_rook_move(Board, Move, Info*);

void execute_team_castle(Board, Move, Info*);

void update_king_point(Info*, Team, Point);

void turn_off_rook_switch(Info*, Team);

void execute_knight_move(Board, Move);

void execute_bishop_move(Board, Move);

void execute_queen_move(Board, Move);

void execute_king_move(Board, Move, Info*);

void make_pawn_queen(Board, Point);

#endif