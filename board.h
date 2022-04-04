#ifndef _board_h_
#define _board_h_


#define BOARD_LEFT   100
#define BOARD_TOP    0
#define BOARD_RIGHT  500
#define BOARD_BOTTOM 399

#define BOARD_W (BOARD_RIGHT - BOARD_LEFT + 1)
#define BOARD_H (BOARD_BOTTOM - BOARD_TOP + 1)

#define PIECE_LIGHT_MAN  0
#define PIECE_LIGHT_KING 1
#define PIECE_DARK_MAN   2
#define PIECE_DARK_KING  3
#define PIECE_NONE       4

#define PIECE_RADIUS         17
#define PIECE_RADIUS_INNER_1 11
#define PIECE_RADIUS_INNER_2 5


void board_draw_grid_lines(int handle);
void board_draw_gray_squares(int handle);

void board_draw_piece(int handle, int pos, int piece);


#endif /* _board_h_ */
