#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

struct ObjectContainer
{
    void* objectPtr;
    void (*destructor)(void*);
};
struct StackObjectNode
{
    int objStackDepth;
    struct StackObjectNode* prev;
    struct ObjectContainer* objContainerPtr;
};

thread_local int objStackDepth = 1;
thread_local struct StackObjectNode head = {0, NULL, NULL};
thread_local struct StackObjectNode* end = NULL;

void AddObject(struct ObjectContainer* obj) __attribute__((no_instrument_function));
void __cyg_profile_func_enter(void* func, void* caller) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void* func, void* caller) __attribute__((no_instrument_function));

void __cyg_profile_func_enter(void* func, void* caller)
{
    objStackDepth++;
}
void __cyg_profile_func_exit(void* func, void* caller)
{
    //pop off objects till stackDepth < currentDepth
    struct StackObjectNode* currNode = end;
    if(currNode != NULL)
    {
        while(currNode != &head)
        {
            if(currNode->objStackDepth < objStackDepth)
                break;
            
            if(currNode->objContainerPtr != NULL)
                if(currNode->objContainerPtr->destructor != NULL)
                    currNode->objContainerPtr->destructor(currNode->objContainerPtr->objectPtr);
            
            currNode = end->prev;
            //remove end node
            free(end);
            end = currNode;
        }
    }
    objStackDepth--;
}

void AddObject(struct ObjectContainer* obj)
{
    if(end == NULL)
        end = &head;
    
    struct StackObjectNode* newNode = malloc(sizeof(struct StackObjectNode));
    newNode->objContainerPtr = obj;
    newNode->objStackDepth = objStackDepth;
    newNode->prev = end;
    end = newNode;
}