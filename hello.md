---
title: Hello object
permalink: /hello
nav_order: 3
---

# Your first Max/MSP object

The objects developed in this Max/MSP externals repository always have the prefix "meg.", that stands for Max Externals Guide. It is a good rule to give your objects a personal prefix (different from those of other Max libraries) to distinguish them from the native objects.

Here we cerate an object called [ meg.hello ]. This object counts the number of "bangs" it receives form its first inlet outputting this number from its outlet. At the same time it posts in the Max console how many times you said the so much loved "Hello World".  

![](https://raw.githubusercontent.com/max-externals/site/main/media/meg.hello_video.gif)


## Import the SDK

```c
#include "ext.h" // standard Max library include, always required
#include "ext_obex.h" // library required for new style Max objects
```

Every external object must start with the include of the Max/MSP `ext` library, that defines all the variables and method needed to interact with Max.

## Defining object properties

In order to control the state of the object we create a [struct](https://en.wikipedia.org/wiki/Struct_(C_programming_language))

```c
typedef struct _hello
{
    t_object s_ob;  // this is the Max object itself. Always required and must be always first

    void *m_outlet1; // we define 1 outlet. The outlets are always void pointers

    //state
    long count; // we definre a long variable as a counter
    
} t_hello; // struct instantiation
```

we declared a very simple structure for the Max object, every max object needs:
1. the reference to itself (t_object)
1. the list of outlets (the output of our object), just m_outlet1 in our very simple case
1. the state of the object, just a simple counter for the current case

## Define prototypes

As usual in C we first define the methods we are going to use, and implement them later on

```c
void *hello_new(); // this is the constructor
void hello_bang(t_hello *x); // this is the method that is called from Max with the "bang" message
```

Moreover we need to store as global variable the pointer of the class

TODO why is that?

```c
/* 
Global Class Pointer Variable.
We define a pointer of type t_class
*/

static t_class *hello_class;

```

## Max entrypoint

Initialization routine
The ext_main function is the entry point. This is called from Max when we create the object.

```c
void ext_main(void *r)
{
    t_class *c;
    
    // "meg.hello is the name of the object that we create (meg.hello.mxo)
    // (method)hello_new is the new instance routine (the constructor); 
    // (method)NULL is a free function
    // (long)sizeof(t_hello) is the memory size of the struct
    // 0L not use anymore
    // description of the arguments when we create the objects (0 in this case)
    c = class_new("meg.hello", (method)hello_new, (method)NULL,  (long)sizeof(t_hello), 0L , 0);
    
    // adding a method
    class_addmethod(c, (method)hello_bang, "bang", 0);
    
    // adding the "class" to the namespace CLASS_BOX, so that it can be found from Max
    class_register(CLASS_BOX, c);
    
    // assign the newly created object to the global variable
    hello_class = c;
}
```


## New instance routine

In object oriented programming we would call this the constructor.
We allocate the memory for creating an instance of the class and initialize it.

```c
void *hello_new()
{
    t_hello *x = (t_hello *)object_alloc(hello_class); // instantiation of the object
    x->count = 0;
    
    // we create an outlet of type int
    x->m_outlet1 = intout((t_object *)x);
    
    return (x);
}
```

Here we first allocate the space for the class (TODO not fully clear to me why we allocate the t_class and not the structure)

then we initialize the counter and the outlet


## Bang Handler

Finally we need to define the handler for the "bang", as declared in the initialization routine

```c
void hello_bang(t_hello *x) {
    x->count++;
    post("Hello World for %ld times", x->count);
    outlet_int(x->m_outlet1, x->count));
}
```
First we change the state of the object.

The method `post` provided by the SDK prints on the Max console, it may be used like a standard printf method of C.

`outlet_int` is the method that Max provides to send data (integer number in this case) on the outlet, the parameters are:
1. the outlet, remeber we defined it in the struct
1. the value that we want to send


