#include <stdio.h>
#include "header.h"

int path(adjmat mat,  int u, int v)
{
	int i; /* loop variable */

	if(u < 0 || u >= N || v < 0 || v >= N)
	{
		return FALSE; /* out of range */
	}
	if(u == v)
	{
		return TRUE; /* found a path */
	}

	for(i=0; i < N; i++)
	{
		if (mat[u][i] == 1) /* There is a path from u to i */
		{
			if (path(mat, i, v)) /* Check recursively if there is a path from i to v */
			{
				return TRUE; /* Path found */
			}
		}
	}
	return FALSE; /* no path found */
}
void printMatrix(adjmat mat)
{
	/* print the matrix */
	int i, j; /* loop variables */
	for(i=0; i < N; i++)
	{
		for(j=0; j < N; j++)
		{
			printf("%d ", mat[i][j]); /* print the value */
			if(j != N-1)
			{
				printf("\t"); /* separate columns */
			}else
			{
				printf("\n"); /* separate rows */
			}
		}
	}
}
void fillMatrix(adjmat mat)
{
	int i, j; /* loop variables */
	/* fill the matrix with values from the user */
	printf("Enter values for %d:%d adjacency matrix:\n", N, N);
	for(i=0; i < N; i++)
	{
		for(j=0; j < N; j++)
		{
			scanf("%d", &mat[i][j]); /* read the value */
		}
	}
}
int main()
{
	int startNode, endNode; /* the nodes to check if there is a path between them */
	adjmat mat; /* declare an adjacency matrix */
	fillMatrix(mat); /* fill the matrix with values from the user */
	printf("The adjacency matrix you entered:\n");
	printMatrix(mat); /* print the matrix to the console */

	printf("\nEnter two nodes to check if one is a ancestor of the other, enter -1 -1 or EOF to stop:\n");

	while(scanf("%d %d", &startNode, &endNode) == 2 && startNode !=-1 && endNode != -1)
	{
		printf("Checking if %d is an ancestor of %d\n", startNode, endNode);
		if(path(mat, startNode, endNode))
			printf("%d is an ancestor of %d\n", startNode, endNode);
		else
			printf("%d is not an ancestor of %d\n", startNode, endNode);

		printf("\nEnter two nodes to check if there is a path between them, enter -1 -1 or EOF to stop:\n");
	}
	return 0;
}