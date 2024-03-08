/*Vrinceanu Dan 315 CD*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

typedef struct QuadTreeNode
{
    struct QuadTreeNode *child[4];
    struct pixel pix;
    unsigned int tip;
} QuadTreeNode;

typedef struct TNode
{
    struct TNode *next;
    QuadTreeNode *treeNode;
} TNode;

typedef struct TCoada
{
    TNode *head;
    TNode *tail;
} TCoada;

QuadTreeNode *create()
{
    QuadTreeNode *root = (QuadTreeNode *)malloc(sizeof(QuadTreeNode));
    if (root == NULL) {
        printf("EROARE");
    }

    for (int i = 0; i < 4; i++) {
        root->child[i] = NULL;
    }
    return root;
}

// Functie pentru a eliberea memoria din matrici

void free_mat(pixel **A, unsigned int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        free(A[i]);
    }
    free(A);
}
// Functie pentru a elibera memoria din coada

void freeQueue(TCoada *coada)
{
    while (coada->head != NULL)
    {
        TNode *tmp = coada->head->next;
        free(coada->head);
        coada->head = tmp;
    }
    free(coada);
}

void img_decompresie(QuadTreeNode *root, pixel **A, pixel **A11, pixel **A12,
        pixel **A21, pixel **A22, unsigned int size1, int max_siz[], int prag)
{
    unsigned long long red = 0, green = 0, blue = 0, mean = 0;
    unsigned int size = size1 * size1;
    int i = 0, j = 0;

    // Se calculeaza scorul si valoarea medie 

    for (i = 0; i < size1; i++) {
        for (j = 0; j < size1; j++) {
            red += A[i][j].red;
            green += A[i][j].green;
            blue += A[i][j].blue;
        }
    }
    red = red / size;
    green = green / size;
    blue = blue / size;

    for (i = 0; i < size1; i++) {
        for (j = 0; j < size1; j++) {
            mean += (red - A[i][j].red) * (red - A[i][j].red) +
                    (green - A[i][j].green) * (green - A[i][j].green) +
                    (blue - A[i][j].blue) * (blue - A[i][j].blue);
        }
    }
    mean = mean / (3 * size);

    // Daca scorul este mai mic decat pragul
    // Atunci nodul va fi frunza(nu va mai avea copii)

    if (mean <= prag) {

        root->tip = 1;
        root->pix.red = red;
        root->pix.green = green;
        root->pix.blue = blue;

        if (max_siz[0] < size1) {
            max_siz[0] = size1;
        }

    } else {

        // Daca scorul este mai mic decat pragul
        // Se imparte matricea in alte 4 matrici

        root->tip = 0;

        // Se aloca memorie pentru alte 4 matrici

        A11 = (pixel **)malloc(size1 / 2 * sizeof(pixel *));
        A12 = (pixel **)malloc(size1 / 2 * sizeof(pixel *));
        A21 = (pixel **)malloc(size1 / 2 * sizeof(pixel *));
        A22 = (pixel **)malloc(size1 / 2 * sizeof(pixel *));

        if (A11 == NULL && A12 == NULL && A21 == NULL && A22 == NULL) {
            printf("EROARE");

            return;
        }

        for (i = 0; i < size1 / 2; i++) {
            A11[i] = (pixel *)malloc(size1 / 2 * sizeof(pixel));
            A12[i] = (pixel *)malloc(size1 / 2 * sizeof(pixel));
            A21[i] = (pixel *)malloc(size1 / 2 * sizeof(pixel));
            A22[i] = (pixel *)malloc(size1 / 2 * sizeof(pixel));

            if (A11[i] == NULL && A12[i] == NULL &&
                A21[i] == NULL && A22[i] == NULL) {
                printf("EROARE");
                return;
            }
        }

        // Fiecare matrice primeste a 4 parte din matricea initiala

        for (i = 0; i < size1 / 2; i++) {
            for (j = 0; j < size1 / 2; j++) {
                A11[i][j] = A[i][j];
                A12[i][j] = A[i][j + size1 / 2];
                A21[i][j] = A[i + size1 / 2][j];
                A22[i][j] = A[i + size1 / 2][j + size1 / 2];
            }
        }

        // Se aloca memorie pentru nodurile copil

        for (i = 0; i < 4; i++) {
            root->child[i] = create();
        }

        img_decompresie(root->child[0], A11, A11, A12,
                        A21, A22, size1 / 2, max_siz, prag);

        img_decompresie(root->child[1], A12, A11, A12,
                        A21, A22, size1 / 2, max_siz, prag);

        img_decompresie(root->child[2], A22, A11, A12,
                        A21, A22, size1 / 2, max_siz, prag);

        img_decompresie(root->child[3], A21, A11, A12,
                        A21, A22, size1 / 2, max_siz, prag);

        // Se elibereaza memoria pentru toate matricile

        free_mat(A11, size1 / 2);
        free_mat(A12, size1 / 2);
        free_mat(A21, size1 / 2);
        free_mat(A22, size1 / 2);
    }
}

// Functia calculeaza recursiv nivelul cel mai mare din arbore

int maxNivel(QuadTreeNode *root)
{
    int max = 0, nivel = 0;

    if (root != NULL) {
        for (int i = 0; i < 4; i++) {
            nivel = maxNivel(root->child[i]);

            if (nivel < max) {
                max = max;
            } else {
                max = nivel;
            }
        }
        return max + 1;
    }
    return 0;
}

// Functia calculeaza cate frunze are arborele

int nrFrunze(QuadTreeNode *root) 
{
    int nr = 0;

    if (root != NULL) {

        if (root->tip == 1) {
            nr++;
        }

       for(int i = 0; i < 4; i++) {
            nr += nrFrunze(root->child[i]);
        }

        return nr;
    }

    return 0;
}

/* Funcție pentru a crea o coadă goală */
TCoada *create_queue()
{
    TCoada *queue = (TCoada *)malloc(sizeof(TCoada));
    queue->head = queue->tail = NULL;

    return queue;
}

// Functie pentru a insera nodurile arborelui in coada

void enqueue(TCoada *queue, QuadTreeNode *treeNode)
{
    TNode *new_node = (TNode *)malloc(sizeof(TNode));
    new_node->treeNode = treeNode;
    new_node->next = NULL;
    if (queue->head == NULL)
    {
        queue->head = queue->tail = new_node;
    }
    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
}

void freeQuadTree(QuadTreeNode *root)
{
    if (root != NULL) {
        freeQuadTree(root->child[0]);
        freeQuadTree(root->child[1]);
        freeQuadTree(root->child[2]);
        freeQuadTree(root->child[3]);

        free(root);
    }
}

void tree_nivel(QuadTreeNode *root, FILE *outfile)
{

    // Se adauga primul nod in coada
    TCoada *queue = create_queue();
    enqueue(queue, root);

    while (queue->head != NULL) {
        TNode *tmp = queue->head;
        // Se scoate primul nod din coada

        QuadTreeNode *current = tmp->treeNode;
        queue->head = queue->head->next;

        free(tmp);

        // Daca nodul extras din coada este de tip 1, se introduce tipul
        // si datele culorilor rgb, daca nu, doar tipul
        if (current->tip == 1) {
            fwrite(&current->tip, sizeof(unsigned char), 1, outfile);
            fwrite(&current->pix.red, sizeof(unsigned char), 1, outfile);
            fwrite(&current->pix.green, sizeof(unsigned char), 1, outfile);
            fwrite(&current->pix.blue, sizeof(unsigned char), 1, outfile);
        } else {
            fwrite(&current->tip, sizeof(unsigned char), 1, outfile);
        }

        // Daca exista copii, se adauga in coada
        if (current->tip == 0) {
            for (int i = 0; i < 4; i++) {
                enqueue(queue, current->child[i]);
            }
        }
    }
    // Se elibereaza memoria din coada
    freeQueue(queue);
}
