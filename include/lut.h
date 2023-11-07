/* This file defines the symbol lookup table that will be used inside the parser. */
#ifndef LUT_H
#define LUT_H

#include "splcdef.h"

#include <stddef.h>

#define LUT_UNORDERED_IMPL

#define LUT_DEFAULT_CAPACITY 1000

/* Pointer to struct for holding properties of a node of linked list. */
typedef struct lut_entry_struct *lut_entry;

/* Struct for holding properties of a node of linked list. */
typedef struct lut_entry_struct
{
    splc_entry_t type;              /* Type of this entry */
    char *id;                      /* The name of the entry, if any */
    struct lut_entry_struct *next; /* `lut_entry_struct` internal variable: point to the next element in case of hash collision */

    splc_loc first_occur; /* first occurrence of this token */
    ast_node ast_scope_root; /* root of scope */

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

lut_table lut_new_table(void);

void lut_free_table(lut_table *table);

int lut_exists(const lut_table table, const char *name);

lut_entry lut_find(const lut_table table, const char *name);

lut_entry lut_insert(lut_table table, const char *name);

void lut_delete(lut_table table, const char *name);

void lut_reset_all(lut_table table);

#endif