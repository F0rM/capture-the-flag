# Break Xor

## Description 

Le but est de déchiffrer les données suivantes du fichier *data/encrypted.txt*: 

```hex
1a1d132921702f781b1d366270
3a2d4f132c6e222b2d1c01366e
316e3c0a11372736282a0a007f
2d356e2b061f2f22356e200000
716e062f3406163a6e31383d0c
52332f702d340a522e3b356e2c
1a523e3d703a2a0007292b706f
```
On sait que le mot de passe utilisé est de 7 octets et le chiffrement utilisé 
est un XOR.

## Analyse

- On sait que le mot de passe utilisé est une chaîne de caractères. Et que le 
message déchiffré est également une suite de caractères (pas juste des données brutes
encodées dans un autre format) : cela réduit les possibilités pour 
le mot de passe.

- Pour chaque octet du mot de passe, on va énumérer les possibilités 
et insérer une solution possible pour la portion de clé si le XOR entre la 
portion de clé et tous les octets affectés par cette dernière donne un 
caractère possible pour le message déchiffré. 

- On obtient donc : 

```txt
1er octet  : 59 58 4e 
2eme octet : 6f 6e 
3eme octet : 72 65 
4eme octet : 5f 
5eme octet : 4e 
6eme octet : 50 47 46 45 44 43 41 40 
7eme octet : 5f 5e 5d 5c 5b 59 4e 
```
Cela nous donne donc `3x2x2x1x1x8x7 = 672 possibilités de messages possibles`. 
Pour trouver le bon, l'attaquant les regarde à la main et on trouve : 

```txt
0x586f725f4e504e : 
Bravo a
 toi, t
u as re
ussi a 
dechiff
rer ce 
simple 
xor. Va
lide av
ec la c
le que 
tu as t
rouve !
``` 

Le message sans découpage est `Bravo a toi, tu as reussi a dechiffrer ce simple xor. Valide avec la cle que tu as trouve !`
La clé est `0x586f725f4e504e` et en ascii vaut `Xor_NPN`.
