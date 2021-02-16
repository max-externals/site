
---
title: Hello object
permalink: /hello
nav_order: 3
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

But of course we don't handle only bangs, that would be boring, so we need a way to dispatch beteen different types, and that is achived using the third parameter of `class_addmethod` native possible values are:
- `int` 
- `float`
- `bang`
Or you can define your own simbol to select the method you prefer.

The forth parameters of `class_addmethod` define the type handled, that of course is strictly correlated with the symbol used for native symbols, and can arbitrarly chosen for custom symbols. You can find the list of all available types on the [SDK Docs](https://cycling74.com/sdk/max-sdk-8.0.3/html/group__atom.html#ga8aa6700e9f00b132eb376db6e39ade47)
The most important are:
- A_LONG
- A_FLOAT
- A_SYM
- A_GIMME
The selection of the type has a direct impact on the signature of the trigger function.




### Result

```c

#include "ext.h"
#include "ext_obex.h"

typedef struct _inout
{
    t_object s_ob;
    
    void *outlet1_int;
    void *outlet2_double;
    void *outlet3_bang;
    void *outlet4_string;

} t_inout;

void *inout_new();


void inlet_int(t_inout *x, long n); // inlet left-most
void inlet_left_bang(t_inout *x); // bang left-most
void inlet_left_sym(t_inout *x, t_symbol *s); // sym left-most
void inlet_right_float(t_inout *x, double n); // inlet right

static t_class *inout_class;

void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("meg.inout", (method)inout_new, (method)NULL,  (long)sizeof(t_inout), 0L, 0);
    
    // aggiungo alla classe i metodi; associamo un metodo in base al tipo di messaggio che l'oggetto riceve e in base all'inlet; i selettori dei messaggi non sono stringhe, ma speciali strutture dati chiamati symbol (t_symbol)
    // ogni simbolo in max è unico e contiene una stringa e un valore. Il dato del messaggio è passato come array di atoms (t_atom), una struttura che può contenere interi, float, o anche pointers, identificati da un tag.
    
    // i tipi possibili sono: A_LONG - A_FLOAT - A_SYM - A_GIMME; quest'ultimo passa la lista ti atoms
    // i TIPED METHOD sono quelli in cui Max controlla il tipo di ogni atom in un messaggio per controllare la consistenza con ciò che si aspetta di ottenere

    class_addmethod(c, (method)inlet_left_int, "int", A_LONG, 0); // left most inlet - A_LONG dice che la funzione accetta un intero long e lo zero successivo termina la lista degli argomenti.
    class_addmethod(c, (method)inlet_left_bang, "bang", 0); 
    class_addmethod(c, (method)inlet_left_sym, "sym", A_SYM, 0); 
    class_addmethod(c, (method)inlet_right_float, "float", A_FLOAT, 0); // right most inlet
    
    class_register(CLASS_BOX, c);
    
    inout_class = c;
}

void *inout_new()
{
    // istanzio un oggetto della classe
    t_inout *x = (t_inout *)object_alloc(inout_class);
    
    // first inlet is created by default
    // creo il secondo inlet che accetta float
    // floatin(x,1);

    // l'ordine con cui vengono qui creati definisce l'ordine degli outlet dell'oggetto in Max
    // il "più in alto" è il "più a destra"
    x->outlet4_string = outlet_new((t_object *)x,NULL);
    x->outlet3_bang = bangout((t_object *)x);
    x->outlet2_double = floatout((t_object *)x); // equivale a: outlet_new((t_object *)x, "float");
    x->outlet1_int = intout((t_object *)x); // equivale a: outlet_new((t_object *)x, "int");
    
    // per generico outlet: x->m_outlet2 = outlet_new((t_object *)x, NULL); // con NULL possiamo mandare fuori qualsiasi tipo
    return (x);
}

void inlet_left_int(t_inout *x, long n){
    outlet_int(x->outlet1_int, n);
}

void inlet_right_float(t_inout *x, double n) {
    outlet_float(x->outlet2_double,n);
}

void inlet_left_bang(t_inout *x) {
    outlet_bang(x->outlet3_bang);
}

void inlet_left_sym(t_inout *x, t_symbol *s) {
    outlet_anything(x->outlet4_string,s,0,NIL);
}
```