
#include "../Header-Program-Folder/chess-engine-includer.h"

bool best_possible_move(Move* move, Board board, Info info, int depth, Team team)
{
	if(!piece_team_exists(team)) return false;

	if(depth <= 0) return false;

	time_t startTime = time(NULL);

	Move* moves = all_possible_moves(board, info, team);
	int amount = moves_array_amount(moves);

	if(amount <= 0)
	{
		free(moves);

		return false;
	}

	Move bestMove = moves[0], currMove;
	int bestValue = MIN_VAL, currValue;

	Info dummyInfo; Board boardCopy; Team nextTeam;

	for(int index = 0; index < amount; index += 1)
	{
		dummyInfo = info; dummyInfo.current = team;

		currMove = moves[index];

		boardCopy = copy_chess_board(board);

		if(!move_chess_piece(boardCopy, currMove, &dummyInfo))
		{
			// For some reson, the computer cant move!

			free_chess_board(boardCopy);

			continue;
		}

		nextTeam = (team == WHITE) ? BLACK : WHITE;
		currValue = board_depth_value(boardCopy, dummyInfo, (depth - 1), MIN_VAL, MAX_VAL, team, nextTeam);

		free_chess_board(boardCopy);

		if(currValue > bestValue) { bestMove = currMove; bestValue = currValue; }
	}

	free(moves);

	time_t stopTime = time(NULL);
	time_t time = difftime(stopTime, startTime);

	display_found_move(bestMove, bestValue, time);

	*move = bestMove; return true;
}

int board_depth_value(Board board, Info info, int depth, int alpha, int beta, Team team, Team currTeam)
{
	// Base-case, Should return the value of the board
	if(depth <= 0)
	{
		return team_state_value(board, info, team);
	}

	Info dummyInfo = info; dummyInfo.current = currTeam;

	Move* moves = all_possible_moves(board, dummyInfo, currTeam);
	int amount = moves_array_amount(moves);

	// If the computer cant move, it will return the worst score
	if(amount <= 0)
	{
		free(moves);

		return team_state_value(board, info, team);
	}

	int bestValue = (currTeam == team) ? MIN_VAL : MAX_VAL;

	// This is very slow, and makes the program run MUCH SLOWER (3s vs 73s)
	//sort_pruning_moves(moves, amount, board, info, currTeam);

	Move currMove; int currValue; Board boardCopy; Team nextTeam;

	for(int index = 0; index < amount; index += 1)
	{
		currMove = moves[index];

		dummyInfo = info; dummyInfo.current = currTeam;

		boardCopy = copy_chess_board(board);

		if(!move_chess_piece(boardCopy, currMove, &dummyInfo))
		{
			free_chess_board(boardCopy);

			continue;
		}

		nextTeam = (currTeam == WHITE) ? BLACK : WHITE;
		currValue = board_depth_value(boardCopy, dummyInfo, (depth - 1), alpha, beta, team, nextTeam);

		free_chess_board(boardCopy);

		if(currTeam == team && currValue > bestValue) 	bestValue = currValue;
		if(currTeam != team && currValue < bestValue) 	bestValue = currValue;

		if(currTeam == team && currValue > alpha) 		alpha = currValue;
		if(currTeam != team && currValue < beta) 		beta = currValue;

		if(beta <= alpha) break;
	}

	free(moves);

	return bestValue;
}

/*
This function goes through all pieces in the inputted team.
You may can store every piece position in the info struct
and then go through that array, but I dont know.
*/
Move* all_possible_moves(Board board, Info info, Team team)
{
	Move* moves = create_moves_array(1024);
	Move* adding = NULL;

	Point point; Team currTeam;

	for(int height = 0; height < BOARD_HEIGHT; height += 1)
	{
		for(int width = 0; width < BOARD_WIDTH; width += 1)
		{
			point = (Point) {height, width};
			currTeam = board_point_team(board, point);

			if(currTeam != team) continue;

			// The queen can do the most moves, and she can do 32 moves
			adding = create_moves_array(32);

			if(!piece_possible_moves(adding, board, info, point))
			{
				free(adding);

				continue;
			}

			append_moves_array(moves, adding);

			free(adding);
		}
	}
	return moves;
}