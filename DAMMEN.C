#include <string.h>
#include <stddef.h>

#include <tos.h>
#include <vdi.h>
#include <aes.h>

#include "board.h"
#include "menu.h"


static void debug_msg(int handle, const char *msg);

static void init_board(int handle);
static void clear_board(int handle);
static void new_game(int handle);
static void do_menu(int handle);
static void where_is_mouse(int handle);
static void draw_piece(int handle, int place, int color);
static void remove_piece(int handle, int place);

static void bestzwart(int handle);
static void controleerwzet(int van, int naar);
static void conztroleerzet(int van, int naar);
static void doewzet(int handle, int van, int naar);
static void doewheenslag(int handle, int wixscore, int warray[25]);
static void doewterugslag(int handle, int wixscore, int warray[25]);
static void doezwheenzet(int handle, int zfrom, int zto);
static void doezwterugslag(int handle, int zixscore, int zwarray[25]);
static void doezzet(int handle, int van, int naar);
static void dowheenslag(int handle, int wixscore, int warray[25]);
static void dozwheenslag(int handle, int zixscore, int zwarray[25]);
static void doewheenzet(int handle, int wfrom, int wto);
static void doewtrugzet(int handle, int wfrom, int wto);
static void doezwheenslag(int handle, int zixscore, int zwarray[25]);
static void doezwtrugzet(int handle, int zfrom, int zto);
static void dozwterugslag(int handle, int zixscore, int zwarray[25]);
static void dowterugslag(int handle, int wixscore, int warray[25]);
static void dozwheenzet(int handle, int zfrom, int zto);
static void dozwtrugzet(int handle, int zfrom, int zto);
static void dowheenzet(int handle, int wfrom, int wto);
static void dowtrugzet(int handle, int wfrom, int wto);
static void edit(int handle);
static void levelomhoog(int handle);
static void levelomlaag(int handle);
static void meesteburenzwart(int handle);
static void scanlawdamslag(int hok);
static void scanlazdamslag(int hok);
static void scanlvwdamslag(int hok);
static void scanlvzdamslag(int hok);
static void scanrawdamslag(int hok);
static void scanrazdamslag(int hok);
static void scanrvwdamslag(int hok);
static void scanrvzdamslag(int hok);
static void scanwslaan(int hok);
static void scanzwslaan(int hok);
static void scwitslag(int handle);
static void sczwartslag(int handle);

/*static void tekenveld(int handle);*/
static void tekenwittedam(int handle, int plaats);
static void tekenzwartedam(int handle, int plaats);
static void voorruit(int handle);
static void wacht(int til);
static void wittedamzet(int handle);
static void witteslag(int handle);
static void zetterug(int handle);
static void zwatteslag(int handle);


int g_exit;

int level = 3, wisselbitje, mz, bitje, eerste, slag, zet, bestaantal, anfang,
	ende, rendom, hulpje, daar, teller, goed, kut, cont, ok,
	score, maxaantburen, maxvan, maxnaar, anzahl, zettenteller,
	illegalezet, maxscore, tel, count, mixscore, diepte, kat, aantal, hidden,
	moizet, moislag, witgezet;

int pxyarray[8], veld[60];

int posx[51] = {
	50,
	160,240,320,400,480,120,200,280,360,440,
	160,240,320,400,480,120,200,280,360,440,
	160,240,320,400,480,120,200,280,360,440,
	160,240,320,400,480,120,200,280,360,440,
	160,240,320,400,480,120,200,280,360,440
};
int posy[51] = {
	50,
	20,20,20,20,20,60,60,60,60,60,
	100,100,100,100,100,140,140,140,140,140,
	180,180,180,180,180,220,220,220,220,220,
	260,260,260,260,260,300,300,300,300,300,
	340,340,340,340,340,380,380,380,380,380
};

int lv[51] = {0,0,0,0,0,0,0,1,2,3,4,6,7,8,9,10,0,11,12,13,14,16,17,18,19,20,0,
	21,22,23,24,26,27,28,29,30,0,31,32,33,34,36,37,38,39,40,0,41,42,43,44};
int rv[51] = {0,0,0,0,0,0,1,2,3,4,5,7,8,9,10,0,11,12,13,14,15,17,18,19,20,0,
	21,22,23,24,25,27,28,29,30,0,31,32,33,34,35,37,38,39,40,0,41,42,43,44,45};
int ra[51] = {0,7,8,9,10,0,11,12,13,14,15,17,18,19,20,0,21,22,23,24,25,27,28,29,
	30,0,31,32,33,34,35,37,38,39,40,0,41,42,43,44,45,47,48,49,50,0,0,0,0,0,0};
int rra[51] = {0,12,13,14,15,0,17,18,19,20,0,22,23,24,25,0,27,28,29,30,0,32,33,
	34,35,0,37,38,39,40,0,42,43,44,45,0,47,48,49,50,0,0,0,0,0,0,0,0,0,0,0};
int la[51] = {0,6,7,8,9,10,0,11,12,13,14,16,17,18,19,20,0,21,22,23,24,26,27,28,
	29,30,0,31,32,33,34,36,37,38,39,40,0,41,42,43,44,46,47,48,49,50,0,0,0,0,0};
int lla[51] = {0,0,11,12,13,14,0,16,17,18,19,0,21,22,23,24,0,26,27,28,29,0,31,
	32,33,34,0,36,37,38,39,0,41,42,43,44,0,46,47,48,49,0,0,0,0,0,0,0,0,0,0};
int llv[51] = {0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,0,6,7,8,9,0,11,12,13,14,0,16,
	17,18,19,0,21,22,23,24,0,26,27,28,29,0,31,32,33,34,0,36,37,38,39};
int rrv[51] = {0,0,0,0,0,0,0,0,0,0,0,2,3,4,5,0,7,8,9,10,0,12,13,14,15,0,17,
	18,19,20,0,22,23,24,25,0,27,28,29,30,0,32,33,34,35,0,37,38,39,40,0};


int array[21],
	zslag[25][25], wslag[25][25],
	bestvan[20], bestnaar[20], bestarray[10][30],
	buf[30], bord[110][60],
	minwit[10], maxwit[10], minzwart[10],
	slagveld[51],
    maxzwart[10], wit[10], zwart[10];

/*
minsubhigh[30]
witheeftgezet plekkie minimaalverlies aantalburen subhigh
int slarray[20][20],bestslarray[20] skore[10] totzwart totwit
eerstarray[30] eerstaantal hiscore eerstvan eerstnaar
int testbitje, diep, pointwit[30], pointzwart[30],oud;
int contrl[12], intin[128], ptsin[128], intout[128], ptsout[128];
int maxarray[21], hulp[20], from[30], to[30], vorige[30], volgende[30];
*/


void hide_mouse(void)
{
	if (!hidden)
	{
		graf_mouse(M_OFF, NULL);
		hidden = 1;
	}
}

 
void show_mouse(void)
{
	if (hidden)
	{
		graf_mouse(M_ON, NULL);
		hidden = 0;
	}
}


static void draw_piece(int handle, int pos, int piece)
{
#ifdef OLD_DRAW_PIECE
	int x, y;
#endif

	/*vsf_interior(handle,2);*/  
 	if (piece == PIECE_LIGHT_KING)
	{
		/* white */
		tekenwittedam(handle, pos);
		return;
	}
 	if (piece == PIECE_DARK_KING)
	{
		/* black */
		tekenzwartedam(handle, pos);
		return;
	}
#ifdef OLD_DRAW_PIECE
	vsf_interior(handle, piece);
	vswr_mode(handle, MD_REPLACE);
	vsf_style(handle, 8);
	x = posx[pos];
	y = posy[pos];
	v_circle(handle, x, y, 17);/*19);*/

	vsf_interior(handle, 0);
	if (piece == PIECE_LIGHT_MAN)
	{
		v_circle(handle, x, y, 13);
		v_circle(handle, x, y, 6);
		veld[pos] = 0;
	}
	if (piece == PIECE_DARK_MAN)
	{
		vsl_type(handle, 1);
		vsf_interior(handle, 0);
		vswr_mode(handle, MD_XOR);
		vsf_style(handle, 8);
		vsm_color(handle, 2);
		veld[pos] = 2;
		v_circle(handle, x, y, 13);
		v_circle(handle, x, y, 6);
	}
	vsf_interior(handle, 2);
	vsl_type(handle, 1);
	vswr_mode(handle, 1);
#else
	board_draw_piece(handle, pos, piece);
	veld[pos] = piece;
#endif
}


/* drawing white */
static void tekenwittedam(int handle, int place)
{
	remove_piece(handle, place);

	vsf_interior(handle, FIS_HOLLOW);
	vsf_style(handle, 8);
	vsl_type(handle, SOLID);

	pxyarray[0] = posx[place] - 15;
	pxyarray[1] = posy[place] - 10;
	pxyarray[2] = posx[place] + 15;
	pxyarray[3] = posy[place] + 10;
	vr_recfl(handle, pxyarray);

	pxyarray[0] = posx[place] - 15;
	pxyarray[1] = posy[place];
	pxyarray[2] = posx[place] + 15;
	pxyarray[3] = posy[place];
	v_pline(handle, 2, pxyarray);

	veld[place] = PIECE_LIGHT_KING;
}


/* drawing black */
static void tekenzwartedam(int handle, int place)
{
	remove_piece(handle, place);

	vsf_interior(handle, FIS_PATTERN);
	vsf_style(handle, 8);
	vsl_type(handle, USERLINE);
	vswr_mode(handle, MD_REPLACE);

	pxyarray[0] = posx[place] - 15;
	pxyarray[1] = posy[place] - 10;
	pxyarray[2] = posx[place] + 15;
	pxyarray[3] = posy[place] + 10;
	vr_recfl(handle, pxyarray);

	vsl_type(handle, SOLID);
	vswr_mode(handle, MD_XOR);

	pxyarray[0] = posx[place] - 15;
	pxyarray[1] = posy[place];
	pxyarray[2] = posx[place] + 15;
	pxyarray[3] = posy[place];
	v_pline(handle, 2, pxyarray);

	veld[place] = PIECE_DARK_KING;
}


static void remove_piece(int handle, int place)
{
	int pxyarray[4];

	vsf_interior(handle, FIS_PATTERN);
	vsf_style(handle, 4);
	vswr_mode(handle, MD_REPLACE);
	/*vsf_color(handle,4);*/

	pxyarray[0] = posx[place] - 20;
	pxyarray[1] = posy[place] - 20;
	pxyarray[2] = posx[place] + 20;
	pxyarray[3] = posy[place] + 20;
	v_bar(handle, pxyarray);

	veld[place] = PIECE_NONE;
}


/* ??? put */
static void doewzet(int handle, int from, int to)
{
	int pstatus = 0, x, y, tel;

	while (pstatus != 1)
		vq_mouse(handle, &pstatus, &x, &y);
	while (pstatus != 0)
		vq_mouse(handle, &pstatus, &x, &y);

	for (tel = 1; tel < 51; tel++)
	{
		if( (posx[tel] - 21) < x && x < (posx[tel] + 21) &&
			(posy[tel] - 21) < y && y < (posy[tel] + 21) )
		{
			from = tel;
		}
	}
	if (veld[from] == PIECE_LIGHT_KING)
	{
		/* white insert */
		wittedamzet(handle);
		return;
	}

	while (pstatus != 1)
		vq_mouse(handle, &pstatus, &x, &y);
	while (pstatus != 0)
		vq_mouse(handle, &pstatus, &x, &y);

	for (tel = 1; tel < 51; tel++)
	{
		if( (posx[tel] - 21) < x && x < (posx[tel] + 21) &&
			(posy[tel] - 21) < y && y < (posy[tel] + 21) )
		{
			to = tel;
		}
	}

	controleerwzet(from, to);
	if (ok == 0)
	{
		illegalezet = 1;
		return;
	}
	hide_mouse();
	remove_piece(handle, from);
	draw_piece(handle, to, PIECE_LIGHT_MAN);
	veld[from] = PIECE_NONE;
	veld[to] = PIECE_LIGHT_MAN;
	mz = zettenteller;
	show_mouse();
}


static void controleerwzet(int from, int to)
{
	ok = 0;
	if (lv[from] == to)
		ok = 1;
	if (rv[from] == to)
		ok = 1;
	if (veld[from] != 0)
		ok = 0;
	if (veld[to] != 4)
		ok = 0;
}


static void wittedamzet(int handle)
{
	int pstatus = 0, x, y, ok;

	/*while( pstatus != 1 )
		vq_mouse(handle, &pstatus, &x, &y);
	while( pstatus != 0 )
		vq_mouse(handle, &pstatus, &x, &y);*/
	teller = 0;
	bitje = 1;
	where_is_mouse(handle);
	buf[teller] = daar;
	hulpje = daar;
	while( pstatus != 1 )
	{
		where_is_mouse(handle);
		if( hulpje != daar && veld[hulpje] == PIECE_LIGHT_KING )
		{
			hide_mouse();
			remove_piece(handle, hulpje);
			veld[buf[0]] = PIECE_LIGHT_KING;
			show_mouse();
		}
		if( veld[daar] == PIECE_NONE )
		{
			hide_mouse();
			bitje = 1;
			draw_piece(handle, daar, PIECE_LIGHT_KING);
			show_mouse();
		}
		hulpje = daar;
		if( veld[daar] == PIECE_NONE && buf[teller] != daar )
		{
			teller++;
			buf[teller] = daar;
			if( teller > 20 )
				teller = 0;
		}
		vq_mouse(handle, &pstatus, &x, &y);
	}
	where_is_mouse(handle);
	teller++;
	buf[teller] = daar;
	goed = 0;
	ok = 1;
	for( kut = 1; kut < teller; kut++ )
	{
		if( buf[kut] == rv[buf[kut - 1]] && ok == 1 )
		{
			ok = 1;
			goto h;
		}
		ok = 0;
h:;
	}
	if( ok == 1 )
		goed = 1;
	ok = 1;
	for( kut = 1; kut < teller; kut++ )
	{
		if( (buf[kut] == lv[buf[kut - 1]]) && (ok == 1) )
		{
			ok = 1;
			goto i;
		}
		ok = 0;
i:;
	}
	if( ok == 1 )
		goed = 1;
	ok = 1;
	for( kut = 1; kut < teller; kut++ )
	{
		if( (buf[kut] == ra[buf[kut - 1]]) && (ok == 1) )
		{
			ok = 1;
			goto j;
		}
		ok = 0;
j:;
	}
	if( ok == 1 )
		goed = 1;
	ok = 1;
	for( kut = 1; kut < teller; kut++ )
	{
		if( (buf[kut] == la[buf[kut - 1]]) && (ok == 1) )
		{
			ok = 1;
			goto k;
		}
		ok = 0;
k:;
	}
	if( ok == 1)
		goed = 1;
	ok = goed;
	if( veld[buf[0]] != PIECE_LIGHT_KING )
		ok = 0;
	/*if( veld[buf[0]] == veld[buf[teller]] )
		ok = 0;
	if( veld[buf[teller]] != PIECE_NONE )
		ok = 0;*/
	if( ok == 0 )
	{
		illegalezet = 1;
		return;
	}
	/*goto klaasje;*/
	hide_mouse();
	remove_piece(handle, buf[0]);
	tekenwittedam(handle, buf[teller]);
	/*for( kut = 0; kut <= teller; kut++ )
		tekenwittedam(handle, buf[kut]);*/
	veld[buf[teller]] = PIECE_LIGHT_KING;
	show_mouse();
	mz = zettenteller;
}


static void doezzet(int handle, int van, int naar)
{
	int pstatus = 0, x, y, tel;

nogeens:
	while (pstatus != 1)
		vq_mouse(handle, &pstatus, &x, &y);

	for (tel = 1; tel < 51; tel++)
	{
		if( (posx[tel] - 21) < x && x < (posx[tel] + 21) &&
			(posy[tel] - 21) < y && y < (posy[tel] + 21) )
		{
			van = tel;
		}
	}

	while (pstatus != 0)
		vq_mouse(handle, &pstatus, &x, &y);

	for (tel = 1; tel < 51; tel++)
	{
		if( (posx[tel] - 21) < x && x < (posx[tel] + 21) &&
			(posy[tel] - 21) < y && y < (posy[tel] + 21) )
		{
			naar = tel;
		}
	}

	conztroleerzet(van, naar);
	if (ok== 0)
		goto nogeens;

	hide_mouse();
	remove_piece(handle, van);
	draw_piece(handle, naar, 2);
	veld[van] = 4;
	veld[naar] = 2;
	show_mouse();
}


static void conztroleerzet(int from, int to)
{
	ok = 0;
	if (la[from] == to)
		ok = 1;
	if (ra[from] == to)
		ok = 1;
	if (veld[from] != 2)
		ok = 0;
	if (veld[to] != 4)
		ok = 0;
}


static void witteslag(int handle)
{
	int pstatus = 0, x, y, ok;

	scwitslag(handle);
	/*klaasje:;*/
	while( pstatus != 1 )
		vq_mouse(handle, &pstatus, &x, &y);
	while( pstatus != 0 )
		vq_mouse(handle, &pstatus, &x, &y);
	teller = 0;
	bitje = 1;
	where_is_mouse(handle);
	buf[teller] = daar;
	hulpje = daar;
	buf[teller + 1] = veld[daar];
	while( pstatus != 1 )
	{
		where_is_mouse(handle);
		if( hulpje != daar && veld[hulpje] == buf[1] )
		{
			hide_mouse();
			remove_piece(handle, hulpje);
			show_mouse();
		}
		bitje = 0;
		if( veld[daar] == PIECE_NONE )
		{
			hide_mouse();
			bitje = 1;
			draw_piece(handle, daar, buf[1]);
			show_mouse();
		}
		hulpje = daar;
		if( (veld[daar] == 2 || veld[daar] == 3) && buf[teller] != daar )
		{
			teller += 2;
			buf[teller] = daar;
			buf[teller + 1] = veld[daar];
			if( teller > 20 )
				teller = 0;
		}
		vq_mouse(handle, &pstatus, &x, &y);
	}
	while( pstatus != 0 )
		vq_mouse(handle, &pstatus, &x, &y);
	where_is_mouse(handle);
	teller += 2;
	buf[teller] = daar;
	buf[teller + 1] = veld[daar];
	if( teller != mixscore + 2 )
	{
		illegalezet = 1;
		hide_mouse();
		draw_piece(handle, buf[0], buf[1]);
		remove_piece(handle, buf[teller]/*,buf[teller+1]*/);
		show_mouse();
		return;
	}
	goed = 0;
	for( kut = 1; kut <= aantal; kut++ )
	{
		ok = 1;
		for( cont = 0; cont <= mixscore + 2; cont++ )
		{
			if( buf[cont] != wslag[kut][cont] )
				ok = 0;
		}
		if( ok == 1 )
			goed = 1;
	}
	if( goed == 0 )
	{
		illegalezet = 1;
		return;
	}
	/*goto klaasje;*/
	hide_mouse();
	dowheenslag(handle, mixscore, buf);
	show_mouse();
	mz = zettenteller;
}


static void zwatteslag(int handle)
{
	int pstatus = 0, x, y, ok;

	sczwartslag(handle);
bolle:;
	while( pstatus != 1 )
		vq_mouse(handle, &pstatus, &x, &y);
	teller = 0;
	where_is_mouse(handle);
	buf[teller] = daar;
	hulpje = daar;
	while( pstatus != 0 )
	{
		where_is_mouse(handle);
		hulpje = daar;
		if( veld[daar] < 2 && buf[teller] != daar )
		{
			teller += 2;
			buf[teller] = daar;
			if( teller > 20 )
				teller = 0;
		}
		vq_mouse(handle, &pstatus, &x, &y);
	}
	where_is_mouse(handle);
	teller += 2;
	buf[teller] = daar;
	if( teller != mixscore + 2)
		goto bolle;
	goed = 0;
	for( kut = 1; kut <= aantal; kut++ )
	{
		ok = 1;
		for( cont = 0; cont <= (mixscore + 2); cont += 2 )
		{
			if( buf[cont] != zslag[kut][cont] )
				ok = 0;
		}
		if( ok == 1 )
			goed = 1;
	}
	if( goed == 0 )
		goto bolle;
	hide_mouse();
	dozwheenslag(handle, mixscore, buf);
	show_mouse();
}


static void where_is_mouse(int handle)
{
	int pstatus, x, y, i;

	vq_mouse(handle, &pstatus, &x, &y);
	for( i = 1; i < 51; i++ ) 
	{
		if( (posx[i] - 21) < x && x < (posx[i] + 21) &&
			posy[i] < (y + 21) && y < (posy[i] + 21) )
		{
			daar = i;
		}
	}
}


static void scanwslaan(int hok)
{int slachtoffer;
 veld[0]=10;
if(((veld[lv[hok]]==2)||(veld[lv[hok]]==3))&&(veld[llv[hok]]==4))  
 {slachtoffer=veld[lv[hok]];
  score=score+2;array[score]=lv[hok];array[score+1]=slachtoffer;
  array[score+2]=llv[hok];veld[lv[hok]]=4;
  if(score==maxscore)
   {aantal++;for (kat=0;kat<=score+2;kat++)wslag[aantal][kat]=array[kat];};
  if(score>maxscore)
   {maxscore=score;aantal=1;
    for(kat=0;kat<=score+2;kat++)wslag[1][kat]=array[kat];};
  scanwslaan(llv[hok]);veld[lv[hok]]=slachtoffer;
  veld[llv[hok]]=4;score=score-2;};  

if(((veld[rv[hok]]==2)||(veld[rv[hok]]==3))&&(veld[rrv[hok]]==4))  
 {slachtoffer=veld[rv[hok]];
  score=score+2;array[score]=rv[hok];array[score+1]=slachtoffer;
  array[score+2]=rrv[hok];veld[rv[hok]]=4;
  if(score==maxscore)
   {aantal++;for (kat=0;kat<=score+2;kat++)wslag[aantal][kat]=array[kat];};
  if(score>maxscore)
   {maxscore=score;aantal=1;
    for(kat=0;kat<=score+2;kat++)wslag[1][kat]=array[kat];};
  scanwslaan(rrv[hok]);veld[rv[hok]]=slachtoffer;
  veld[rrv[hok]]=4;score=score-2;};  

if(((veld[ra[hok]]==2)||(veld[ra[hok]]==3))&&(veld[rra[hok]]==4))  
 {slachtoffer=veld[ra[hok]];
  score=score+2;array[score]=ra[hok];array[score+1]=slachtoffer;
  array[score+2]=rra[hok];veld[ra[hok]]=4;
  if(score==maxscore)
   {aantal++;for (kat=0;kat<=score+2;kat++)wslag[aantal][kat]=array[kat];};
  if(score>maxscore)
   {maxscore=score;aantal=1;
    for(kat=0;kat<=score+2;kat++)wslag[1][kat]=array[kat];};
  scanwslaan(rra[hok]);veld[ra[hok]]=slachtoffer;
  veld[rra[hok]]=4;score=score-2;};  

if(((veld[la[hok]]==2)||(veld[la[hok]]==3))&&(veld[lla[hok]]==4))  
 {slachtoffer=veld[la[hok]];
  score=score+2;array[score]=la[hok];array[score+1]=slachtoffer;
  array[score+2]=lla[hok];veld[la[hok]]=4;
  if(score==maxscore)
   {aantal++;for (kat=0;kat<=score+2;kat++)wslag[aantal][kat]=array[kat];};
  if(score>maxscore)
   {maxscore=score;aantal=1;
    for(kat=0;kat<=score+2;kat++)wslag[1][kat]=array[kat];};
  scanwslaan(lla[hok]);veld[la[hok]]=slachtoffer;
  veld[lla[hok]]=4;score=score-2;};  
 
}

 
static void scanlvwdamslag(int hok)
{int wijs,helpje,bitje,slachtoffer;
 wijs=lv[hok];
 a: ;   if(veld[wijs]==PIECE_NONE){wijs=lv[wijs];goto a;};
 if(((veld[wijs]==PIECE_DARK_MAN)||(veld[wijs]==PIECE_DARK_KING))
   &&(veld[lv[wijs]]==PIECE_NONE)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=lv[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==PIECE_NONE)
    {scanlawdamslag(wijs);
     if(bitje==0){scanlvwdamslag(wijs);bitje=1;};
     scanrvwdamslag(wijs);
     wijs=lv[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==PIECE_NONE)
       { aantal++;for(kat=0;kat<=score+1;kat++)
                 {
                  wslag[aantal][kat]=array[kat];
                  wslag[aantal][score+2]=wijs;
                 };
         wijs=lv[wijs];
       };
    };score=score-2;slagveld[ra[helpje]]=0;
  }; 
}

 
static void scanrvwdamslag(int hok)
{int wijs,helpje,bitje,slachtoffer;
 wijs=rv[hok];
 a: ;   if(veld[wijs]==PIECE_NONE){wijs=rv[wijs];goto a;};
 if(((veld[wijs]==PIECE_DARK_MAN)||(veld[wijs]==PIECE_DARK_KING))
   &&(veld[rv[wijs]]==PIECE_NONE)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=rv[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==PIECE_NONE)
    {scanlvwdamslag(wijs);
     if(bitje==0){scanrvwdamslag(wijs);bitje=1;};
     scanrawdamslag(wijs);
     wijs=rv[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==PIECE_NONE)
       { aantal++;for(kat=0;kat<=score+1;kat++)
                 {
                  wslag[aantal][kat]=array[kat];
                  wslag[aantal][score+2]=wijs;
                 };
         wijs=rv[wijs];
       };
    };score=score-2;slagveld[la[helpje]]=0;
  }; 
}

 
static void scanlawdamslag(int hok)
{int wijs,helpje,bitje,slachtoffer;
 wijs=la[hok];
 a: ;   if(veld[wijs]==PIECE_NONE){wijs=la[wijs];goto a;};
 if(((veld[wijs]==PIECE_DARK_MAN)||(veld[wijs]==PIECE_DARK_KING))
   &&(veld[la[wijs]]==PIECE_NONE)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=la[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==PIECE_NONE)
    {scanlvwdamslag(wijs);
     if(bitje==0){scanlawdamslag(wijs);bitje=1;};
     scanrawdamslag(wijs);
     wijs=la[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==PIECE_NONE)
       { aantal++;for(kat=0;kat<=score+1;kat++)
                 {
                  wslag[aantal][kat]=array[kat];
                  wslag[aantal][score+2]=wijs;
                 };
         wijs=la[wijs];
       };
    };score=score-2;slagveld[rv[helpje]]=0;
  }; 
}

 
static void scanrawdamslag(int hok)
{int wijs,helpje,bitje,slachtoffer;
 wijs=ra[hok];
 a: ;   if(veld[wijs]==PIECE_NONE){wijs=ra[wijs];goto a;};
 if(((veld[wijs]==PIECE_DARK_MAN)||(veld[wijs]==PIECE_DARK_KING))
   &&(veld[ra[wijs]]==PIECE_NONE)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=ra[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==PIECE_NONE)
    {scanlawdamslag(wijs);
     if(bitje==0){scanrawdamslag(wijs);bitje=1;};
     scanrvwdamslag(wijs);
     wijs=ra[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==PIECE_NONE)
       { aantal++;for(kat=0;kat<=score+1;kat++)
                 {
                  wslag[aantal][kat]=array[kat];
                  wslag[aantal][score+2]=wijs;
                 };
         wijs=ra[wijs];
       };
    };score=score-2;slagveld[lv[helpje]]=0;
  }; 
}



static void scwitslag(int handle)
{
score=0;maxscore=0;mixscore=0;aantal=0;
for (tel=1;tel<51;tel++){if (veld[tel]==0){veld[tel]=4;array[0]=tel;
                          array[1]=0;scanwslaan(tel);veld[tel]=0;};
                         if (veld[tel]==1)
                          {veld[tel]=4;array[0]=tel;array[1]=1;
                           scanlvwdamslag(tel);scanrvwdamslag(tel);
                           scanlawdamslag(tel);scanrawdamslag(tel);
                           veld[tel]=1;};

                         if (maxscore>mixscore) mixscore=maxscore;                 
                                      count=0;};score=maxscore;
if(aantal>5)aantal=5;
}


static void scanzwslaan(int hok)
{int slachtoffer;
 veld[0]=10;
if(((veld[lv[hok]]==0)||(veld[lv[hok]]==1))&&(veld[llv[hok]]==4))  
 {slachtoffer=veld[lv[hok]];
  score=score+2;array[score]=lv[hok];array[score+1]=slachtoffer;
  array[score+2]=llv[hok];veld[lv[hok]]=4;
  if(score==maxscore)
   {aantal++;for (kat=0;kat<=score+2;kat++)zslag[aantal][kat]=array[kat];};
  if(score>maxscore)
   {maxscore=score;aantal=1;
    for(kat=0;kat<=score+2;kat++)zslag[1][kat]=array[kat];};
  scanzwslaan(llv[hok]);veld[lv[hok]]=slachtoffer;
  veld[llv[hok]]=4;score=score-2;};  
if(((veld[rv[hok]]==0)||(veld[rv[hok]]==1))&&(veld[rrv[hok]]==4))  
 {slachtoffer=veld[rv[hok]];
  score=score+2;array[score]=rv[hok];array[score+1]=slachtoffer;
  array[score+2]=rrv[hok];veld[rv[hok]]=4;
  if(score==maxscore)
   {aantal++;for (kat=0;kat<=score+2;kat++)zslag[aantal][kat]=array[kat];};
  if(score>maxscore)
   {maxscore=score;aantal=1;
    for(kat=0;kat<=score+2;kat++)zslag[1][kat]=array[kat];};
  scanzwslaan(rrv[hok]);veld[rv[hok]]=slachtoffer;
  veld[rrv[hok]]=4;score=score-2;};  
if(((veld[ra[hok]]==0)||(veld[ra[hok]]==1))&&(veld[rra[hok]]==4))  
 {slachtoffer=veld[ra[hok]];
  score=score+2;array[score]=ra[hok];array[score+1]=slachtoffer;
  array[score+2]=rra[hok];veld[ra[hok]]=4;
  if(score==maxscore)
   {aantal++;for (kat=0;kat<=score+2;kat++)zslag[aantal][kat]=array[kat];};
  if(score>maxscore)
   {maxscore=score;aantal=1;
    for(kat=0;kat<=score+2;kat++)zslag[1][kat]=array[kat];};
  scanzwslaan(rra[hok]);veld[ra[hok]]=slachtoffer;
  veld[rra[hok]]=4;score=score-2;};  
if(((veld[la[hok]]==0)||(veld[la[hok]]==1))&&(veld[lla[hok]]==4))  
 {slachtoffer=veld[la[hok]];
  score=score+2;array[score]=la[hok];array[score+1]=slachtoffer;
  array[score+2]=lla[hok];veld[la[hok]]=4;
  if(score==maxscore)
   {aantal++;for (kat=0;kat<=score+2;kat++)zslag[aantal][kat]=array[kat];};
  if(score>maxscore)
   {maxscore=score;aantal=1;
    for(kat=0;kat<=score+2;kat++)zslag[1][kat]=array[kat];};
  scanzwslaan(lla[hok]);veld[la[hok]]=slachtoffer;
  veld[lla[hok]]=4;score=score-2;};  



}

 
static void scanlvzdamslag(int hok)
{int wijs,helpje,bitje,slachtoffer;
 wijs=lv[hok];
 a: ;   if(veld[wijs]==PIECE_NONE){wijs=lv[wijs];goto a;};
 if(((veld[wijs]==PIECE_LIGHT_MAN)||(veld[wijs]==PIECE_LIGHT_KING))
   &&(veld[lv[wijs]]==PIECE_NONE)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=lv[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==PIECE_NONE)
    {scanlazdamslag(wijs);
     if(bitje==0){scanlvzdamslag(wijs);bitje=1;};
     scanrvzdamslag(wijs);
     wijs=lv[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==PIECE_NONE)
       { aantal++;for(kat=0;kat<=score+1;kat++)
                 {
                  zslag[aantal][kat]=array[kat];
                  zslag[aantal][score+2]=wijs;
                 };
         wijs=lv[wijs];
       };
    };score=score-2;slagveld[ra[helpje]]=0;
  }; 
}

 
static void scanrvzdamslag(int hok)
{int wijs,helpje,bitje,slachtoffer;
 wijs=rv[hok];
 a: ;   if(veld[wijs]==PIECE_NONE){wijs=rv[wijs];goto a;};
 if(((veld[wijs]==PIECE_LIGHT_MAN)||(veld[wijs]==PIECE_LIGHT_KING))
   &&(veld[rv[wijs]]==PIECE_NONE)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=rv[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==PIECE_NONE)
    {scanlvzdamslag(wijs);
     if(bitje==0){scanrvzdamslag(wijs);bitje=1;};
     scanrazdamslag(wijs);
     wijs=rv[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==PIECE_NONE)
       { aantal++;for(kat=0;kat<=score+1;kat++)
                 {
                  zslag[aantal][kat]=array[kat];
                  zslag[aantal][score+2]=wijs;
                 };
         wijs=rv[wijs];
       };
    };score=score-2;slagveld[la[helpje]]=0;
  }; 
}

 
static void scanlazdamslag(int hok)
{int wijs,helpje,bitje,slachtoffer;
 wijs=la[hok];
 a: ;   if(veld[wijs]==PIECE_NONE){wijs=la[wijs];goto a;};
 if(((veld[wijs]==PIECE_LIGHT_MAN)||(veld[wijs]==PIECE_LIGHT_KING))
   &&(veld[la[wijs]]==PIECE_NONE)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=la[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==PIECE_NONE)
    {scanlvzdamslag(wijs);
     if(bitje==0){scanlazdamslag(wijs);bitje=1;};
     scanrazdamslag(wijs);
     wijs=la[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==PIECE_NONE)
       { aantal++;for(kat=0;kat<=score+1;kat++)
                 {
                  zslag[aantal][kat]=array[kat];
                  zslag[aantal][score+2]=wijs;
                 };
         wijs=la[wijs];
       };
    };score=score-2;slagveld[rv[helpje]]=0;
  }; 
}

 
static void scanrazdamslag(int hok)
{int wijs,helpje,bitje,slachtoffer;
 wijs=ra[hok];
 a: ;   if(veld[wijs]==PIECE_NONE){wijs=ra[wijs];goto a;};
 if(((veld[wijs]==PIECE_LIGHT_MAN)||(veld[wijs]==PIECE_LIGHT_KING))
   &&(veld[ra[wijs]]==PIECE_NONE)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=ra[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==PIECE_NONE)
    {scanlazdamslag(wijs);
     if(bitje==0){scanrazdamslag(wijs);bitje=1;};
     scanrvzdamslag(wijs);
     wijs=ra[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==PIECE_NONE)
       { aantal++;for(kat=0;kat<=score+1;kat++)
                 {
                  zslag[aantal][kat]=array[kat];
                  zslag[aantal][score+2]=wijs;
                 };
         wijs=ra[wijs];
       };
    };score=score-2;slagveld[lv[helpje]]=0;
  }; 
}



static void sczwartslag(int handle)
{
	score = 0;
	maxscore = 0;
	mixscore = 0;
	aantal = 0;
	for (tel = 1; tel < 51; tel++)
	{
		if (veld[tel] == 2)
		{
			veld[tel] = 4;
			array[0] = tel;
			array[1] = 2;
			scanzwslaan(tel);
			veld[tel] = 2;
		}
		if (veld[tel] == 3)
		{
			veld[tel] = 4;
			array[0] = tel;
			array[1] = 3;
			scanlvzdamslag(tel);
			scanrvzdamslag(tel);
			scanlazdamslag(tel);
			scanrazdamslag(tel);
			veld[tel] = 3;
		}
		if (maxscore > mixscore)
			mixscore = maxscore;
		count = 0;
	}
	score = maxscore;
	if (aantal > 5)
		aantal = 5;
}


static void bestzwart(int handle)
{
	register zwartaantal, witaantal, zmixscore, wmixscore, teller,
		pipo, tul, zwarray[6][25], warray[6][25], num, zum, wfrom, wto, zfrom, zto;

	diepte++;
	if (diepte < 2)
		witgezet = 0;
minzwart[diepte+1]=100;minwit[diepte+1]=100;
maxzwart[diepte+1]=-100;maxwit[diepte+1]=-100;
minzwart[diepte]=100;maxzwart[diepte]=-100;       
/*minwit[diepte]=100;maxwit[diepte]=-100;*/
sczwartslag(handle);zwartaantal=aantal;zmixscore=mixscore;
if (zwartaantal>0)
   {for (teller=1;teller<=zwartaantal;teller++)
        {for (tul=0;tul<=zmixscore+2;tul++)
             zwarray[teller][tul]=zslag[teller][tul];};
    /*minwit[diepte]=100;maxwit[diepte]=-100;*/
    for (teller=1;teller<=zwartaantal;teller++)
       {doezwheenslag(handle,zmixscore,zwarray[teller]);
        if((zwartaantal==1)&&(diepte==1))
          {for (tul=0;tul<=zmixscore+2;tul++)
              {bestarray[1][tul]=zwarray[teller][tul];};bestaantal=zmixscore;eerste=slag;
               doezwterugslag(handle,zmixscore,zwarray[teller]);return;};  
        scwitslag(handle);witaantal=aantal;wmixscore=mixscore;
        minwit[diepte]=100;maxwit[diepte]=-100;
        if (witaantal>0)
           {for (pipo=1;pipo<=witaantal;pipo++)
                {for (tul=0;tul<=wmixscore+2;tul++)
                     warray[pipo][tul]=wslag[pipo][tul];}; 
            for (pipo=1;pipo<=witaantal;pipo++)
                 {doewheenslag(handle,wmixscore,warray[pipo]);
                  if((diepte<level)&&(witgezet!=1))bestzwart(handle);
                  doewterugslag(handle,wmixscore,warray[pipo]);
                  if(diepte==level)wit[diepte]=-wmixscore;
                  if(diepte<level)wit[diepte]=
                            -wmixscore+maxzwart[diepte+1];
                  if(wit[diepte]<minwit[diepte])                                     
                            minwit[diepte]=wit[diepte];
                  if(wit[diepte]>maxwit[diepte])              
                            maxwit[diepte]=wit[diepte];
                };
           };
        if (witaantal==0)
           {witgezet=0;
            if(diepte>1){/*wit[diepte]=minwit[diepte]=maxwit[diepte]=0;*/
                              goto sabrina;};
            for (num=6;num<=50;num++)
                 {if ((veld[num]==0)&&(veld[lv[num]]==4))  
                     { wfrom=num;wto=lv[num];
                       doewheenzet(handle,wfrom,wto);
                       if (diepte<level)bestzwart(handle);
                       doewtrugzet(handle,wfrom,wto);
                       if(diepte==level)wit[diepte]=0;
                       if(diepte<level)wit[diepte]=
                            maxzwart[diepte+1];
                       if(wit[diepte]<minwit[diepte])                                     
                            minwit[diepte]=wit[diepte];
                       if(wit[diepte]>maxwit[diepte])              
                            maxwit[diepte]=wit[diepte];
                     };
                  if ((veld[num]==0)&&(veld[rv[num]]==4))  
                     { wfrom=num;wto=rv[num];
                       doewheenzet(handle,wfrom,wto);
                       if (diepte<level)bestzwart(handle);
                       doewtrugzet(handle,wfrom,wto);
                       if(diepte==level)wit[diepte]=0;
                       if(diepte<level)wit[diepte]=
                            maxzwart[diepte+1];
                       if(wit[diepte]<minwit[diepte])                                     
                            minwit[diepte]=wit[diepte];
                       if(wit[diepte]>maxwit[diepte])              
                            maxwit[diepte]=wit[diepte];
                     };
                 };goto the_moon;sabrina:; if(diepte==level)wit[diepte]=0;
                            if(diepte<level)wit[diepte]=0;
                               /*maxzwart[diepte+1];*/
                            if(wit[diepte]<minwit[diepte])                                     
                               minwit[diepte]=wit[diepte];
                            if(wit[diepte]>maxwit[diepte])              
                               maxwit[diepte]=wit[diepte];the_moon:;  
           };        
        doezwterugslag(handle,zmixscore,zwarray[teller]);
        zwart[diepte]=minwit[diepte]+zmixscore;
        if(zwart[diepte]<minzwart[diepte])
          minzwart[diepte]=zwart[diepte];
        if(zwart[diepte]>maxzwart[diepte])
          {maxzwart[diepte]=zwart[diepte];moislag=1;};
        if((diepte==1)&&(zwart[1]==maxzwart[1]))
           {for (tul=0;tul<=zmixscore+2;tul++)
                {bestarray[1][tul]=zwarray[teller][tul];moislag++;};
            bestaantal=zmixscore;eerste=slag;}; 
       };    
   };        
if (zwartaantal==0)
   {/*minwit[diepte]=100;maxwit[diepte]=-100;*/
    for (zum=45;zum>0;zum--)
         {if ((veld[zum]==2)&&(veld[ra[zum]]==4))  
             { zfrom=zum;zto=ra[zum];
               doezwheenzet(handle,zfrom,zto);
               scwitslag(handle);witaantal=aantal;wmixscore=mixscore;
               minwit[diepte]=100;maxwit[diepte]=-100;
               if (witaantal>0)
                  {for (pipo=1;pipo<=witaantal;pipo++)
                       {for (tul=0;tul<=wmixscore+2;tul++)
                             warray[pipo][tul]=wslag[pipo][tul];}; 
                   for (pipo=1;pipo<=witaantal;pipo++)
                       {doewheenslag(handle,wmixscore,warray[pipo]);
                        if((diepte<level)&&(witgezet!=1))bestzwart(handle);
                        doewterugslag(handle,wmixscore,warray[pipo]);
                        if(diepte==level)wit[diepte]=-wmixscore;
                        if(diepte<level)wit[diepte]=
                            -wmixscore+maxzwart[diepte+1];
                        if(wit[diepte]<minwit[diepte])                                     
                            minwit[diepte]=wit[diepte];
                        if(wit[diepte]>maxwit[diepte])              
                            maxwit[diepte]=wit[diepte];
                       };  
                  };
               if (witaantal==0)
                  {witgezet=0;
                   if(diepte>1){/*wit[diepte]=minwit[diepte]=maxwit[diepte]=0*/;
                              goto sylvia;};
                   for (num=6;num<=50;num++)
                       {if ((veld[num]==0)&&(veld[lv[num]]==4))  
                           {wfrom=num;wto=lv[num];
                            doewheenzet(handle,wfrom,wto);
                            if (diepte<level)bestzwart(handle);
                            doewtrugzet(handle,wfrom,wto);
                            if(diepte==level)wit[diepte]=0;
                            if(diepte<level)wit[diepte]=
                               maxzwart[diepte+1];
                            if(wit[diepte]<minwit[diepte])                                     
                               minwit[diepte]=wit[diepte];
                            if(wit[diepte]>maxwit[diepte])              
                               maxwit[diepte]=wit[diepte];
                           };
                        if ((veld[num]==0)&&(veld[rv[num]]==4))  
                           {wfrom=num;wto=rv[num];
                            doewheenzet(handle,wfrom,wto);
                            if (diepte<level)bestzwart(handle);
                            doewtrugzet(handle,wfrom,wto); 
                            if(diepte==level)wit[diepte]=0;
                            if(diepte<level)wit[diepte]=
                               maxzwart[diepte+1];
                            if(wit[diepte]<minwit[diepte])                                     
                               minwit[diepte]=wit[diepte];
                            if(wit[diepte]>maxwit[diepte])              
                               maxwit[diepte]=wit[diepte];
                           };
                       }goto the_sun;sylvia:; if(diepte==level)wit[diepte]=0;
                            if(diepte<level)wit[diepte]=0;
                               /*maxzwart[diepte+1];*/
                            if(wit[diepte]<minwit[diepte])                                     
                               minwit[diepte]=wit[diepte];
                            if(wit[diepte]>maxwit[diepte])              
                               maxwit[diepte]=wit[diepte];the_sun:;
                  };  
               doezwtrugzet(handle,zfrom,zto);
               zwart[diepte]=minwit[diepte];
               if(zwart[diepte]<minzwart[diepte])
                  minzwart[diepte]=zwart[diepte];
               if(zwart[diepte]>maxzwart[diepte])
                  {maxzwart[diepte]=zwart[diepte];if(diepte==1)moizet=1;};
               if((diepte==1)&&(zwart[1]==maxzwart[1]))
                 {eerste=zet;bestvan[moizet]=zfrom;
                  bestnaar[moizet]=zto;moizet++;}; 
             };
          if ((veld[zum]==2)&&(veld[la[zum]]==4))  
             { zfrom=zum;zto=la[zum];
               doezwheenzet(handle,zfrom,zto);
               scwitslag(handle);witaantal=aantal;wmixscore=mixscore;
               minwit[diepte]=100;maxwit[diepte]=-100;
               if (witaantal>0)
                  {for (pipo=1;pipo<=witaantal;pipo++)
                       {for (tul=0;tul<=wmixscore+2;tul++)
                             warray[pipo][tul]=wslag[pipo][tul];}; 
                   for (pipo=1;pipo<=witaantal;pipo++)
                       {doewheenslag(handle,wmixscore,warray[pipo]);
                        if((diepte<level)&&(witgezet!=1))bestzwart(handle);
                        doewterugslag(handle,wmixscore,warray[pipo]);
                        if(diepte==level)wit[diepte]=-wmixscore;
                        if(diepte<level)wit[diepte]=
                            -wmixscore+maxzwart[diepte+1];
                        if(wit[diepte]<minwit[diepte])                                     
                            minwit[diepte]=wit[diepte];
                        if(wit[diepte]>maxwit[diepte])              
                            maxwit[diepte]=wit[diepte];
                       };  
                  };
               if (witaantal==0)
                  {witgezet=0;
                   if(diepte>1){/*wit[diepte]=minwit[diepte]=maxwit[diepte]=0;*/
                              goto miranda;};
                   for (num=6;num<=50;num++)
                       {if ((veld[num]==0)&&(veld[lv[num]]==4))  
                           {wfrom=num;wto=lv[num];
                            doewheenzet(handle,wfrom,wto);
                            if (diepte<level)bestzwart(handle);
                            doewtrugzet(handle,wfrom,wto);
                            if(diepte==level)wit[diepte]=0;
                            if(diepte<level)wit[diepte]=
                               maxzwart[diepte+1];
                            if(wit[diepte]<minwit[diepte])                                     
                               minwit[diepte]=wit[diepte];
                            if(wit[diepte]>maxwit[diepte])              
                               maxwit[diepte]=wit[diepte];
                           };
                        if ((veld[num]==0)&&(veld[rv[num]]==4))  
                           {wfrom=num;wto=rv[num];
                            doewheenzet(handle,wfrom,wto);
                            if (diepte<level)bestzwart(handle);
                            doewtrugzet(handle,wfrom,wto); 
                            if(diepte==level)wit[diepte]=0;
                            if(diepte<level)wit[diepte]=
                               maxzwart[diepte+1];
                            if(wit[diepte]<minwit[diepte])                                     
                               minwit[diepte]=wit[diepte];
                            if(wit[diepte]>maxwit[diepte])              
                               maxwit[diepte]=wit[diepte];
                           };
                       }goto the_stars;miranda:; if(diepte==level)wit[diepte]=0;
                            if(diepte<level)wit[diepte]=0;
                               /*maxzwart[diepte+1];*/
                            if(wit[diepte]<minwit[diepte])                                     
                               minwit[diepte]=wit[diepte];
                            if(wit[diepte]>maxwit[diepte])              
                               maxwit[diepte]=wit[diepte];the_stars:; 
                  };  
             doezwtrugzet(handle,zfrom,zto);
             zwart[diepte]=minwit[diepte];
             if(zwart[diepte]<minzwart[diepte])
               minzwart[diepte]=zwart[diepte];
             if(zwart[diepte]>maxzwart[diepte])
               {maxzwart[diepte]=zwart[diepte];if(diepte==1)moizet=1;};
             if((diepte==1)&&(zwart[1]==maxzwart[1]))
                 {eerste=zet;bestvan[moizet]=zfrom;
                  bestnaar[moizet]=zto;moizet++;}; 
             };
         };           
   };
/*eind:;*/  diepte--;if(diepte<2)witgezet=0;
}


static void doezwheenslag(int handle, int zixscore, int zwarray[25])
{
	/*wacht(5);*/
	for (count = 0; count <= zixscore; count += 2)
	{
		/*wacht(5);*/
		veld[zwarray[count]] = 4;
	}
	veld[zwarray[zixscore + 2]] = zwarray[1];
	/*wacht(5);*/                       
}


static void doezwterugslag(int handle, int zixscore, int zwarray[25])
{
	/*wacht(5);*/
	veld[zwarray[zixscore + 2]] = 4;
	for( count = zixscore; count >= 0; count = count - 2 )
	{
		veld[zwarray[count]] = zwarray[count + 1];
	}
	/*veld[zwarray[0]] = 2;
	wacht(5);*/
}


static void doewheenslag(int handle, int wixscore, int warray[25])
{
	/*wacht(5);*/
	for( count = 0; count <= wixscore; count += 2 )
	{
		/*wacht(5);*/
		veld[warray[count]] = PIECE_NONE;
	}
	veld[warray[wixscore + 2]] = warray[1];
	/*wacht(5);*/
}


static void doewterugslag(int handle, int wixscore, int warray[25])
{
	/*wacht(5);*/
	veld[warray[wixscore + 2]] = PIECE_NONE;
	for( count = wixscore; count >= 0; count -= 2 )
	{
		veld[warray[count]] = warray[count + 1];
	}
	/*veld[warray[0]] = 0;
	wacht(5);*/
}


static void doezwheenzet(int handle, int zfrom, int zto)
{
	veld[zfrom] = PIECE_NONE;
	veld[zto] = PIECE_DARK_MAN;
	if( zto > 45 )
		veld[zto] = PIECE_DARK_KING;
}


static void doezwtrugzet(int handle, int zfrom, int zto)
{
	veld[zto] = PIECE_NONE;
	veld[zfrom] = PIECE_DARK_MAN;
}


static void doewheenzet(int handle, int wfrom, int wto)
{
	veld[wfrom] = PIECE_NONE;
	veld[wto] = PIECE_LIGHT_MAN;
	if( wto < 6 )
		veld[wto] = PIECE_LIGHT_KING;
}
static void doewtrugzet(int handle, int wfrom, int wto)
{
	veld[wto] = PIECE_NONE;
	veld[wfrom] = PIECE_LIGHT_MAN;
}


static void dozwheenslag(int handle, int zixscore, int zwarray[25])
{
	wacht(5);
	for( count = 0; count <= zixscore; count = count + 2 )
	{
		remove_piece(handle, zwarray[count]);
		veld[zwarray[count]] = PIECE_NONE;
		wacht(5);
	};
	draw_piece(handle, zwarray[zixscore + 2], zwarray[1]);
	veld[zwarray[zixscore + 2]] = zwarray[1];
}


static void dozwterugslag(int handle, int zixscore, int zwarray[25])
{
	wacht(5);
	remove_piece(handle, zwarray[zixscore + 2]);
	veld[zwarray[zixscore+2]] = PIECE_NONE;
	for( count = zixscore; count >= 0; count = count - 2 )
	{
		draw_piece(handle, zwarray[count], zwarray[count + 1]);
		veld[zwarray[count]] = zwarray[count + 1];
		wacht(5);
	}
	/*draw_piece(handle, zwarray[0], 2);
	veld[zwarray[0]] = 2;*/
	wacht(5);
}


static void dowheenslag(int handle, int wixscore, int warray[25])
{
	for( count = 0; count <= wixscore; count += 2 )
	{
		remove_piece(handle, warray[count]);
		veld[warray[count]] = PIECE_NONE;
	}
	draw_piece(handle, warray[wixscore + 2], warray[1]);
	veld[warray[wixscore + 2]] = warray[1];
}


static void dowterugslag(int handle, int wixscore, int warray[25])
{
	wacht(5);
	remove_piece(handle, warray[wixscore + 2]);
	veld[warray[wixscore + 2]] = PIECE_NONE;
	for( count = wixscore; count >= 0; count -= 2 )
	{
		draw_piece(handle, warray[count], warray[count + 1]);
		veld[warray[count]] = warray[count + 1];
	}
	/*draw_piece(handle, warray[0], 0);
	veld[warray[0]] = 0;*/
	wacht(5);
}


static void dozwheenzet(int handle, int zfrom, int zto)
{
	remove_piece(handle, zfrom);
	veld[zfrom] = PIECE_NONE;
	draw_piece(handle, zto, PIECE_DARK_MAN);
	veld[zto] = PIECE_DARK_MAN;
}


static void dozwtrugzet(int handle, int zfrom, int zto)
{
	remove_piece(handle, zto);
	veld[zto] = PIECE_NONE;
	draw_piece(handle, zfrom, PIECE_DARK_MAN);
	veld[zfrom] = PIECE_DARK_MAN;
}


static void dowheenzet(int handle, int wfrom, int wto)
{
	remove_piece(handle, wfrom);
	veld[wfrom] = PIECE_NONE;
	draw_piece(handle, wto, PIECE_LIGHT_MAN);
	veld[wto] = PIECE_LIGHT_MAN;
}


static void dowtrugzet(int handle, int wfrom, int wto)
{
	remove_piece(handle, wto);
	veld[wto] = PIECE_NONE;
	draw_piece(handle, wfrom, PIECE_LIGHT_MAN);
	veld[wfrom] = PIECE_LIGHT_MAN;
}


static void wacht(int til)
{
	int c, d, tal;

	for( tal = 0; tal < 15000; tal++ )
	{
		for( c = 0; c < til; c++ )
		{
			d = 1;
			if( d != 1 ) /* use d to avoid warning */
				return;
		}
	}
}


static void init_board(int handle)
{
	int pos = 1;

	veld[0] = 10;
	while( pos < 21 )
	{
		draw_piece(handle, pos, PIECE_DARK_MAN);
		veld[pos++] = PIECE_DARK_MAN;
	}
	while( pos < 31 )
	{
		remove_piece(handle, pos);
		veld[pos++] = PIECE_NONE;
	}
	while( pos < 51 )
	{
		draw_piece(handle, pos, PIECE_LIGHT_MAN);
		veld[pos++] = PIECE_LIGHT_MAN;
	}
}

/* drawing field */
/*static void tekenveld(int handle)
{
	int pos;

	for( pos = 1; pos < 51; pos++ )
	{
		if( veld[pos] == PIECE_DARK_MAN )
			draw_piece(handle, pos, PIECE_DARK_MAN);
		if( veld[pos] == PIECE_LIGHT_MAN )
			draw_piece(handle, pos, PIECE_LIGHT_MAN);
		if( veld[pos] == PIECE_NONE)
			remove_piece(handle, pos);
	}
}*/

/* most neighbors black */
static void meesteburenzwart(int handle)
{
	maxvan = bestvan[1];
	maxnaar = bestnaar[1];
/*	goto fietsen;
	if( moizet > 5 )
	{
		maxvan = bestvan[moizet - 4];
		maxnaar = bestnaar[moizet - 4];
	}*/
	maxaantburen = -1;
	wisselbitje = -wisselbitje;
	if( wisselbitje == 1 )
	{
		anfang = 1;
		ende = moizet;
	}
	if( wisselbitje == -1 )
	{
		anfang = moizet;
		ende = 0;
	}
lopen:
	maxvan = bestvan[(rendom * moizet) / 50];
	maxnaar = bestnaar[(rendom * moizet) / 50];
	if( maxvan == 0 || maxnaar == 0 || maxvan == maxnaar )
	{
		rendom++;
		if( rendom > 50 )
			rendom = 0;
		goto lopen;
	}
/*	goto fietsen;
	for( berta = anfang; berta != ende; berta = berta + wisselbitje )
	{
		doezwheenzet(bestvan[berta], bestnaar[berta]);
		aantalburen = 0;
		if( veld[la[bestnaar[berta]]] == 2 )
			aantalburen++;
		if( veld[ra[bestnaar[berta]]] == 2 )
			aantalburen++;
		if( veld[lv[bestnaar[berta]]] == 2 )
			aantalburen++;
		if( veld[rv[bestnaar[berta]]] == 2 )
			aantalburen++;
		if( aantalburen > maxaantburen )
		{
			maxaantburen = aantalburen;
			maxvan = bestvan[berta];
			maxnaar = bestnaar[berta];
		}
		doezwtrugzet(bestvan[berta], bestnaar[berta]);
	}
fietsen:*/
}


static void do_menu(int handle)
{
	int pstatus, x, y;

	menu_show(handle, level);

	do
	{
		vq_mouse(handle, &pstatus, &x, &y);
		if (pstatus == 1) /* left click */
		{
			switch (menu_button_at(x, y))
			{
				case IDM_EXIT:
					g_exit = 1;
					break;
				case IDM_CLEAR:
					clear_board(handle);
					break;
				case IDM_NEW:
					new_game(handle);
					break;
				case IDM_UNDO:
					zetterug(handle);
					break;
				case IDM_WINDOW:
					voorruit(handle);
					break;
				case IDM_EDIT:
					edit(handle);
					break;
				case IDM_LEVUP:
					if (level < 6)
						menu_update_level(handle, ++level);
					break;
				case IDM_LEVDN:
					if (level > 1)
						menu_update_level(handle, --level);
					break;
			}
			while (pstatus == 1)
				vq_mouse(handle, &pstatus, &x, &y);
		}
	} while (g_exit == 0 && menu_contains(x, y));

	menu_hide(handle);
}


static void zetterug(int handle)
{hide_mouse();
 if(zettenteller>0){zettenteller=zettenteller-1;wisselbitje=-wisselbitje;};
 for(tel=1;tel<51;tel++)
   {if(veld[tel]==bord[zettenteller][tel]) goto f;
    if(bord[zettenteller][tel]==0){veld[tel]=0;draw_piece(handle,tel,0);};
    if(bord[zettenteller][tel]==2){veld[tel]=2;draw_piece(handle,tel,2);};
    if(bord[zettenteller][tel]==1){veld[tel]=1;tekenwittedam(handle,tel);};
    if(bord[zettenteller][tel]==3){veld[tel]=3;tekenzwartedam(handle,tel);};
    if(bord[zettenteller][tel]==4){veld[tel]=4;remove_piece(handle,tel);};
    f:;
   };show_mouse();
}

/* windshield */
static void voorruit(int handle)
{
	hide_mouse();
	if( zettenteller < mz )
	{
		zettenteller = zettenteller + 1;
		wisselbitje = -wisselbitje;
	}
	for( tel = 1; tel < 51; tel++ )
	{
		if( veld[tel] == bord[zettenteller][tel] )
			goto l;
		if(bord[zettenteller][tel]==0){veld[tel]=0;draw_piece(handle,tel,0);}
		if(bord[zettenteller][tel]==2){veld[tel]=2;draw_piece(handle,tel,2);}
		if(bord[zettenteller][tel]==1){veld[tel]=1;tekenwittedam(handle,tel);}
		if(bord[zettenteller][tel]==3){veld[tel]=3;tekenzwartedam(handle,tel);}
		if(bord[zettenteller][tel]==4){veld[tel]=4;remove_piece(handle,tel);}
l:;
	}
	show_mouse();
}


static void new_game(int handle)
{
	zettenteller = 0;
	hide_mouse();
	wisselbitje = 1;
	for( tel = 1; tel < 51; tel++ )
	{
		if( veld[tel] != bord[0][tel] )
		{
			if( bord[0][tel] == PIECE_LIGHT_MAN )
			{
				veld[tel] = PIECE_LIGHT_MAN;
				draw_piece(handle, tel, PIECE_LIGHT_MAN);
			}
			if( bord[0][tel] == PIECE_DARK_MAN )
			{
				veld[tel] = PIECE_DARK_MAN;
				draw_piece(handle, tel, PIECE_DARK_MAN);
			}
			if( bord[0][tel] == PIECE_NONE )
			{
				veld[tel] = PIECE_NONE;
				remove_piece(handle, tel);
			}
		}
	}
	show_mouse();
}


static void edit(int handle)
{
	int pstatus, x, y;

	menu_set_edit(handle, 1);

	do {
		vq_mouse(handle, &pstatus, &x, &y);
	} while( pstatus == 1 );

	while( x < 100 )
	{
		vq_mouse(handle, &pstatus, &x, &y);
		if( pstatus == 1 )
			goto noot;
	}
aap:
	while( pstatus != 1 )
	{
		vq_mouse(handle, &pstatus, &x, &y);
		if( x < 100 )
			goto noot;
	}

	where_is_mouse(handle);

	hide_mouse();
	switch( veld[daar] )
	{
		case PIECE_LIGHT_MAN:
			veld[daar] = PIECE_DARK_MAN;
			draw_piece(handle, daar, PIECE_DARK_MAN);
			break;
		case PIECE_LIGHT_KING:
			veld[daar] = PIECE_DARK_KING;
			tekenzwartedam(handle, daar);
			break;
		case PIECE_DARK_MAN:
			veld[daar] = PIECE_LIGHT_KING;
			tekenwittedam(handle, daar);
			break;
		case PIECE_DARK_KING:
			veld[daar] = PIECE_NONE;
			remove_piece(handle, daar);
			break;
		case PIECE_NONE:
			veld[daar] = PIECE_LIGHT_MAN;
			draw_piece(handle, daar, PIECE_LIGHT_MAN);
			break;
	}
	while( pstatus == 1 )
		vq_mouse(handle, &pstatus, &x, &y);
	show_mouse();
	if( x < 100 )
		goto noot;
	goto aap;
noot:
	zettenteller++;
	if( zettenteller > mz )
		mz = zettenteller;
	for( tel = 1; tel < 51; tel++ )
		bord[zettenteller][tel] = veld[tel];
/*	hide_mouse();*/
/*mies:;*/
	if( zettenteller > 105 )
		zettenteller = 0;

#ifdef JUNK
	pxyarray[0] = 20;
	pxyarray[1] = 222;
	pxyarray[2] = 80;
	vsf_interior(handle, 2);
	pxyarray[3] = 239;
	vsl_type(handle, 1);
	vswr_mode(handle, MD_REPLACE);
	vsf_style(handle, 8);
	vr_recfl(handle, pxyarray);

	vst_effects(handle, 0);
	vswr_mode(handle, MD_XOR);
	v_gtext(handle, 33, 235,"edit");

	vswr_mode(handle, MD_XOR);
	show_mouse();
#endif
	menu_set_edit(handle, 0);
}


static void clear_board(int handle)
{
	for( tel = 1; tel < 51; tel++ )
	{
		if( veld[tel] != PIECE_NONE )
			remove_piece(handle, tel);
	}
	zettenteller++;
	if( zettenteller > mz )
		mz = zettenteller;
	for( tel = 1; tel < 51; tel++ )
		bord[zettenteller][tel] = veld[tel];
	if( zettenteller > 105 )
		zettenteller = 0;
}


static void debug_msg(int handle, const char *msg)
{
	static int xy[] = {BOARD_RIGHT + 1, 0, 639, 20};
	int i;

	vswr_mode(handle, MD_REPLACE);
	vsf_interior(handle, FIS_SOLID);
	vsf_color(handle, 1);
	vr_recfl(handle, xy);

	vswr_mode(handle, MD_XOR);
	/*
		horz: 0=left, 1=center, 2=right
		vert: 0=baseline, 1=half, 2=ascent, 3=bottom, 4=descent, 5=top
	*/
	vst_alignment(handle, 0, 5, &i, &i);
	v_gtext(handle, BOARD_RIGHT + 1, 0, (char*)msg);
}


int main(void)
{
	int handle, work_in[16], work_out[57];
	int pstatus, x, y;
	int i, from = 0, to = 0;

	veld[0] = 10;
	wisselbitje = 1;

	memset(work_in, 0, 16 * sizeof(int));
	for (i = 0; i < 10; i++)
		work_in[i] = 1;
	work_in[10] = 2;

	appl_init();
	graf_mouse(ARROW, NULL);
	hide_mouse();
	v_opnvwk(work_in, &handle, work_out);
	v_clrwk(handle);

	menu_hide(handle);

	board_draw_grid_lines(handle);
	board_draw_gray_squares(handle);

	pxyarray[0] = BOARD_RIGHT + 1;
	pxyarray[1] = 0;
	pxyarray[2] = 639;
	pxyarray[3] = 399;
	vsf_style(handle, 8);
	vr_recfl(handle, pxyarray);

	init_board(handle);
	rendom = 1;
	for( tel = 1; tel < 51; tel++ )
		bord[0][tel] = veld[tel];

	show_mouse();

	/*maxarray[1] = 4;
	maxarray[2] = 5;*/

	zettenteller = 0;
	level = 3;
	vq_mouse(handle, &pstatus, &x, &y);
	g_exit = 0;
	while (g_exit == 0)
	{
		for (tel = 1; tel < 51; tel++)
			bord[zettenteller][tel] = veld[tel];
g:
		vq_mouse(handle, &pstatus, &x, &y);
		while (pstatus != 1 && g_exit == 0)
		{
			vq_mouse(handle, &pstatus, &x, &y);
			rendom++;
			if (rendom > 50)
				rendom = 1;
			if (menu_contains(x, y))
				do_menu(handle);
			/*if (x > 100)
				goto c;*/
		}
/*c:*/
		if (g_exit == 1)
			break;/*goto e;*/

		scwitslag(handle);
		if (aantal == 0)
			doewzet(handle, from, to);
		if (aantal > 0)
			witteslag(handle);
		if (illegalezet == 1)
		{
			illegalezet = 0;
			goto g;
		}
		hide_mouse();
		for (tel = 1; tel < 6; tel++)
		{
			if (veld[tel] == 0)
				tekenwittedam(handle, tel);
		}
		/*totzwart = 0;
		totwit = 0;*/
		slag = 1;
		zet = 0;
		/*eerstaantal = 0;
		hiscore = -10;*/
		witgezet = 0;
		diepte = 0;
		/*testbitje = 1;
		subhigh = -10;
		for (tel = 1; tel < 51; tel++)
			xbios(32, tel);*/
		zettenteller++;
		if (zettenteller > mz)
			mz = zettenteller;
		for (tel = 1; tel <= level + 1; tel++)
		{
			zwart[tel] = wit[tel] = 0;
			maxzwart[tel] = 0;
			maxwit[tel] = -100;
			minzwart[tel] = minwit[tel] = 100;
		}
		if (zettenteller > 100)
			zettenteller = 1;
		moizet = 0;
		bestzwart(handle);
		/*tekenzwartedam(handle,(hiscore+1));*/

		sczwartslag(handle);
		anzahl = aantal;
		if (anzahl == 0)
		{
			meesteburenzwart(handle);
			dozwheenzet(handle, maxvan, maxnaar);
		}
		if (anzahl > 0)
		{
			dozwheenslag(handle, bestaantal, bestarray[1]);
		}

		/*{
			meesteburenzwart(handle);
			dozwheenzet(handle, maxvan, maxnaar);
		}*/

		moizet = 0;
		moislag = 1;
		for (tel = 46; tel < 51; tel++)
		{
			if (veld[tel] == PIECE_DARK_MAN)
				tekenzwartedam(handle, tel);
		}
		show_mouse();
	}

/*e:*/

	v_clsvwk(handle);
	appl_exit();
	return(0);
}
