# basic-c-objects
A basic approach to creating object oriented programming in C.

Requires instrumentation functions to be overridden. For GCC or clang, compile with option -finstrument-functions

MSVC requires a little bit more work. Function name changes and ensuring that the appropriate functions are not instrumented which may be done differently

# Usage Examples
Setting up an Object with a Constructor and Destructor
```C
struct TestClass
{
  struct ObjetContainer objInfo;
  int someValue;
};

void DestroyTestObject(void* o)
{
  //Doesn't have to do anything
  struct TestObject* reinterpretCastedObject = (struct TestObject*)o;
  printf("Value: %d\n", reinterpretCastedObject.someValue);
}
void CreateTestObject(struct TestObject* obj, int k)
{
  obj->someValue = k;
  SetupObject(&(obj->objInfo), obj, DestroyTestObject);
}
```

Using that object in practice
```C
int main()
{
  struct TestClass obj;
  CreateTestObject(&obj, 32);
  return 0; //The destructor is called here and will print "Value: 32"
}
```

# Additional Notes
Because -finstrument-functions is required here, it could potential cause problems with profilers that are compiled with your code.
If the profiler you are using can be modified, this could be added alongside that profiler.

This is designed to be a basic approach so it is not robust nor is it designed to be better than any other existing approach to C objects.
It is just designed to show a simple approach to the problem.

This does not have the concept of public, protected, or private variables or functions. It does not have the concept of inheritance but it could still be done.

Also note that this doesn't necessarily handle object pointers. Additional work can be done to prevent deletions of pointers.
