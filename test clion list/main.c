#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo_ {
    char *key;
    struct nodo_ *left, *right, *parent;
    unsigned int color;
    unsigned int ok;
} nodo_t;
typedef struct elem_ {
    struct elem_ *next, *nextok;
    nodo_t *ptr;
} elem_t;

elem_t *primo, *ultimo, *primook, *ultimook;
nodo_t *root = NULL;
unsigned int k;

static inline unsigned int minoreLessic(char const *a, char  const *b){ //0 no, 1 si
    register unsigned int i;

    for (i = 0; a[i] == b[i] && i < k; ++i); //confronto lessicografico

    if (i == k) return 0;
    if(a[i] < b[i]) return 1;
    else return 0;
}

static inline nodo_t* creaNodo(char *key){
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
static inline void ruotaDestra(nodo_t *temp) {
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
static inline void ruotaSinistra(nodo_t *temp) {
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
static inline void fixup(nodo_t *temp, nodo_t *new) {
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
static inline unsigned int inserimenti(unsigned int ingame, char **list, char** list2, unsigned int dim) { //si avvale di func inserisci, creazione dizionario
    char c, *word, *k2, cmd[16];
    nodo_t *new, *temp, *backup;
    elem_t *elem;

    register unsigned int i, j, w, found;

    k2 = malloc(sizeof(char) * k);

    new = NULL;
    while(1) {

        c = getchar_unlocked(); //inserimento parola
        if (c == '+')
            if(scanf("%s\r", cmd)) {
                if (!memcmp("nuova_partita", cmd, 13)) {
                    free(k2);
                    return 0;
                }
                else if(!memcmp("inserisci_fine", cmd, 14)) {
                    free(k2);
                    if (ingame) return 0; else return 1;
                }
                else if(!memcmp("inserisci_inizio", cmd, 16)) continue; //caso inserimenti iniziali
            }

        word = malloc(sizeof(char) * k);
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

        elem = malloc(sizeof(elem_t));
        elem -> next = NULL;
        elem -> nextok = NULL;

        new = creaNodo(word);
        elem -> ptr = new;
        if (!backup) {
            primo = elem;
            ultimo = elem;

            root = new;
        }
        else if (minoreLessic(word, backup -> key)) {
            ultimo -> next = elem;
            ultimo -> nextok = elem;
            ultimo = ultimo -> next;

            backup -> left = new;
            backup -> left -> parent = backup;
        } else {
            ultimo -> next = elem;
            ultimo -> nextok = elem;
            ultimo = ultimo -> next;

            backup -> right = new;
            backup -> right -> parent = backup;
        }

        fixup(root, new);

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
                    }
                }

                for (i = k; i--; ) {
                    if (list[w][i] == '|' && list2[w][i] == k2[i]) {
                        new->ok = 0;
                        ++w;
                        goto here2;
                    }
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

                ultimook -> nextok = elem;
                ultimook = elem;
                ultimook -> nextok = NULL;
                ++w;
            }
        }
    }
}
static inline void checker(char const *str, char *word, unsigned int *cnt) {
    register unsigned int i, j, ok, found;
    char *k2;

    nodo_t *temp;
    elem_t *elem, *prec;

    prec = NULL;
    elem = primook;

    k2 = malloc(sizeof(char) * k);

    ok = 1;
here: //goto comes here
    while(elem) {
        temp = elem -> ptr;

        if(temp -> ok){
            memcpy(k2, temp->key, k); // '$' è mio carattere blank

            for (i = k; i--; ) { //controllo lettere in posizione esatta
                if (str[i] == '+') {
                    if (k2[i] != word[i]) {
                        temp->ok = 0;
                        elem = elem->nextok;
                        goto here;
                    }
                    k2[i] = '$';
                } else if (str[i] == '/' && k2[i] == word[i]) {
                    temp->ok = 0;
                    elem = elem->nextok;
                    goto here;
                }
            }

            for (i = k; i--;) {
                if (str[i] == '|' && word[i] == k2[i]) {
                    temp->ok = 0;
                    elem = elem->nextok;
                    goto here;
                }
                for (j = k, found = 0; j--; ) //i è su stringa tester, j è su stringa da controllare
                    if (str[i] == '|' && k2[j] == word[i] && j != i) {
                        found = 1;
                        k2[j] = '$';
                        break;
                    }
                if (str[i] == '|' && !found) {
                    temp->ok = 0;
                    elem = elem->nextok;
                    goto here;
                }
            }

            for (i = k; i--; ) { //controllo lettere non presenti
                for (j = k, found = 0; j--; )
                    if (str[i] == '/' && k2[j] == word[i]) {
                        found = 1;
                        k2[j] = '$';
                        break;
                    }
                if (found) {
                    temp->ok = 0;
                    elem = elem->nextok;
                    goto here;
                }
            }

            if (temp->ok) (*cnt)++;
        }

        if(!ok) {
            prec -> nextok = elem;
            prec = elem;
        } else {
            ok = 0;
            primook = elem;
            prec = elem;
        }

        elem = elem -> nextok;
    }

    ultimook = prec;
    ultimook -> nextok = NULL;

    free(k2);

    return;
}
static inline void esploraAlbero() {
    nodo_t *temp;
    elem_t *elem;
    elem = primo;

    while(elem) {
        temp = elem -> ptr;
        temp->ok = 1;
        elem -> nextok = elem -> next;
        elem = elem -> next;
    }
}
static inline void inOrdine(nodo_t *temp) {
    if(!temp) return;
    inOrdine(temp -> left);
    if(temp -> ok) puts(temp -> key);
    inOrdine(temp -> right);
}

int main() {
    elem_t *elem;
    nodo_t *temp;
    char c, *p, *p2, *r, *r2, cmd[16], **storico, **storico2;

    register unsigned int i, t, j, ok;
    unsigned int n, count; //k lunghezza parole, n numero tentativi

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

        if(scanf("%s\r", cmd)) {

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

    r = malloc(sizeof(char)*k);
    r2 = malloc(sizeof(char)*k);

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

        storico = malloc(sizeof(char*) * n);
        storico2 = malloc(sizeof(char*) * n);

        for(t = n; t > 0; --t) { //tentativi

            c = getchar_unlocked();
            if (c == '+') {
                if(scanf("%s\r", cmd)) {

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

            p = malloc(sizeof(char) * k);
            p2 = malloc(sizeof(char) * k);

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

            storico[n - t] = p;
            storico2[n - t] = p2;

            //vittoria
            for(i = 0; i < k && p[i] == '+'; ++i);
            if (i < k) puts(p);
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

        for(i = 0; i < n - t + 1; ++i) {
            free(storico[i]);
            free(storico2[i]);
        }

        free(storico);
        free(storico2);

        cmd[0] = '\0';
        do {
            getchar_unlocked(); //prendo + iniziale
            if(scanf(" %s\r", cmd))
                if(!memcmp(cmd, "inserisci_inizio", 16)) inserimenti(0, NULL, NULL, 0);
        } while(!memcmp(cmd, "inserisci_inizio", 16));

        if(!memcmp(cmd, "nuova_partita", 13)) continue; else break;
    }

    free(r);
    free(r2);

    //distruggiAlbero();

    elem = primo;
    while(primo) {
        elem = elem -> next;
        free(primo -> ptr); //al posto di distruggiAlbero()
        free(primo);
        primo = elem;
    }

    return 0;
}