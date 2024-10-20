// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 143091;

// Number of words in dictionary
int wordcount = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int n = strlen(word);
    char lower[LENGTH + 1];
    for (int i = 0; i < n; i++)
    {
        lower[i] = tolower(word[i]);
    }
    // Hash word
    lower[n] = '\0';
    int h = hash(lower);
    node *cursor = table[h];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, lower) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
// Used the DJB hash funstion, an algorithm produced by Professor Daniel J. Bernstein.
unsigned int hash(const char *word)
{
    // TODO
    unsigned int h = 5381;
    int l = strlen(word);
    for (unsigned int i = 0; i < l; word++, i++)
    {
        h = ((h << 5) + h) + (*word);
    }
    return (h % N);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open file.");
        return false;
    }
    char buffer[LENGTH + 1];
    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *newnode = malloc(sizeof(node));
        if (newnode == NULL)
        {
            unload();
            return false;
        }
        strcpy(newnode->word, buffer);
        newnode->next = NULL;
        //Hash word
        int h = hash(newnode->word);
        node *head = table[h];
        //Insert node in table
        if (head == NULL)
        {
            table[h] = newnode;
        }
        else
        {
            newnode->next = table[h];
            table[h] = newnode;
        }
        wordcount ++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *head = table[i];
            while (head->next != NULL)
            {
                node *cursor = head->next;
                free(head);
                head = cursor;
            }
            free(head);
        }
    }
    return true;
}
