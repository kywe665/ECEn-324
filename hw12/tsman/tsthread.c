/* tsthread.c    last modified: 2 Mar 2011       James Archibald

   This program is a multi-threaded version of the code to solve the
   traveling salesman problem. It is based on the sequential version
   in tsbase.c and the multi-process version in tsprocess.c. In
   particular, it uses an identical means to divide up the work to be
   done, using the prefix pool.

   In creating a multi-process version, one challenge is to create
   variables that can be shared. In a multi-threaded version, the
   challenge is that all globals are shared. Thus, data structures
   that represent, for example, the current path during searching,
   must somehow be made local for each thread. To avoid passing too
   many parameters (and increasing the cost of recursion), they are
   retained as global variables but replicated so that each thread
   has its own. This is the motivation behind the creation of the
   Threadglobals struct -- it is an identical collection of global
   values for each struct indexed by the thread ID (tid) of the code
   using it.

   As was the case with the multi-process code, this code does not
   address synchronization problems with shared variables in order to
   keep the code simple. In practice, problems are unlikely and would
   probably result in nothing more serious than a particular prefix
   being searched multiple times. To be safe, one should use
   semaphores to enforce mutual exclusion.
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

/* These parameters directly control the parallelism */
/* Default: 4 threads, 5 node prefix */ 
#define NUMTHREADS 4   /* number of thread to create */
#define PREFLEN    5   /* length of stored prefixes to split up work */

/* Leave these as they are unless you input a different graph */
#define NUMNODES  18   /* total nodes in graph */
#define STARTNODE  8   /* node to start tour from */
#define ARRAYSZ  100   /* number of nodes in longest path */
#define MAXPREF 1000   /* max # of unique prefixes to store */

typedef struct adjstr *Adjptr;
typedef struct adjstr
{
    int nodeindex;
    int amark;
    double edgelength;
    Adjptr next;
}   Adjacent;

typedef struct
{
    int x;
    int y;
    int nmark;
    Adjptr adjlist;
}   Node;

typedef struct
{
    int currpatharray[ARRAYSZ];
    int bestpatharray[ARRAYSZ];
    int currpathindex;
    int bestpathindex;
    double mybestpathlen;
    int nummarkednodes;
    int nodemark[NUMNODES];  /* use instead of nmark */
    int edgemark[NUMNODES][NUMNODES]; /* use instead of amark */
}  Threadglobals;


Node nodearray[NUMNODES];
int totalnumnodes;
int startnodeindex;
int patharray[ARRAYSZ];
int pathindex;

double shbestpathlen;
int prefix[MAXPREF][PREFLEN];
int prefixindex;
double dist[MAXPREF];

/* tdata separates all writable data for each thread spatially */
Threadglobals tdata[NUMTHREADS];

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
    int j;
    
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

void fillprefix(int currnode, double currpathlen)
{
    int i;
    Adjptr adjtmp;
    
    if (pathindex == PREFLEN)
    {
	/* insert in prefix array */
	prefixindex++;
	for (i = 0; i < PREFLEN; i++)
	    prefix[prefixindex][i] = patharray[i];
	dist[prefixindex] = currpathlen;
	return;
    }
    
    adjtmp = nodearray[currnode].adjlist;
    while (adjtmp != NULL)
    {
	/* okay to revisit nodes, but don't repeat edges */
	if (adjtmp->amark == 0)
	{
	    adjtmp->amark = 1;
	    patharray[pathindex++] = adjtmp->nodeindex;
	    fillprefix(adjtmp->nodeindex,currpathlen+adjtmp->edgelength);
	    pathindex--;
	    adjtmp->amark = 0;
	}
	adjtmp = adjtmp->next;
    }
}

/* calls itself recursively to check all possible paths */
void search(int currnode, double currpathlen, long tid)
{
    int i;
    Adjptr adjtmp;

    if (currpathlen >= shbestpathlen)
	return;     /* not better than current best -- discontinue */

    /* otherwise, does path visit all nodes AND end at start node? */
    if (tdata[tid].nummarkednodes == totalnumnodes && 
	tdata[tid].currpatharray[tdata[tid].currpathindex-1] == startnodeindex)
    {
	/* best global path so far: record path and path length */
	shbestpathlen = currpathlen;
	tdata[tid].mybestpathlen = currpathlen;
	for (i = 0; i < tdata[tid].currpathindex; i++)
	    tdata[tid].bestpatharray[i] = tdata[tid].currpatharray[i];
	tdata[tid].bestpathindex = tdata[tid].currpathindex;
	printf("\tNew best path: [len=%.1f] [tid=%ld]\n", 
	       tdata[tid].mybestpathlen, tid);
	return;
    }
    
    adjtmp = nodearray[currnode].adjlist;
    while (adjtmp != NULL)
    {
	if (tdata[tid].edgemark[currnode][adjtmp->nodeindex] == 0)
	{
	    if (tdata[tid].nodemark[adjtmp->nodeindex]== 0)
	    {
		tdata[tid].nodemark[adjtmp->nodeindex] = 1;
		tdata[tid].nummarkednodes++;
		tdata[tid].edgemark[currnode][adjtmp->nodeindex] = 1;
		tdata[tid].currpatharray[tdata[tid].currpathindex++] = adjtmp->nodeindex;
		search(adjtmp->nodeindex, currpathlen+adjtmp->edgelength, tid);
		tdata[tid].currpathindex--;
		tdata[tid].nodemark[adjtmp->nodeindex] = 0;
		tdata[tid].nummarkednodes--;
		tdata[tid].edgemark[currnode][adjtmp->nodeindex] = 0;
	    }
	    else
	    {
		tdata[tid].edgemark[currnode][adjtmp->nodeindex] = 1;
		tdata[tid].currpatharray[tdata[tid].currpathindex++] = adjtmp->nodeindex;
		search(adjtmp->nodeindex, currpathlen+adjtmp->edgelength,tid);
		tdata[tid].currpathindex--;
		tdata[tid].edgemark[currnode][adjtmp->nodeindex] = 0;
	    }
	}
	adjtmp = adjtmp->next;
    }
    /* else just return: tried all possible edges out */
}

void *wrapper(void *arg)
{
    long tid;
    int i,tmp;
    Adjptr edge;

    /* display thread id */
    tid = (long) arg;
    printf("Thread %ld starting\n", tid);

    /* each process loops, getting next free prefix & searching it */
    tdata[tid].bestpathindex = 0;
    while ((tmp = prefixindex--) >= 0)
    {
	tdata[tid].nummarkednodes = 0;

	/* copy prefix, mark nodes and edges, and start search */
	for (i = 0; i < PREFLEN; i++)
	{
	    tdata[tid].currpatharray[i] = prefix[tmp][i];
	    if (tdata[tid].nodemark[tdata[tid].currpatharray[i]] == 0)
	    {   /* increment count of marked nodes */
		tdata[tid].nodemark[tdata[tid].currpatharray[i]] = 1;
		tdata[tid].nummarkednodes++;
	    }
	    /* mark edge out from this node, if not last in prefix */
	    if (i < PREFLEN-1)
	    {
		edge = nodearray[prefix[tmp][i]].adjlist;
		while (edge != NULL && edge->nodeindex != prefix[tmp][i+1])
		    edge = edge->next;
		if (edge == NULL)
		{
		    fprintf(stderr,"missing edge in graph\n");
		    exit(-1);
		}
		tdata[tid].edgemark[tdata[tid].currpatharray[i]][edge->nodeindex] = 1;
	    }
	}
	tdata[tid].currpathindex = PREFLEN;
	search(tdata[tid].currpatharray[tdata[tid].currpathindex-1], dist[tmp],tid);

	/* now loop again and undo marks from above */
	for (i = 0; i < PREFLEN; i++)
	{
	    tdata[tid].nodemark[tdata[tid].currpatharray[i]] = 0;
	    edge = nodearray[tdata[tid].currpatharray[i]].adjlist;
	    while (edge != NULL)
	    {
		tdata[tid].edgemark[tdata[tid].currpatharray[i]][edge->nodeindex] = 0;
		edge = edge->next;
	    }
	}
    }
    
    /* output results of thread after it is complete */
    printf("Thread (%ld) exiting: its best path = [len=%.1f] [vis=%d] ", 
	       tid, tdata[tid].mybestpathlen, tdata[tid].bestpathindex);
    for (i = 0; i < tdata[tid].bestpathindex; i++)
	printf("%d ", tdata[tid].bestpatharray[i]);
    printf("\n");
}

int main(int argc, char *argv[])
{
    int res;
    long i,mytid;
    pthread_t ptid[NUMTHREADS];

    printf("Traveling salesman, thread version\n");
    printf("Number of threads: %d,  Prefix length: %d\n",NUMTHREADS,PREFLEN);

    totalnumnodes = NUMNODES;
    startnodeindex = STARTNODE;
    creategraph();

    /* now compute possible path prefixes (from starting node) and
     * store them in prefix array. function is recursive, uses
     * currpatharray in similar manner to search function */
    prefixindex = -1;
    pathindex = 0;
    patharray[pathindex++] = STARTNODE;
    fillprefix(STARTNODE,0.0);
    
    /* compute initial upper bound for path length */
    shbestpathlen = estpathlength();
    for (i = 0; i < NUMTHREADS; i++)
	tdata[i].mybestpathlen = shbestpathlen;

    /* create and run NUMTHREADS total threads */
    for (i = 0; i < NUMTHREADS; i++)
	pthread_create(&ptid[i], NULL, wrapper, (void *) i);

    /* join all threads before proceeding */
    for (i = 0; i < NUMTHREADS; i++)
	pthread_join(ptid[i], NULL);

    /* main thread will wait for all threads, then output best path */
    printf("Shortest overall path length: %.1f\n", shbestpathlen);
}
