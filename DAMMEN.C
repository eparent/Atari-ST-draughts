/*#include "init.c"*/
#include "tos.h"
/*#include "gembind.h"*/
#include "vdi.h"
#include "stddef.h"
#include "aes.h"                                                        
/*#include "obdefs.h"*/                                         /*scwslaan*/               
/*# define STACKSIZE 15000 ;*/
#define TRUE=1
#define FALSE=0

int contrl[12], intin[128], ptsin[128], intout[128], ptsout[128],
    work_in[11], work_out[57];                                /*zwdamslag*/           
int handle, pxyarray[8],hidden,veld[60],ok,score,testbitje;   /*bestzwart*/
int posx[51]={50,160,240,320,400,480,120,200,280,360,440,
                160,240,320,400,480,120,200,280,360,440,
                160,240,320,400,480,120,200,280,360,440, 
                160,240,320,400,480,120,200,280,360,440,
                160,240,320,400,480,120,200,280,360,440};

int posy[51]={50,20,20,20,20,20,60,60,60,60,60,100,100,100,100,100,
              140,140,140,140,140,180,180,180,180,180,220,220,220,220,220,
              260,260,260,260,260,300,300,300,300,300,340,340,340,340,340,
              380,380,380,380,380};

int lv[51]={0,0,0,0,0,0,0,1,2,3,4,6,7,8,9,10,0,11,12,13,14,16,17,18,19,20
  ,0,21,22,23,24,26,27,28,29,30,0,31,32,33,34,36,37,38,39,40,0,41,42,43,44}; 

int rv[51]={0,0,0,0,0,0,1,2,3,4,5,7,8,9,10,0,11,12,13,14,15,17,18,19,20,0,
 21,22,23,24,25,27,28,29,30,0,31,32,33,34,35,37,38,39,40,0,41,42,43,44,45};

int ra[51]={0,7,8,9,10,0,11,12,13,14,15,17,18,19,20,0,21,22,23,24,25,27,
 28,29,30,0,31,32,33,34,35,37,38,39,40,0,41,42,43,44,45,47,48,49,50,0,0,0,
 0,0,0};
 
int rra[51]={0,12,13,14,15,0,17,18,19,20,0,22,23,24,25,0,27,28,29,30,0,
 32,33,34,35,0,37,38,39,40,0,42,43,44,45,0,47,48,49,50,0,0,0,0,0,0,0
 ,0,0,0,0};

int la[51]={0,6,7,8,9,10,0,11,12,13,14,16,17,18,19,20,0,21,22,23,24,26,27,
 28,29,30,0,31,32,33,34,36,37,38,39,40,0,41,42,43,44,46,47,48,49,50
 ,0,0,0,0,0};

int lla[51]={0,0,11,12,13,14,0,16,17,18,19,0,21,22,23,24,0,26,27,28,29,0,
 31,32,33,34,0,36,37,38,39,0,41,42,43,44,0,46,47,48,49,0,0,0,0,0,0,0,0,0,0};

int llv[51]={0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,0,6,7,8,9,0,11,12,
 13,14,0,16,17,18,19,0,21,22,23,24,0,26,27,28,29,0,31,32,33,34,0,36,37,
 38,39};


int rrv[51]={0,0,0,0,0,0,0,0,0,0,0,2,3,4,5,0,7,8,9,10,0,12,13,14,15,0,
 17,18,19,20,0,22,23,24,25,0,27,28,29,30,0,32,33,34,35,0,37,38,39,40,0};

int array[21],maxarray[21],maxscore,tel,tal,c,d,hulp[20],count,mixscore,
    vorige[30],volgende[30],from[30],to[30],diepte,diep,pointwit[30],
    pointzwart[30],kat,zslag[25][25],wslag[25][25],aantal,oud;
int slarray[20][20],bestslarray[20],level,wisselbitje,mz,bitje,
    skore[10],totzwart,totwit,eerstarray[30],eerste,slag,zet,
    eerstaantal,hiscore,eerstvan,eerstnaar,bestvan[20],bestnaar[20],
    bestarray[10][30],witheeftgezet,bestaantal,anfang,ende,rendom;
int hulpje,daar,teller,buf[30],goed,kut,cont,zwartesteen,wittesteen,leeg,
    plekkie,minimaalverlies,maxaantburen,aantalburen,maxvan,maxnaar,anzahl,
    subhigh,minsubhigh[30],klaar,zettenteller,bord[110][60],illegalezet;
int minwit[10],maxwit[10],minzwart[10],moizet,moislag,slagveld[51],
    maxzwart[10],wit[10],zwart[10],zwartedam,wittedam,witgezet;

void hide_mouse(void)
{if(!hidden){graf_mouse( M_OFF, (void *)0 );hidden=1;}}
 
void show_mouse(void)
{if(hidden){graf_mouse(M_ON,(void *)0);hidden=0;}}

tekenschijf(handle,plek,kleur) int handle,plek,kleur;
{int x,y;/*vsf_interior(handle,2);*/  
 if(kleur==1){tekenwittedam(handle,plek);goto guppy;};
 if(kleur==3){tekenzwartedam(handle,plek);goto guppy;};
  vsf_interior(handle,kleur);vswr_mode(handle,1);
  vsf_style(handle,8);   
  x=posx[plek];
  y=posy[plek];
  v_circle(handle,x,y,19);vsf_interior(handle,0);
if(kleur==0){v_circle(handle,x,y,13);v_circle(handle,x,y,6);veld[plek]=0;};
if(kleur==2){vsl_type(handle,1);vsf_interior(handle,0);vswr_mode(handle,3);
             vsf_style(handle,8);vsm_color(handle,2);veld[plek]=2;  
             v_circle(handle,x,y,13);v_circle(handle,x,y,6);};  
vsf_interior(handle,2);
vsl_type(handle,1);vswr_mode(handle,1);guppy: ;
}


static tekenwittedam(handle,plaats) int handle,plaats;
{
 verwijderschijf(handle,plaats);
 pxyarray[0]=posx[plaats]-15;pxyarray[1]=posy[plaats]-10;
 pxyarray[2]=posx[plaats]+15;pxyarray[3]=posy[plaats]+10;
 vsf_interior(handle,0);vsf_style(handle,8);vsl_type(handle,1);
 vr_recfl(handle,pxyarray); 
 pxyarray[0]=posx[plaats]-15;pxyarray[1]=posy[plaats];
 pxyarray[2]=posx[plaats]+15;pxyarray[3]=posy[plaats];
 v_pline(handle,2,pxyarray);
 veld[plaats]=1;
}


static tekenzwartedam(handle,plaats) int handle,plaats;
{
 verwijderschijf(handle,plaats);vswr_mode(handle,1);
 pxyarray[0]=posx[plaats]-15;pxyarray[1]=posy[plaats]-10;
 pxyarray[2]=posx[plaats]+15;pxyarray[3]=posy[plaats]+10;
 vsf_interior(handle,2);vsf_style(handle,8);vsl_type(handle,7);
 vr_recfl(handle,pxyarray);vsl_type(handle,1);vswr_mode(handle,3);
 pxyarray[0]=posx[plaats]-15;pxyarray[1]=posy[plaats];
 pxyarray[2]=posx[plaats]+15;pxyarray[3]=posy[plaats];
 v_pline(handle,2,pxyarray);
 veld[plaats]=3;
}



static verwijderschijf(handle,plek) int handle,plek;
{ int x,y,pxyarray[4];
  vsf_interior(handle,2);vswr_mode(handle,1);
  vsf_style(handle,4);/*vsf_color(handle,4);*/
  pxyarray[0]=posx[plek]-20;pxyarray[1]=posy[plek]-20;
  pxyarray[2]=posx[plek]+20;pxyarray[3]=posy[plek]+20;
  v_bar(handle,pxyarray);veld[plek]=4;
}

doewzet(handle,van,naar) int handle,van,naar;
{ int pstatus,x,y,tel;
nogeens: while (pstatus!=1) vq_mouse(handle,&pstatus,&x,&y);
while (pstatus!=0) vq_mouse(handle,&pstatus,&x,&y);
for (tel=1;tel<51;tel++) {if(( ((posx[tel]-21)<x)&&(x<(posx[tel]+21)) )&&
                     ( ((posy[tel]-21)<y)&&(y<(posy[tel]+21)) )) van=tel;};
if(veld[van]==1){wittedamzet(handle);return;};
while (pstatus!=1) vq_mouse(handle,&pstatus,&x,&y);
while (pstatus!=0) vq_mouse(handle,&pstatus,&x,&y);
for (tel=1;tel<51;tel++) {if(( ((posx[tel]-21)<x)&&(x<(posx[tel]+21)) )&&
                     ( ((posy[tel]-21)<y)&&(y<(posy[tel]+21)) )) naar=tel;};
controleerwzet(van,naar);
if (ok==0){illegalezet=1;return;};/* goto nogeens;*/
hide_mouse();
verwijderschijf(handle,van);
tekenschijf(handle,naar,0);
veld[van]=4;veld[naar]=0;
mz=zettenteller;
show_mouse();

}

controleerwzet(van,naar) int van,naar;
{ ok=0;
if (lv[van]==naar) ok=1;
if (rv[van]==naar) ok=1;
if (veld[van]!=0) ok=0;
if (veld[naar]!=4) ok=0;

}

static wittedamzet(handle) int handle;
{ int pstatus,x,y,ok;
/*while (pstatus!=1) vq_mouse(handle,&pstatus,&x,&y);*/
/*while (pstatus!=0) vq_mouse(handle,&pstatus,&x,&y);*/
teller=0;bitje=1;
waarismuis(handle);buf[teller]=daar;hulpje=daar;
while (pstatus!=1) 
  {waarismuis(handle);
   if((hulpje!=daar)&&(veld[hulpje]==1))
     {hide_mouse();verwijderschijf(handle,hulpje);veld[buf[0]]=1;show_mouse();};
   if(veld[daar]==leeg)
     {hide_mouse();bitje=1;tekenschijf(handle,daar,1);show_mouse();};
   hulpje=daar;
   if((veld[daar]==4)&&(buf[teller]!=daar))
     {teller++;buf[teller]=daar;if(teller>20) teller=0;}
   vq_mouse(handle,&pstatus,&x,&y);
  };   
waarismuis(handle);teller++;buf[teller]=daar;
goed=0;ok=1;
for(kut=1;kut<teller;kut++)
    {if((buf[kut]==rv[buf[kut-1]])&&(ok==1)){ok=1;goto h;};ok=0;h:;}; 
if(ok==1)goed=1;ok=1;
for(kut=1;kut<teller;kut++)
    {if((buf[kut]==lv[buf[kut-1]])&&(ok==1)){ok=1;goto i;};ok=0;i:;}; 
if(ok==1)goed=1;ok=1;
for(kut=1;kut<teller;kut++)
    {if((buf[kut]==ra[buf[kut-1]])&&(ok==1)){ok=1;goto j;};ok=0;j:;}; 
if(ok==1)goed=1;ok=1;
for(kut=1;kut<teller;kut++)
    {if((buf[kut]==la[buf[kut-1]])&&(ok==1)){ok=1;goto k;};ok=0;k:;}; 
if(ok==1)goed=1;ok=goed;
if(veld[buf[0]]!=1)ok=0;/*if(veld[buf[0]]==veld[buf[teller]])ok=0;*/
/*if(veld[buf[teller]]!=4)ok=0;*/
if (ok==0){illegalezet=1;return;}/* goto klaasje;*/
hide_mouse();
verwijderschijf(handle,buf[0]);tekenwittedam(handle,buf[teller]);
/*for (kut=0;kut<=teller;kut++)tekenwittedam(handle,buf[kut]);*/
veld[buf[teller]]=1;
show_mouse();mz=zettenteller;
}

doezzet(handle,van,naar) int handle,van,naar;
{ int pstatus,x,y,tel;
nogeens: while (pstatus!=1) vq_mouse(handle,&pstatus,&x,&y);
for (tel=1;tel<51;tel++) {if(( ((posx[tel]-21)<x)&&(x<(posx[tel]+21)) )&&
                     ( ((posy[tel]-21)<y)&&(y<(posy[tel]+21)) )) van=tel;};
while (pstatus!=0) vq_mouse(handle,&pstatus,&x,&y);
for (tel=1;tel<51;tel++) {if(( ((posx[tel]-21)<x)&&(x<(posx[tel]+21)) )&&
                     ( ((posy[tel]-21)<y)&&(y<(posy[tel]+21)) )) naar=tel;};
conztroleerzet(van,naar);
if (ok==0) goto nogeens;
hide_mouse();
verwijderschijf(handle,van);
tekenschijf(handle,naar,2);
veld[van]=4;veld[naar]=2;

show_mouse();

}

conztroleerzet(van,naar) int van,naar;
{ ok=0;
if (la[van]==naar) ok=1;
if (ra[van]==naar) ok=1;
if (veld[van]!=2) ok=0;
if (veld[naar]!=4) ok=0;

}

witteslag(handle) int handle;
{ int pstatus,x,y,ok;
scwitslag(handle);
klaasje:;
while (pstatus!=1) vq_mouse(handle,&pstatus,&x,&y);
while (pstatus!=0) vq_mouse(handle,&pstatus,&x,&y);
teller=0;bitje=1;
waarismuis(handle);buf[teller]=daar;hulpje=daar;buf[teller+1]=veld[daar];
while (pstatus!=1) 
  {waarismuis(handle);
   if((hulpje!=daar)&&(veld[hulpje]==buf[1]))
     {hide_mouse();verwijderschijf(handle,hulpje);show_mouse();};bitje=0;
   if(veld[daar]==leeg)
     {hide_mouse();bitje=1;tekenschijf(handle,daar,buf[1]);show_mouse();};
   hulpje=daar;
   if(((veld[daar]==2)||(veld[daar]==3))&&(buf[teller]!=daar))
     {teller+=2;buf[teller]=daar;buf[teller+1]=veld[daar];
      if(teller>20) teller=0;}
   vq_mouse(handle,&pstatus,&x,&y);
  };   
while(pstatus!=0)vq_mouse(handle,&pstatus,&x,&y);
waarismuis(handle);teller+=2;buf[teller]=daar;buf[teller+1]=veld[daar];
if (teller!=mixscore+2)
 {illegalezet=1;hide_mouse();tekenschijf(handle,buf[0],buf[1]);
  verwijderschijf(handle,buf[teller],buf[teller+1]);show_mouse();return;};  
goed=0;
for (kut=1;kut<=aantal;kut++)
    {ok=1;for (cont=0;cont<=(mixscore+2);cont++)
              {if (buf[cont]!=wslag[kut][cont]) ok=0;};
     if(ok==1) goed=1;
    };
if (goed==0){illegalezet=1;return;}/* goto klaasje;*/
hide_mouse();
dowheenslag(handle,mixscore,buf);
show_mouse();mz=zettenteller;
}

zwatteslag(handle) int handle;
{ int pstatus,x,y,ok;
sczwartslag(handle);
bolle:;
while (pstatus!=1) vq_mouse(handle,&pstatus,&x,&y);
teller=0;
waarismuis(handle);buf[teller]=daar;hulpje=daar;
while (pstatus!=0) 
  {waarismuis(handle);
   hulpje=daar;
   if((veld[daar]<2)&&(buf[teller]!=daar))
     {teller+=2;buf[teller]=daar;if(teller>20) teller=0;}
   vq_mouse(handle,&pstatus,&x,&y);
  };   
waarismuis(handle);teller+=2;buf[teller]=daar;
if (teller!=mixscore+2) goto bolle;
goed=0;
for (kut=1;kut<=aantal;kut++)
    {ok=1;for (cont=0;cont<=(mixscore+2);cont+=2)
              {if (buf[cont]!=zslag[kut][cont]) ok=0;};
     if(ok==1) goed=1;
    };
if (goed==0) goto bolle;
hide_mouse();
dozwheenslag(handle,mixscore,buf);
show_mouse();
}


waarismuis(handle) int handle;
{int pstatus,x,y,tel;
jopie:; vq_mouse(handle,&pstatus,&x,&y);
for (tel=1;tel<51;tel++) 
    {if(( ((posx[tel]-21)<x)&&(x<(posx[tel]+21)) )&&
        ( (posy[tel]<(y+21))&&(y<(posy[tel]+21)) )) daar=tel;
    };
}

scanwslaan(hok) int hok;
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

 
scanlvwdamslag(hok) int hok;
{int wijs,helpje,bitje,slachtoffer;
 wijs=lv[hok];
 a: ;   if(veld[wijs]==leeg){wijs=lv[wijs];goto a;};
 if(((veld[wijs]==zwartesteen)||(veld[wijs]==zwartedam))
   &&(veld[lv[wijs]]==leeg)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=lv[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==leeg)
    {scanlawdamslag(wijs);
     if(bitje==0){scanlvwdamslag(wijs);bitje=1;};
     scanrvwdamslag(wijs);
     wijs=lv[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==leeg)
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

 
scanrvwdamslag(hok) int hok;
{int wijs,helpje,bitje,slachtoffer;
 wijs=rv[hok];
 a: ;   if(veld[wijs]==leeg){wijs=rv[wijs];goto a;};
 if(((veld[wijs]==zwartesteen)||(veld[wijs]==zwartedam))
   &&(veld[rv[wijs]]==leeg)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=rv[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==leeg)
    {scanlvwdamslag(wijs);
     if(bitje==0){scanrvwdamslag(wijs);bitje=1;};
     scanrawdamslag(wijs);
     wijs=rv[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==leeg)
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

 
scanlawdamslag(hok) int hok;
{int wijs,helpje,bitje,slachtoffer;
 wijs=la[hok];
 a: ;   if(veld[wijs]==leeg){wijs=la[wijs];goto a;};
 if(((veld[wijs]==zwartesteen)||(veld[wijs]==zwartedam))
   &&(veld[la[wijs]]==leeg)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=la[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==leeg)
    {scanlvwdamslag(wijs);
     if(bitje==0){scanlawdamslag(wijs);bitje=1;};
     scanrawdamslag(wijs);
     wijs=la[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==leeg)
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

 
scanrawdamslag(hok) int hok;
{int wijs,helpje,bitje,slachtoffer;
 wijs=ra[hok];
 a: ;   if(veld[wijs]==leeg){wijs=ra[wijs];goto a;};
 if(((veld[wijs]==zwartesteen)||(veld[wijs]==zwartedam))
   &&(veld[ra[wijs]]==leeg)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=ra[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==leeg)
    {scanlawdamslag(wijs);
     if(bitje==0){scanrawdamslag(wijs);bitje=1;};
     scanrvwdamslag(wijs);
     wijs=ra[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==leeg)
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



scwitslag(handle) int handle;
{int pstatus,x,y;
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


scanzwslaan(hok) int hok;
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

 
scanlvzdamslag(hok) int hok;
{int wijs,helpje,bitje,slachtoffer;
 wijs=lv[hok];
 a: ;   if(veld[wijs]==leeg){wijs=lv[wijs];goto a;};
 if(((veld[wijs]==wittesteen)||(veld[wijs]==wittedam))
   &&(veld[lv[wijs]]==leeg)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=lv[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==leeg)
    {scanlazdamslag(wijs);
     if(bitje==0){scanlvzdamslag(wijs);bitje=1;};
     scanrvzdamslag(wijs);
     wijs=lv[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==leeg)
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

 
scanrvzdamslag(hok) int hok;
{int wijs,helpje,bitje,slachtoffer;
 wijs=rv[hok];
 a: ;   if(veld[wijs]==leeg){wijs=rv[wijs];goto a;};
 if(((veld[wijs]==wittesteen)||(veld[wijs]==wittedam))
   &&(veld[rv[wijs]]==leeg)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=rv[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==leeg)
    {scanlvzdamslag(wijs);
     if(bitje==0){scanrvzdamslag(wijs);bitje=1;};
     scanrazdamslag(wijs);
     wijs=rv[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==leeg)
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

 
scanlazdamslag(hok) int hok;
{int wijs,helpje,bitje,slachtoffer;
 wijs=la[hok];
 a: ;   if(veld[wijs]==leeg){wijs=la[wijs];goto a;};
 if(((veld[wijs]==wittesteen)||(veld[wijs]==wittedam))
   &&(veld[la[wijs]]==leeg)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=la[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==leeg)
    {scanlvzdamslag(wijs);
     if(bitje==0){scanlazdamslag(wijs);bitje=1;};
     scanrazdamslag(wijs);
     wijs=la[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==leeg)
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

 
scanrazdamslag(hok) int hok;
{int wijs,helpje,bitje,slachtoffer;
 wijs=ra[hok];
 a: ;   if(veld[wijs]==leeg){wijs=ra[wijs];goto a;};
 if(((veld[wijs]==wittesteen)||(veld[wijs]==wittedam))
   &&(veld[ra[wijs]]==leeg)&&(slagveld[wijs]==0)) 
  {slachtoffer=veld[wijs];
   score=score+2;array[score]=wijs;slagveld[wijs]=1;wijs=helpje=ra[wijs];
   array[score+1]=slachtoffer;array[score+2]=wijs;bitje=0;
   while(veld[wijs]==leeg)
    {scanlazdamslag(wijs);
     if(bitje==0){scanrazdamslag(wijs);bitje=1;};
     scanrvzdamslag(wijs);
     wijs=ra[wijs];
    };wijs=helpje;
   if(score>=maxscore)
    {if (score>maxscore){aantal=0;maxscore=score;};wijs=helpje;
     while(veld[wijs]==leeg)
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



sczwartslag(handle) int handle;
{int x,y,pstatus,tal;
score=0;maxscore=0;mixscore=0;aantal=0;
for (tel=1;tel<51;tel++){if (veld[tel]==2){veld[tel]=4;array[0]=tel;
                          array[1]=2;scanzwslaan(tel);veld[tel]=2;};
                         if (veld[tel]==3){veld[tel]=4;array[0]=tel;
                          array[1]=3;
                          scanlvzdamslag(tel);scanrvzdamslag(tel);
                          scanlazdamslag(tel);scanrazdamslag(tel);
                          veld[tel]=3;};
                         if (maxscore>mixscore) mixscore=maxscore;                 
                         count=0;};score=maxscore;
if(aantal>5)aantal=5;
}

bestzwart(handle) int handle;
{register zwartaantal,witaantal,zmixscore,wmixscore,teller,cunt,skore[10],
     pipo,tul,zwarray[6][25],warray[6][25],num,zum,wfrom,wto,zfrom,zto;
     


diepte++;if(diepte<2)witgezet=0;
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
eind:;  diepte--;if(diepte<2)witgezet=0;
}

static doezwheenslag(handle,zixscore,zwarray) int handle,zixscore,zwarray[25];
{
/*wacht(5);*/
for(count=0;count<=zixscore;count=count+2)
   {
/*    wacht(5);*/veld[zwarray[count]]=4;
   };

veld[zwarray[zixscore+2]]=zwarray[1];/*wacht(5);*/                       
}

doezwterugslag(handle,zixscore,zwarray) int handle,zixscore,zwarray[25];
{
/*wacht(5);*/
veld[zwarray[zixscore+2]]=4;
for(count=zixscore;count>=0;count=count-2)  
   {
    veld[zwarray[count]]=zwarray[count+1];
   };
/*veld[zwarray[0]]=2;wacht(5);*/
  
}

static doewheenslag(handle,wixscore,warray) int handle,wixscore,warray[25];
{
/*wacht(5);*/
for(count=0;count<=wixscore;count+=2)
   {
 /*   wacht(5);*/veld[warray[count]]=4;
   };

veld[warray[wixscore+2]]=warray[1];/*wacht(5);*/                       
}

doewterugslag(handle,wixscore,warray) int handle,wixscore,warray[25];
{
/*wacht(5);*/
veld[warray[wixscore+2]]=4;
for(count=wixscore;count>=0;count-=2)  
   {
    veld[warray[count]]=warray[count+1];
   };
/*veld[warray[0]]=0;wacht(5);*/
}

static doezwheenzet(handle,zfrom,zto) int handle,zfrom,zto;
{
veld[zfrom]=4;
veld[zto]=2;
if(zto>45)veld[zto]=3;
}
doezwtrugzet(handle,zfrom,zto) int handle,zfrom,zto;
{
veld[zto]=4;
veld[zfrom]=2;
}
static doewheenzet(handle,wfrom,wto) int handle,wfrom,wto;
{
veld[wfrom]=4;
veld[wto]=0;
if(wto<6)veld[wto]=1;
}
doewtrugzet(handle,wfrom,wto) int handle,wfrom,wto;
{
veld[wto]=4;
veld[wfrom]=0;
}





static dozwheenslag(handle,zixscore,zwarray) int handle,zixscore,zwarray[25];
{
wacht(5);
for(count=0;count<=zixscore;count=count+2)
   {verwijderschijf(handle,zwarray[count]);
    veld[zwarray[count]]=4;wacht(5);
   };
tekenschijf(handle,zwarray[zixscore+2],zwarray[1]);
veld[zwarray[zixscore+2]]=zwarray[1];                       
}

dozwterugslag(handle,zixscore,zwarray) int handle,zixscore,zwarray[25];
{
wacht(5);
verwijderschijf(handle,zwarray[zixscore+2]);veld[zwarray[zixscore+2]]=4;
for(count=zixscore;count>=0;count=count-2)  
   {tekenschijf(handle,zwarray[count],zwarray[count+1]);
    veld[zwarray[count]]=zwarray[count+1];wacht(5);
   };
/*tekenschijf(handle,zwarray[0],2);veld[zwarray[0]]=2;*/wacht(5);
  
}

static dowheenslag(handle,wixscore,warray) int handle,wixscore,warray[25];
{

for(count=0;count<=wixscore;count+=2)
   {verwijderschijf(handle,warray[count]);
    veld[warray[count]]=4;
   };
tekenschijf(handle,warray[wixscore+2],warray[1]);
veld[warray[wixscore+2]]=warray[1];                       
}

dowterugslag(handle,wixscore,warray) int handle,wixscore,warray[25];
{
wacht(5);
verwijderschijf(handle,warray[wixscore+2]);veld[warray[wixscore+2]]=4;
for(count=wixscore;count>=0;count-=2)  
   {tekenschijf(handle,warray[count],warray[count+1]);
    veld[warray[count]]=warray[count+1];
   };
/*tekenschijf(handle,warray[0],0);veld[warray[0]]=0;*/wacht(5);
}

static dozwheenzet(handle,zfrom,zto) int handle,zfrom,zto;
{
verwijderschijf(handle,zfrom);veld[zfrom]=4;
tekenschijf(handle,zto,2);veld[zto]=2;
}
dozwtrugzet(handle,zfrom,zto) int handle,zfrom,zto;
{
verwijderschijf(handle,zto);veld[zto]=4;
tekenschijf(handle,zfrom,2);veld[zfrom]=2;
}
static dowheenzet(handle,wfrom,wto) int handle,wfrom,wto;
{
verwijderschijf(handle,wfrom);veld[wfrom]=4;
tekenschijf(handle,wto,0);veld[wto]=0;
}
dowtrugzet(handle,wfrom,wto) int handle,wfrom,wto;
{
verwijderschijf(handle,wto);veld[wto]=4;
tekenschijf(handle,wfrom,0);veld[wfrom]=0;
}

wacht(til) int til;
{
for (tal=0;tal<15000;tal++)
      {for(c=0;c<til;c++) d=1;}
}

initbord(handle) int handle;
{int tel;veld[0]=10;
for (tel=1;tel<21;tel++) {tekenschijf(handle,tel,2);veld[tel]=2;}
for (tel=31;tel<51;tel++){tekenschijf(handle,tel,0);veld[tel]=0;}
for (tel=21;tel<31;tel++){verwijderschijf(handle,tel);veld[tel]=4;}
}

tekenveld(handle) int handle;
{int tel;
for (tel=1;tel<51;tel++)
       {if (veld[tel]==2) tekenschijf(handle,tel,2);
        if (veld[tel]==0) tekenschijf(handle,tel,0);
        if (veld[tel]==4) verwijderschijf(handle,tel);
       }
}

meesteburenzwart(handle) int handle;
{int berta;
 maxvan=bestvan[1];maxnaar=bestnaar[1];/*goto fietsen;*/
/* if(moizet>5){maxvan=bestvan[moizet-4];maxnaar=bestnaar[moizet-4];};*/
 maxaantburen=-1;wisselbitje=-wisselbitje;
 if (wisselbitje==1){anfang=1;ende=moizet;}; 
 if (wisselbitje==-1){anfang=moizet;ende=0;};lopen:;
 maxvan=bestvan[(rendom*moizet)/50];
 maxnaar=bestnaar[(rendom*moizet)/50];
 if((maxvan==0)||(maxnaar==0)||(maxvan==maxnaar))
          {rendom++;if(rendom>50)rendom=0;goto lopen;};

 goto fietsen;
 for(berta=anfang;berta!=ende;berta=berta+wisselbitje)
    {       doezwheenzet(bestvan[berta],bestnaar[berta]);
            aantalburen=0;
            if (veld[la[bestnaar[berta]]]==2) aantalburen++;
            if (veld[ra[bestnaar[berta]]]==2) aantalburen++;
            if (veld[lv[bestnaar[berta]]]==2) aantalburen++;
            if (veld[rv[bestnaar[berta]]]==2) aantalburen++;
            if(aantalburen>maxaantburen)
              {maxaantburen=aantalburen;maxvan=bestvan[berta];
               maxnaar=bestnaar[berta];};
            doezwtrugzet(bestvan[berta],bestnaar[berta]);
    }; fietsen: ;
}


linkermenu(handle) int handle;
{int pstatus,x,y;
hide_mouse();
pxyarray[0]=10;pxyarray[1]=20;pxyarray[2]=90;pxyarray[3]=40;
vsl_type(handle,1);vsf_interior(handle,0);vswr_mode(handle,3);
v_rbox(handle,pxyarray);
pxyarray[1]=60;pxyarray[3]=80;v_rbox(handle,pxyarray);
pxyarray[1]=100;pxyarray[3]=120;v_rbox(handle,pxyarray);
pxyarray[1]=140;pxyarray[3]=160;v_rbox(handle,pxyarray);
pxyarray[1]=180;pxyarray[3]=200;v_rbox(handle,pxyarray);
pxyarray[1]=220;pxyarray[3]=240;v_rbox(handle,pxyarray);
pxyarray[1]=260;pxyarray[3]=280;v_rbox(handle,pxyarray);
pxyarray[1]=340;pxyarray[3]=360;v_rbox(handle,pxyarray);
vswr_mode(handle,3);
v_gtext(handle,33,35,"stop");
v_gtext(handle,17,75,"leegbord");
v_gtext(handle,15,115,"nieuwspel");
v_gtext(handle,17,155,"zetterug");
v_gtext(handle,17,195,"voorruit");
v_gtext(handle,33,235,"edit");
v_gtext(handle,27,275,"level++");
level++;levelomlaag(handle);vswr_mode(handle,3);
v_gtext(handle,27,355,"level--");
vswr_mode(handle,1);
show_mouse();
b:;vq_mouse(handle,&pstatus,&x,&y);
if((pstatus==1)&&(x>10)&&(x<90)&&(y>20)&&(y<40)){klaar=1;goto d;};
if((pstatus==1)&&(x>10)&&(x<90)&&(y>60)&&(y<80))leegbord(handle);
if((pstatus==1)&&(x>10)&&(x<90)&&(y>100)&&(y<120))nieuwspel(handle);
if((pstatus==1)&&(x>10)&&(x<90)&&(y>140)&&(y<160))zetterug(handle);
if((pstatus==1)&&(x>10)&&(x<90)&&(y>180)&&(y<200))voorruit(handle);
if((pstatus==1)&&(x>10)&&(x<90)&&(y>220)&&(y<240))edit(handle);
if((pstatus==1)&&(x>10)&&(x<90)&&(y>260)&&(y<280))levelomhoog(handle);
if((pstatus==1)&&(x>10)&&(x<90)&&(y>340)&&(y<360))levelomlaag(handle);
if(x<100) goto b;
pxyarray[0]=0;pxyarray[1]=0;pxyarray[2]=100;vsf_interior(handle,2);     
pxyarray[3]=400;vsl_type(handle,1);vswr_mode(handle,1);
vsf_style(handle,8);vr_recfl(handle,pxyarray); 
d:;
}



zetterug(handle) int handle;
{hide_mouse();
 if(zettenteller>0){zettenteller=zettenteller-1;wisselbitje=-wisselbitje;};
 for(tel=1;tel<51;tel++)
   {if(veld[tel]==bord[zettenteller][tel]) goto f;
    if(bord[zettenteller][tel]==0){veld[tel]=0;tekenschijf(handle,tel,0);};
    if(bord[zettenteller][tel]==2){veld[tel]=2;tekenschijf(handle,tel,2);};
    if(bord[zettenteller][tel]==1){veld[tel]=1;tekenwittedam(handle,tel);};
    if(bord[zettenteller][tel]==3){veld[tel]=3;tekenzwartedam(handle,tel);};
    if(bord[zettenteller][tel]==4){veld[tel]=4;verwijderschijf(handle,tel);};
    f:;
   };show_mouse();
}

voorruit(handle) int handle;
{hide_mouse();
if(zettenteller<mz){zettenteller=zettenteller+1;wisselbitje=-wisselbitje;};
for(tel=1;tel<51;tel++)
   {if(veld[tel]==bord[zettenteller][tel]) goto l;
    if(bord[zettenteller][tel]==0){veld[tel]=0;tekenschijf(handle,tel,0);};
    if(bord[zettenteller][tel]==2){veld[tel]=2;tekenschijf(handle,tel,2);};
    if(bord[zettenteller][tel]==1){veld[tel]=1;tekenwittedam(handle,tel);};
    if(bord[zettenteller][tel]==3){veld[tel]=3;tekenzwartedam(handle,tel);};
    if(bord[zettenteller][tel]==4){veld[tel]=4;verwijderschijf(handle,tel);};
    l:;
   };show_mouse();
}


nieuwspel(handle) int handle;
{zettenteller=0;hide_mouse();wisselbitje=1;
 for(tel=1;tel<51;tel++)
   {if(veld[tel]==bord[0][tel]) goto g;
    if(bord[0][tel]==0){veld[tel]=0;tekenschijf(handle,tel,0);};
    if(bord[0][tel]==2){veld[tel]=2;tekenschijf(handle,tel,2);};
    if(bord[0][tel]==4){veld[tel]=4;verwijderschijf(handle,tel);};
    g:;
   };show_mouse();
}

edit(handle) int handle;
{int x,y,pstatus;
hide_mouse();
vst_effects(handle,2);vswr_mode(handle,3);v_gtext(handle,33,235,"edit");
show_mouse();vq_mouse(handle,&pstatus,&x,&y);
while(pstatus==1)
vq_mouse(handle,&pstatus,&x,&y);
while(x<100)
  {vq_mouse(handle,&pstatus,&x,&y);if(pstatus==1)goto noot;};
aap:;while(pstatus!=1){vq_mouse(handle,&pstatus,&x,&y);if(x<100)goto noot;};
waarismuis(handle);hide_mouse();
if(veld[daar]==0){veld[daar]=2;tekenschijf(handle,daar,2);goto m;};
if(veld[daar]==1){veld[daar]=3;tekenzwartedam(handle,daar);goto m;};
if(veld[daar]==2){veld[daar]=1;tekenwittedam(handle,daar);goto m;};
if(veld[daar]==3){veld[daar]=4;verwijderschijf(handle,daar);goto m;};
if(veld[daar]==4){veld[daar]=0;tekenschijf(handle,daar,0);goto m;};
m:;while(pstatus==1)vq_mouse(handle,&pstatus,&x,&y);
show_mouse();
if(x<100)goto noot;
goto aap;
noot:; zettenteller++;if(zettenteller>mz)mz=zettenteller;
for(tel=1;tel<51;tel++)bord[zettenteller][tel]=veld[tel];hide_mouse();
mies:;if(zettenteller>105)zettenteller=0;
pxyarray[0]=20;pxyarray[1]=222;pxyarray[2]=80;vsf_interior(handle,2);     
pxyarray[3]=239;vsl_type(handle,1);vswr_mode(handle,1);
vsf_style(handle,8);vr_recfl(handle,pxyarray); 
vst_effects(handle,0);vswr_mode(handle,3);v_gtext(handle,33,235,"edit");
vswr_mode(handle,3);show_mouse();
}


leegbord(handle) int handle;
{for(tel=1;tel<51;tel++)
    {if(veld[tel]!=4)verwijderschijf(handle,tel);};
zettenteller++;if(zettenteller>mz)mz=zettenteller;
for(tel=1;tel<51;tel++)bord[zettenteller][tel]=veld[tel];
if(zettenteller>105)zettenteller=0;
}

levelomhoog(handle) int handle;
{int pstatus,x,y;
 level++;vq_mouse(handle,&pstatus,&x,&y);
 if(level>6)level=6;


pxyarray[0]=20;pxyarray[1]=303;pxyarray[2]=80;vsf_interior(handle,2);     
pxyarray[3]=318;vsl_type(handle,1);vswr_mode(handle,1);
vsf_style(handle,8);vr_recfl(handle,pxyarray); 
vst_effects(handle,0);vswr_mode(handle,3);

 if(level==0)v_gtext(handle,24,315,"level:0"); 
 if(level==1)v_gtext(handle,24,315,"level:1"); 
 if(level==2)v_gtext(handle,24,315,"level:2"); 
 if(level==3)v_gtext(handle,24,315,"level:3"); 
 if(level==4)v_gtext(handle,24,315,"level:4"); 
 if(level==5)v_gtext(handle,24,315,"level:5"); 
 if(level==6)v_gtext(handle,24,315,"level:6");vswr_mode(handle,3); 
while(pstatus==1)vq_mouse(handle,&pstatus,&x,&y);
}
levelomlaag(handle) int handle;
{int pstatus,x,y;
 level--;vq_mouse(handle,&pstatus,&x,&y);
 if(level<1)level=1;
pxyarray[0]=20;pxyarray[1]=303;pxyarray[2]=80;vsf_interior(handle,2);     
pxyarray[3]=318;vsl_type(handle,1);vswr_mode(handle,1);
vsf_style(handle,8);vr_recfl(handle,pxyarray); 
vst_effects(handle,0);vswr_mode(handle,3); 
 if(level==0)v_gtext(handle,24,315,"level:0"); 
 if(level==1)v_gtext(handle,24,315,"level:1"); 
 if(level==2)v_gtext(handle,24,315,"level:2"); 
 if(level==3)v_gtext(handle,24,315,"level:3"); 
 if(level==4)v_gtext(handle,24,315,"level:4"); 
 if(level==5)v_gtext(handle,24,315,"level:5"); 
 if(level==6)v_gtext(handle,24,315,"level:6");vswr_mode(handle,3); 
while(pstatus==1)vq_mouse(handle,&pstatus,&x,&y);
}

main(void)


{   int i,startx,starty,grootte,x,y,zw,tol,van,naar,pstatus;
    zwartesteen=2;wittesteen=0;leeg=4;zwartedam=3;wittedam=1;
    veld[0]=10;wisselbitje=1;
    for(i=0;i<10;work_in[i++]=1);
   work_in[10]=2;
      startx=100;starty=0;grootte=300;
 
  
     appl_init();
         graf_mouse(ARROW,0x0L);
         hide_mouse();
         v_opnvwk(work_in,&handle,work_out);
         v_clrwk(handle);
               
   for(tel=1;tel<12;tel++) 
     { pxyarray[0]=startx;
      pxyarray[1]=starty;
      pxyarray[2]=startx+400;
      pxyarray[3]=starty;
      v_pline(handle,2,pxyarray);  
      starty=starty+40;};  

vsl_udsty(handle,0);
startx=100;starty=0;
x=101;y=1;zw=-1;
 for(tel=1;tel<12;tel++) 
     { pxyarray[0]=startx;
      pxyarray[1]=starty;
      pxyarray[2]=startx;
      pxyarray[3]=starty+400;
      v_pline(handle,2,pxyarray);  
      startx=startx+40;};  

vsf_interior(handle,2);
vsf_style(handle,4);tol=1;
for(tel=1;tel<101;tel++)
    {if (zw==1){pxyarray[0]=x;pxyarray[1]=y;pxyarray[2]=x+38;
     pxyarray[3]=y+38;vr_recfl(handle,pxyarray);};
     if (x>459){x=61;zw=-zw;y=y+40;}; 
     x=x+40;zw=-zw;};pxyarray[0]=0;pxyarray[1]=0;pxyarray[2]=100;     
    pxyarray[3]=400;
    vsf_style(handle,8);vr_recfl(handle,pxyarray); 
    pxyarray[0]=500;pxyarray[2]=640;
    vr_recfl(handle,pxyarray);
    initbord(handle);rendom=1;
    for(tel=1;tel<51;tel++)bord[0][tel]=veld[tel];
    show_mouse();maxarray[1]=4;maxarray[2]=5;zettenteller=0;
    level=3;vq_mouse(handle,&pstatus,&x,&y);klaar=0;graf_mouse(1,7);
while(klaar==0)
   {for(tel=1;tel<51;tel++)bord[zettenteller][tel]=veld[tel];
    g:;vq_mouse(handle,&pstatus,&x,&y);
     while((pstatus!=1)&&(klaar==0))
       {vq_mouse(handle,&pstatus,&x,&y);rendom++;if(rendom>50)rendom=1;
        if(x<100)linkermenu(handle);
    /*    if(x>100) goto c;*/
       };
 c:;if(klaar==1)goto e;
   
    scwitslag(handle);
    if (aantal==0) doewzet(handle,van,naar);
    if (aantal>0)  witteslag(handle);
    if(illegalezet==1){illegalezet=0; goto g;};
    hide_mouse();
    for(tel=1;tel<6;tel++)
      {if(veld[tel]==0)tekenwittedam(handle,tel);}; 
    totzwart=0;totwit=0;slag=1;zet=0;eerstaantal=0;hiscore=-10;
    witgezet=0;diepte=0;testbitje=1;subhigh=-10;
/*    for(tel=1;tel<51;tel++)xbios(32,tel);*/
    zettenteller++;if(zettenteller>mz)mz=zettenteller;
    for(tel=1;tel<=level+1;tel++)
        {zwart[tel]=wit[tel]=0;maxzwart[tel]=0;
  maxwit[tel]=-100;minzwart[tel]=minwit[tel]=100;};
    if(zettenteller>100)zettenteller=1;moizet=0;
    bestzwart(handle);/*tekenzwartedam(handle,(hiscore+1));*/
    
sczwartslag(handle);anzahl=aantal;    
        if(anzahl==0)
          {meesteburenzwart(handle);dozwheenzet(handle,maxvan,maxnaar);};         
        if(anzahl>0)
          {dozwheenslag(handle,bestaantal,bestarray[1]);};
      
 

/*     {meesteburenzwart(handle);dozwheenzet(handle,maxvan,maxnaar);};*/

    moizet=0;moislag=1;
    for(tel=46;tel<51;tel++)
      {if(veld[tel]==2)tekenzwartedam(handle,tel);}; 
   show_mouse();
   };

  e:;

   v_clsvwk(handle);
   appl_exit();return(0);
}
