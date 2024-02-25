#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include<time.h>

typedef struct nodo_ {
    char key;
    struct nodo_ *left, *right, *parent;
    int color;
    int ok;
} nodo_t;

typedef struct elem_ {
    struct elem_ *next;
    nodo_t *ptr;
} elem_t;

int minoreLessic(char *, char *, int);

int inserimenti(int, int, char **, char **, int);

int checker(char *, char *, int, int *);
int esploraAlbero(nodo_t *, int);
int distruggiAlbero(nodo_t *);
void ruotaDestra(nodo_t *);
void ruotaSinistra(nodo_t *);
nodo_t* creaNodo(char *);
void fixup(nodo_t *, nodo_t *);

elem_t *primo, ultimo;
nodo_t *root = NULL;
int main() {

    //int start = clock();

    nodo_t *temp;
    char c, *p, *p2, *r, *r2, cmd[25], **storico, **storico2;

    register int i, t, j, ok, err;
    int k, n, count; //k lunghezza parole, n numero tentativi

    err = scanf(" %d\r", &k);
    if(err) //costrutto per bypassare warning
        ;

    //composizione iniziale dizionario
    while(inserimenti(k, 0, NULL, NULL, 0)) { //se entro ho inserisci_inizio
        c = getchar_unlocked();
        err = scanf("%s\r", cmd);
        if (!strcmp("nuova_partita", cmd)) break;
        if (strcmp("inserisci_inizio", cmd)) return 0;
    }

    r = malloc(sizeof(char)*k);
    r2 = malloc(sizeof(char)*k);

    while(1) { //inizio nuova partita
        esploraAlbero(root, 1);

        err = scanf(" %s\r", r);

        strcpy(r2, r);

        err = scanf(" %d\r", &n);

        storico = malloc(sizeof(char*) * n);
        storico2 = malloc(sizeof(char*) * n);

        for(t = n; t > 0; t--) { //tentativi

            c = getchar_unlocked();
            if (c == '+') {
                err = scanf("%s\r", cmd);
                if (!strcmp("inserisci_inizio", cmd)) {
                    inserimenti(k, 1, storico, storico2, n - t); //integrato checker per inserimenti ingame
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

            ok = 0;
            temp = root;

            while (temp)
                if (minoreLessic(p, temp -> key, k)) temp = temp -> left;
                else if (minoreLessic(temp -> key, p, k)) temp = temp -> right;
                else {
                    ok = 1;
                    break;
                }

            if (!ok) {
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
            for(i = 0; i < k && p[i] == '+'; i++);
            if (i == k) {
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
            getchar_unlocked(); //prendo + iniziale
            err = scanf(" %s\r", cmd);
            if(!strcmp(cmd, "inserisci_inizio")) inserimenti(k, 0, NULL, NULL, 0);
        } while(!strcmp(cmd, "inserisci_inizio"));

        if(!strcmp(cmd, "nuova_partita")) continue; else break;
    }

    free(r);
    free(r2);

    distruggiAlbero(root);

    //int end = clock();
    //printf("clocks: %d\n", (end - start));

    return 0;

}

int minoreLessic(char *a, char *b, int len){ //0 no (o gia presente), 1 si
    register int i;
    for (i = 0; a[i] == b[i] && i < len; i++); //confronto lessicografico

    if (i == len) {
        return 0;
    } else {

        //percorro ramo sinistro
        if (a[i] < b[i]) {
            return 1;
        } else return 0;
    }

}

int inserimenti(int len, int ingame, char **list, char** list2, int dim) { //si avvale di func inserisci, creazione dizionario
    char c, *word, *k2, cmd[25];
    nodo_t *new, *temp, *backup;
    elem_t *elem;

    register int i, j, k, found;

    new = NULL;
    while(1) {

        c = getchar_unlocked(); //inserimento parola
        if (c == '+')
            if(scanf("%s\r", cmd)) {
                if (!strcmp("nuova_partita", cmd)) return 0;
                else if(!strcmp("inserisci_fine", cmd)) {if (ingame) return 0; else return 1;}
                else if(!strcmp("inserisci_inizio", cmd)) continue; //caso inserimenti iniziali
            }

        word = malloc(sizeof(char) * len);
        word[0] = c;

        if(scanf(" %s\r", word + 1)) {

            backup = NULL;
            temp = root;
            while (temp != NULL){ //caso gia presente: si supponga non ci siano duplicati
                backup = temp;
                if (minoreLessic(word, temp -> key, len))
                    temp = temp -> left;
                else
                    temp = temp -> right;
            }

            elem = malloc(sizeof(elem_t));
            elem -> next = NULL;

            new = creaNodo(word);
            elem -> ptr = new;
            if (!backup) {
                primo = elem;
                ultimo = elem;

                root = new;
            }
            else if (minoreLessic(word, backup -> key, len)) {
                ultimo -> next = elem;
                ultimo = ultimo -> next;

                backup -> left = new;
                backup -> left -> parent = backup;
            } else {
                ultimo -> next = elem;
                ultimo = ultimo -> next;

                backup -> right = new;
                backup -> right -> parent = backup;
            }
        }

        fixup(root, new);

        if(ingame) //checker midgame
            for(k = 0; k < dim; k++){ //k parola nello storico

                k2 = malloc(sizeof(char) * len);
                strcpy(k2, new -> key); // '$' è mio carattere blank

                for (i = 0; i < len && new -> ok; i++) { //controllo lettere in posizione esatta
                    if (list[k][i] == '+') {
                        if (k2[i] != list2[k][i]) new -> ok = 0;
                        k2[i] = '$';
                    }
                    if (list[k][i] == '/' && k2[i] == list2[k][i]) new -> ok = 0;
                }

                for (i = 0; i < len && new -> ok; i++) {
                    if(list[k][i] == '|' && list2[k][i] == k2[i]) new -> ok = 0;
                    for (j = 0, found = 0; j < len; j++) //i è su stringa tester, j è su stringa da controllare
                        if(list[k][i] == '|' && k2[j] == list2[k][i] && j != i) {found = 1; k2[j] = '$'; break;}
                    if (list[k][i] == '|' && !found) new -> ok = 0;
                }

                for (i = 0; i < len && new -> ok; i++) { //controllo lettere non presenti
                    for (j = 0, found = 0; j < len; j++)
                        if(list[k][i] == '/' && k2[j] == list2[k][i]) {found = 1; k2[j] = '$'; break;}
                    if (found) new -> ok = 0;
                }

                free(k2);
            }

    }
}
int checker(char *str, char *word, int len, int *cnt) {
    register int i, j, found;
    char *k2;

    nodo_t *temp;
    elem_t *elem;
    temp = primo;

    k2 = malloc(sizeof(char) * len);

    while(elem) {
        temp = elem -> ptr;

        if (!(temp -> ok)) continue;

        strcpy(k2, temp -> key); // '$' è mio carattere blank

        if (!strcmp(word, k2)) temp -> ok = 0;  //scarto parola errata
        for (i = 0; i < len && temp -> ok; i++) { //controllo lettere in posizione esatta
            if (str[i] == '+') {
                if (k2[i] != word[i]) temp -> ok = 0;
                k2[i] = '$';
            }
            if (str[i] == '/' && k2[i] == word[i]) temp -> ok = 0;
        }

        for (i = 0; i < len && temp -> ok; i++) {
            if(str[i] == '|' && word[i] == k2[i]) temp -> ok = 0;
            for (j = 0, found = 0; j < len; j++) //i è su stringa tester, j è su stringa da controllare
                if(str[i] == '|' && k2[j] == word[i] && j != i) {found = 1; k2[j] = '$'; break;}
            if (str[i] == '|' && !found) temp -> ok = 0;
        }

        for (i = 0; i < len && temp -> ok; i++) { //controllo lettere non presenti
            for (j = 0, found = 0; j < len; j++)
                if(str[i] == '/' && k2[j] == word[i]) {found = 1; k2[j] = '$'; break;}
            if (found) temp -> ok = 0;
        }

        if(temp -> ok) (*cnt)++;

        elem = elem -> next;
    }
    free(k2);

    return 0;
}
int esploraAlbero(nodo_t *temp, int func){ //procede in ordine crescente, func: 0 stampa, 1 set ok to true, 2 stampa filtrate, 3

    if (temp -> left) esploraAlbero(temp -> left, func);
    switch(func) {
        case 0:
            printf("%s\n", temp -> key);
            break;
        case 1:
            temp -> ok = 1;
            break;
        case 2:
            if (temp -> ok) printf("%s\n", temp -> key);
            break;
        case 3:
            break;
    }
    if (temp -> right) esploraAlbero(temp -> right, func);
    return 0;
}
int distruggiAlbero(nodo_t *nodo){
    if (nodo -> left) distruggiAlbero(nodo -> left);
    if (nodo -> right) distruggiAlbero(nodo -> right);
    free(nodo -> key);
    free(nodo);

    return 0;
}
void ruotaDestra(nodo_t *temp) {
    nodo_t *left = temp -> left;
    temp -> left = left -> right;
    if (temp -> left)
        temp -> left -> parent = temp;
    left -> parent = temp -> parent;
    if (!temp -> parent)
        root = left;
    else if (temp == temp -> parent -> left)
        temp -> parent -> left = left;
    else
        temp -> parent -> right = left;
    left -> right = temp;
    temp -> parent = left;
}
void ruotaSinistra(nodo_t *temp) {
    nodo_t *right = temp -> right;
    temp -> right = right -> left;
    if (temp -> right)
        temp -> right -> parent = temp;
    right -> parent = temp -> parent;
    if (!temp -> parent)
        root = right;
    else if (temp == temp -> parent -> left)
        temp -> parent -> left = right;
    else
        temp -> parent -> right = right;
    right -> left = temp;
    temp -> parent = right;
}
nodo_t* creaNodo(char *key){
    nodo_t *nodo;
    nodo = malloc(sizeof(nodo_t));

    nodo -> key = key;
    nodo -> left = NULL;
    nodo -> right = NULL;
    nodo -> parent = NULL;
    nodo -> color = 1; //1 red, 0 black
    nodo -> ok = 1;

    return nodo;
}
void fixup(nodo_t *temp, nodo_t *new) {
    nodo_t *parent = NULL;
    nodo_t *grandparent = NULL;

    while ((new != temp) && (new -> color != 0) && (new -> parent -> color == 1)) {
        parent = new -> parent;
        grandparent = new -> parent -> parent;

        /*  Case : A
             Parent of pt is left child
             of Grand-parent of
           pt */
        if (parent == grandparent -> left) {

            nodo_t *uncle = grandparent -> right;

            /* Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if (uncle != NULL && uncle -> color == 1) {
                grandparent -> color = 1;
                parent -> color = 0;
                uncle -> color = 0;
                new = grandparent;
            }

            else {

                /* Case : 2
                     pt is right child of its parent
                     Left-rotation required */
                if (new == parent -> right) {
                    ruotaSinistra(parent);
                    new = parent;
                    parent = new -> parent;
                }

                /* Case : 3
                     pt is left child of its parent
                     Right-rotation required */
                ruotaDestra(grandparent);
                int t = parent -> color;
                parent -> color = grandparent -> color;
                grandparent -> color = t;
                new = parent;
            }
        }

            /* Case : B
                 Parent of pt is right
                 child of Grand-parent of
               pt */
        else {
            nodo_t *uncle = grandparent -> left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle != NULL) && (uncle -> color == 1))
            {
                grandparent -> color = 1;
                parent -> color = 0;
                uncle -> color = 0;
                new = grandparent;
            }
            else {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (new == parent -> left) {
                    ruotaDestra(parent);
                    new = parent;
                    parent = new -> parent;
                }

                /* Case : 3
                     pt is right child of its parent
                     Left-rotation required */
                ruotaSinistra(grandparent);
                int t = parent -> color;
                parent -> color = grandparent -> color;
                grandparent -> color = t;
                new = parent;
            }
        }
    }

    temp -> color = 0;

}
