#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
comandi: +nuova_partita, +inserisci_inizio, +inserisci_fine, +stampa_filtrate
*/

typedef struct nodo_ {
    char *key;
    struct nodo_ *left, *right;
    int height;
    int ok;
} nodo_t;

int max(int, int);
int minoreLessic(char *, char *, int);

nodo_t* inserimenti(nodo_t *, int);

int checker(nodo_t *, char *, char *, int, int *);
int cercaStr(nodo_t *, char *);
int esploraAlbero(nodo_t *, int);
int distruggiAlbero(nodo_t *);
//int stampaAlbero(nodo_t *);
int altezza(nodo_t *);
int bilanciamento(nodo_t *);
nodo_t* ruotaDestra(nodo_t *);
nodo_t* ruotaSinistra(nodo_t *);
nodo_t* creaNodo(char *);
nodo_t* inserisci(nodo_t *, char *, int);


int main() {
    nodo_t *root;
    char c, *p, *p2, *r, *r2, cmd[25], **storico, **storico2;

    int k, n, i; //k lunghezza parole
    int t, j, ok, err, count;

    //printf("inserisci lunghezza: ");
    err = scanf(" %d\r", &k);
    if(err) //costrutto per bypassare warning
        ;

    root = NULL;

    //composizione iniziale dizionario
    root = inserimenti(root, k);

    r = malloc(sizeof(char)*k);
    r2 = malloc(sizeof(char)*k);

    while(1) { //inizio nuova partita
        esploraAlbero(root, 1);

        while(1) {
            //printf("parola di riferimento: ");
            err = scanf(" %s\r", r);

            if(!cercaStr(root, r))
                ;//printf("non in dizionario\n");
            else
                break;
        }
        strcpy(r2, r);

        //printf("numero di tentativi: ");
        err = scanf(" %d\r", &n);

        storico = malloc(sizeof(char*) * n);
        storico2 = malloc(sizeof(char*) * n);


        for(t = n; t > 0; t--) { //tentativi

            fflush(stdin);
            c = getchar();
            if (c == '+') {

                err = scanf("%s\r", cmd);
                if (!strcmp("inserisci_inizio", cmd)) {
                    root = inserimenti(root, k);
                    for (i = 0, count = 0; i < n - t; i++)
                        checker(root, storico[i], storico2[i], k, &count);
                    t++;
                    continue;
                }
                if (!strcmp("stampa_filtrate", cmd)) { //stampa filtrate
                    esploraAlbero(root, 2);
                    t++;
                    continue;
                }
            }

            p = malloc(sizeof(char) * k);
            p2 = malloc(sizeof(char) * k);

            p[0] = c;
            err = scanf(" %s\r", p + 1);

            strcpy(p2, p);
            strcpy(r, r2);
            if(!cercaStr(root, p)) {
                printf("not_exists\n");
                t++;
                continue;
            }

            //wordcheck, p stringa simbolica, r parola di riferimento con blank, p2 e r2 copie
            for(i = 0; i < k; i++) if (p[i] == r[i]) {
                    p[i] = '+';
                    r[i] = '$';
                }
            for(i = 0; i < k; i++) {
                for (j = 0; j < k; j++)
                    if(p[i] != '+' && p[i] == r[j]) {
                        p[i] = '|';
                        r[j] = '$';
                        break;
                    }
                if (p[i] != '+' && j == k) p[i] = '/';
            }

            storico[n - t] = p;
            storico2[n - t] = p2;

            //vittoria
            for(i = 0, ok = 1; i < k; i++) if (p[i] != '+') ok = 0;
            if (ok) {
                printf("ok\n");
                break;
            } else printf("%s\n", p);

            count = 0;
            checker (root, p, p2, k, &count);

            printf("%d\n", count);


        }

        if (t == 0) {
            printf("ko\n");
            t++;
        }

        for(i = 0; i < n - t + 1; i++) {
            free(storico[i]);
            free(storico2[i]);
        }

        free(storico);
        free(storico2);


        cmd[0] = '\0';
        do {
            fflush(stdin);
            getchar(); //prendo + iniziale
            err = scanf(" %s\r", cmd);
            if(!strcmp(cmd, "inserisci_inizio")) root = inserimenti(root, k);
        } while(!strcmp(cmd, "inserisci_inizio"));

        if(!strcmp(cmd, "nuova_partita")) continue; else break;
    }


    free(r);
    free(r2);


    distruggiAlbero(root);

    //printf("fine\n");
    return 0;

}

int max(int a, int b) {
    if(a > b)
        return a;
    else
        return b;
}
int minoreLessic(char *a, char *b, int len){ //0 no (o gia presente), 1 si
    int i;
    for (i = 0; a[i] == b[i] && i<len; i++); //confronto lessicografico

    if (i == len) {
        return 0;
    } else {

        //percorro ramo destro
        if (a[i] < b[i]) {
            return 1;
        } else return 0;
    }

}

nodo_t* inserimenti(nodo_t *root, int len) { //si avvale di func inserisci, creazione dizionario
    char c,*word, cmd[25];


    while(1) {

        //printf("inserisci parola: ");

        fflush(stdin);
        c = getchar();
        if (c == '+') {
            if(scanf("%s\r", cmd))
                if (!strcmp("nuova_partita", cmd) || !strcmp("inserisci_fine", cmd)) break;
        }

        word = malloc(sizeof(char) * len);
        word[0] = c;
        if(scanf(" %s\r", word + 1))
            root = inserisci(root, word, len);

    }

    return root;
}

int checker(nodo_t *root, char *str, char *word, int len, int *cnt) {
    int i, j, found;
    char *k2;

    if (root -> right) checker(root -> right, str, word, len, cnt);
    if (root -> left) checker(root -> left, str, word, len, cnt);

    if (!(root -> ok)) return 0;

    k2 = malloc(sizeof(char) * len);
    strcpy(k2, root -> key); // '$' è mio carattere blank

    if (!strcmp(word, k2)) root -> ok = 0;  //scarto parola errata
    for (i = 0; i < len; i++) //controllo lettere in posizione esatta
        if(str[i] == '+') {
            if(k2[i] != word[i]) root -> ok = 0;
            k2[i] = '$';
        }


    for (i = 0; i < len; i++) {
        for (j = 0, found = 0; j < len; j++) //i è su stringa tester, j è su stringa da controllare
            if(str[i] == '|' && k2[j] == word[i] && j != i) {found = 1; k2[j] = '$'; break;}
        if (str[i] == '|' && !found) root -> ok = 0;
    }

    for (i = 0; i < len; i++) { //controllo lettere non presenti
        for (j = 0, found = 0; j < len; j++)
            if(str[i] == '/' && k2[j] == word[i]) {found = 1; k2[j] = '$';}
        if (found) root -> ok = 0;
    }


    free(k2);

    if(root -> ok) (*cnt)++;

    return 0;
}
int cercaStr(nodo_t *root, char *str){
    if (root == NULL) return 0;
    if (!strcmp(str, root -> key)) return 1;

    return cercaStr(root -> left, str) + cercaStr(root -> right, str);
}
int esploraAlbero(nodo_t *root, int func){ //procede in ordine crescente, func: 0 stampa, 1 set ok to true, 2 stampa filtrate, 3

    if (root -> left) esploraAlbero(root -> left, func);
    switch(func) {
        case 0:
            printf("%s\n", root -> key);
            break;
        case 1:
            root -> ok = 1;
            break;
        case 2:
            if (root -> ok) printf("%s\n", root -> key);
            break;
        case 3:
            break;
    }
    if (root -> right) esploraAlbero(root -> right, func);
    return 0;
}
int distruggiAlbero(nodo_t *nodo){
    if (nodo -> left) distruggiAlbero(nodo -> left);
    if (nodo -> right) distruggiAlbero(nodo -> right);
    free(nodo -> key);
    free(nodo);

    return 0;
}
/*int stampaAlbero(nodo_t *nodo) {
    if (nodo -> left) stampaAlbero(nodo -> left);
    if (nodo -> right) stampaAlbero(nodo -> right);
    printf("%s\n", nodo -> key);

    return 0;
};*/
int altezza(nodo_t *nodo) {
    if (nodo == NULL)
        return 0;
    return 1 + max(altezza(nodo -> left), altezza(nodo -> right));
}
int bilanciamento(nodo_t *nodo) {
    if (nodo == NULL)
        return 0;
    return altezza(nodo -> left) - altezza(nodo -> right);
}
nodo_t* ruotaDestra(nodo_t *nodo) {
    nodo_t *f2, *nodo2;

    nodo2 = nodo -> left;
    f2 = nodo2 -> right;

    nodo2 -> right = nodo;
    nodo -> left = f2;

    nodo -> height = altezza(nodo);
    nodo2 -> height = altezza(nodo2);

    return nodo2; //nuova radice
}
nodo_t* ruotaSinistra(nodo_t *nodo2) {
    nodo_t *f2, *nodo;

    nodo = nodo2 -> right;
    f2 = nodo -> left;

    nodo -> left = nodo2;
    nodo2 -> right = f2;

    nodo2 -> height = altezza(nodo2);
    nodo -> height = altezza(nodo);

    return nodo;
}
nodo_t* creaNodo(char *key){
    nodo_t *nodo;
    nodo = malloc(sizeof(nodo_t));

    nodo -> key = key;
    nodo -> left = NULL;
    nodo -> right = NULL;
    nodo -> height = 0;
    nodo -> ok = 1;

    return nodo;
}
nodo_t* inserisci(nodo_t *root, char *key, int len){

    //implementato come albero AVL

    if (root == NULL)
        return(creaNodo(key));

    nodo_t *temp;
    temp = root;

    if (minoreLessic(key, temp -> key, len))
        temp -> left  = inserisci(temp -> left, key, len);
    else if (minoreLessic(temp -> key, key, len))
        temp -> right = inserisci(temp -> right, key, len);
    else { //gia presente
        //printf("word already in dictionary\n");
        return temp;
    }

    temp -> height = altezza(temp);
    int balance = bilanciamento(temp);

    //se sbilanciato
    //Left Left Case
    if (balance > 1 && minoreLessic(key, temp -> left -> key, len))
        return ruotaDestra(temp);

    //Right Right Case
    if (balance < -1 && minoreLessic(temp -> right -> key, key, len))
        return ruotaSinistra(temp);

    //Left Right Case
    if (balance > 1 && minoreLessic(temp -> left -> key, key, len)){
        temp -> left = ruotaSinistra(temp -> left);
        return ruotaDestra(temp);
    }

    //Right Left Case
    if (balance < -1 && minoreLessic(key, temp -> right -> key, len)){
        temp -> right = ruotaDestra(temp -> right);
        return ruotaSinistra(temp);
    }

    return temp; //ritorna puntatore al nodo invariato
}
