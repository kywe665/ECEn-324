/* define abstract data type for vector */

/* to change base data type or operations performed in combine(),
 * change the next lines of code  */
typedef int data_t;  /* base data type: try int, long, float, double */
#define OP +         /* operator types: + and * */
#define IDENT 0      /* set to identity element of above operator */

typedef struct {
    long int len;
    data_t *data;
    long int allocated_len; /* NOTE: not shown in code in the book */
} vec_rec, *vec_ptr;

/* macro version of function */
#define GET_VEC_ELEMENT(v,index,dest) \
    !((index) < 0 || (index) >= (v)->len) && \
    *(dest) = (v)->data[(index)], 1;

/* function prototypes defined in vector.c */
vec_ptr new_vec(long int len);
int get_vec_element(vec_ptr v, long int index, data_t *dest);
data_t *get_vec_start(vec_ptr v);
int set_vec_element(vec_ptr v, long int index, data_t val);
long int vec_length(vec_ptr v);
void set_vec_length(vec_ptr v, long int newlen);
