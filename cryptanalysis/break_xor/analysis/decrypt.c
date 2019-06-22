#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// structure de liste 
struct elem{
	int val;
	struct elem *suiv;
};
typedef struct elem* liste;

// Fonction de création de liste vide
liste creer_liste(){return NULL;}

// Fonction qui teste si une liste est vide
int est_vide(liste l){
	if(l == NULL)
		return 1;
	return 0;
}

// Fonction qui affiche la liste en base 10
void affiche_liste(liste l){
	
	if(est_vide(l))
	{
		printf("La liste est vide \n");
		return;
	}
	while(l)
	{
		printf("%d ",l->val);
		l = l->suiv;
	}
	printf("\n");
}

// Fonction qui affiche la liste en base 16
void affiche_liste_hexa(liste l)
{
	
	if(est_vide(l))
	{
		printf("La liste est vide \n");
		return;
	}
	while(l)
	{
		printf("%x ",l->val);
		l = l->suiv;
	}
	printf("\n");
}

// Fonction qui libère la mémoire d'une liste
liste libere_liste(liste l)
{
	liste tmp;
	while(l)
	{
		tmp = l->suiv;
		free(l);
		l = tmp;
	}
	return NULL;
}

// Fonction qui ajoute un élément au début d'une liste
liste ajoute_elem_debut(liste l,int i)
{
	liste new = malloc(sizeof(struct elem));
	new->val = i;
	new->suiv = l;
	return new;
}

// Longueur de la clé en octets
#define LENGTH_KEY 7

// Longueur du message
#define LENGTH_MESSAGE 91

// Message a dechiffrer
uint8_t encrypted[LENGTH_MESSAGE] = { 0x1a, 0x1d, 0x13, 0x29, 0x21, 0x70, 0x2f, 0x78, 0x1b, 0x1d, 0x36, 0x62, 0x70,
									  0x3a, 0x2d, 0x4f, 0x13, 0x2c, 0x6e, 0x22, 0x2b, 0x2d, 0x1c, 0x01, 0x36, 0x6e,
									  0x31, 0x6e, 0x3c, 0x0a, 0x11, 0x37, 0x27, 0x36, 0x28, 0x2a, 0x0a, 0x00, 0x7f,
									  0x2d, 0x35, 0x6e, 0x2b, 0x06, 0x1f, 0x2f, 0x22, 0x35, 0x6e, 0x20, 0x00, 0x00,
									  0x71, 0x6e, 0x06, 0x2f, 0x34, 0x06, 0x16, 0x3a, 0x6e, 0x31, 0x38, 0x3d, 0x0c,
									  0x52, 0x33, 0x2f, 0x70, 0x2d, 0x34, 0x0a, 0x52, 0x2e, 0x3b, 0x35, 0x6e, 0x2c,
									  0x1a, 0x52, 0x3e, 0x3d, 0x70, 0x3a, 0x2a, 0x00, 0x07, 0x29, 0x2b, 0x70, 0x6f};

// Liste de portions de clés possibles pour chaque octet
liste possible_keys[LENGTH_KEY];

// Fonction qui teste si un caractère est une lettre, un nombre, une ponctuation etc...
int test_ascii(char c){
	if(c==' ') return 1;
	if(c=='!') return 1;
	if(c=='.') return 1;
	if(c==',') return 1;
	if((c>='0')&&(c<=';')) return 1;
	if((c>='?')&&(c<='Z')) return 1;
	if((c>='a')&&(c<='z')) return 1;
	return 0;
}

// Fonction qui teste si une portion de clé key_char est possible à la position rank
int possible_key(uint8_t key_char, int rank){
	
	char decrypted_char;
	int i; int test=1;
	for(i=0;i<LENGTH_MESSAGE;i++){
		if((i%LENGTH_KEY)==rank){
			decrypted_char = encrypted[i]^key_char;
			if(!test_ascii(decrypted_char)) test = 0;
		}
	}
	if(test==1)return 1;
	return 0;
}

// Fonction qui trouve toutes les portions de clés candidates pour chaque octet
void init_candidate_keys(){
	
	int i, key;
	for(i=0;i<LENGTH_KEY;i++){
		possible_keys[i]=creer_liste();
	}
	
	for(i=0;i<LENGTH_KEY;i++){
		for(key=0;key<127;key++){
			if(possible_key(key, i)==1) possible_keys[i]=ajoute_elem_debut(possible_keys[i], key);
		}
	}
}

// Fonction qui réalise l'attaque et génère toutes les clés produisant un message (possible)
void brute_force_attack(){
	
	printf("Attaque par force brute : \n\n");
	
	liste cpy_keys[LENGTH_KEY];
	liste temp_keys[LENGTH_KEY];
	
	int j;
	for(j=0;j<LENGTH_KEY;j++){
		cpy_keys[j]=temp_keys[j]=possible_keys[j];
	}

	int i; char c; int test;
	
	// on teste toutes les possibilités pour chaque portion de clés
	while(cpy_keys[0]){
		cpy_keys[1] = temp_keys[1];
		while(cpy_keys[1]){
			cpy_keys[2] = temp_keys[2];
			while(cpy_keys[2]){
				cpy_keys[3] = temp_keys[3];
				while(cpy_keys[3]){
					
					cpy_keys[4] = temp_keys[4];
					while(cpy_keys[4]){
						
						cpy_keys[5] = temp_keys[5];
						while(cpy_keys[5]){
							
							cpy_keys[6] = temp_keys[6];
							while(cpy_keys[6]){
								test=1;
								
								printf("0x%x%x%x%x%x%x%x : \n", cpy_keys[0]->val, cpy_keys[1]->val, cpy_keys[2]->val, cpy_keys[3]->val , cpy_keys[4]->val, cpy_keys[5]->val, cpy_keys[6]->val);
								
								for(i=0;i<LENGTH_MESSAGE;i++){

									c=encrypted[i]^cpy_keys[i%LENGTH_KEY]->val;
									printf("%c", c);
									if(test_ascii(c)==0) test=0;
									if((i%LENGTH_KEY)==(LENGTH_KEY-1)) printf("\n");
								}
								printf("\n");
								cpy_keys[6] = cpy_keys[6]->suiv;
							}
							cpy_keys[5] = cpy_keys[5]->suiv;
						}
						cpy_keys[4] = cpy_keys[4]->suiv;
					}
					cpy_keys[3] = cpy_keys[3]->suiv;
				}
				cpy_keys[2] = cpy_keys[2]->suiv;
			}
			cpy_keys[1] = cpy_keys[1]->suiv;
		}
		cpy_keys[0] = cpy_keys[0]->suiv;
	}
}

// Fonction qui affiche les différentes sous clés candidates pour chaque octet du mot de passe
void print_candidate_keys(){
	printf("Recherche des clés possibles : \n\n");
	
	int i;
	for(i=0;i<7;i++){
		affiche_liste_hexa(possible_keys[i]);
	}
	printf("\n\n");
}

// Fonction qui libère la mémoire pour les listes de clés candidates
void free_possible_keys(){
	int i;
	for(i=0;i<7;i++){
		possible_keys[i]=libere_liste(possible_keys[i]);
	}
}

// Fonction principale
int main(){
	
	// déduction des clés possibles pour chaque octet du mot de passe
	init_candidate_keys();
	print_candidate_keys();
	
	// recherche des différentes possibilités du message en fonction des mots de passe possibles
	brute_force_attack();
	
	// libération de la mémoire
	free_possible_keys();
	
	return EXIT_SUCCESS;
}
