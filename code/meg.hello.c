#include "ext.h" // standard Max library include, always required
#include "ext_obex.h" // library required for new style Max object

/* 
Defining a struct. In it we define the variables. 
*/

typedef struct _hello
{
    t_object s_ob;  // this is the Max object itself. Always required and always first. 
    long count; // we definre a long variable as a counter
    
    void *m_outlet1; // we define 1 outlet. The outlets are always void pointers

} t_hello; // struct instantiation

/*
Function prototypes. Here we define the methods we are going to use.
*/

void *hello_new(); // this is the constructor
void hello_bang(t_hello *x); // this is the method that is called from Max with the "bang" message

/* 
Global Class Pointer Variable.
We define a pointer of type t_class
*/

static t_class *hello_class;

/* 
Initialization routine
The ext_main function is the entry point. This is called from Max when we create the object.
*/

void ext_main(void *r)
{
    t_class *c;
    
    // "meg.hello is the name of the object that we create (meg.hello.mxo)
    // (method)hello_new is the new instance routine (the constructor); 
    // (method)NULL is a free function
    // (long)sizeof(t_hello) is the memory size of the struct
    // 4. la size della struttura; 5. un argomento non più usato;
    // 0L not use anymore
    // description of the arguments when we create the objects (0 in this case)
    c = class_new("meg.hello", (method)hello_new, (method)NULL,  (long)sizeof(t_hello), 0L , 0);
    
    // adding a method
    class_addmethod(c, (method)hello_bang, "bang", 0);
    
    // adding the "class" to the namespace CLASS_BOX, so that it can be found from Max
    class_register(CLASS_BOX, c);
    
    hello_class = c;
}

/* 
New Instance Routine
Allocate the memory for creating an instance of the class and initialize it.
*/

void *hello_new()
{
    t_hello *x = (t_hello *)object_alloc(hello_class); // instantiation of the object
    x->count = 0;
    
    // we create an outlet of type int
    x->m_outlet1 = intout((t_object *)x);
    
    return (x);
}

/* 
Message Handler.
We define the methods that handle the messages from Max
*/

void hello_bang(t_hello *x) {
    x->count++;
    post("Hello World for %ld times", x->count);
    outlet_int(x->m_outlet1, x->count));
}

