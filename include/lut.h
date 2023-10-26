/* This file defines the symbol lookup table that will be used inside the parser. */
#ifndef LUT_H
#define LUT_H

/* Included libraries */

#include <stddef.h>

/* Definitions */

/* This shows that the implementation of LUT is unordered. */
#define _LUT_UNORDERED_IMPL

#define _LUT_DEFAULT_CAPACITY 1000

/* Declarations */

/* Pointer to struct for holding properties of a node of linked list. */
typedef struct lut_node_struct *lut_node;

/* Struct for holding properties of a node of linked list. */
typedef struct lut_node_struct
{
    
    char *id;                     /* The name of the node */
    sap_num val;                  /* The value of the entry */
    struct lut_node_struct *next; /* Point to the next element in case of Hash Collision */
} lut_node_struct;

/* Pointer to struct for holding properties of a hashtable. */
typedef struct lut_table_struct *lut_table;

/* Struct for holding properties of a hashtable. */
typedef struct lut_table_struct
{
    lut_node *entries; /* Pointer to the start of array of pointer to entries */
    size_t capacity;   /* Capacity of this lut_table */
} lut_table_struct;

/* Function prototypes */

lut_table lut_new_table(void);

void lut_free_table(lut_table *table);

sap_num lut_find(lut_table table, char *key);

void lut_insert(lut_table table, char *key, sap_num val);

void lut_delete(lut_table table, char *key);

void lut_reset_all(lut_table table);

#endif