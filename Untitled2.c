#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "include/glut.h"

void fareButon(int , int , int , int );

int tik = 1;
int tik2= 1;

float butonx=50, butony=50;
//klavyeden alýnan string
char alinanKelime[2][20] ;
char turkceKelime[20] = "";
char ingKelime[20] = "";
char okYonu[3] = "-->";

int kelimeArama(FILE *, FILE *, char [20], char [20], char [20], int , int *);
void siralama(char [][20], int);
int dosyaOkuma(FILE *, char [][20]);
void dosyaYazma(FILE *, FILE *, FILE *, char [][20], fpos_t *, int );
void degistir(char [], char []);

char siralanacakKelimeler[100][20];

char dialog_tr_1[] = "turkce.txt dosyasinda aradiginiz '%s' kelimesi %d. kelimedir.";
char dialog_tr_2[] = "'%s' kelimesinin Ingilizce karsiligi '%s' kelimesidir.";

char dialog_ing_1[] = "ingilizce.txt dosyasinda aradiginiz '%s' kelimesi %d. kelimedir.";
char dialog_ing_2[] = "'%s' kelimesinin Turkce karsiligi '%s' kelimesidir.";

char hata_mesaji[] = "Aradiginiz Kelime Sozlukte Bulunmamaktadir!";

int arananKelimeninYeri = 12;
int alinanKelimeUzunlugu=0;
int durum = 1;
int durum1 = 1;

int aramaDurumu = 3;

// default font
void *font = GLUT_BITMAP_HELVETICA_18;
void *font1 = GLUT_BITMAP_HELVETICA_12;

#define MAX_KELIME_SAYISI 1000

#define INT_GLUT_BITMAP_HELVETICA_12  6
#define INT_GLUT_BITMAP_HELVETICA_18  7

//Dosyalardaki imleçlerinin konumlarýný tutmak için deðiþkenler tanýmlandý
//fpos_t konum, konumTr, konumIng;
fpos_t konum, konumTr, konumIng;

//Dosya degiskenlerinin tanimlanmasi
//FILE *sozluk, *turkce, *ingilizce;
FILE *sozluk, *turkce, *ingilizce;

void renderBitmapString(float x, float y, float z, void *font, char *string)
{
  char *c;
  
  glRasterPos3f(x, y,z);
  
  for (c=string; *c != '\0'; c++)
  {
    glutBitmapCharacter(font, *c);
  }
}

void renderBitmapString1(float x, float y, float z, void *font, char *string, char *kelime1, char *kelime2, int yer)
{
  char *karakter;
  char *kelime;
  char sayi[3];
  int index = 0;
  
  glRasterPos3f(x, y,z);
  
  for (karakter=string; *karakter != '\0'; karakter++)
  {
  	if (*karakter == '%' && *(karakter + 1) == 's')
  	{
  		//if else yapisinin kisasi
  		(index % 2 == 0) ? (kelime = kelime1) : (kelime = kelime2);
  		
  		for( ; *kelime != '\0'; kelime++)
  		{
		  glutBitmapCharacter(font, *kelime);
  		}
  		// %s iki karakter olduðu için iki adres atliyoruz
  		karakter +=2;
  		index++;
  	}
  	else if(*karakter == '%' && *(karakter + 1) == 'd')
  	{
  		sayi[0] = '0' + yer / 100;
  		sayi[1] = '0' + ( yer - ( (yer / 100) * 100) ) / 10;
  		sayi[2] = '0' + yer % 10;
  		
  		if(sayi[0]== '0')
  		{
  			if( sayi[1]=='0')
  			{
  				glutBitmapCharacter(font, sayi[2]);
  			}
  			else
  			{
  				glutBitmapCharacter(font, sayi[1]);
  				glutBitmapCharacter(font, sayi[2]);
  			}
  		}
  		else
  		{
  			glutBitmapCharacter(font, sayi[0]);
  			glutBitmapCharacter(font, sayi[1]);
  			glutBitmapCharacter(font, sayi[2]);
  		}
  		//%d iki karakter olduðu için iki adres atliyoruz
  		karakter +=2;
  	}
    glutBitmapCharacter(font, *karakter);
  }
}

void ekranAyarlari()
{
    glClearColor(0, 0, 0, 0); // moved this line to be in the init function
    glDisable(GL_DEPTH_TEST);

    // next four lines are new
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 640-1, 480-1, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void ekranaCizim(void)
{
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	
	//butonlarýn cizimi
	
	//turkce buton
	glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.9f);
        glVertex2f(butonx, butony);
        glVertex2f(butonx+100, butony);
        glVertex2f(butonx+100, butony+50);
        glVertex2f(butonx, butony+50);
    glEnd();
	
	//Ingilizce buton
	glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.9f);
        glVertex2f(butonx+200, butony);
        glVertex2f(butonx+300, butony);
        glVertex2f(butonx+300, butony+50);
        glVertex2f(butonx+200, butony+50);
    glEnd();
    
    //bul butonu
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.9f);
        glVertex2f(butonx, butony+110);
        glVertex2f(butonx+50, butony+110);
        glVertex2f(butonx+50, butony+140);
        glVertex2f(butonx, butony+140);
    glEnd();
    
    //Okyönü butonu
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.9f);
        glVertex2f(butonx+125, butony+10);
        glVertex2f(butonx+175, butony+10);
        glVertex2f(butonx+175, butony+40);
        glVertex2f(butonx+125, butony+40);
    glEnd();
    
    //Yazi kutusu
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.9f);
        glVertex2f(butonx+165, butony+65);
        glVertex2f(butonx+300, butony+65);
        glVertex2f(butonx+300, butony+80);
        glVertex2f(butonx+165, butony+80);
    glEnd();
	
	//Yazilari öne getirmek için
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
		glColor3f(1.0f, 0.f, 0.f);
		renderBitmapString(butonx+10, butony+30, 0.0f, (void *)font ,"Turkce");
		renderBitmapString(butonx+210, butony+30, 0.0f, (void *)font ,"Ingilizce");
		renderBitmapString(butonx+135, butony+30, 0.0f, (void *)font , okYonu);
		renderBitmapString(butonx+10, butony+75, 0.0f, (void *)font1 ,"Aranacak Kelimeyi Giriniz : ");
		
		glColor3f(1.0f, 0.f, 0.f);
		renderBitmapString(butonx+170, butony+75, 0.0f, (void *)font1 , alinanKelime[0]);
		
		glColor3f(0.0f, 0.f, 1.f);
		renderBitmapString(butonx+10, butony+130, 0.0f, (void *)font , "Bul");
		
		glColor3f(1.0f, 1.f, 1.f);
		
		if(aramaDurumu == 0 && alinanKelime[0][0] != '\0')
		{
			//Turkce - Ing arama sonucunu ekrana yazdirma
			renderBitmapString1(butonx+60, butony+120, 0.0f, (void *)font1 , dialog_tr_1, turkceKelime, ingKelime, arananKelimeninYeri);
			renderBitmapString1(butonx+60, butony+135, 0.0f, (void *)font1 , dialog_tr_2, turkceKelime, ingKelime, arananKelimeninYeri);
		}
		else if(aramaDurumu == 1 && alinanKelime[0][0] != '\0')
		{
			//Ing - Turkce arama sonucunu ekrana yazdirma
			renderBitmapString1(butonx+60, butony+120, 0.0f, (void *)font1 , dialog_ing_1, ingKelime, turkceKelime, arananKelimeninYeri);
			renderBitmapString1(butonx+60, butony+135, 0.0f, (void *)font1 , dialog_ing_2, ingKelime, turkceKelime, arananKelimeninYeri);
		}
		else if(aramaDurumu == 2 && alinanKelime[0][0] != '\0')
		{
			renderBitmapString(butonx+60, butony+120, 0.0f, (void *)font1 ,hata_mesaji);
		}
		else if(aramaDurumu == 3)
		{
			
		}
		
	glPopMatrix();
	
    glutSwapBuffers();
} 

void klavyeIsleme(unsigned char key, int xx, int yy)
{ 	
	//esc basilinca programdan cikiyor
	if (key == 27)
		exit(0);
	
	alinanKelimeUzunlugu = strlen(alinanKelime[0]);
	
	//Silme islemi -- Backspace
	if(key == 8)
	{
		alinanKelimeUzunlugu -= 1;
		alinanKelime[0][alinanKelimeUzunlugu] = 0;
		alinanKelime[1][alinanKelimeUzunlugu] = 0;
	}
	else
	{
		alinanKelime[1][alinanKelimeUzunlugu] = key;
		
		switch(key)
		{
			case 253 : { key = 105; break; }
			case 246 : { key = 11; break; }
			case 240 : { key = 103; break; }
			case 252 : { key = 117; break; }
			case 254 : { key = 115; break; }
			case 231 : { key = 99; break; }
		}
		alinanKelime[0][alinanKelimeUzunlugu] = key;
    	alinanKelimeUzunlugu+=1;
	}
	aramaDurumu = 3;
}

void mouseMove(int x, int y) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{
	//Degiskenlere degerleri atilarak islemin yapilip yapilmadigiyla alakali kontroller
	//ve kullaniciya geri bildirim
	if( (sozluk = fopen("sozluk.txt", "r") ) == NULL)
	{
		printf("sozluk.txt Açýlamadý! \n");
		getch();
		exit(0);
	}
	else if( ( turkce = fopen("turkce.txt", "w") ) == NULL)
	{
		printf("turkce.txt Açýlamadý! \n");
		getch();
		exit(0);
	}
	else if ( ( ingilizce = fopen("ingilizce.txt", "w") ) == NULL )
	{
		printf("ingilizce.txt Açýlamadý! \n");
		getch();
		exit(0);
	}
	
	//sozluk.txt deki imleç konumu daha sonra kullanýlmak üzere alýnýyor
	fgetpos(sozluk, &konum);
	
	int uzunluk = dosyaOkuma(sozluk, siralanacakKelimeler);
	
	siralama(siralanacakKelimeler, uzunluk);
	
	dosyaYazma(sozluk,  turkce,  ingilizce, siralanacakKelimeler,&konum, uzunluk);
	
	//Dosyalar yazma modunda açýlmýþtý. O nedenle kapatýp tekrar okuma modunda açýyoruz
	fclose(turkce);
	fclose(ingilizce);
	
	if( ( turkce = fopen("turkce.txt", "r") ) == NULL)
	{
		printf("turkce.txt Açýlamadý! \n");
		exit(0);
	}
	else if ( ( ingilizce = fopen("ingilizce.txt", "r") ) == NULL )
	{
		printf("ingilizce.txt Açýlamadý! \n");
		exit(0);
	}
	
	//Dosyalardaki konumlar daha sonra tekrar kullanýlmak üzere alýnýyor
	fgetpos(ingilizce, &konumIng);
	fgetpos(turkce, &konumTr);
	
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Turkce-Ingilizce  /  Ingilizce-Turkce  Sozluk");
	
	ekranAyarlari();
	glEnable(GL_DEPTH_BITS);
	
	// register callbacks
	glutDisplayFunc(ekranaCizim);
	glutIdleFunc(ekranaCizim);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(klavyeIsleme);

	// here are the two new functions
	glutMouseFunc(fareButon);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	fclose(turkce);
	fclose(ingilizce);
	fclose(sozluk);
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

int dosyaOkuma(FILE *sozluk1, char siralanacakKelimeler[][20])
{
	//Sozluk.txt icindeki kelimelerin ayrilmasi
	char temp[20];
	int okunanKelimeSayisi = 0;
	int index = 0;
	
	while( !feof( sozluk1 ))//Dosya sonuna gelinip gelinmediðin kontrolü
	{
		//Sadece türkçe kelimleri okumak için 0 dan baþladýðýmýzdan 2 nin katlarýnda okuma yaptýk
		//böylece sadece türkçe kelimeleri okuduk
		if( index % 2 == 0)
		{
			fscanf(sozluk1, "%s", siralanacakKelimeler[okunanKelimeSayisi]);
			okunanKelimeSayisi++;
		}
		//Ýngilizce olanlarý da imlecin ilerlemesi için geçici bir diziye attýk
		else
		{
			fscanf(sozluk1, "%s", temp);
		}
		
		index++;
	}
	
	//Kaç kelime okunduðunu return ediyoruz
	return okunanKelimeSayisi;
}

void degistir(char kelime1[20], char kelime2[20])
{
	char temp[20];
	
	//2 kelimenin yer deðiþtirmesi
	//1 i geçici olana atýyoruz , 2 yi 1 e atýyoruz
	//geçiciyi 2 ye atýyoruz
	strcpy(temp, kelime1);
	strcpy(kelime1, kelime2);
	strcpy(kelime2, temp);
}

//  http://tr.wikipedia.org/wiki/S%C4%B1ralama_algoritmas%C4%B1
//  http://tr.wikipedia.org/wiki/Kabarc%C4%B1k_s%C4%B1ralamas%C4%B1
//  http://en.wikipedia.org/wiki/Bubble_sort

void siralama(char kelimeler[][20], int uzunluk1)
{
	int index1;
	int index2;
	
	for(index1 = 0; index1 < uzunluk1; index1++)
	{
		for(index2  = 0; index2 < uzunluk1; index2++)
		{
			if( strcasecmp( kelimeler[index1], kelimeler[index2]) < 0)
			{
				degistir(kelimeler[index1], kelimeler[index2]);
			}
		}
	}
}

void indexArama(FILE *sozluk2, char turkce[20], char ingKelime[20])
{
	char temp[20];
	int index = 0;
	
	while( !feof( sozluk2 ) )
	{	
		if( index % 2 == 0 )
		{
			fscanf(sozluk2, "%s", temp);
			
			//sozluk.txt den okunan kelime ile Türkçe kelime karþýlaþtýrýlýyor
			if( strcasecmp(turkce, temp) == 0 )
			{
				//Eðer kelime bulunduysa bir sonraki kelime Ýngilizce olan olacaðý için
				//Bir okuma daha yapýlýp ingilizce kelime bulunmuþ oluyor
				fscanf(sozluk2, "%s", ingKelime);
			}
		}
		else
		{
			//Ýmlecin yer deðiþtirmesi için geçici okuma yapýlýyor
			fscanf(sozluk2, "%s", &temp);
		}
		
	}
}

void dosyaYazma(FILE *sozluk1, FILE *turkce1, FILE *ingilizce1, char kelimeler[][20], fpos_t *konum, int uzunluk)
{
	char ingKelime[20];
	int index = 0;
	
	while(index < uzunluk)
	{
		//Kelimeyi turkce.txt ye yazýyoruz
		fprintf(turkce1, "%s\n", kelimeler[index]);
		
		//sozluk.txt deki imleci en baþa alýyoruz
		fsetpos(sozluk1, konum);
		
		//Türkçe kelimenin çevirisi olan Ýngilizce kelimeyi arýyoruz
		indexArama(sozluk1, kelimeler[index], ingKelime);
		
		//Bulduðumuz Ýngilizce kelimeyi ingilizce.txt ye yazýyoruz
		fprintf(ingilizce1, "%s\n", ingKelime);
		
		index++;
	}
}

int kelimeArama(FILE *turkce3, FILE *ingilizce3, char trKelime[20], char ingKelime[20], char aranan[20], int secim, int *yer)
{
	int index = 1;
	*yer = 0;
	int index2 = 0;
	
	switch(secim)
	{
		case 1 : // Turkce - Ingilizce Sozluk
			{	
				printf("\n\nasdasd");
				
				while(!feof(turkce3) || !feof(ingilizce3))
				{
					fscanf(turkce3, "%s", trKelime);
					fscanf(ingilizce3, "%s", ingKelime);
					
					if( strcasecmp(trKelime, aranan) == 0 )
					{
						for(index2 = 0; index2 < strlen(trKelime); index2++)
						{
							switch(( unsigned char )trKelime[index2])
							{
								case 253 : { trKelime[index2] = 105; break; }
								case 246 : { trKelime[index2] = 11; break; }
								case 240 : { trKelime[index2] = 103; break; }
								case 252 : { trKelime[index2] = 117; break; }
								case 254 : { trKelime[index2] = 115; break; }
								case 231 : { trKelime[index2] = 99; break; }
							}
						}
						
						printf("turkce.txt dosyasýnda aradýðýnýz %s kelimesi %d. kelimedir.\n", trKelime, index);
						printf("%s kelimesinin Ýngilizce karþýlýðý %s kelimesidir.\n\n\n", trKelime, ingKelime);
						break;
					}
					
					if(feof(turkce3))
					{
						printf("Aradýðýnýz Kelime Sözlükte Bulunmamaktadýr!\n\n");
						return 2;
					}
					
					index++;
				}
				
				*yer = index;
				return 0;
				break;
			}
		case 2 : // Ingilizce - Turkce Sozluk
			{
				while(!feof(turkce3) || !feof(ingilizce3))
				{
					fscanf(turkce3, "%s", trKelime);
					fscanf(ingilizce3, "%s", ingKelime);
					
					if( strcasecmp(ingKelime, aranan) == 0 )
					{
						for(index2 = 0; index2 < strlen(trKelime); index2++)
						{
							switch(( unsigned char )trKelime[index2])
							{
								case 253 : { trKelime[index2] = 105; break; }
								case 246 : { trKelime[index2] = 11; break; }
								case 240 : { trKelime[index2] = 103; break; }
								case 252 : { trKelime[index2] = 117; break; }
								case 254 : { trKelime[index2] = 115; break; }
								case 231 : { trKelime[index2] = 99; break; }
							}
						}
						
						printf("ingilizce.txt dosyasýnda aradýðýnýz %s kelimesi %d. kelimedir.\n", ingKelime, index);
						printf("%s kelimesinin Türkçe karþýlýðý %s kelimesidir.\n\n\n", ingKelime, trKelime);
						break;
					}
					
					if(feof(ingilizce3))
					{
						printf("Aradýðýnýz Kelime Sözlükte Bulunmamaktadýr!\n\n");
						return 2;
					}
					
					index++;
				}
				
				*yer = index;
				return 1;
				break;
			}
		default :
			{
				printf("Çýkýþ Yaptýnýz!\n\n");
				
				return 0;
				break;
			}
	}
}

void fareButon(int button, int state, int x, int y)
{
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON)
	{
		//Ok yönü butonu
		if( (x > butonx + 125) && ( x < butonx + 175 ) && ( y > butony+10 ) && ( y < butony+40 ) && tik ==1)
		{
			if(durum == 1 )
			{
				strcpy(okYonu,"<--");
				durum = 0;
				aramaDurumu = 3;
			}
			else if(durum == 0)
			{
				strcpy(okYonu,"-->");
				durum = 1;
				aramaDurumu = 3;
			}
			tik = 0;
		}
		else
		{
			tik=1;
		}
		
		//Bul butonu
		if( (x > butonx + 0) && ( x < butonx + 50 ) && ( y > butony+110 ) && ( y < butony+140 ) && tik2==1)
		{
			if(durum == 1)//Turkce-Ingilizce
			{
				if(alinanKelime[0] != '\0')
				{
					fsetpos(turkce,&konumTr);
					fsetpos(ingilizce,&konumIng);
					aramaDurumu = kelimeArama(turkce,ingilizce, turkceKelime, ingKelime, alinanKelime[1], 1, &arananKelimeninYeri);
				}
			}
			else if( durum == 0)//Ingilizce - Turkce
			{
				if(alinanKelime[0] != '\0')
				{
					fsetpos(turkce,&konumTr);
					fsetpos(ingilizce,&konumIng);
					aramaDurumu = kelimeArama(turkce,ingilizce, turkceKelime, ingKelime, alinanKelime[1], 2, &arananKelimeninYeri);
				}
				
			}
			tik2 = 0;
		}
		else
		{
			tik2 = 1;
		}
		
	}
}
