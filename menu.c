#include <string.h>
#include <stddef.h>

#include <vdi.h>
#include <aes.h>

#include "menu.h"

#define MENU_LEFT   0
#define MENU_TOP    0
#define MENU_RIGHT  99
#define MENU_BOTTOM 399

#define MENU_W (MENU_RIGHT - MENU_LEFT + 1)
#define MENU_H (MENU_BOTTOM - MENU_TOP + 1)

#define MENU_CENTER_X (MENU_LEFT + (MENU_W >> 1))
#define MENU_PADDING  10

#define MENU_FONT_W 8
#define MENU_FONT_H 16

#define MENU_ITEM_X   (MENU_LEFT + MENU_PADDING)
#define MENU_ITEM_Y   (MENU_TOP + MENU_PADDING)
#define MENU_ITEM_W   (MENU_W - MENU_PADDING - MENU_PADDING)
#define MENU_ITEM_H   28
#define MENU_ITEM_GAP 16

#define MENU_TEXT_X       (MENU_ITEM_X + 8)
#define MENU_TEXT_W       (MENU_ITEM_W - 16)
#define MENU_TEXT_YOFFSET ((MENU_ITEM_H - MENU_FONT_H) >> 1)


typedef struct {
	int id;
	int top, left, bottom, right;
	char *text;
} MenuItem_t;


static MenuItem_t s_items[] = {
	{IDM_EXIT, 20, 10, 40, 90, "Exit"},
	{IDM_CLEAR, 60, 10, 80, 90, "Clear"},
	{IDM_NEW, 100, 10, 120, 90, "New"},
	{IDM_UNDO, 140, 10, 160, 90, "Undo"},
	{IDM_WINDOW, 180, 10, 200, 90, "Window"},
	{IDM_EDIT, 220, 10, 240, 90, "Edit"},
	{IDM_LEVUP, 260, 10, 280, 90, "Level++"},
	{IDM_LEVEL, 300, 10, 320, 90, "Level #"},
	{IDM_LEVDN, 340, 10, 360, 90, "Level--"},
	{IDM_NONE, 0, 0, 0, 0, NULL}
};

static MenuItem_t *s_level = NULL;
static MenuItem_t *s_edit = NULL;


static void init(void)
{
	MenuItem_t *item = s_items;
	int top = MENU_ITEM_Y;

	while (item->id != IDM_NONE)
	{
		if (item->id == IDM_LEVEL)
			s_level = item;
		else if (item->id == IDM_EDIT)
			s_edit = item;

		item->top = top;
		item->bottom = item->top + MENU_ITEM_H;
		item->left = MENU_ITEM_X;
		item->right = item->left + MENU_ITEM_W;

		top += MENU_ITEM_H + MENU_ITEM_GAP;
		item++;
	}
}


static void update_level_text(int level)
{
	static size_t digitIndex = 0;

	if (digitIndex == 0)
		digitIndex = strlen(s_level->text) - 1;
	s_level->text[digitIndex] = '0' + level;
}


static void erase_item(int handle, MenuItem_t *item)
{
	static int xy[] = {MENU_ITEM_X, 0, (MENU_ITEM_X + MENU_ITEM_W), 0};

	xy[1] = item->top;
	xy[3] = item->bottom;
	
	vswr_mode(handle, MD_REPLACE);
	vsf_interior(handle, FIS_SOLID);
	vsf_color(handle, 1);
	vr_recfl(handle, xy);
}


static void draw_item(int handle, MenuItem_t *item)
{
	static int xy[] = {MENU_ITEM_X, 0, (MENU_ITEM_X + MENU_ITEM_W), 0};

	xy[1] = item->top;
	xy[3] = item->bottom;

	vswr_mode(handle, MD_REPLACE);
	if (item->id == IDM_LEVEL)
	{
		vsf_interior(handle, FIS_SOLID);
		vsf_color(handle, 1);
		vr_recfl(handle, xy);
	}
	else
	{
		vsl_type(handle, SOLID);
		vsl_color(handle, 0);
		vsf_interior(handle, FIS_HOLLOW);
		v_rbox(handle, xy);
	}
	vswr_mode(handle, MD_XOR);
	v_gtext(handle, MENU_CENTER_X, item->top + MENU_TEXT_YOFFSET, (char*)item->text);
}


void menu_show(int handle, int level)
{
	MenuItem_t *item = s_items;
	int i;

	if (s_level == NULL)
		init();
	update_level_text(level);

	graf_mouse(M_OFF, 0L);

	vst_alignment(handle, 1, 5, &i, &i);
	while (item->id != IDM_NONE)
		draw_item(handle, item++);

	graf_mouse(M_ON, 0L);
}


void menu_hide(int handle)
{
	static int xy[] = {MENU_LEFT, MENU_TOP, MENU_RIGHT, MENU_BOTTOM};

	graf_mouse(M_OFF, 0L);

	vswr_mode(handle, MD_REPLACE);
	vsf_interior(handle, FIS_SOLID);
	vsf_color(handle, 1);
	vr_recfl(handle, xy);

	graf_mouse(M_ON, 0L);
}


void menu_update_level(int handle, int level)
{
	graf_mouse(M_OFF, 0L);

	update_level_text(level);
	draw_item(handle, s_level);

	graf_mouse(M_ON, 0L);
}


void menu_set_edit(int handle, int edit)
{
	graf_mouse(M_OFF, 0L);

	erase_item(handle, s_edit);
	vst_effects(handle, edit == 0 ? 0 : 2);
	draw_item(handle, s_edit);
	vst_effects(handle, 0);

	graf_mouse(M_ON, 0L);
}


int menu_button_at(int x, int y)
{
	MenuItem_t *item = s_items;
	while (item->id != IDM_NONE)
	{
		if (item->left <= x && x <= item->right && item->top <= y && y <= item->bottom)
			break;
		item++;
	}
	return item->id;
}


int menu_contains(int x, int y)
{
	return (MENU_LEFT <= x && x <= MENU_RIGHT) && (MENU_TOP <= y && y <= MENU_BOTTOM);
}
