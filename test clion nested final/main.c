#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//rivedi caratteri terminatori

#pragma pack(2)

typedef struct nodo_ {
    struct nodo_ *left, *right, *parent;
    struct nodo_ *next, *nextok;
    unsigned int color : 1;
    unsigned int ok : 1;
    char key[];
} nodo_t;

nodo_t *primo, *ultimo, *primook, *ultimook;
nodo_t *root = NULL;
unsigned int k, count;

static inline unsigned int minoreLessic(char const *a, char  const *b){ //0 no, 1 si
    register unsigned int i;

    for (i = 0; i < k && a[i] == b[i]; ++i); //confronto lessicografico

    if (i == k) return 0;
    if(a[i] < b[i]) return 1;
    else return 0;
}

static inline nodo_t* creaNodo(char *key){
    nodo_t *nodo;
    nodo = malloc(sizeof(nodo_t) + sizeof(char) * (k + 1));

    memcpy(nodo -> key, key, k);
    nodo -> left = NULL;
    nodo -> right = NULL;
    nodo -> parent = NULL;

    nodo -> next = NULL;
    nodo -> nextok = NULL;

    nodo -> color = 1; //1 red, 0 black
    nodo -> ok = 1;

    return nodo;
}
static inline void ruotaDestra(nodo_t *temp) {
    nodo_t *left;

    left = temp -> left;
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
static inline void ruotaSinistra(nodo_t *temp) {
    nodo_t *right;

    right = temp -> right;
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
static inline void fixUp(nodo_t *temp, nodo_t *new) {
    register unsigned int c;
    nodo_t *uncle, *parent, *grandparent;

    c = 0;
    uncle = NULL;
    parent = NULL;
    grandparent = NULL;

    while ((new != temp) && (new -> color) && (new -> parent -> color)) {

        parent = new -> parent;
        grandparent = new -> parent -> parent;

        // parent di new è figlio destro di grandparent di new
        if (parent == grandparent -> right) {

            uncle = grandparent -> left;

            // a: anche uncle di new è rosso, ricoloro
            if ((uncle) && (uncle -> color)) {
                grandparent -> color = 1;
                parent -> color = 0;
                uncle -> color = 0;
                new = grandparent;
            } else {

                // b: new è figlio sinistro, ruotaDestra
                if (new == parent -> left) {
                    ruotaDestra(parent);
                    new = parent;
                    parent = new -> parent;
                }

                // c: new è figlio destro, ruotaSinistra
                ruotaSinistra(grandparent);
                c = parent -> color;
                parent -> color = grandparent -> color;
                grandparent -> color = c;
                new = parent;

            }

        // parent di new è figlio sinistro di grandparent di new
        } else {

            uncle = grandparent -> right;

            // a: anche uncle di new è rosso, ricoloro
            if (uncle && uncle -> color) {
                grandparent -> color = 1;
                parent -> color = 0;
                uncle -> color = 0;
                new = grandparent;
            } else {

                // b: new è figlio destro, ruotaSinistra
                if (new == parent -> right) {
                    ruotaSinistra(parent);
                    new = parent;
                    parent = new -> parent;
                }

                // c: new è figlio sinistro, ruotaDestra
                ruotaDestra(grandparent);
                c = parent -> color;
                parent -> color = grandparent -> color;
                grandparent -> color = c;
                new = parent;

            }
        }
    }

    temp -> color = 0;

} //togli temp root
static inline unsigned int inserimenti(unsigned int ingame, char **list, char** list2, unsigned int dim) { //si avvale di func inserisci, creazione dizionario
    char c, *word, *k2, cmd[17];
    register unsigned int i, j, w, found;

    nodo_t *new, *temp, *backup;


    word = malloc(sizeof(char) * (k + 1));
    k2 = malloc(sizeof(char) * (k + 1));
    word[k] = '\0';
    k2[k] = '\0';

    new = NULL;
    while(1) {

        c = getchar_unlocked(); //inserimento parola
        if (c == '+')
            if(scanf("%s\n", cmd)) {
                if (!memcmp("nuova_partita", cmd, 13)) {
                    free(word);
                    free(k2);
                    return 0;
                }
                else if(!memcmp("inserisci_fine", cmd, 14)) {
                    free(word);
                    free(k2);
                    if (ingame) return 0; else return 1;
                }
                else if(!memcmp("inserisci_inizio", cmd, 16)) continue; //caso inserimenti iniziali
            }

        word[0] = c;

        for(i = 1; i < k; ++i)
            word[i] = getchar_unlocked();
        getchar_unlocked(); //scarto '\n'

        backup = NULL;
        temp = root;
        while (temp != NULL){ //caso gia presente: si supponga non ci siano duplicati
            backup = temp;
            if (minoreLessic(word, temp -> key))
                temp = temp -> left;
            else
                temp = temp -> right;
        }


        new = creaNodo(word);
        if (backup) {
            ultimo -> next = new;
            ultimo -> nextok = new;
            ultimo = ultimo -> next;

            if (minoreLessic(word, backup -> key)) {
                backup -> left = new;
                backup -> left -> parent = backup;
            } else {
                backup->right = new;
                backup->right->parent = backup;
            }
        } else {
            primo = new;
            ultimo = new;

            root = new;
        }

        fixUp(root, new);

        if(ingame) {//checker midgame

            w = 0;
here2:
            while (w < dim) {
                memcpy(k2, new->key, k); // '$' è mio carattere blank

                for (i = k; i--; ) { //controllo lettere in posizione esatta
                    if (list[w][i] == '+') {
                        if (k2[i] != list2[w][i]) {
                            new->ok = 0;
                            ++w;
                            goto here2;
                        }
                        k2[i] = '$';
                    } else if (list[w][i] == '/' && k2[i] == list2[w][i]) {
                        new->ok = 0;
                        ++w;
                        goto here2;
                    } else if (list[w][i] == '|' && list2[w][i] == k2[i]) {
                        new->ok = 0;
                        ++w;
                        goto here2;
                    }
                }

                for (i = k; i--; ) {
                    for (j = k, found = 0; j--; ) //i è su stringa tester, j è su stringa da controllare
                        if (list[w][i] == '|' && k2[j] == list2[w][i] && j != i) {
                            found = 1;
                            k2[j] = '$';
                            break;
                        }
                    if (list[w][i] == '|' && !found) {
                        new->ok = 0;
                        ++w;
                        goto here2;
                    }
                }

                for (i = k; i--; ) { //controllo lettere non presenti
                    for (j = k, found = 0; j--; )
                        if (list[w][i] == '/' && k2[j] == list2[w][i]) {
                            found = 1;
                            k2[j] = '$';
                            break;
                        }
                    if (found) {
                        new->ok = 0;
                        ++w;
                        goto here2;
                    }
                }

                ultimook -> nextok = new;
                ultimook = new;
                ultimook -> nextok = NULL;

                count++;
                ++w;
            }
        }
    }
}
static inline void checker(char const *str, char const *word, unsigned int *cnt) {
    register unsigned int i, j; //ok, found;
    char *k2;

    struct {
        unsigned int ok :1;
        unsigned int found :1;
    } flags;

    nodo_t *temp, *prec;

    prec = NULL;
    temp = primook;

    k2 = malloc(sizeof(char) * (k + 1));
    k2[k] = '\0';

    flags.ok = 1;
here: //goto comes here
    while(temp) {

        if(temp -> ok){
            memcpy(k2, temp->key, k); // '$' è mio carattere blank

            for (i = k; i--; ) { //controllo lettere in posizione esatta
                if (str[i] == '+') {
                    if (k2[i] != word[i]) {
                        temp->ok = 0;
                        temp = temp->nextok;
                        goto here;
                    }
                    k2[i] = '$';
                } else if (str[i] == '/' && k2[i] == word[i]) {
                    temp->ok = 0;
                    temp = temp->nextok;
                    goto here;
                } else if (str[i] == '|' && word[i] == k2[i]) {
                    temp->ok = 0;
                    temp = temp->nextok;
                    goto here;
                }
            }
            for (i = k; i--;) {
                for (j = k, flags.found = 0; j--; ) //i è su stringa tester, j è su stringa da controllare
                    if (str[i] == '|' && k2[j] == word[i] && j != i) {
                        flags.found = 1;
                        k2[j] = '$';
                        break;
                    }
                if (str[i] == '|' && !flags.found) {
                    temp->ok = 0;
                    temp = temp->nextok;
                    goto here;
                }
            }

            for (i = k; i--; ) { //controllo lettere non presenti
                for (j = k, flags.found = 0; j--; )
                    if (str[i] == '/' && k2[j] == word[i]) {
                        flags.found = 1;
                        k2[j] = '$';
                        break;
                    }
                if (flags.found) {
                    temp->ok = 0;
                    temp = temp->nextok;
                    goto here;
                }
            }

            if (temp->ok) (*cnt)++;
        }

        if(!flags.ok) {
            prec -> nextok = temp;
            prec = temp;
        } else {
            flags.ok = 0;
            primook = temp;
            prec = temp;
        }

        temp = temp -> nextok;
    }

    ultimook = prec;
    ultimook -> nextok = NULL;

    free(k2);
}
static inline void esploraAlbero() {
    nodo_t *temp;

    temp = primo;
    while(temp) {
        temp->ok = 1;
        temp -> nextok = temp -> next;
        temp = temp -> next;
    }
}
static inline void inOrdine(nodo_t *temp) {
    if(!temp) return;
    inOrdine(temp -> left);
    if(temp -> ok) printf("%.*s\n", k, temp -> key);
    inOrdine(temp -> right);
} //vedi se possibile non ricorsivo

int main() {
    nodo_t *temp;
    char c, *p, *p2, *r, *r2, cmd[17], **storico, **storico2;

    register unsigned int i, t, j, ok;
    unsigned int n, top; //top massimo numero tentativi, n numero tentativi

    c = getchar_unlocked();
    k = c - '0';

    c = getchar_unlocked();
    while (c != '\n') {
        k = (k<<3) + (k<<1);
        k += c - '0';
        c = getchar_unlocked();
    }

    //composizione iniziale dizionario
    while(inserimenti(0, NULL, NULL, 0)) { //se entro ho inserisci_inizio
        c = getchar_unlocked();

        if(scanf("%s\n", cmd)) {

        //c = getchar_unlocked();
        //for(i = 0; c != '\n'; i++) {
        //    cmd[i] = c;
        //    c = getchar_unlocked();
        //}
        //cmd[i] = '\0';

            if (!memcmp("nuova_partita", cmd, 13)) break;
            else if (memcmp("inserisci_inizio", cmd, 16)) return 0;
        }
    }

    r = malloc(sizeof(char) * (k + 1));
    r2 = malloc(sizeof(char) * (k + 1));
    r[k] = '\0';
    r2[k] = '\0';

    top = 0;
    storico = NULL;
    storico2 = NULL;

    while(1) { //inizio nuova partita
        esploraAlbero();

        primook = primo;
        ultimook = ultimo;

        for(i = 0; i < k; ++i)
            r[i] = getchar_unlocked();
        getchar_unlocked();

        memcpy(r2, r, k);

        c = getchar_unlocked();
        n = c - '0';

        c = getchar_unlocked();
        while (c != '\n') {
            n = (n<<3) + (n<<1);
            n += c - '0';
            c = getchar_unlocked();
        }

        if (n > top) {
            storico = realloc(storico, sizeof(char *) * n);
            storico2 = realloc(storico2, sizeof(char *) * n);
        }

        for(t = n; t > 0; --t) { //tentativi

            c = getchar_unlocked();
            if (c == '+') {
                if(scanf("%s\n", cmd)) {

                //c = getchar_unlocked();
                //for(i = 0; c != '\n'; i++) {
                //    cmd[i] = c;
                //    c = getchar_unlocked();
                //}
                //cmd[i] = '\0';

                    if (!memcmp("inserisci_inizio", cmd, 16)) {
                        inserimenti(1, storico, storico2, n - t); //integrato checker per inserimenti ingame
                        ++t;
                        continue;
                    } else if (!memcmp("stampa_filtrate", cmd, 15)) { //stampa filtrate
                        inOrdine(root);
                        ++t;
                        continue;
                    }
                }
            }

            if (n - t + 1 > top) {
                ++top;
                storico[n - t] = malloc(sizeof(char) * (k + 1));
                storico2[n - t] = malloc(sizeof(char) * (k + 1));
            }

            p = storico[n - t];
            p2 = storico2[n - t];

            p[k] = '\0';
            p2[k] = '\0';

            p[0] = c;
            for (i = 1; i < k; ++i)
                p[i] = getchar_unlocked();
            getchar_unlocked();

            memcpy(p2, p, k);
            memcpy(r, r2, k);

            ok = 0;
            temp = root;

            while (temp)
                if (minoreLessic(p, temp -> key)) temp = temp -> left;
                else if (minoreLessic(temp -> key, p)) temp = temp -> right;
                else {
                    ok = 1;
                    break;
                }

            if (!ok) {
                puts("not_exists");
                ++t;
                continue;
            }

            //wordcheck, p stringa simbolica, r parola di riferimento con blank, p2 e r2 copie
            for(i = 0; i < k; ++i) if (p[i] == r[i]) {
                    p[i] = '+';
                    r[i] = '$';
            }
            for(i = 0; i < k; ++i) {
                for (j = 0; j < k; ++j)
                    if(p[i] != '+' && p[i] == r[j]) {
                        p[i] = '|';
                        r[j] = '$';
                        break;
                    }
                if (p[i] != '+' && j == k) p[i] = '/';
            }

            //vittoria
            for(i = 0; i < k && p[i] == '+'; ++i);
            if (i < k) printf("%.*s\n", k, p); //.* prende solo k caratteri
            else {
                puts("ok");
                break;
            }

            count = 0;
            checker (p, p2, &count);

            printf("%d\n", count);
        }

        if (!t) {
            puts("ko");
            ++t;
        }

        do {
            cmd[0] = '\0';
            getchar_unlocked(); //prendo + iniziale
            if(scanf("%s\n", cmd))
                if (!memcmp(cmd, "inserisci_inizio", 16)) inserimenti(0, NULL, NULL, 0);
        } while(!memcmp(cmd, "inserisci_inizio", 16));

        if(!memcmp(cmd, "nuova_partita", 13)) continue; else break;
    }

    for(i = 0; i < top; ++i) {
        free(storico[i]);
        free(storico2[i]);
    }

    free(storico);
    free(storico2);

    free(r);
    free(r2);

    temp = primo;
    while(primo) {
        temp = temp -> next;
        free(primo);
        primo = temp;
    }

    return 0;
}
