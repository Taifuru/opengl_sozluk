int strcmptrk(char kelime1[20], char kelime2[20])
{
	char duzgunKelime[] = "abc�defg�h�ijklmno�prs�tu�vyzABC�DEFG�HI�JKLMNO�PRS�TUVYZ";
	int bekci = 1;
	int index,index2;
	for(index = 0; index < 20; index++)
	{
		for(index2 = 0; index < 20; index2++)
		{
			if( kelime1[index] > kelime2[index2])
			{
				bekci = 1;
			}
		}
	}
	
	return 0;
}

int kelimeArama(FILE *turkce3, FILE *ingilizce3)
{
	int secim;
	int index = 1;
	
	//Kullanciya sozluk secimi yaptiriliyor
	printf("T�rk�e - �ngilizce S�zl�k ��in --> 1 Giriniz!\n�ngilizce - T�rk�e S�zl�k ��in --> 2 Giriniz!\n��k�� i�in herhangi bir say� giriniz!\nSe�iminiz : ");
	scanf("%d", &secim);
	printf("\n\n");
	
	switch(secim)
	{
		case 1 : // Turkce - Ingilizce Sozluk
			{
				char kelime[20];
				char turkce[20];
				char ingilizce[20];
				
				printf("Aranacak Kelimeyi giriniz : ");
				scanf("%s", kelime);
				
				int i;
				//Kelimenin uzunlu�u al�narak for d�g�s� kuruldu
				for(i=0;i< strlen(kelime); i++)
				{
					//T�rk�e harflerde s�k�nt� ��kt��� i�in b�yle bir ��z�me gidildi
					switch( kelime[i] )
					{
						case -121 :
							{
								kelime[i] = -25;
								break;
							}
						case -89 :
							{
								kelime[i] = -16;
								break;
							}
						case -115 :
							{
								kelime[i] = -3;
								break;
							}
						case -108 :
							{
								kelime[i] = -10;
								break;
							}
						case -97 :
							{
								kelime[i] = -2;
								break;
							}
						case -127 :
							{
								kelime[i] = -4;
								break;
							}
						default :
							break;
					}
					
				}
				
				printf("\n\n");
				
				while(!feof(turkce3) || !feof(ingilizce3))
				{
					fscanf(turkce3, "%s", turkce);
					fscanf(ingilizce3, "%s", ingilizce);
					
					if( strcasecmp(turkce, kelime) == 0 )
					{
						printf("turkce.txt dosyas�nda arad���n�z %s kelimesi %d. kelimedir.\n", turkce, index);
						printf("%s kelimesinin �ngilizce kar��l��� %s kelimesidir.\n\n\n", turkce, ingilizce);
						break;
					}
					
					if(feof(turkce3))
					{
						printf("Arad���n�z Kelime S�zl�kte Bulunmamaktad�r!\n\n");
					}
					
					index++;
				}
				
				return 1;
				break;
			}
		case 2 : // Ingilizce - Turkce Sozluk
			{
				char kelime[20];
				char turkce[20];
				char ingilizce[20];
				
				printf("Aranacak Kelimeyi giriniz : ");
				scanf("%s", &kelime[0]);
				printf("\n\n");
				
				while(!feof(turkce3) || !feof(ingilizce3))
				{
					fscanf(turkce3, "%s", turkce);
					fscanf(ingilizce3, "%s", ingilizce);
					
					if( strcasecmp(ingilizce, kelime) == 0 )
					{
						printf("ingilizce.txt dosyas�nda arad���n�z %s kelimesi %d. kelimedir.\n", ingilizce, index);
						printf("%s kelimesinin T�rk�e kar��l��� %s kelimesidir.\n\n\n", ingilizce, turkce);
						break;
					}
					
					if(feof(ingilizce3))
					{
						printf("Arad���n�z Kelime S�zl�kte Bulunmamaktad�r!\n\n");
					}
					
					index++;
				}
				
				return 1;
				break;
			}
		default :
			{
				printf("��k�� Yapt�n�z!\n\n");
				
				return 0;
				break;
			}
	}
}

int dosyaOkuma(FILE *sozluk1, char siralanacakKelimeler[][20])
{
	//Sozluk.txt icindeki kelimelerin ayrilmasi
	char temp[20];
	int okunanKelimeSayisi = 0;
	int index2 = 0;
	
	while( !feof( sozluk1 ))//Dosya sonuna gelinip gelinmedi�in kontrol�
	{
		//Sadece t�rk�e kelimleri okumak i�in 0 dan ba�lad���m�zdan 2 nin katlar�nda okuma yapt�k
		//b�ylece sadece t�rk�e kelimeleri okuduk
		if( index2 % 2 == 0)
		{
			fscanf(sozluk1, "%s", siralanacakKelimeler[okunanKelimeSayisi]);
			okunanKelimeSayisi++;
		}
		//�ngilizce olanlar� da imlecin ilerlemesi i�in ge�ici bir diziye att�k
		else
		{
			fscanf(sozluk1, "%s", temp);
		}
		
		index2++;
	}
	
	//Ka� kelime okundu�unu return ediyoruz
	return okunanKelimeSayisi;
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
			
			//sozluk.txt den okunan kelime ile T�rk�e kelime kar��la�t�r�l�yor
			if( strcasecmp(turkce, temp) == 0 )
			{
				//E�er kelime bulunduysa bir sonraki kelime �ngilizce olan olaca�� i�in
				//Bir okuma daha yap�l�p ingilizce kelime bulunmu� oluyor
				fscanf(sozluk2, "%s", ingKelime);
			}
		}
		else
		{
			//�mlecin yer de�i�tirmesi i�in ge�ici okuma yap�l�yor
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
		//Kelimeyi turkce.txt ye yaz�yoruz
		fprintf(turkce1, "%s\n", kelimeler[index]);
		
		//sozluk.txt deki imleci en ba�a al�yoruz
		fsetpos(sozluk1, konum);
		
		//T�rk�e kelimenin �evirisi olan �ngilizce kelimeyi ar�yoruz
		indexArama(sozluk1, kelimeler[index], ingKelime);
		
		//Buldu�umuz �ngilizce kelimeyi ingilizce.txt ye yaz�yoruz
		fprintf(ingilizce1, "%s\n", ingKelime);
		
		index++;
	}
}

void degistir(char kelime1[1][20], char kelime2[1][20])
{
	char temp[20];
	
	//2 kelimenin yer de�i�tirmesi
	//1 i ge�ici olana at�yoruz , 2 yi 1 e at�yoruz
	//ge�iciyi 2 ye at�yoruz
	strcpy(temp, kelime1[0]);
	strcpy(kelime1[0], kelime2[0]);
	strcpy(kelime2[0], temp);
}

//  http://tr.wikipedia.org/wiki/S%C4%B1ralama_algoritmas%C4%B1
//  http://tr.wikipedia.org/wiki/Kabarc%C4%B1k_s%C4%B1ralamas%C4%B1
//  http://en.wikipedia.org/wiki/Bubble_sort

void siralama(char kelimeler[][20], int uzunluk)
{
	int bekci = 1;
	int index = 1;
	
	while(bekci >0)
	{
		bekci = 0;
		index = 1;
		
		//E�er ilk kelime daha b�y�kse degistir() ile ikisini yer de�i�tiriyoruz
		if( strcmp( kelimeler[index-1], kelimeler[index] ) > 0 )
		{
			degistir(&kelimeler[index-1], &kelimeler[index]);
			bekci = 1;//E�er de�i�me yap�ld�ysa i�leme devam ediyoruz -- De�i�tirme yap�lmad�ysa bu kelimeden daha
			//b�y��� olmad���n� anl�yoruz ve i�lemi bitiriyoruz
		}
		
		index++;
	}
}
