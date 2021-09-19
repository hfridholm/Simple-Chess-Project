
#ifndef COMMON_FUNCTIONS_PROGRAM_H
#define COMMON_FUNCTIONS_PROGRAM_H

// bool number_inside_bounds(int, int, int);

// bool point_inside_board(Point);

bool chess_team_string(char*, Team);

// bool points_inside_board(Point, Point);

// bool piece_team_exists(Team);

// Piece board_point_piece(Board, Point);

// void append_board_piece(Board, Point, Piece);

// bool move_inside_board(Move);

// void remove_board_piece(Board, Point);

// void move_board_piece(Board, Point, Point);

// int create_random_number(int, int);

// void switch_chess_pieces(Board, Point, Point);

// bool board_point_exists(Board, Point);
//
// bool board_point_clear(Board, Point);

int point_array_amount(Point[]);

Point* create_point_array(int);

int string_letter_index(char[], int, char);

// bool board_piece_equal(Piece, Piece);

// bool board_piece_points(Point*, Board, Piece);
//
// bool board_piece_point(Point*, Board, Piece);

char convert_char_upper(char);

void convert_string_upper(char*, int);

// Team board_point_team(Board, Point);

// Point team_king_point(Info, Team);

// bool board_points_enemy(Board, Point, Point);

// bool board_points_team(Board, Point, Point);

// bool board_point_empty(Board, Point);

// Type board_point_type(Board, Point);

// bool board_points_equal(Point, Point);

bool board_point_string(char*, Point);

bool chess_move_string(char*, Move);

int moves_array_amount(Move[]);

void append_moves_array(Move*, Move[]);

void clear_moves_array(Move*);

void switch_array_moves(Move*, int, int);

Move* create_moves_array(int);


extern const char letters[];
extern const char numbers[];

#endif
