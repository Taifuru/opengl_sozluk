int strcmptrk(char kelime1[20], char kelime2[20])
{
	char duzgunKelime[] = "abcçdefgðhýijklmnoöprsþtuüvyzABCÇDEFGÐHIÝJKLMNOÖPRSÞTUVYZ";
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
	printf("Türkçe - Ýngilizce Sözlük Ýçin --> 1 Giriniz!\nÝngilizce - Türkçe Sözlük Ýçin --> 2 Giriniz!\nÇýkýþ için herhangi bir sayý giriniz!\nSeçiminiz : ");
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
				//Kelimenin uzunluðu alýnarak for dögüsü kuruldu
				for(i=0;i< strlen(kelime); i++)
				{
					//Türkçe harflerde sýkýntý çýktýðý için böyle bir çözüme gidildi
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
						printf("turkce.txt dosyasýnda aradýðýnýz %s kelimesi %d. kelimedir.\n", turkce, index);
						printf("%s kelimesinin Ýngilizce karþýlýðý %s kelimesidir.\n\n\n", turkce, ingilizce);
						break;
					}
					
					if(feof(turkce3))
					{
						printf("Aradýðýnýz Kelime Sözlükte Bulunmamaktadýr!\n\n");
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
						printf("ingilizce.txt dosyasýnda aradýðýnýz %s kelimesi %d. kelimedir.\n", ingilizce, index);
						printf("%s kelimesinin Türkçe karþýlýðý %s kelimesidir.\n\n\n", ingilizce, turkce);
						break;
					}
					
					if(feof(ingilizce3))
					{
						printf("Aradýðýnýz Kelime Sözlükte Bulunmamaktadýr!\n\n");
					}
					
					index++;
				}
				
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

int dosyaOkuma(FILE *sozluk1, char siralanacakKelimeler[][20])
{
	//Sozluk.txt icindeki kelimelerin ayrilmasi
	char temp[20];
	int okunanKelimeSayisi = 0;
	int index2 = 0;
	
	while( !feof( sozluk1 ))//Dosya sonuna gelinip gelinmediðin kontrolü
	{
		//Sadece türkçe kelimleri okumak için 0 dan baþladýðýmýzdan 2 nin katlarýnda okuma yaptýk
		//böylece sadece türkçe kelimeleri okuduk
		if( index2 % 2 == 0)
		{
			fscanf(sozluk1, "%s", siralanacakKelimeler[okunanKelimeSayisi]);
			okunanKelimeSayisi++;
		}
		//Ýngilizce olanlarý da imlecin ilerlemesi için geçici bir diziye attýk
		else
		{
			fscanf(sozluk1, "%s", temp);
		}
		
		index2++;
	}
	
	//Kaç kelime okunduðunu return ediyoruz
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

void degistir(char kelime1[1][20], char kelime2[1][20])
{
	char temp[20];
	
	//2 kelimenin yer deðiþtirmesi
	//1 i geçici olana atýyoruz , 2 yi 1 e atýyoruz
	//geçiciyi 2 ye atýyoruz
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
		
		//Eðer ilk kelime daha büyükse degistir() ile ikisini yer deðiþtiriyoruz
		if( strcmp( kelimeler[index-1], kelimeler[index] ) > 0 )
		{
			degistir(&kelimeler[index-1], &kelimeler[index]);
			bekci = 1;//Eðer deðiþme yapýldýysa iþleme devam ediyoruz -- Deðiþtirme yapýlmadýysa bu kelimeden daha
			//büyüðü olmadýðýný anlýyoruz ve iþlemi bitiriyoruz
		}
		
		index++;
	}
}
