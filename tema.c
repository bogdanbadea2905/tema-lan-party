#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
 
struct Player       //structura pentru fiecare jucator
{
 
    char *firstName;
    char *secondName;
    int points;
};
typedef struct Player Player;
 
struct Elem             //structura pentru lista de echipe
{
 
    int nr_players;
    char *nume_echipa;
    float puncte;
    Player *jucatori;
    struct Elem *next;
};
typedef struct Elem Node;
 
struct EchipeCoada      //structura pentru coada cu cate 2 echipe
{
 
    Node *echipa1, *echipa2;
    struct EchipeCoada *next;
};
typedef struct EchipeCoada EchipeCoada;
 
struct Q                //coada
{
 
    EchipeCoada *fata, *spate;
};
typedef struct Q Queue;

struct Stiva {          //structura pentru stive

    Node *echipa;
    struct Stiva *next;
};
typedef struct Stiva Stiva;

struct BST {
    
    Node *echipa;
    struct BST *left, *right;
};
typedef struct BST BST;

float punctaj(Node *head)   //calculul punctajului fiecarei echipe
{
 
    float s = 0;
    for (int i = 0; i < head->nr_players; i++)
        s = s + head->jucatori[i].points;
 
    s = s / head->nr_players;
 
    return s;
}
 
void addAtBeginning(Node **head, FILE *f)   //inserare la inceput+citire din fisier pentru lista de la ex1
{
 
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->nume_echipa = malloc(50);
    fscanf(f, "%d", &newNode->nr_players);
    fgetc(f);       //pentru stergerea spatiului de dinaintea numelor echipelor
    fgets(newNode->nume_echipa, 50, f);
    newNode->jucatori = (Player *)malloc(newNode->nr_players * sizeof(Player));
 
    for (int i = 0; i < newNode->nr_players; i++)
    {
 
        newNode->jucatori[i].firstName = (char *)malloc(50);
        newNode->jucatori[i].secondName = (char *)malloc(50);
        fscanf(f, "%s", newNode->jucatori[i].firstName);
        fscanf(f, "%s", newNode->jucatori[i].secondName);
        fscanf(f, "%d", &newNode->jucatori[i].points);
    }
    newNode->puncte = punctaj(newNode);
    newNode->next = *head;
    *head = newNode;
}
 
void afisareLista(Node *head, FILE *g)      //afisarea listei de la ex1
{
    while (head != NULL)
    {
        
        for (int i = 0; i < strlen(head->nume_echipa); i++)                     //pentru stergerea spatiului de dupa numele unei echipe
            if (i == strlen(head->nume_echipa) && head->nume_echipa[i] == ' ')
                break;
            else
                fprintf(g, "%c", head->nume_echipa[i]);
        head->nume_echipa[strcspn(head->nume_echipa, "\r")] = 0;        //pentru ca scrierea sa se faca pe aceeasi linie pt ex3 (am vazut pe internet de carriage return)
        head->nume_echipa[strcspn(head->nume_echipa, "\n")] = 0;        //pentru stergerea tabului
        head = head->next;
    }
}
 
void stergereElement(Node **head, char *nume)   //stergerea unei echipe
{
    Node *c;
    Node *copie;
 
    if ((*head)->next == NULL)
        return;
 
    if (strcmp((*head)->nume_echipa, nume) == 0)
    {
        c = (*head);
        (*head) = (*head)->next;
        free(c);
        return;
    }
 
    copie = *head;
 
    while (copie->next != NULL)
    {
        if (strcmp(copie->next->nume_echipa, nume) == 0)
        {
            c = copie->next;
            copie->next = copie->next->next;
 
            free(c);
            return;
        }
 
        copie = copie->next;
    }
}
 
int numarMaxim(int numarEchipe)     //calculul numarului N, adica exponentul pentru care 2 la puterea N este maxim si mai mic decat numarul de echipe
{
 
    int N = 0;
    while (pow(2, N) <= numarEchipe)
    {
        N++;
    }
    N--;
    return N;
}
 
float minim(Node *head)     //calculul punctajului minim
{
 
    float min = INT_MAX;
    Node *copie = head;
    while (copie != NULL)
    {
 
        float aux = punctaj(copie);
        // printf("Aux: %f", aux);
        if (aux < min)
            min = aux;
 
        copie = copie->next;
    }
 
    return min;
}
 
int coadaGoala(Queue *q)
{
 
    return (q->fata == NULL);
}
 
Queue *creareCoada()
{
 
    Queue *q;
    q = (Queue *)malloc(sizeof(Queue));
 
    if (q == NULL)
        return NULL;
    q->fata = q->spate = NULL;
    return q;
}
 
void adaugareCoada(Queue *q, Node *primaEchipa, Node *aDouaEchipa)  //adaugarea echipelor in coada, 2 cate 2
{
 
    EchipeCoada *newNode = (EchipeCoada *)malloc(sizeof(EchipeCoada));
 
    newNode->echipa1 = primaEchipa;
    newNode->echipa2 = aDouaEchipa;
 
    newNode->next = NULL;
 
    if (q->spate == NULL)
    {
 
        q->spate = newNode;
    }
    else
    {
 
        (q->spate)->next = newNode;
        q->spate = newNode;
    }
    if (q->fata == NULL)
        q->fata = q->spate;
}
 
void adaugareStiva(Stiva **top, Node *echipa)
{
 
    Stiva *newNode = (Stiva *)malloc(sizeof(Stiva));
    newNode->echipa = echipa;
    newNode->next = *top;
    *top = newNode;
}
 
void meci(Stiva **topW, Stiva **topL, Node *echipa1, Node *echipa2)   //functie pentru un meci dintre 2 echipe
{
 
    if (echipa1->puncte > echipa2->puncte)
    {
 
        echipa1->puncte++;
        adaugareStiva(topW, echipa1);
        adaugareStiva(topL, echipa2);
    }
    else
    {
 
        echipa2->puncte++;
        adaugareStiva(topW, echipa2);
        adaugareStiva(topL, echipa1);
    }
}
 
void stiveCastigPierz(Queue *q, Stiva **topWin, Stiva **topLose)  //creare stivelor de castigatori si de pierzatori
{
 
    EchipeCoada *aux;
    Node *primaEchipa, *aDouaEchipa;
 
    if (coadaGoala(q))
        return;
 
    aux = q->fata;
    primaEchipa = aux->echipa1;
    aDouaEchipa = aux->echipa2;
    meci(topWin, topLose, primaEchipa, aDouaEchipa);
 
    q->fata = (q->fata)->next;
    free(aux);
}
 
int stivaGoala(Stiva *top)
{
    return (top == NULL);
}
 
Node *pop(Stiva **top)   //eliminare element din stiva
{
 
    Stiva *temp = (*top);
    Node *aux;
    aux = temp->echipa;
    (*top) = (*top)->next;
 
    return aux;
}
 
void afisareStiva(Stiva *top, FILE *g)   //afisarea echipelor castigatoare dupa o runda
{
    while (top != NULL)
    {
        fprintf(g, "%s", top->echipa->nume_echipa);
        for (int i = 0; i < 34 - strlen(top->echipa->nume_echipa); i++)
            fprintf(g, " ");
        fprintf(g, "-  %.2f\n", top->echipa->puncte);
 
        top = top->next;
    }
}
 
void stergereStiva(Stiva **top)
{
 
    Stiva *temp;
    while ((*top) != NULL)
    {
 
        temp = (*top);
        (*top) = (*top)->next;
        free(temp);
    }
}
 
void stergereCoada(Queue *q)
{
 
    EchipeCoada *aux;
    while (!coadaGoala(q))
    {
 
        aux = q->fata;
        q->fata = q->fata->next;
        free(aux);
    }
    free(q);
}
 
void adLista8Echipe(Node **head, Node *echipa)  //adaugare la sfarsit pentru lista cu primele 8 echipe din concurs
{
    Node *aux = *head;
    
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->nume_echipa = malloc(50);
    newNode->jucatori = (Player*)malloc(newNode->nr_players * sizeof(Player));
    for(int i = 0; i < newNode->nr_players; i ++) {
        newNode->jucatori->firstName = malloc(50);
        newNode->jucatori->secondName = malloc(50);
    }
    newNode = echipa;
    
    if(*head == NULL) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode = echipa;
        newNode->next = *head;
        *head = newNode;
    }
    else {

        while(aux->next != NULL)
            aux = aux->next;
        aux->next = newNode;
        newNode->next = NULL;
    }
}
 
void afisareEchipe(Queue *q, FILE *g)   //afisarea echipelor din coada
{
    EchipeCoada *aux;
    Node *primaEchipa, *aDouaEchipa;
    if (coadaGoala(q))
        return;
 
    aux = q->fata;
    primaEchipa = aux->echipa1;
    aDouaEchipa = aux->echipa2;
 
    fprintf(g, "%s", primaEchipa->nume_echipa);
    for (int i = 0; i < 33 - strlen(primaEchipa->nume_echipa); i++)
        fprintf(g, " ");
    fprintf(g, "-");
    for (int j = 0; j < 33 - strlen(aDouaEchipa->nume_echipa); j++)
        fprintf(g, " ");
    fprintf(g, "%s\n", aDouaEchipa->nume_echipa);
 
    q->fata = q->fata->next;
 
    free(aux);
}
 
Queue *createCoada(Node *head)
{
    Queue *q;
    q = creareCoada();
 
    while (head != NULL)
    {
 
        adaugareCoada(q, head, head->next);
        head = (head->next)->next;
    }
 
    return q;
}
 
void meciIntreg(FILE *g, Node *head, int numarEchipe, Stiva **topW, Stiva **topL, Node **lista8) //functia pentru meci pana la aflarea castigatorului
{
    int runda = 1;
    Queue *q = createCoada(head);
    Queue *aux1 = createCoada(head);
    
    topW = (Stiva **)malloc(sizeof(Stiva *));
    topL = (Stiva **)malloc(sizeof(Stiva *));
    lista8 = (Node **)malloc(sizeof(Node*));
    *topW = NULL;
    *topL = NULL;
    

    while (numarEchipe > 1)
    {
        if (runda == 1)                     //coada o formez din lista initiala
        {
            fprintf(g, "--- ROUND NO:1\n");
 
            while (!coadaGoala(aux1))
            {
                afisareEchipe(aux1, g);
            }
 
            while (!coadaGoala(q))
            {
                stiveCastigPierz(q, topW, topL);
            }
 
            fprintf(g, "\n");
 
            fprintf(g, "WINNERS OF ROUND NO:1\n");
 
            afisareStiva(*topW, g);
 
            stergereCoada(q);
            stergereCoada(aux1);
        }
        else                                //coada o formez din stiva de castigatori
        {   
            q = creareCoada();
            aux1 = creareCoada();
            fprintf(g, "--- ROUND NO:%d\n", runda);

            while (!stivaGoala(*topW))
            {
                Node *echipaAux1 = NULL;
                Node *echipaAux2 = NULL;
 
                echipaAux1 = pop(topW);
 
                echipaAux2 = pop(topW);
 
                adaugareCoada(q, echipaAux1, echipaAux2);
                adaugareCoada(aux1, echipaAux1, echipaAux2);
                
            }
           
            while (!coadaGoala(aux1))
            {
                afisareEchipe(aux1, g);
            }
 
            while (!coadaGoala(q))
            {
                stiveCastigPierz(q, topW, topL);
                
            }

            // if(numarEchipe == 8) {
                    
            //     Stiva *copieW = *topW;
            //     for(int i = 0; i < 8; i++) {

            //         Node *echipa = pop(&copieW);
            //         printf("%s", echipa->nume_echipa);
            //         //adLista8Echipe(lista8, echipa);
            //     }

            // }
    

            fprintf(g, "\n");
 
            fprintf(g, "WINNERS OF ROUND NO:%d\n", runda);
 
            afisareStiva(*topW, g);
            stergereCoada(q);
            stergereCoada(aux1);
        }
        numarEchipe = numarEchipe / 2;
        runda++;
        fprintf(g, "\n");
    }
    stergereStiva(topL);
    stergereStiva(topW);
}

BST *newNode(Node *echipa) {                    //functie pentru adaugare unui singur nod

    BST *node = (BST*)malloc(sizeof(BST));
    node->echipa = echipa;
    node->left = node->right = NULL;
    return node;
}

BST *inserareArbore(BST *node, Node *team) {    //functie pentru inserare in arbore binar de cautare

    if(node == NULL) 
        return newNode(team);
    if(team->puncte < node->echipa->puncte)
        node->left = inserareArbore(node->left, team);
    else if(team->puncte > node->echipa->puncte)
            node->right = inserareArbore(node->right, team);
        else if(strcmp(team->nume_echipa, node->echipa->nume_echipa) < 0)
                    node->left = inserareArbore(node->left, team);
            else node->right = inserareArbore(node->right, team);
    return node;
}

void inordine(BST *root, FILE *g) {             //functie pentru afisare in ordine descrescatoare

    if(root) {

        inordine(root->left,g);
        fprintf(g, "%s", root->echipa->nume_echipa);
        for (int i = 0; i < 34 - strlen(root->echipa->nume_echipa); i++)
            fprintf(g, " ");
        fprintf(g, "-  %.2f\n", root->echipa->puncte);
        inordine(root->right,g);
    }
}

int main(int argc, char **argv)
{
 
    Node *head = NULL;
    int i, nrEchipe, Nmax;
    float mini;
    FILE *f, *c, *output;
    int cerinte[5];
    f = fopen(argv[2], "r");
 
    if (f == NULL)
    {
        printf("Fisierul nu poate fi deschis\n");
        exit(1);
    }
 
    c = fopen(argv[1], "r");
    if (c == NULL)
    {
        printf("Fisierul nu se poate deschide!\n");
        exit(1);
    }
 
    for (int i = 0; i < 5; i++)
    {
 
        fscanf(c, "%d", &cerinte[i]);
    }
 
    output = fopen(argv[3], "w");
    if (output == NULL)
    {
        printf("Fisierul nu poate fi deschis\n");
        exit(1);
    }
 
    fscanf(f, "%d", &nrEchipe);
    for (i = 0; i < nrEchipe; i++)
        addAtBeginning(&head, f);
 
    if (cerinte[0] == 1 && cerinte[1] == 0)
        afisareLista(head, output);
 
    if (cerinte[1] == 1)
    {
        Node *copie;
        Nmax = numarMaxim(nrEchipe);
 
        while (nrEchipe != pow(2, Nmax))
        {
            mini = minim(head);
            copie = head;
 
            while (copie != NULL)
            {
 
                if (copie->puncte == mini)
                {
                    stergereElement(&head, copie->nume_echipa);
                    nrEchipe--;
                    break;
                }
                copie = copie->next;
            }
        }
 
        afisareLista(head, output);
        fprintf(output, "\n");
    }
    if (cerinte[2] == 1)
    {
        Stiva *topW, *topL;
        Node *lista8 = NULL;
        meciIntreg(output, head, nrEchipe, &topW, &topL, &lista8);
    }
    fclose(f);
    fclose(c);
    fclose(output);
    return 0;
}