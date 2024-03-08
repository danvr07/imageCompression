/*Vrinceanu Dan 315 CD*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functii.h"

int main(int argc, char const *argv[])
{
    int prag = atoi(argv[2]);
    FILE *infile = fopen(argv[3], "rb");
    FILE *outfile = fopen(argv[4], "wb");

    if (infile == NULL) {
        printf("Eroare\n");
        return -1;
    }

    int i = 0, j = 0, max_siz[1] = {0};
    char tip[10];
    unsigned int size1, size2, max_color;

    fscanf(infile, "%s %d %d %d", tip, &size1, &size2, &max_color);
    fseek(infile, 1, SEEK_CUR);

    // Se aloca memorie pentru matricea initiala
    pixel **mat = (pixel **)malloc(size1 * sizeof(pixel *));
    if (mat == NULL) {
        printf("EROARE");
    }

    for (i = 0; i < size1; i++) {
        mat[i] = (pixel *)malloc(size2 * sizeof(pixel));
        if (mat[i] == NULL) {
            printf("EROARE");
        }
    }

    for (i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            fread(&mat[i][j].red, sizeof(unsigned char), 1, infile);
            fread(&mat[i][j].green, sizeof(unsigned char), 1, infile);
            fread(&mat[i][j].blue, sizeof(unsigned char), 1, infile);
        }
    }

    QuadTreeNode *root = create();

    pixel **A11 = NULL;
    pixel **A12 = NULL;
    pixel **A21 = NULL;
    pixel **A22 = NULL;

    // Se apeleaza functia pentru a se creea arborele cuaternar(4 copii)

    img_decompresie(root, mat, A11, A11, A21, A22, size1, max_siz, prag);

    // Cerinta 1

    if (strcmp(argv[1], "-c1") == 0) {

        int max = maxNivel(root);
        int frunz = nrFrunze(root);

        fprintf(outfile, "%d\n", max);
        fprintf(outfile, "%d\n", frunz);
        fprintf(outfile, "%d\n", max_siz[0]);
    }

    // Cerinta 2

    if (strcmp(argv[1], "-c2") == 0) {
        fwrite(&size1, sizeof(unsigned int), 1, outfile);
        tree_nivel(root, outfile);
    }

    // Se dealoca memoria

    free_mat(mat, size1);
    freeQuadTree(root);

    fclose(infile);
    fclose(outfile);

    return 0;
}