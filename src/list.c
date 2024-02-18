#include <stdlib.h>
#include <stdint.h>
#include "utils.h"
#include "list.h"

typedef struct ListElement
{
    void *data;
    struct ListElement *next;
    struct ListElement *prev;
} ListElement;

struct List
{
    ListElement *head;
    ListElement *tail;
    size_t size;
    ListElement *current;
};

List *listInit(size_t size)
{
    List *list = malloc(sizeof(List));
    if (!list)
        return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->current = NULL;
    while (size-- > 0)
    {
        if (listPush(list, NULL) == 0)
        {
            listDestroy(list);
            return NULL;
        }
    }
    return list;
}

size_t listPushLeft(List *list, void *data)
{
    if (list->size == SIZE_MAX)
    {
        return 0;
    }
    ListElement *elem = malloc(sizeof(ListElement));
    if (!elem)
        return 0;
    elem->data = data;
    elem->next = list->head;
    elem->prev = NULL;
    list->head = elem;
    if (!list->tail)
    {
        list->tail = elem;
    }
    return ++list->size;
}

size_t listPushRight(List *list, void *data)
{
    if (list->size == SIZE_MAX)
    {
        return 0;
    }
    ListElement *elem = malloc(sizeof(ListElement));
    if (!elem)
        return 0;
    elem->data = data;
    elem->prev = list->tail;
    elem->next = NULL;
    list->tail = elem;
    if (!list->head)
    {
        list->head = elem;
    }
    return ++list->size;
}

void *listPopLeft(List *list)
{
    if (list->size == 0)
    {
        return NULL;
    }
    ListElement *elem = list->head;
    list->head = elem->next;
    if (!list->head)
    {
        list->tail = NULL;
    }
    list->size--;
    void *data = elem->data;
    free(elem);
    return data;
}

void *listPopRight(List *list)
{
    if (list->size == 0)
    {
        return NULL;
    }
    ListElement *elem = list->tail;
    list->tail = elem->prev;
    if (!list->tail)
    {
        list->head = NULL;
    }
    list->size--;
    void *data = elem->data;
    free(elem);
    return data;
}

void listDestroy(List *list)
{
    if (!list)
        return;
    while (list->size)
    {
        listRemove(list);
    }
    free(list);
}
