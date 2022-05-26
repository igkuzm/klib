/**
 * File              : translit.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 07.03.2022
 * Last Modified Date: 26.05.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */


#ifndef k_lib_translit_h__
#define k_lib_translit_h__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>	

static char * k_lib_translit(const char* string);

#ifndef translit
#define translit(string)	\
({	\
	char* ___c = k_lib_translit(string);\
	___c;	\
})
#endif

char*
k_lib_translit(const char *string)
{
	int i;
	int k=0;
	char *translit=(char *)malloc(BUFSIZ*sizeof(char));
	if (translit == NULL) {
		perror("translit malloc");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < strlen(string); ++i) {
		char chara[3];
		sprintf(chara, "%c%c", string[i], string[i+1]);

		if (strcmp(chara, "а")==0){
				translit[k]='a';
				k++;
				i++;			
		}		
		else if (strcmp(chara, "А")==0){
				translit[k]='A';
				k++;
				i++;			
		}				
		else if (strcmp(chara, "б")==0){
				translit[k]='b';
				k++;
				i++;			
		}				
		else if (strcmp(chara, "Б")==0){
				translit[k]='B';
				k++;
				i++;			
		}				
		else if (strcmp(chara, "в")==0){
				translit[k]='v';
				k++;
				i++;			
		}				
		else if ((strcmp(chara, "В")==0)) {
				translit[k]='V';
				k++;			
				i++;
		}		
		else if (strcmp(chara, "г")==0){
				translit[k]='g';
				k++;
				i++;			
		}				
		else if (strcmp(chara, "Г")==0){
				translit[k]='G';
				k++;
				i++;			
		}				
		else if (strcmp(chara, "д")==0){
				translit[k]='d';
				k++;
				i++;			
		}				
		else if (strcmp(chara, "Д")==0){
				translit[k]='D';
				k++;
				i++;			
		}				
		else if (strcmp(chara, "е")==0){
				translit[k]='e';
				k++;				
				i++;			
		}				
		else if (strcmp(chara, "Е")==0){
				translit[k]='E';
				k++;				
				i++;			
		}				
		else if (strcmp(chara, "ё")==0){
				translit[k]='y';
				k++;				
				translit[k]='o';
				k++;								
				i++;			
		}				
		else if (strcmp(chara, "Ё")==0){
				translit[k]='Y';
				k++;				
				translit[k]='o';
				k++;								
				i++;			
		}				
		else if (strcmp(chara, "ж")==0){
				translit[k]='z';
				k++;				
				translit[k]='h';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "Ж")==0){
				translit[k]='Z';
				k++;				
				translit[k]='h';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "з")==0){
				translit[k]='z';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "З")==0){
				translit[k]='z';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "и")==0){
				translit[k]='i';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "И")==0){
				translit[k]='I';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "й")==0){
				translit[k]='y';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "Й")==0){
				translit[k]='Y';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "к")==0){
				translit[k]='k';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "К")==0){
				translit[k]='K';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "л")==0){
				translit[k]='l';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "Л")==0){
				translit[k]='L';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "м")==0){
				translit[k]='m';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "М")==0){
				translit[k]='M';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "н")==0){
				translit[k]='n';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "Н")==0){
				translit[k]='N';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "о")==0){
				translit[k]='o';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "О")==0){
				translit[k]='O';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "п")==0){
				translit[k]='p';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "П")==0){
				translit[k]='P';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "р")==0){
				translit[k]='r';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "Р")==0){
				translit[k]='R';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "с")==0){
				translit[k]='s';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "С")==0){
				translit[k]='S';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "т")==0){
				translit[k]='t';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "Т")==0){
				translit[k]='T';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "у")==0){
				translit[k]='u';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "У")==0){
				translit[k]='U';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "ф")==0){
				translit[k]='f';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "Ф")==0){
				translit[k]='F';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "х")==0){
				translit[k]='k';
				k++;								
				translit[k]='h';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "Х")==0){
				translit[k]='K';
				k++;								
				translit[k]='h';
				k++;								
				i++;			
		}						
		else if (strcmp(chara, "ц")==0){
				translit[k]='t';
				k++;
				translit[k]='s';
				k++;
				i++;
		} 
		else if (strcmp(chara, "Ц")==0){
				translit[k]='T';
				k++;
				translit[k]='s';
				k++;
				i++;
		} 
		else if (strcmp(chara, "ч")==0){
				translit[k]='t';
				k++;
				translit[k]='c';
				k++;
				translit[k]='h';
				k++;
				i++;
		} 
		else if (strcmp(chara, "Ч")==0){
				translit[k]='T';
				k++;
				translit[k]='c';
				k++;
				translit[k]='h';
				k++;
				i++;
		} 
		else if (strcmp(chara, "ш")==0){
				translit[k]='s';
				k++;
				translit[k]='h';
				k++;
				i++;
		} 
		else if (strcmp(chara, "Ш")==0){
				translit[k]='S';
				k++;
				translit[k]='h';
				k++;
				i++;
		} 
		else if (strcmp(chara, "щ")==0){
				translit[k]='t';
				k++;
				translit[k]='s';
				k++;
				translit[k]='h';
				k++;
				i++;
		} 
		else if (strcmp(chara, "Щ")==0){
				translit[k]='T';
				k++;
				translit[k]='s';
				k++;
				translit[k]='h';
				k++;
				i++;
		} 
		else if (strcmp(chara, "ъ")==0){
				translit[k]='"';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "Ъ")==0){
				translit[k]='"';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "ы")==0){
				translit[k]='y';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "Ы")==0){
				translit[k]='Y';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "ь")==0){
				translit[k]='\'';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "Ь")==0){
				translit[k]='\'';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "э")==0){
				translit[k]='e';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "Э")==0){
				translit[k]='E';
				k++;								
				i++;			
		}								
		else if (strcmp(chara, "ю")==0){
				translit[k]='y';
				k++;				
				translit[k]='u';
				k++;								
				i++;			
		}				
		else if (strcmp(chara, "Ю")==0){
				translit[k]='Y';
				k++;				
				translit[k]='u';
				k++;								
				i++;			
		}				
		
		else if (strcmp(chara, "я")==0){
				translit[k]='y';
				k++;				
				translit[k]='a';
				k++;								
				i++;			
		}				
		else if (strcmp(chara, "Я")==0){
				translit[k]='Y';
				k++;				
				translit[k]='a';
				k++;								
				i++;			
		}				
		else if (strcmp(chara, "№")==0){
				translit[k]='#';
				k++;				
				i++;			
		}						
		else {
			translit[k]=string[i];
			k++;
		}

	}

	return translit;
}


#ifdef __cplusplus
}
#endif

#endif //k_lib_translit_h__
