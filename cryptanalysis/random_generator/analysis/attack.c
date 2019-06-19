#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_SIZE 32
#define BUFF_SIZE 1024

unsigned int holdrand = 0;

static void Srand (unsigned int seed) {
  holdrand = seed;
}

static int Rand (void) {
  return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
}

char* genere_key(void) {
  int i;
  static char key[KEY_SIZE+1];
  const char charset[] = 
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "123456789";
  
  for(i = 0; i < KEY_SIZE; i++) {
    key[i] = charset[Rand() % (sizeof(charset) - 1)];
  }
  key[KEY_SIZE] = '\0';

  return key;
}

void crypt_buffer(unsigned char *buffer, size_t size, char *key) {
  size_t i;
  int j;

  j = 0;
  for(i = 0; i < size; i++) {
    if(j >= KEY_SIZE)
      j = 0;
    buffer[i] ^= key[j];
    j++;
  }
}

void crypt_file(FILE *in, FILE *out) {
  unsigned char buffer[BUFF_SIZE];
  char *key;
  size_t size;

  key = genere_key();

  printf("[+] Using key : %s\n", key);

  do {
    size = fread(buffer, 1, BUFF_SIZE, in);
    crypt_buffer(buffer, size, key);
    fwrite(buffer, 1, size, out);

  }while(size == BUFF_SIZE);  
}

int test_format_b2z(FILE* in){
	int bytes[7];
	
	int i;
	for(i=0;i<7;i++){
		bytes[i]=fgetc(in);
	}
	if(bytes[0]!=66) return 0;
	if(bytes[1]!=90) return 0;
	if(bytes[2]!=104) return 0;
	
	return 1;
}

int main(int argc, char **argv) {
  
  FILE *in, *out, *result;
  char path[128];
  
  snprintf(path, sizeof(path)-1, "%s.crypt", argv[1]);
  
  // 1 decembre 2012 à minuit
  unsigned int seed=1354320000; 
  unsigned int i;
  
  // on fait une attaque brute force sur les 20 premiers jours de décembre 2012
  for(i=seed;i<(1355875200+(86400*20));i++){
	  
	  // on initialise la cle avec chacune des possibilites
	  Srand(i);
	  if((in = fopen(argv[1], "r")) == NULL) {
		perror("[-] fopen (in) ");
		return EXIT_FAILURE;
	  }

	  if((out = fopen(path, "w")) == NULL) {
		perror("[-] fopen (out) ");
		return EXIT_FAILURE;
      }

	  crypt_file(in, out);
	  
	  fclose(in);
	  fclose(out);
	  
	  if((result = fopen(path, "r")) == NULL) {
		perror("[-] fopen (result) ");
		return EXIT_FAILURE;
	  }
	  
	  if(test_format_b2z(result)==1){
		  fclose(result);
		  rename("oDjbNkIoLpaMo.bz2.crypt.crypt", "oDjbNkIoLpaMo.bz2");
		  printf("Le dossier compresse a ete dechiffre sous le nom de oDjbNkIoLpaMo.bz2");
		  return EXIT_SUCCESS;
	  }
	  else fclose(result);
	  
  }
  printf("Impossible de dechiffrer le dossier compresse oDjbNkIoLpaMo.bz2.crypt\n");

  return EXIT_FAILURE;
}
