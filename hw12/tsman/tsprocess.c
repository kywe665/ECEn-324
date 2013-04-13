/* tsprocess.c    last modified: 2 Mar 2011       James Archibald

   This program is a multi-process version of the code to solve the
   traveling salesman problem. It is based on the sequential version
   in tsbase.c and preserves as much of that approach as makes sense.

   Before creating multiple processes, it uses mmap() to create shared
   variables used by all processes, and it creates a pool of work to
   be done by creating a "prefix pool" (the prefix array) of all
   possible starting combinations (of a given length) for paths
   originating from the starting node. Each process grabs an entry
   from this pool, searches it completely, then grabs another one,
   terminating its search if none are left. At that point, each
   process outputs the best path it found, and then it terminates. The
   parent process waits until all child processes complete, and then
   it outputs the path length of the best overall path discovered.

   As in the sequential version, the discovery of each new best path
   causes a line of output to be generated that includes the process
   ID (pid) of the process that discovered it. Shared writeable data
   is limited, consisting only of the shortest path length so far and
   an index into the prefix pool for the next "job" to be assigned.
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys/mman.h>

/* These parameters directly control the parallelism */
/* Default: 4 processes, 5 node prefix */ 
#define MAXPROC    4   /* number of processes to create */
#define PREFLEN    5   /* length of stored prefixes to split up work */

/* Leave these as they are unless you use a different graph */
#define NUMNODES  18   /* total nodes in graph */
#define STARTNODE  8   /* node to start tour from */
#define ARRAYSZ  100   /* number of nodes in longest path */
#define MAXPREF 1000   /* max # of unique prefixes to store */
#define FILESIZE  (2*sizeof(double)) /* size to mmap */

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

Node nodearray[NUMNODES];
int nummarkednodes;
int totalnumnodes;
int startnodeindex;

int currpatharray[ARRAYSZ];
int currpathindex;
int bestpatharray[ARRAYSZ];
int bestpathindex;
double mybestpathlen;
double *shbestpathlen;
int prefix[MAXPREF][PREFLEN];
int *prefixindex;
double dist[MAXPREF];
int pid;
int parent;

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

void fillprefix(int currnode, double currpathlen)
{
    int i;
    Adjptr adjtmp;
    
    if (currpathindex == PREFLEN)
    {
	/* insert in prefix array */
	(*prefixindex)++;
	for (i = 0; i < PREFLEN; i++)
	    prefix[*prefixindex][i] = currpatharray[i];
	dist[*prefixindex] = currpathlen;
	return;
    }
    
    adjtmp = nodearray[currnode].adjlist;
    while (adjtmp != NULL)
    {
	/* okay to revisit nodes, but don't repeat edges */
	if (adjtmp->amark == 0)
	{
	    adjtmp->amark = 1;
	    currpatharray[currpathindex++] = adjtmp->nodeindex;
	    fillprefix(adjtmp->nodeindex,currpathlen+adjtmp->edgelength);
	    currpathindex--;
	    adjtmp->amark = 0;
	}
	adjtmp = adjtmp->next;
    }
}

/* calls itself recursively to check all possible paths */
void search(int currnode, double currpathlen)
{
    int i;
    Adjptr adjtmp;

    if (currpathlen >= *shbestpathlen)
	return;     /* not better than current best -- discontinue */

    /* otherwise, does path visit all nodes AND end at start node? */
    if (nummarkednodes == totalnumnodes && 
	currpatharray[currpathindex-1] == startnodeindex)
    {
	/* best global path so far: record path and path length */
	*shbestpathlen = currpathlen;
	mybestpathlen = currpathlen;
	for (i = 0; i < currpathindex; i++)
	    bestpatharray[i] = currpatharray[i];
	bestpathindex = currpathindex;
	printf("\tNew best path: [len=%.1f] [pid=%d]\n",mybestpathlen,pid);
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
    int i,fd,res,tmp;
    Adjptr edge;

    printf("Traveling salesman, process version\n");
    printf("Number of processes: %d,  Prefix length: %d\n",MAXPROC,PREFLEN);

    totalnumnodes = NUMNODES;
    startnodeindex = STARTNODE;
    creategraph();

    /* mmap the prefix array and the best path length */
    fd = open("prefix.dat", O_RDWR|O_CREAT|O_TRUNC,0600);
    if (fd == -1)
    {
	fprintf(stderr,"open failed\n");
	exit(-1);
    }
    
    /* make file long enough by writing a zero byte at the right point */
    res = lseek(fd,FILESIZE,SEEK_SET);
    if (res == -1)
    {
	fprintf(stderr,"lseek failed\n");
	exit(-1);
    }
    res = write(fd,"", 1);
    if (res != 1)
    {
	fprintf(stderr,"write failed\n");
	exit(-1);
    }
    
    /* allocate space for the array plus a few rows */
    shbestpathlen = (double *) mmap(NULL,FILESIZE,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (shbestpathlen == MAP_FAILED)
    {
	fprintf(stderr,"mmap failed\n");
	exit(-1);
    }
    
    /* use last half as (int) prefixindex */
    prefixindex = (int *) &(shbestpathlen[1]);

    /* now compute possible path prefixes (from starting node) and
     * store them in prefix array. function is recursive, uses
     * currpatharray in similar manner to search function */
    *prefixindex = -1;
    currpathindex = 0;
    currpatharray[currpathindex++] = STARTNODE;
    fillprefix(STARTNODE,0.0);
    
    /* compute initial upper bound for path length */
    mybestpathlen = estpathlength();
    *shbestpathlen = mybestpathlen;

    /* now fork MAXPROC-1 new processes */
    parent = 1;
    for (i = 0; i < MAXPROC-1; i++)
    {
	tmp = fork();
	if (tmp < 0)
	{
	    fprintf(stderr, "fork failed \n");
	    exit(-1);
	}
	if (tmp == 0)   /* child */
	{
	    parent = 0;
	    break;
	}
    }

    /* set pid for output */
    pid = (int) getpid();
    printf("Process %d starting ",pid);
    if (parent)
	printf("(parent)\n");
    else
	printf("(child)\n");
        
    /* each process loops, getting next free prefix & searching it */
    bestpathindex = 0;
    while ((tmp = (*prefixindex)--) >= 0)
    {
	nummarkednodes = 0;

	/* copy prefix, mark nodes and edges, and start search */
	for (i = 0; i < PREFLEN; i++)
	{
	    currpatharray[i] = prefix[tmp][i];
	    if (nodearray[currpatharray[i]].nmark == 0)
	    {   /* increment count of marked nodes */
		nodearray[currpatharray[i]].nmark = 1;
		nummarkednodes++;
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
		edge->amark = 1;
	    }
	}
	currpathindex = PREFLEN;
	search(currpatharray[currpathindex-1], dist[tmp]);

	/* now loop again and undo marks from above */
	for (i = 0; i < PREFLEN; i++)
	{
	    nodearray[currpatharray[i]].nmark = 0;
	    edge = nodearray[currpatharray[i]].adjlist;
	    while (edge != NULL)
	    {
		edge->amark = 0;
		edge = edge->next;
	    }
	}
	
    }

    /* have each process output its best path */
    if (parent)
    {
	printf("Parent (%d) exiting: its best path = [len=%.1f] [vis=%d] ", 
	       pid, mybestpathlen,bestpathindex);
	for (i = 0; i < bestpathindex; i++)
	    printf("%d ", bestpatharray[i]);
	printf("\n");
	while (waitpid(-1,NULL,0) > 0);
	printf("Shortest overall path length: %.1f\n", *shbestpathlen);
    }
    else
    {
	printf("Child  (%d) exiting: its best path = [len=%.1f] ", 
	       pid, mybestpathlen);
	for (i = 0; i < bestpathindex; i++)
	    printf("%d ", bestpatharray[i]);
	printf("\n");
    }
}
