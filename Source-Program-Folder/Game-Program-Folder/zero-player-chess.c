
#include "../Header-Program-Folder/zero-player-chess.h"

int main(int argAmount, char* arguments[])
{
	srand(time(NULL));

	char* filename = extract_file_name(arguments, argAmount);

	Board board; Color winner = NONE; Info info;

	if(!setup_game_variables(&board, &info, filename))
	{
		setup_variables_error();
		free(board); free(filename); return false;
	}

	free(filename); 

	if(zero_player_chess(&winner, board, &info))
	{
		display_chess_result(board, winner);
	}
	else chess_game_quitted(board, info);

	free(board); return false;
}

bool zero_player_chess(Color* winner, Board board, Info* info)
{
	char input[20];
	while(game_still_running(winner, board, *info))
	{	
		display_game_round(board, *info);

		fgets (input, 20, stdin);
		input[strlen(input) - 1] = '\0';
		
		convert_string_upper(input, strlen(input));
		if(!strcmp(input, "STOP")) return false;

		MOVE_UP(12);

		if(!computer_move_handler(board, info))
		{
			*winner = (info->current == WHITE) ? BLACK : WHITE;
			break;
		}

		if(!update_kings_point(board, info)) return false;

		info->turns += 1;
		info->current = (info->current == WHITE) ? BLACK : WHITE;
	}
	return true;
}

bool computer_move_handler(Board board, Info* info)
{
	Move move = {(Point) {-1, -1}, (Point) {-1, -1}};

	if(!find_computer_move(&move, board, *info, info->current)) return false;

	printf("Computer moved [%d-%d] to [%d-%d]\n",
		move.start.height, move.start.width, move.stop.height, move.stop.width);

	if(!move_chess_piece(board, move, info)) return false;

	return true;
}