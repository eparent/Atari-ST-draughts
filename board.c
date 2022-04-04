#include <string.h>
#include <stddef.h>

#include <vdi.h>
#include <aes.h>

#include "board.h"


extern int posx[51];
extern int posy[51];


void board_draw_grid_lines(int handle)
{
	int i, xy[4];

	/* draw horizontal grid lines */

	xy[0] = BOARD_LEFT;
	xy[2] = BOARD_RIGHT;
	i = BOARD_TOP;
	while( i <= BOARD_BOTTOM )
	{
		xy[1] = i;
		xy[3] = i;
		v_pline(handle, 2, xy);
		i += 40;
	}

	/* draw vertical grid lines */

	xy[1] = BOARD_TOP;
	xy[3] = BOARD_BOTTOM;
	i = BOARD_LEFT;
	while( i <= BOARD_RIGHT )
	{
		xy[0] = i;
		xy[2] = i;
		v_pline(handle, 2, xy);
		i += 40;
	}
}


void board_draw_gray_squares(int handle)
{
	int i, x, y, zw, xy[4];

	vsl_udsty(handle, 0);
	vsf_interior(handle, FIS_PATTERN);
	vsf_style(handle, 4);

	x = BOARD_LEFT + 1;
	y = BOARD_TOP + 1;
	zw = -1;

	for( i = 1; i < 101; i++ )
	{
		if( zw == 1 )
		{
			xy[0] = x;
			xy[1] = y;
			xy[2] = x + 38;
			xy[3] = y + 38;
			vr_recfl(handle, xy);
		}
		if (x > 459)
		{
			x = 61;
			zw = -zw;
			y = y + 40;
		}
		x = x + 40;
		zw = -zw;
	}
}


static void board_draw_light_man(int handle, int pos)
{
	int x = posx[pos], y = posy[pos];

	vswr_mode(handle, MD_REPLACE);
	vsf_style(handle, 8);

	vsf_color(handle, 0);
	vsf_interior(handle, FIS_SOLID);
	v_circle(handle, x, y, PIECE_RADIUS);

	vsf_color(handle, 1);
	vsf_interior(handle, FIS_HOLLOW);
	v_circle(handle, x, y, PIECE_RADIUS_INNER_1);
	v_circle(handle, x, y, PIECE_RADIUS_INNER_2);
}


static void board_draw_dark_man(int handle, int pos)
{
	int x = posx[pos], y = posy[pos];

	vswr_mode(handle, MD_REPLACE);
	vsf_style(handle, 8);

	vsf_color(handle, 1);
	vsf_interior(handle, FIS_SOLID);
	v_circle(handle, x, y, PIECE_RADIUS);

	vswr_mode(handle, MD_XOR);
	/*vsf_color(handle, 1);*/
	vsf_interior(handle, FIS_HOLLOW);
	v_circle(handle, x, y, PIECE_RADIUS_INNER_1);
	v_circle(handle, x, y, PIECE_RADIUS_INNER_2);
}


void board_draw_piece(int handle, int pos, int piece)
{
	switch( piece )
	{
		case PIECE_LIGHT_MAN:
			board_draw_light_man(handle, pos);
			break;
		case PIECE_DARK_MAN:
			board_draw_dark_man(handle, pos);
			break;
	}
}
