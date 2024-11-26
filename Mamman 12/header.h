#ifndef HEADER_H
#define HEADER_H

#define FALSE 0 /* Boolean values */
#define TRUE 1 /* Boolean values */
#define N 11 /* Number of nodes in tree */

typedef int adjmat[N][N]; /* type for N*N neighboring matrix */

/* This method checks if one node in a tree is an ancestor or a direct parent of another.
 * It operates on an adjacency matrix representing the tree, where each row and column correspond
 * to nodes. The method returns TRUE (1) if the first node is an ancestor or parent of the second,
 * FALSE (0) otherwise. If the nodes are the same, it returns TRUE. If either node is out of bounds,
 * it returns FALSE.
 */
int path(adjmat mat, int u, int v);

/* This method prints an adjacency matrix to the console. */
void printMatrix(adjmat mat);

/* This method fills an adjacency matrix with values from the user. */
void fillMatrix(adjmat mat);

#endif
