/* This file defines the symbol lookup table that will be used inside the parser. */
#ifndef LUT_H
#define LUT_H

#include "splcdef.h"

#include <stddef.h>

#define LUT_UNORDERED_IMPL

#define LUT_DEFAULT_CAPACITY 4000

/* Pointer to struct for holding properties of a node of linked list. */
typedef struct lut_entry_struct *lut_entry;

/* Struct for holding properties of a node of linked list. */
typedef struct lut_entry_struct
{
    splc_entry_t type;             /* Type of this entry */
    splc_entry_t extra_type;       /* Extra type of this entry */
    char* spec_type;               /* Specific type of this entry */
    char *id;                      /* The name of the entry, if any */
    struct lut_entry_struct *next; /* `lut_entry_struct` internal variable:
                                      point to the next element in case of hash collision */

    splc_loc first_occur;    /* first occurrence of this token */
    ast_node ast_scope_root; /* EXPERIMENTAL: root of scope */

    ast_node root; /* EXPERIMENTAL: Contain the root of this entry.
                      AST has full control of this root, as it is a deep copy. */

} lut_entry_struct;

/* Pointer to struct for holding properties of a hashtable. */
typedef struct lut_table_struct *lut_table;

/* Struct for holding properties of a hashtable. */
typedef struct lut_table_struct
{
    lut_entry *entries; /* Pointer to the start of array of pointer to entries */
    size_t capacity;    /* Capacity of this lut_table */
    int ref_count;      /* Number of references to this lut. */
    int scope;          /* Scope of this symbol table. 0 represents the global scope. */
} lut_table_struct;

/* Initialize a new LUT table. */
lut_table lut_new_table(int scope);

/* Accepts NULL parameter. Do a shallow copy on the table, maintaining the internal reference count. */
lut_table lut_copy_table(lut_table table);

/* Free a hashtable and all its related resources. */
void lut_free_table(lut_table *table);

/* Find the value of a name in the hashtable. Table must be valid and nonnull, and key must be nonnull.
   Return NULL if such entry doesn't exist. */
lut_entry lut_find(const lut_table table, const char *name, const splc_entry_t type);

/* Find the value of a name in the hashtable. Table must be valid and nonnull, and key must be nonnull.
   Return NULL if such entry doesn't exist.
   Return the FIRST valid entry. */
lut_entry lut_find_name_first(const lut_table table, const char *name);

/* Find the value of a name in the hashtable. Table must be valid and nonnull, and key must be nonnull.
   Return 0 if such entry doesn't exist. Else 1. */
int lut_exists(const lut_table table, const char *name, const splc_entry_t type);

/* Find the value of a name in the hashtable. Table must be valid and nonnull, and key must be nonnull.
   Return 0 if such entry doesn't exist. Else 1.
   Note: This function will return true if any of the name matches the given. */
int lut_name_exists(const lut_table table, const char *name);

/* Insert a value to hashtable. If the name already correspond to an entry, overwrite it.
   Name will be copied.
   This method will also deep copy the root, so that the original root won't be affected when the entry gets modified.
 */
lut_entry lut_insert(lut_table table, const char *name, const splc_entry_t type, const splc_entry_t extra_type,
                     const char *spec_type, const ast_node root, const splc_loc first_occur);

/* Delete a value from hashtable. Accept NULL key, and allows nonexisting entry to be deleted. */
void lut_delete(lut_table table, const char *name, const splc_entry_t type);

/* Reset a hashtable. */
void lut_reset_all(lut_table table);

/* This method guarantees that a string will be generated regardless of the table pointer. */
char *lut_get_info_string(lut_table table);

void lut_debug_print(FILE stream, lut_table table);

void lut_entry_print(const lut_entry ent);

/* Macros */
#define SPLC_LUT_IS_GLOBAL_SCOPE(x) ((x)->scope == 0)

#endif /* LUT_H */