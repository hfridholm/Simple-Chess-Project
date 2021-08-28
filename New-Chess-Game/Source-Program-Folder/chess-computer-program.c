
#include "../Header-Program-Folder/chess-computer-program.h"

bool computer_chess_move(Board board, Info* info, Color color)
{
	Move move = {(Point) {-1, -1}, (Point) {-1, -1}};

	if(!find_computer_move(&move, board, *info, color)) return false;

	printf("Found move! (%d-%d) to (%d-%d)\n",
		move.start.height, move.start.width, move.stop.height, move.stop.width);

	Piece piece = board_point_piece(board, move.start);
	Point* king = (color == WHITE) ? &info->wKing : &info->bKing;

	if(piece.type == KING) *king = move.stop;

	printf("Moving piece soon...\n");

	if(!move_chess_piece(board, move, info)) return false;

	return true;
}

bool simulate_enemy_check(Board board, Move move, Info info)
{
	Point start = move.start, stop = move.stop;

	Color sColor = board_point_color(board, start);

	if(sColor == NONE) return false;

	Color color = (sColor == WHITE) ? BLACK : WHITE;
	Point king = (sColor == WHITE) ? info.bKing : info.wKing;

	Board copy = copy_chess_board(board);
	move_board_piece(copy, start, stop);

	if(king_check_situation(copy, king, color))
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

bool find_computer_move(Move* move, Board board, Info info, Color color)
{
	Move* moves = all_possible_moves(board, info, color);
	int amount = moves_array_amount(moves);

	if(amount == 0) { free(moves); return false; }

	shuffle_moves_array(moves, amount);

	BestMove bestMove = create_bestMove_data(board, moves[0], info);

	Move current;

	for(int index = 0; index < amount; index += 1)
	{
		current = moves[index];

		update_best_move(&bestMove, board, current, info);
	}

	printf("Best computable move out of [%d]: (%d-%d) to (%d-%d)\n",
		amount, 
		bestMove.move.start.height, 
		bestMove.move.start.width, 
		bestMove.move.stop.height, 
		bestMove.move.stop.width);

	printf("setsCheck=%d\ncheckMate=%d\ntakeEnemy=%d\ngetsTaken=%d\nExposed=%d\nType=%d\nEnemy=%d\n",
		bestMove.setsCheck, 
		bestMove.checkMate, 
		bestMove.takeEnemy, 
		bestMove.getsTaken, 
		bestMove.exposed, 
		bestMove.type,
		bestMove.enemy);

	*move = bestMove.move; free(moves); return true;
}

void shuffle_moves_array(Move* moves, int amount)
{
	for(int index = 0; index < amount; index += 1)
	{
		int random = create_random_number(0, amount - 1);
		switch_array_moves(moves, index, random);
	}
}

void switch_array_moves(Move* moves, int first, int second)
{
	Move firstMove = moves[first];
	Move secondMove = moves[second];

	moves[first] = secondMove;
	moves[second] = firstMove;
}

BestMove create_bestMove_data(Board board, Move move, Info info)
{
	Point currStart = move.start, currStop = move.stop;

	BestMove bestMove;

	bestMove.move = move;

	// This function is not working:
	bestMove.setsCheck = simulate_enemy_check(board, move, info);

	// This function is not working:
	bestMove.checkMate = simulate_check_mate(board, move, info);

	bestMove.takeEnemy = board_points_enemy(board, currStart, currStop);

	// This function is not working:
	bestMove.getsTaken = piece_move_exposed(board, move, info);

	// This function is not working:
	bestMove.exposed = board_piece_exposed(board, info, currStart);

	bestMove.type = board_point_type(board, move.start);
	bestMove.enemy = board_point_type(board, move.stop);

	return bestMove;
}

void update_best_move(BestMove* bestMove, Board board, Move move, Info info)
{
	BestMove current = create_bestMove_data(board, move, info);

	printf("\nsetsCheck=%d\ncheckMate=%d\ntakeEnemy=%d\ngetsTaken=%d\nExposed=%d\nType=%d\nEnemy=%d\n",
			current.setsCheck, 
			current.checkMate, 
			current.takeEnemy, 
			current.getsTaken, 
			current.exposed, 
			current.type,
			current.enemy);

	if(smart_bot_algorithm1(board, *bestMove, current)) 
	{
		*bestMove = current;

		printf("\n=== NEW BEST MOVE ===\n");

		printf("\nsetsCheck=%d\ncheckMate=%d\ntakeEnemy=%d\ngetsTaken=%d\nExposed=%d\nType=%d\nEnemy=%d\n",
			bestMove->setsCheck, 
			bestMove->checkMate, 
			bestMove->takeEnemy, 
			bestMove->getsTaken, 
			bestMove->exposed, 
			bestMove->type,
			bestMove->enemy);
	}
}

bool piece_move_exposed(Board board, Move move, Info info)
{
	Point start = move.start, stop = move.stop;

	Board copy = copy_chess_board(board);
	move_board_piece(copy, start, stop);

	if(board_piece_exposed(copy, info, stop))
	{ 
		free(copy); return true; 
	}

	free(copy); return false;
}

bool board_piece_exposed(Board board, Info info, Point point)
{
	Point start; Move move;
	for(int height = 0; height < B_HEIGHT; height += 1)
	{
		for(int width = 0; width < B_WIDTH; width += 1)
		{
			start = (Point) {height, width};
			move = (Move) {start, point};

			if(!board_points_enemy(board, point, start)) continue;
			if(piece_move_acceptable(board, move, info)) return true;
		}
	}
	return false;
}

Move* all_possible_moves(Board board, Info info, Color color)
{
	Move* moves = create_moves_array(1024);
	Move* adding;

	for(int height = 0; height < B_HEIGHT; height += 1)
	{
		for(int width = 0; width < B_WIDTH; width += 1)
		{
			Point point = {height, width};
			Color currColor = board_point_color(board, point);
			if(currColor != color) continue;

			adding = create_moves_array(40);

			if(!piece_possible_moves(adding, board, info, point)) continue;

			append_moves_array(moves, adding);

			free(adding);
		}
	}
	return moves;
}

void append_moves_array(Move* moves, Move* adding)
{
	int amount = moves_array_amount(moves);
	int addAmount = moves_array_amount(adding);

	for(int index = 0; index < addAmount; index += 1)
	{
		moves[amount + index] = adding[index];
	}
}

int moves_array_amount(Move* moves)
{
	int amount = 0;
	while(points_inside_board(moves[amount].start, moves[amount].stop))
	{
		amount += 1;
	}
	return amount;
}

Move* create_moves_array(int amount)
{
	Move* moves = malloc(sizeof(Move) * amount);
	Point start, stop;
	for(int index = 0; index < amount; index += 1)
	{
		start = (Point) {-1, -1}, stop = (Point) {-1, -1};
		moves[index] = (Move) {start, stop};
	}
	return moves;
}

bool piece_possible_moves(Move* moves, Board board, Info info, Point start)
{
	Point stop; Move currMove; int amount = 0;
	for(int height = 0; height < B_HEIGHT; height += 1)
	{
		for(int width = 0; width < B_WIDTH; width += 1)
		{
			stop = (Point) {height, width};
			currMove = (Move) {start, stop};

			if(!piece_move_acceptable(board, currMove, info)) continue;
			if(!move_prevent_check(board, currMove, info)) continue;

			moves[amount] = currMove; 
			amount += 1;
		}
	}
	return (amount != 0);
}
