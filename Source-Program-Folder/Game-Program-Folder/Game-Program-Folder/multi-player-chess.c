
#include "../Header-Program-Folder/game-files-includer.h"

int main(int argAmount, char* arguments[])
{
	srand(time(NULL));

	char filename[200]; extract_file_name(filename, arguments, argAmount);

	Board board = create_board_memory(BOARD_HEIGHT, BOARD_WIDTH);
	Team winner = NONE;
	Info info;

	if(!setup_game_variables(board, &info, filename))
	{
		setup_variables_error();

		free_chess_board(board);

		return false;
	}

	if(multi_player_chess(&winner, board, &info))
	{
		display_chess_result(board, winner);
	}
	else chess_game_quitted(board, info);

	free_chess_board(board);

	return false;
}

bool multi_player_chess(Team* winner, Board board, Info* info)
{
	Move move = EMPTY_MOVE;
	char input[20];

	while(game_still_running(winner, board, *info))
	{
		display_game_round(board, *info);

		if(!input_current_move(input)) continue;

		MOVE_UP_BOARD; MOVE_UP_INFO; MOVE_UP_INPUT;

		if(!strcmp(input, "STOP")) return false;

		if(!parse_chess_move(&move, board, *info, input)) continue;

		if(!move_chess_piece(board, move, info)) continue;

		info->lastMove = move;

		info->turns += 1;
		info->current = piece_team_enemy(info->current);
	}
	return true;
}
