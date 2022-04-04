#ifndef _menu_h_
#define _menu_h_


enum {
	IDM_NONE,
	IDM_EXIT,
	IDM_CLEAR,
	IDM_NEW,
	IDM_UNDO,
	IDM_WINDOW,
	IDM_EDIT,
	IDM_LEVUP,
	IDM_LEVEL,
	IDM_LEVDN
};


void menu_show(int handle, int level);
void menu_hide(int handle);
void menu_update_level(int handle, int level);
void menu_set_edit(int handle, int edit);

int menu_contains(int x, int y);
int menu_button_at(int x, int y);


#endif /* _menu_h_ */
