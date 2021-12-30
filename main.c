#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#define clear() printf("\033[H\033[J")


int sayac = 0;
static char* konum;
int sayici = 0;
int sayici2 = 0;
int pid;
int pidler[5];
char komut[80];
char* komutlar[10];
char yol[100000];


void prompt(){// Prompt yaz
	char hostname[1204] = "";
    	gethostname(hostname, sizeof(hostname));
    	printf(MAG "%s@%s:"RESET CYN "%s prompt> " RESET, getenv("LOGNAME"), hostname, getcwd(konum, 1024));
};

void dizin_degistir(){// Klasorler arasinda gezinir
	chdir(komutlar[1]);
	getcwd(yol, sizeof(yol));
};
void pid_yaz(){ // En sonda olusan 5 pid'i ekrana yazdirmayi saglar
	for(int i=0;i<5;i++){
		printf("%d\n",pidler[i]);
	}
};
void giris_oku(){// Girilen ifadeler ile ilgili islemler
	fgets(komut, sizeof(komut), stdin);
	komut[strlen(komut) - 1] = '\0';
	komutlar[0]=strtok(komut," ");
	for(int i = 1; i < 10; i++){
		komutlar[i]=strtok(NULL," ");
	}
};
void bitir(){// Kabugu sonlandirmaya yarar
	printf("exit\n");
	exit(0);
};

int main()
{
	getcwd(yol, sizeof(yol));// Dosya yolunu dondurmeye yarar
	while(1){
		prompt();
		giris_oku();
		if(strcmp(komutlar[0],"exit") == 0){
			sayici=1;
		}
		else if(strcmp(komutlar[0],"cd") == 0){
			sayici=2;
		}
		else if(strcmp(komutlar[0],"showpid")==0){
			sayici=3;
		}
		else{
			sayici=4;
		}
		
		switch(sayici) // Komut durumuna gore gerekli islemler yapiliyor.
		{
			case 1: 
				bitir();
				break;
			case 2: 
				dizin_degistir();
				break;
			case 3: 
				pid_yaz();
				break;
			case 4:
				pid = fork();
				if(pid==0){
					execvp(komutlar[0],komutlar);
					fprintf(stderr, "Hata: Komut icra edilemiyor.\n");
					return 1;
				}
				else if(pid<0){
					fprintf(stderr, "Hata: Komut icra edilemiyor.\n");
					return 1;
				}
				else{
					waitpid(pid,NULL,0);
				}
				
				pidler[sayac] = pid;
				sayac++;
				if(sayac>=5){ // 5'ten sonra sayaci sifirla.
					sayac=0;
				}
				break;
				
			 
		}
	}
}
