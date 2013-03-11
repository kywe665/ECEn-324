/* define abstract data type for vector */

/* to change base data type or operations performed in combine(),
 * change the next lines of code  */

typedef double data_t;  /* base data type: try int, long, float, double */

typedef struct {
    long int len;
    data_t *data;
    long int allocated_len; /* NOTE: not shown in code in the book */
} vec_rec, *vec_ptr;

/* function prototypes defined in vector.c */
vec_ptr new_vec(long int len);
int get_vec_element(vec_ptr v, long int index, data_t *dest);
data_t *get_vec_start(vec_ptr v);
int set_vec_element(vec_ptr v, long int index, data_t val);
long int vec_length(vec_ptr v);
void set_vec_length(vec_ptr v, long int newlen);
