
#include "../Header-Program-Folder/setup-game-variables.h"

bool setup_game_variables(Board* board, Info* info, char filename[])
{
	if(!create_chess_board(board, filename))
	{
		create_board_error(filename); return false;
	}

	if(!setup_game_info(info, *board))
	{
		setup_info_error(*board); return false;
	}

	return true;
}

bool setup_game_info(Info* info, Board board)
{
	RKSwitch whiteRKS = extract_rks_values(board, WHITE);
	RKSwitch blackRKS = extract_rks_values(board, BLACK);

	Point bKing, wKing;

	if(!board_piece_point(&bKing, board, (Piece) {KING, BLACK}))
		return false;

	if(!board_piece_point(&wKing, board, (Piece) {KING, WHITE}))
		return false;

	Move lastMove = {(Point) {-1, -1}, (Point) {-1, -1}};
	int turns = 0;

	*info = (Info) {WHITE, whiteRKS, blackRKS, wKing, bKing, turns, lastMove};

	return true;
}

RKSwitch extract_rks_values(Board board, Team team)
{
	// Create this function
	return (RKSwitch) {true, true};
}

bool create_chess_board(Board* board, char filename[])
{
	FILE* filePointer = fopen(filename, "r");

	if(filePointer == NULL)
	{
		file_pointer_error(filename);
		fclose(filePointer); return false;
	}

	if(!allocate_file_values(board, filePointer))
	{
		file_values_error(filename);
		fclose(filePointer); return false;
	}

	fclose(filePointer); return true;
}

bool allocate_file_values(Board* board, FILE* filePointer)
{
	(*board) = malloc(sizeof(Piece*) * B_HEIGHT);
	char lineBuffer[1024];

	for(int height = 0; height < B_HEIGHT; height += 1)
	{
		if(fgets(lineBuffer, 1024, filePointer) == NULL) return false;

		(*board)[height] = malloc(sizeof(Piece) * B_WIDTH);

		if(!extract_file_line(*board, lineBuffer, height)) return false;
	}
	return true;
}

bool extract_file_line(Board board, char lineBuffer[], int height)
{
	if(strlen(lineBuffer) < (B_WIDTH * 2)) return false;

	Piece piece;

	for(int index = 0; index < B_WIDTH; index += 1)
	{
		if(!extract_file_value(&piece, lineBuffer, index)) return false;
		append_board_piece(board, (Point) {height, index}, piece);
	}

	return true;
}

bool extract_file_value(Piece* piece, char lineBuffer[], int index)
{
	char typeChar = lineBuffer[(index * 2) + 0];
	char teamChar = lineBuffer[(index * 2) + 1];

	if(!number_inside_bounds(typeChar, '0', '6')) return false;
	if(!number_inside_bounds(teamChar, '0', '2')) return false;

	Type type = (Type) (typeChar - '0');
	Team team = (Team) (teamChar - '0');

	if(type != EMPTY && team == NONE) return false;
	if(team != NONE && type == EMPTY) return false;

	piece->type = type;
	piece->team = team;

	return true;
}
