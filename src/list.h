#pragma once

#include <stdlib.h>

#define listPush(list, data) listPushRight(list, data)
#define listPop(list) listPopRight(list)
#define listRemoveLeft(list) free(listPopLeft(list))
#define listRemoveRight(list) free(listPopRight(list))
#define listRemove(list) listRemoveRight(list)

typedef struct List List;

List *listInit(size_t size);

size_t listPushLeft(List *list, void *data);
size_t listPushRight(List *list, void *data);

void *listPopLeft(List *list);
void *listPopRight(List *list);

void listDestroy(List *list);

void walkList(List *list, void(func)(void *data));