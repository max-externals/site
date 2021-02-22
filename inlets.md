---
title: Inlets/outlets basics
permalink: /inlets-outlets
nav_order: 4
---

# Inlets/outlets basics

In order to present the basic functionality of inlets and outlets we create here a simple object [ meg.inout ] with one inlet, that accepts int, bang, float and a custom sym type, the message is then routed into 4 outlets, each outlet outputs only one type, it can be tought like a pattern matching in others programming languages.

So we have the default inlet, that is present in every max object, and the 4 outlets.

## Main inlet
The main inlets is always created, we just need to attach the C function that needs to be triggered when a new message arrives, to do so we first need to define the function, as example we take the simplest message, bang.

```c
void inlet_bang(t_inout *x);
```

then in the [initialization routine](/hello#max-entrypoint) we need to bind the function with the message symbol, that is used as type definition, here we use `int` as example

```c
class_addmethod(c, (method)inlet_bang, "bang", 0);
```

So every time a bang is recived the method inlet_bang it's executed.

But of course we don't handle only bangs, that would be boring, so we need a way to dispatch beteen different types,
and that is achived using the third parameter of `class_addmethod` native possible values are:
- `int` 
- `float`
- `bang`
Or you can define your own simbol to select the method you prefer.

The forth parameters of `class_addmethod` define the type handled, that of course is strictly correlated with the symbol used for native symbols,
and can arbitrarly chosen for custom symbols. You can find the list of all available types on the
[SDK Docs](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__atom.html#ga8aa6700e9f00b132eb376db6e39ade47)
The most important are:
- A_LONG
- A_FLOAT
- A_SYM
- A_GIMME

The selection of the type has a direct impact on the signature of the trigger function.

| Max Type | SDK Type   | Function paramenters                                  | Example                                                     | Params                                      |
|:---------|:-----------|:------------------------------------------------------|:------------------------------------------------------------|:--------------------------------------------|
|  Bang    |            | `<obj_type> *x`                                       | `void inlet_bang(t_inout *x)`                               |                                             |
|  Integer |  A_LONG    | `<obj_type> *x, long n`                               | `void inlet_int(t_inout *x, long n)`                        | `n`: recived integer                        |
|  Number  |  A_FLOAT   | `<obj_type> *x, double n`                             | `void inlet_float(t_inout *x, double n)`                    | `n`: recived float                          |
|  Symbol  |  A_SYM     | `<obj_type> *x, t_symbol *s`                          | `void inlet_sym(t_inout *x, t_symbol *s)`                   | `s`: pointer to the [t_symbol](https://cycling74.com/sdk/max-sdk-8.0.3/html/structt__symbol.html) recived          |
|  List    |  A_GIMME   | `<obj_type> *x, t_symbol *s, short ac, t_atom *av`    | `inlet_list(t_inout *x, t_symbol *s, short ac, t_atom *av)` | `s`: pointer to the [t_symbol](https://cycling74.com/sdk/max-sdk-8.0.3/html/structt__symbol.html) of the list, `ac`: length of the list, `av`: pointer of the first [t_atom](https://cycling74.com/sdk/max-sdk-8.0.3/html/structt__atom.html) of the list |
|  Assist  |  A_CANT    | `<obj_type> *x, void *b, long m, long a, char *s`     | `void assist(t_inout *x, void *b, long m, long a, char *s)` | `b`: ???, `m`: define if the assists is invoked in inlet or outlet, if m equals the const ASSIST_INLET is an inlet, else an outlet, `a`: index of the in/outlet, `s`: return value need to be stored in s, typically with [sptrinf](https://www.cplusplus.com/reference/cstdio/sprintf/) |

So in our example we need to define all the methods that are listening on the main inlet with:
```c
    class_addmethod(c, (method)inlet_int, "int", A_LONG, 0); 
    class_addmethod(c, (method)inlet_float, "float", A_FLOAT, 0); 
    class_addmethod(c, (method)inlet_bang, "bang", 0);
    class_addmethod(c, (method)inlet_sym, "sym", A_SYM, 0);
    class_addmethod(c, (method)inlet_list, "list", A_GIMME, 0);
    class_addmethod(c, (method)assist, "assist", A_CANT, 0);
```
they all get binded to the main inlet.

## Outlets

Here we create the outlets for the Max object. 

```c
typedef struct _inout
{
    t_object s_ob;
    
    void *outlet1_int;
    void *outlet2_double;
    void *outlet3_bang;
    void *outlet4_string;
    void *outlet5_list;

} t_inout;
```
Then we need to initialize them on the [new instance routine](/hello#new-instance-routine)
The outlets are created in the order of appearance of methods in the code, from right do left (the one at the top is the right-most outlet)

There are different methods that can be invoked to create outlets.
Some methods create a standard typed outlet. 
Patchers execute faster when outlets are typed, since the message lookup can be done before the program executes.


| Max Type | Outlet definition                 | Outlet method   |
|:---------|:----------------------------------|:----------------|
|  Bang    | `bangout((t_object *)x)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#ga69d26d4f2684aab7dbc1b2d18248eae5)          | `outlet_bang(<outlet>)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#ga357498d7143fd266facfbfc4efa59029)                |
|  Integer | `intout((t_object *)x)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#ga9b8d897c728eeafa5638d4fc16ff704e)           | `outlet_int(<outlet>, n)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#ga5367b8d544907c7f66612282b43fd1c8)                   |
|  Number  | `floatout((t_object *)x)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#ga0881da69192bb254b8c0bf767c657461)         | `outlet_float(<outlet>, n)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#gafbb3f62a413f05a394391afde5b3c30f)                   |
|  Symbol  | `outlet_new((t_object *)x,NULL)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#gac25db50a2a7eb76c5e057dd907c11d44)  | `outlet_anything(<outlet>,s,0,NIL)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#ga12798ee897e01dac21ee547c4091d8a8)                   |
|  List    | `listout((t_object *)x)`- [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#ga47841be73d0c90978da818f0f7c899eb)           | `outlet_list(<outlet>,0L,ac,av)` - [sdk](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__inout.html#gabdef4fbe6e1040dc28204b8070bdcda5)                   |

In our case:

```c
    x->outlet5_list = listout((t_object *)x);
    x->outlet4_string = outlet_new((t_object *)x,NULL);
    x->outlet3_bang = bangout((t_object *)x);
    x->outlet2_double = floatout((t_object *)x);
    x->outlet1_int = intout((t_object *)x); 
```

## Implement inlets methods

In order to work we need to implement all the inlets method that we defined, and route the message to the corresponding outlet:

```c

void assist(t_inout *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { //inlet
        sprintf(s, "(int/float/bang/sym) I am inlet 1 "); // what we see in the overlay popup menu of the inlet
    }
    else {  // outlets
        switch(a){
            case 0: sprintf(s, "I am outlet int"); break; // what we see in the overlay popup menu of the left-most outlet
            case 1: sprintf(s, "I am outlet float"); break; 
            case 2: sprintf(s, "I am outlet bang"); break;
            case 3: sprintf(s, "I am outlet sym"); break;
            case 4: sprintf(s, "I am outlet list"); break; // what we see in the overlay popup menu of the right-most outlet
        }
    }
}

void inlet_int(t_inout *x, long n){
    outlet_int(x->outlet1_int, n); // send an Integer to an outlet
}

void inlet_float(t_inout *x, double n) {
    outlet_float(x->outlet2_double,n); // send an Integer to an outlet
}

void inlet_bang(t_inout *x) {
    outlet_bang(x->outlet3_bang); // send a Bang to an outlet
}

void inlet_sym(t_inout *x, t_symbol *s) {
    /**
     * send anything to an outlet (any type)
     * in this case there is not a list of t_atom
     * 0 is the length of the list (usually argc) 
     * NIL means that there is no list of t_atom (usually argv) 
    */
    outlet_anything(x->outlet4_string,s,0,NIL); 
}

void inlet_list(t_inout *x, t_symbol *s, short ac, t_atom *av){
    outlet_list(x->outlet5_list,0L,ac,av); // send a list to an outlet
}

```


## Complete code

```c


/**
 * This object accepts different types of data in the left inlet:
 * int / float / bang  / sym <string>
 * and prints them in 4 different outlets
 * outlet 0: int;
 * outlet 1: float;
 * outlet 2: bang;
 * outlet 3: <string>
 *
 * https://cycling74.com/sdk/max-sdk-8.0.3/html/group__atom.html#ga8aa6700e9f00b132eb376db6e39ade47
 *
 */

#include "ext.h"
#include "ext_obex.h"

typedef struct _inout
{
    t_object s_ob;
    
    void *outlet1_int;
    void *outlet2_double;
    void *outlet3_bang;
    void *outlet4_string;
    void *outlet5_list;

} t_inout;

void *inout_new();
/**
 * method declarations
 * we declare one method for each type of data received in the left-most inlet
*/

void inlet_int(t_inout *x, long n); // inlet left-most (int)
void inlet_float(t_inout *x, double n); // inlet left-most (float)
void inlet_bang(t_inout *x); // inlet left-most (bang)
void inlet_sym(t_inout *x, t_symbol *s); // inlet left-most (sym)
void inlet_list(t_inout *x, t_symbol *s, short ac, t_atom *av); // inlet left-most (sym)

void assist(t_inout *x, void *b, long m, long a, char *s); // assist is a method for dealing the overlay of the mouse on the inlets and outlets

static t_class *inout_class;

void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("meg.inout", (method)inout_new, (method)NULL,  (long)sizeof(t_inout), 0L, 0);
    
    /**
     * we add the methods to the object class c
     * the third parameter defines the message selector:
     * int : if the inlet receives an int it invokes the method inlet_int (A_LONG says that the function accepts an long; 0 is the end of the list of arguments)
     * float : if the inlet receives an float it invokes the method inlet_float (A_FLOAT says that the function accepts an float; 0 is the end of the list of arguments)
     * bang : if the inlet receives a bang it invokes the method inlet_bang (no data after bang)
     * sum : if the inlet receives the keyword sym, it invoke the method inlet_sym (A_SYM says that the function accepts an symbol; 0 is the end of the list of arguments)
     * list : if the inlet receives the keyword list, it invoke the method inlet_list (A_GIMME is a list of t_atoms)
     * assist : it is a method that is invoked with the overlay on the inlets or outlets (A_CANT means that no data are passed to the function)
    */

    class_addmethod(c, (method)inlet_int, "int", A_LONG, 0); 
    class_addmethod(c, (method)inlet_float, "float", A_FLOAT, 0); 
    class_addmethod(c, (method)inlet_bang, "bang", 0);
    class_addmethod(c, (method)inlet_sym, "sym", A_SYM, 0);
    class_addmethod(c, (method)inlet_list, "list", A_GIMME, 0);
    class_addmethod(c, (method)assist, "assist", A_CANT, 0);
    
    class_register(CLASS_BOX, c);
    
    inout_class = c;
}

void *inout_new()
{
    t_inout *x = (t_inout *)object_alloc(inout_class);
    /** 
     * INLET
     * the first inlet is created by default, we don't need to create a new one
    */

    /** 
     * OUTLETS
     * Here we create the outlets for the Max object. There are different methods that can be invoked to create outlets.
     * Some methods create a standard typed outlet. Patchers execute faster when outlets are typed, since the message lookup can be done before the program executes.
     * - intout(): create an outlet that can send an Integer
     * - floatout(): create an outlet that can send an Double
     * - bangout(): create an outlet that can send a Bang
     * 
     * outlet_new(): create a generic outlet that can send a specific non-standard message, or any message.
     * outlet_new() can be used for sending standard types: e.g. outlet_new((t_object *)x, "float"), or outlet_new((t_object *)x, "int")
     * NULL is used when no type is specified
     * 
     * The outlets are created in the order of appearance of methods, from right do left (the one at the top is the right-most outlet)
    */

    x->outlet5_list = listout((t_object *)x);
    x->outlet4_string = outlet_new((t_object *)x,NULL);
    x->outlet3_bang = bangout((t_object *)x);
    x->outlet2_double = floatout((t_object *)x);
    x->outlet1_int = intout((t_object *)x); 
    
    return (x);
}

/**
 * definition of my methods
*/

void assist(t_inout *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { //inlet
        sprintf(s, "(int/float/bang/sym) I am inlet 1 "); // what we see in the overlay popup menu of the inlet
    }
    else {  // outlets
        switch(a){
            case 0: sprintf(s, "I am outlet int"); break; // what we see in the overlay popup menu of the left-most outlet
            case 1: sprintf(s, "I am outlet float"); break; 
            case 2: sprintf(s, "I am outlet bang"); break;
            case 3: sprintf(s, "I am outlet sym"); break;
            case 4: sprintf(s, "I am outlet list"); break; // what we see in the overlay popup menu of the right-most outlet
        }
    }
}

void inlet_int(t_inout *x, long n){
    outlet_int(x->outlet1_int, n); // send an Integer to an outlet
}

void inlet_float(t_inout *x, double n) {
    outlet_float(x->outlet2_double,n); // send an Integer to an outlet
}

void inlet_bang(t_inout *x) {
    outlet_bang(x->outlet3_bang); // send a Bang to an outlet
}

void inlet_sym(t_inout *x, t_symbol *s) {
    /**
     * send anything to an outlet (any type)
     * in this case there is not a list of t_atom
     * 0 is the length of the list (usually argc) 
     * NIL means that there is no list of t_atom (usually argv) 
    */
    outlet_anything(x->outlet4_string,s,0,NIL); 
}

void inlet_list(t_inout *x, t_symbol *s, short ac, t_atom *av){
    outlet_list(x->outlet5_list,0L,ac,av); // send a list to an outlet
}

```