
/**
 * oggetto che presenta 2 inlet: uno intero e uno float;
 * e passa i dati a 2 output: uno intero e uno float
 */

#include "ext.h"
#include "ext_obex.h"

typedef struct _inout
{
    t_object s_ob;
    
    void *outlet1_int;
    void *outlet2_double;

} t_inout;

void *inout_new();

// metodi che rispondono ai messaggi mandati all'oggetto attraverso gli inlets / int - double
// +--o---o----+

void inlet_left_int(t_inout *x, long n); // inlet left-most
void inlet_left_float(t_inout *x, double n); // inlet right

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
    class_addmethod(c, (method)inlet_left_float, "ft1", A_FLOAT, 0); // right most inlet
    
    class_register(CLASS_BOX, c);
    
    inout_class = c;
}

void *inout_new()
{
    // istanzio un oggetto della classe
    t_inout *x = (t_inout *)object_alloc(inout_class);
    
    // first inlet is created by default
    // creo il secondo inlet che accetta float
    floatin(x,1);

    // l'ordine con cui vengono qui creati definisce l'ordine degli outlet dell'oggetto in Max
    // il "più in alto" è il "più a destra"
    x->outlet2_double = floatout((t_object *)x); // equivale a: outlet_new((t_object *)x, "int");
    x->outlet1_int = intout((t_object *)x); // equivale a: outlet_new((t_object *)x, "int");
    
    // per generico outlet: x->m_outlet2 = outlet_new((t_object *)x, NULL); // con NULL possiamo mandare fuori qualsiasi tipo
    return (x);
}

void inlet_left_int(t_inout *x, long n){
    outlet_int(x->outlet1_int, n);
}

void inlet_left_float(t_inout *x, double n) {
    outlet_float(x->outlet2_double,n);
}
