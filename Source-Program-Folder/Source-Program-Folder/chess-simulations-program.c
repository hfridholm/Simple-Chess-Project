
#include "../Header-Program-Folder/chess-simulations-program.h"

bool check_after_kingSwitch(Board board, Move move, Info info)
{
	Color color = board_point_color(board, move.start);

	Board copy = copy_chess_board(board);
	Info infoDummy;

	execute_rook_switch(copy, move, &infoDummy);

	Point king = color_king_point(info, color);

	if(!king_inside_check(copy, king))
	{ 
		free(copy); return true; 
	}

	free(copy); return false;
}

bool move_prevent_check(Board board, Move move, Info info)
{
	Point start = move.start, stop = move.stop;

	Piece piece = board_point_piece(board, start);
	if(board_point_empty(board, start)) return false;

	Point king = color_king_point(info, piece.color);

	Board copy = copy_chess_board(board);
	move_board_piece(copy, start, stop);

	if(piece.type == KING) king = stop;

	if(!king_inside_check(copy, king))
	{
		free(copy); return true;
	}

	free(copy); return false;
}

bool simulate_enemy_check(Board board, Move move, Info info)
{
	Point start = move.start, stop = move.stop;

	Color sColor = board_point_color(board, start);

	if(sColor == NONE) return false;

	Point king = (sColor == WHITE) ? info.bKing : info.wKing;

	Board copy = copy_chess_board(board);
	move_board_piece(copy, start, stop);

	if(king_inside_check(copy, king))
	{ 
		free(copy); return true; 
	}

	free(copy); return false;
}

bool simulate_check_mate(Board board, Move move, Info info)
{
	Point start = move.start, stop = move.stop;

	Color sColor = board_point_color(board, start);

	if(sColor == NONE) return false;

	Point king = (sColor == WHITE) ? info.bKing : info.wKing;

	Board copy = copy_chess_board(board);
	move_board_piece(copy, start, stop);

	if(check_mate_situation(copy, info, king))
	{ 
		free(copy); return true; 
	}

	free(copy); return false;
}