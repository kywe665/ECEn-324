/* tsbase.c    last modified: 2 Mar 2011       James Archibald

   This program solves the traveling salesman problem (complete
   circuit ending at the starting node) for a specific graph. This is
   a fairly straightforward single-process, single-thread version to
   serve as a baseline for comparision with parallel versions.  

   To simplify things for the user, the graph is hardwired in this
   source code rather than being read from a file. The graph was
   chosen to give an execution time in the range of about one minute
   or less, depending on the execution platform.  All distances in the
   graph are Cartesian and computed from the node coordinates.

   The actual heavy-lifting is done by a recursive routine that starts
   with a given path and systematically considers all possible
   extensions to that path that result by taking an edge out of the
   current node. The algorithm is based on the assumption that, while
   nodes may be revisited in an optimal path, each edge should be
   taken at most once. The recursive calls in search() terminate if
   the current pathlength is longer than the pathlength of the best
   solution found so far. A new solution is found if the pathlength is
   better than the previous best, if all nodes have been visited, and
   the ending node is the same as the starting node. The discovery of
   each new "best path" generates a line of output, including its
   length. When the program finishes, it outputs the best path it
   found, in terms of the sequence of nodes visited.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUMNODES  18   /* total nodes in graph */
#define STARTNODE  8   /* node to start tour from */
#define ARRAYSZ  100   /* number of nodes in longest path */

/* this represents edges in the graph */
typedef struct adjstr *Adjptr;
typedef struct adjstr
{
    int nodeindex;
    int amark;
    double edgelength;
    Adjptr next;
}   Adjacent;

/* this represents nodes in the graph */
typedef struct
{
    int x;
    int y;
    int nmark;
    Adjptr adjlist;
}   Node;

Node nodearray[NUMNODES];   /* one entry for each node */
int nummarkednodes;
int totalnumnodes;
int startnodeindex;

int currpatharray[ARRAYSZ];
int currpathindex;
int bestpatharray[ARRAYSZ];
int bestpathindex;
double bestpathlen = -1.0;

double distance(int a, int b)
{
    double tmp1, tmp2;
    tmp1 = nodearray[a].x - nodearray[b].x;
    tmp2 = nodearray[a].y - nodearray[b].y;
    return sqrt(tmp1*tmp1 + tmp2*tmp2);
}

Adjptr createadjlist(int *adjarray)
{
    /* arg is (-1)-terminated array of ints, each an index to
     * nodearray of all adjacent nodes to this node */
    Adjptr head = NULL;
    Adjptr prev = NULL;
    Adjptr tmp;
    int i = 0;
    
    while (adjarray[i] >= 0)
    {
	tmp = (Adjptr) malloc(sizeof(Adjacent));
	tmp->nodeindex = adjarray[i];
	tmp->amark = 0;
	tmp->next = NULL;   /* may shortly be overwritten */
	if (head == NULL)
	    head = tmp;
	if (prev == NULL)
	    prev = tmp;
	else 
	{
	    prev->next = tmp;
	    prev = tmp;
	}
	i++;
    }
    return head;
}

void createnode(int x, int y, int *adjarray)
{
    static int nodearrayindex = 0;
    int i;
    if (nodearrayindex >= NUMNODES)
    {
	fprintf(stderr, "exceeded size of nodearray!\n");
	exit(-1);
    }
    nodearray[nodearrayindex].x = x;
    nodearray[nodearrayindex].y = y;
    nodearray[nodearrayindex].nmark = 0;
    nodearray[nodearrayindex].adjlist = createadjlist(adjarray);
    nodearrayindex++;
}

void creategraph(void)
{
    /* fill up nodearray with desired nodes: for this benchmark, the
     * details of the graph are hardwired rather than read from
     * file. the graph details are:
       0 (0,0)      {6,11,17}
       1 (100,0)    {6,17}
       2 (200,0)    {8,13}
       3 (300,0)    {5,16}
       4 (400,0)    {12,14,17}
       5 (0,100)    {3,9}
       6 (100,100)  {0,1}
       7 (200,100)  {11,12,15}
       8 (300,100)  {2,14}
       9 (400,100)  {5,11,14}
       10 (0,200)   {13,15}
       11 (100,200) {0,7,9,12,13,16}
       12 (200,200) {4,7,11,13}
       13 (300,200) {2,10,11,12}
       14 (400,200) {4,8,9}
       15 (0,300)   {7,10,16}
       16 (100,300) {3,11,15}
       17 (200,300) {0,1,4}
    */

    int i,x,y,a[NUMNODES];
    Adjptr tmp;

    /* node 0 */
    x = 0; y = 0; a[0] = 6; a[1] = 11; a[2] = 17; a[3] = -1;
    createnode(x,y,a);
    
    /* node 1 */
    x = 100; y = 0; a[0] = 6; a[1] = 17; a[2] = -1;
    createnode(x,y,a);
    
    /* node 2 */
    x = 200; y = 0; a[0] = 8; a[1] = 13; a[2] = -1;
    createnode(x,y,a);
    
    /* node 3 */
    x = 300; y = 0; a[0] = 5; a[1] = 16; a[2] = -1;
    createnode(x,y,a);
    
    /* node 4 */
    x = 400; y = 0; a[0] = 12; a[1] = 14; a[2] = 17; a[3] = -1;
    createnode(x,y,a);
    
    /* node 5 */
    x = 0; y = 100; a[0] = 3; a[1] = 9; a[2] = -1;
    createnode(x,y,a);
    
    /* node 6 */
    x = 100; y = 100; a[0] = 0; a[1] = 1; a[2] = -1;
    createnode(x,y,a);
    
    /* node 7 */
    x = 200; y = 100; a[0] = 11; a[1] = 12; a[2] = 15; a[3] = -1;
    createnode(x,y,a);
    
    /* node 8 */
    x = 300; y = 100; a[0] = 2; a[1] = 14; a[2] = -1;
    createnode(x,y,a);
    
    /* node 9 */
    x = 400; y = 100; a[0] = 5; a[1] = 11; a[2] = 14; a[3] = -1;
    createnode(x,y,a);
    
    /* node 10 */
    x = 0; y = 200; a[0] = 13; a[1] = 15; a[2] = -1;
    createnode(x,y,a);
    
    /* node 11 */
    x = 100; y = 200; a[0] = 0; a[1] = 7; a[2] = 9; a[3] = 12; a[4] = 13; a[5] = 16; a[6] = -1;
    createnode(x,y,a);
    
    /* node 12 */
    x = 200; y = 200; a[0] = 4; a[1] = 7; a[2] = 11; a[3] = 13; a[4] = -1;
    createnode(x,y,a);
    
    /* node 13 */
    x = 300; y = 200; a[0] = 2; a[1] = 10; a[2] = 11; a[3] = 12; a[4] = -1;
    createnode(x,y,a);
    
    /* node 14 */
    x = 400; y = 200; a[0] = 4; a[1] = 8; a[2] = 9; a[3] = -1;
    createnode(x,y,a);
    
    /* node 15 */
    x = 0; y = 300; a[0] = 7; a[1] = 10; a[2] = 16; a[3] = -1;
    createnode(x,y,a);
    
    /* node 16 */
    x = 100; y = 300; a[0] = 3; a[1] = 11; a[2] = 15; a[3] = -1;
    createnode(x,y,a);
    
    /* node 17 */
    x = 200; y = 300; a[0] = 0; a[1] = 1; a[2] = 4; a[3] = -1;
    createnode(x,y,a);

    /* now that all node are set up, compute edge lengths */
    for (i = 0; i < NUMNODES; i++)
    {
	tmp = nodearray[i].adjlist;
	while (tmp != NULL)
	{
	    tmp->edgelength = distance(i,tmp->nodeindex);
	    tmp = tmp->next;
	}
    }
}

double estpathlength(void)
{
    /* run through nodearray and determine estimate of max path
     * length based on twice the longest path from each node
     * (obviously not a tight bound) */
    int i;
    double sum, max;
    Adjptr tmp;
    
    sum = 0.0;
    for (i = 0; i < totalnumnodes; i++)
    {
	max = 0.0;
	tmp = nodearray[i].adjlist;
	while (tmp != NULL)
	{
	    if (tmp->edgelength > max)
		max = tmp->edgelength;
	    tmp = tmp->next;
	}
	sum = sum + (2.0) * max;
    }
    return sum;
}

/* calls itself recursively to check all possible paths */
void search(int currnode, double currpathlen)
{
    int i;
    Adjptr adjtmp;

    if (currpathlen >= bestpathlen) 
	return;
    /* have to have visited all nodes AND return to starting node */
    if (nummarkednodes == totalnumnodes && 
	currpatharray[currpathindex-1] == startnodeindex)
    {
	/* success: better path found !! */
	printf("\tNew path: ");
	printf("[len=%.1f]\n", currpathlen);

	/* copy to best */
	for (i = 0; i < currpathindex; i++)
	    bestpatharray[i] = currpatharray[i];
	bestpathindex = currpathindex;
	bestpathlen = currpathlen;
	return;
    }
    
    adjtmp = nodearray[currnode].adjlist;
    while (adjtmp != NULL)
    {
	if (adjtmp->amark == 0)
	{   
	    if (nodearray[adjtmp->nodeindex].nmark == 0)
	    {
		nodearray[adjtmp->nodeindex].nmark = 1;
		nummarkednodes++;
		adjtmp->amark = 1;
		currpatharray[currpathindex++] = adjtmp->nodeindex;
		search(adjtmp->nodeindex, currpathlen+adjtmp->edgelength);
		currpathindex--;
		nodearray[adjtmp->nodeindex].nmark = 0;
		nummarkednodes--;
		adjtmp->amark = 0;
	    }
	    else
	    {
		adjtmp->amark = 1;
		currpatharray[currpathindex++] = adjtmp->nodeindex;
		search(adjtmp->nodeindex, currpathlen+adjtmp->edgelength);
		currpathindex--;
		adjtmp->amark = 0;
	    }
	}
	adjtmp = adjtmp->next;
    }
    /* else just return: tried all possible edges out */
}

int main(int argc, char *argv[])
{
    int i;

    printf("Traveling salesman, sequential version\n");
    totalnumnodes = NUMNODES;
    startnodeindex = STARTNODE;
    creategraph();

    /* compute initial bound for path length */
    bestpathlen = estpathlength();

    /* init state and start search */
    nodearray[startnodeindex].nmark = 1;
    nummarkednodes = 1;
    currpathindex = 0;
    bestpathindex = 0;
    currpatharray[currpathindex++] = startnodeindex;
    search(startnodeindex, 0.0);

    /* output best path at end */
    printf("Best path: [len=%.1f] ", bestpathlen);
    for (i = 0; i < bestpathindex; i++)
	printf("%d ",bestpatharray[i]);
    printf("\n");
}
