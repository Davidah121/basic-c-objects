#include <stdio.h>
#include "CObjects.h"

struct TestObject
{
    struct ObjectContainer objInfo;
    int x;
    int y;
};

void DestroyTestObject(void* o)
{
    struct TestObject* obj = (struct TestObject*)o;
    printf("Values in Destructor. (%d, %d)\n", obj->x, obj->y);
}

void CreateTestObject(struct TestObject* obj)
{
    printf("In Constructor of TestObject\n");
    obj->x = 0;
    obj->y = 2;
    SetupObject(&(obj->objInfo), obj, DestroyTestObject);
}

int main()
{
    struct TestObject obj;
    CreateTestObject(&obj);
    return 0;
}