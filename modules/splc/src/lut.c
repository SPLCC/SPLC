/* Source file for the implementation of the lookup table. */
#include "lut.h"
#include "ast.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

/* Common macros */

#ifdef MIN
#undef MIN
#endif
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))

#ifdef MAX
#undef MAX
#endif
#define MAX(a, b) (((a) >= (b)) ? (a) : (b))

/* Functions */

/* Hash function for converting string to an integer.
   Reference: http://www.cse.yorku.ca/~oz/hash.html */
static unsigned int hash(const char *str)
{
    unsigned int hash = 5381; /* ? */
    int c;

    while ((c = (unsigned)(*(str++))) != '\0') /* c != '\0' */
        hash = ((hash << 5) + hash) + c;       /* hash * 33 + c */

    return hash;
}

/* Create a new node. No validity check.
   Name will be copied. */
static lut_entry lut_new_entry(const char *name)
{
    lut_entry tmp = (lut_entry)malloc(sizeof(lut_entry_struct));

    /* Allocate extra memory for storing the key. */
    char *id = strdup(name);
    SPLC_ALLOC_PTR_CHECK(id, "cannot allocate for ID");

    tmp->type = SPLE_NULL;
    tmp->id = id;
    tmp->next = NULL;
    tmp->first_occur = SPLC_INVALID_LOC;
    tmp->ast_scope_root = NULL;
    tmp->root = NULL;

    return tmp;
}

/* Free the resource of a node and set the corresponding pointer to NULL.
   The relation in the linked list is not handled. */
static void lut_free_entry(lut_entry *entry)
{
    if (entry == NULL || *entry == NULL)
        return;

    free((*entry)->id);
    ast_release_node(&(*entry)->root);
    free(*entry);
    *entry = NULL;
}

lut_table lut_new_table(void)
{
    lut_table result = (lut_table)malloc(sizeof(lut_table_struct));
    SPLC_ALLOC_PTR_CHECK(result, "out of memory when constructing lut_table");

    /* Set entries. */
    result->entries = (lut_entry *)malloc(LUT_DEFAULT_CAPACITY * sizeof(lut_entry));
    SPLC_ALLOC_PTR_CHECK(result->entries, "out of memory when constructing lut_table entry array");
    memset(result->entries, 0, LUT_DEFAULT_CAPACITY * sizeof(lut_entry)); /* Set all entries to NULL. */

    /* Set capacity. */
    result->capacity = LUT_DEFAULT_CAPACITY;
    return result;
}

void lut_free_table(lut_table *table)
{
    if (table == NULL || *table == NULL)
        return;

    for (int i = 0; i < (*table)->capacity; ++i)
        if ((*table)->entries[i] != NULL)
        {
            lut_entry target = (*table)->entries[i], next = NULL;
            do
            {
                next = target->next;
                lut_free_entry(&target);
                target = next;
            } while (target != NULL);
        }
    free((*table)->entries);
    free(*table);
    *table = NULL;
}

int lut_exists(const lut_table table, const char *name)
{
    return lut_find(table, name) != NULL;
}

lut_entry lut_find(const lut_table table, const char *name)
{
    unsigned int key0 = hash(name) % (table->capacity);
    lut_entry target = *(table->entries + key0);

    while (target != NULL)
    {
        if (strcmp(target->id, name) == 0)
            break;
        else
            target = target->next;
    }
    return target;
}

lut_entry lut_insert(lut_table table, const char *name, const splc_entry_t type, const ast_node root, const splc_loc first_occur)
{
    unsigned int key0 = hash(name) % (table->capacity);
    lut_entry target = *(table->entries + key0), prev = NULL, next = NULL;

    while (target != NULL)
    {
        if (strcmp(target->id, name) == 0)
        {
            next = target->next;
            lut_free_entry(&target);
            break;
        }
        else
        {
            prev = target;
            target = target->next;
        }
    }

    target = lut_new_entry(name);
    target->type = type;
    target->first_occur = first_occur;
    target->root = ast_deep_copy(root);
    target->next = next;

    if (prev != NULL)
    {
        prev->next = target;
    }
    else
    {
        *(table->entries + key0) = target;
    }
    return target;
}

void lut_delete(lut_table table, const char *name)
{
    if (name == NULL)
        return;

    unsigned int key0 = hash(name) % (table->capacity);
    lut_entry target = *(table->entries + key0), prev = NULL;

    while (target != NULL)
    {
        if (strcmp(target->id, name) == 0)
        {
            if (prev != NULL)
                prev->next = target->next;
            lut_free_entry(&target);
            break;
        }
        else
        {
            prev = target;
            target = target->next;
        }
    }
}

void lut_reset_all(lut_table table)
{
    for (int i = 0; i < table->capacity; ++i)
        if (table->entries[i] != NULL)
        {
            lut_entry target = table->entries[i], next = NULL;
            do
            {
                next = target->next;
                lut_free_entry(&target);
                target = next;
            } while (target != NULL);
        }
    memset(table->entries, 0, table->capacity * sizeof(lut_entry)); /* Set all entries to NULL. */
}