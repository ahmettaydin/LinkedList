#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/*Karakter dizisi tipinde kelime bilgisi tutulur. Bu dizi izin verilen formatta char w[100] kullanilmistir
Tamsayi tipinde adet bilgisi tutulur
Pointer tipinde sonraki dugumun adresi tutulur*/
struct n {
    char  word[50];
    int piece;
    struct n * next;
};
typedef struct n node;

node * readFile(FILE *, node *);              /*Dosya icerisinden metin okuma islemi yapar*/
int isAdded(node *, char []);                 /*Dosyadan okunan kelimenin daha önce bagli listeye eklenip eklenmedigini kontrol eder*/
int countWords(FILE *, char []);              /*Metin dosyasindan okunan kelimenin dosya içerisinde kaç kez yer aldigini hesaplar*/
node * add(node *, char [], int);             /*Kelime okunup adet sayisi hesaplandiktan sonra hangi bagli liste fonksiyonunun kullanilacagini secer*/
node * addToBeginning(node *, char [], int);  /*Bagli listede basa ekleme islemi yapar*/
void addBetween(node *, char [], int);        /*Bagli listede araya ekleme islemi yapar*/
void addToEnd(node *, char [], int);          /*Bagli listede sona ekleme islemi yapar*/
void printList(node *);                       /*Oluþturulan bagli listenin son halini ekrana yazdirir*/

int main()
{
    FILE * file = NULL;
    node * root = NULL;

    setlocale(LC_ALL, "");  /*Metin dosyasindan Turkce karakterlerin konsolda dogru sekilde goruntulenmesini saglar*/

    if((file = fopen("input.txt", "r")) == NULL) {
        printf("Dosya acilamadi!");
        exit(1);
    }

    root = readFile(file, root);
    fclose(file);

    printList(root);

    return 0;
}

node * readFile(FILE * f, node * r) {
    char word[50];
    int piece;

    while(!feof(f)) {
        fscanf(f, "%s", word);  /*Metin dosyasindan kelime okur*/
        strlwr(word);   /*Kelime okunduktan sonra butun harflerini kucuk harfe cevirir*/

        if(!isAdded(r, word)) {
            piece = countWords(f, word);
            r = add(r, word, piece);
            rewind(f);  /*Dosyanin basina doner*/
        }
    }

    return r;
}

int isAdded(node * r, char w[]) {
    /*Parametre olarak gelen kelime icin bagli listede arama yapar*/
    while(r != NULL) {
        if(strcmp(w, r ->word) == 0)    return 1;
        r = r ->next;
    }
    return 0;
}

int countWords(FILE * f, char w[]) {
    char temp[50];
    int p = 1;

    while(!feof(f)) {
        fscanf(f, "%s", temp);
        strlwr(temp);
        if(strcmp(w, temp) == 0) {
            p++;
        }
    }

    return p;
}

node * add(node * r, char w[], int p) {
    node * iter = r;    /*Sona ekleme kosulunun kontrolunun yapilabilmesi icin tanimlanmistir*/

    if(iter != NULL) {
        while(iter ->next != NULL) {
            iter = iter ->next;
        }
    }

    if(r == NULL || p > r ->piece) {
        r = addToBeginning(r, w, p);
    } else if(p <= iter ->piece) {
        addToEnd(r, w, p);
    } else {
        addBetween(r, w, p);
    }
    return r;
}

node * addToBeginning(node * r, char w[], int p) {
    node * temp = (node *)malloc(sizeof(node)); /*Bagli liste icin yeni bir dugum olusturur*/
    strcpy(temp ->word, w);
    temp ->piece = p;
    temp ->next = r;
    r = temp;

    return r;
}

void addBetween(node * r, char w[], int p) {
    /*Bagli listede son dugume ulaþana kadar r nin gosterdigi adresi degistirir eger kosul saglanirsa donguden cikar*/
    while(r != NULL) {
        if(p <= r ->piece && p > r ->next ->piece)  break;
        r = r ->next;
    }

    node * temp = (node *)malloc(sizeof(node));
    strcpy(temp ->word, w);
    temp ->piece = p;
    temp ->next = r ->next;
    r ->next = temp;
}

void addToEnd(node * r, char w[], int p) {
    /*r bagli listede son dugumu gosterene kadar devam eder*/
    while(r ->next != NULL) {
        r = r ->next;
    }

    node * temp = (node *)malloc(sizeof(node));
    strcpy(temp ->word, w);
    temp ->piece = p;
    temp ->next = NULL;
    r ->next = temp;
}

void printList(node * r) {
    /*Bagli listede ilk dugumden baslayarak son dugume kadar butun elemanlarin bilgilerini ekrana yazdirir*/
    for(int i=1; r != NULL; i++) {
        printf("%2d. %s : %d\n", i, r ->word, r ->piece);
        r = r ->next;
    }
}
