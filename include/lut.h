/* This file defines the symbol lookup table that will be used inside the parser. */
#ifndef LUT_H
#define LUT_H

#include "spldef.h"

#include <stddef.h>

#define LUT_UNORDERED_IMPL

#define LUT_DEFAULT_CAPACITY 1000

/* Pointer to struct for holding properties of a node of linked list. */
typedef struct lut_entry_struct *lut_entry;

/* Struct for holding properties of a node of linked list. */
typedef struct lut_entry_struct
{
    spl_entry_t type;              /* Type of this entry */
    char *id;                      /* The name of the entry, if any */
    size_t pointer_level;          /* Level of pointer */
    struct lut_entry_struct *next; /* Point to the next element in case of Hash Collision */

    int lineno; /* line number */
    int colno;  /* column number */

    ast_node root; /* If this entry is a macro, the root contains the AST of the macro in expanded form */

    union {
        void *val;              /* General data */
        unsigned int ulong_val; /* Interpret the value as integer */
        float float_val;        /* Interpret the value as float  */
    };
} lut_entry_struct;

/* Pointer to struct for holding properties of a hashtable. */
typedef struct lut_table_struct *lut_table;

/* Struct for holding properties of a hashtable. */
typedef struct lut_table_struct
{
    lut_entry *entries; /* Pointer to the start of array of pointer to entries */
    size_t capacity;    /* Capacity of this lut_table */
} lut_table_struct;

extern lut_table global_symbol_table;

void init_global_lut(void);

lut_table lut_new_table(void);

void lut_free_table(lut_table *table);

lut_entry lut_find(lut_table table, const char *name);

lut_entry lut_insert(lut_table table, const char *name);

void lut_delete(lut_table table, const char *name);

void lut_reset_all(lut_table table);

#endif