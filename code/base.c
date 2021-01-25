
#include "ext.h"
#include "ext_obex.h"

typedef struct _inout
{
    t_object s_ob;
    
    // outlets (as void pointers)
    void *outlet1_int;
    void *outlet2_double;
    void *outlet3_string;
    void *outlet4_list;
    void *outlet5_bang;

} t_inout; 

/* 2
====================================================================
FUNCTION PROTOTYPES
Definition of the methods of your max object
====================================================================
*/

// constructor
void *inout_new();

// metodi che rispondono ai messaggi mandati all'oggetto attraverso gli inlets
// int/bang - list - string - double
// +---o-------o-------o--------o---+

void inlet_left_int(t_inout *x, long n); // inlet left-most
void inlet_left_bang(t_inout *x);
void inlet_left_float(t_inout *x, double n); // inlet right-most
void inlet_left_string(t_inout *x, char *str); // in questo caso il centrale
void inlet_left_list(t_inout *x, t_symbol *s, long l, t_atom *av); // in questo caso il centrale

static t_class *inout_class;

void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("meg.inout", (method)inout_new, (method)NULL,  (long)sizeof(t_inout), 0L, 0);
    
    // aggiungo alla classe i metodi
    // associamo un metodo in base al tipo di messaggio che l'oggetto riceve e in base all'inlet
    // i selettori dei messaggi non sono stringhe, ma speciali strutture dati chiamati symbol (t_symbol)
    // ogni simbolo in max è unico e contiene una stringa e un valore. Il dato del messaggio è passato come array di atoms (t_atmo), una struttura che può contenere interi, float, o anche pointers, identificati da un tag.
    
    // i tipi possibili sono: A_LONG - A_FLOAT - A_SYM - A_GIMME; quest'ultimo passa la lista ti atoms
    // i TIPED METHOD sono quelli in cui Max controlla il tipo di ogni atom in un messaggio per controllare la consistenza con ciò che si aspetta di ottenere

    /* 
    name : C-string defining the message (message selector)
...	One or more integers specifying the arguments to the message, in the standard Max type list format (see Chapter 3 of the Writing Externals in Max document for more information).
    */
    class_addmethod(c, (method)inlet_left_int, "int", A_LONG, 0); // left most inlet - A_LONG dice che la funzione accetta un intero long e lo zero successivo termina la lista degli argomenti.
    class_addmethod(c, (method)inlet_left_bang, "bang", 0);
    class_addmethod(c, (method)inlet_left_float, "float", A_FLOAT, 0); // right most inlet
    class_addmethod(c, (method)inlet_left_string, "sym", A_SYM, 0); // ...
    class_addmethod(c, (method)inlet_left_list, "list", A_GIMME, 0); // ...
    
    // aggiungo la classe al name space CLASS_BOX,
    // in modo che questa classe possa essere trovata dall'utente quando crea un oggetto in Max.
    class_register(CLASS_BOX, c);
    
    inout_class = c;
}

/* 5
    ====================================================================
    NEW INSTANCE ROUTINE
    Alloca la memoria per creare una istanza della classe e la inizializza;
    ====================================================================
*/

void *inout_new()
{
    // istanzio un oggetto della classe
    t_inout *x = (t_inout *)object_alloc(inout_class);
    
    // first inlet is created by default
    
    // creo un outlet di tipo intero - right-most outlet
    x->outlet1_int = intout((t_object *)x); // equivale a: outlet_new((t_object *)x, "int");
    x->outlet2_double = floatout((t_object *)x); // equivale a: outlet_new((t_object *)x, "int");
    x->outlet3_string = outlet_new((t_object *)x, "sym"); 
    x->outlet4_list = outlet_new((t_object *)x, "list"); 
    x->outlet5_bang = bangout((t_object *)x); 

    // creo un generico outlet
    //x->m_outlet2 = outlet_new((t_object *)x, NULL); // NULL indica che possiamo mandare fuori dall'outlet qualsiasi tipo di messaggio
    
    return (x);
}

/* 5
    ====================================================================
    MESSAGE HANDLERS
    Definizione dei metodi che trattano i messaggi ricevuti dall'oggetto
    ====================================================================
*/

void inlet_left_int(t_inout *x, long n){
    outlet_int(x->outlet1_int, n);
}

void inlet_left_bang(t_inout *x){
    outlet_bang(x->outlet5_bang);
}

void inlet_left_float(t_inout *x, double n) {
    outlet_float(x->outlet2_double,n);
}
void inlet_left_string(t_inout *x, char *str) {
    t_atom string_to_send;
    atom_setsym(&string_to_send, gensym(str));
    outlet_anything(x->outlet3_string, gensym("sym"), 1, &string_to_send);
}
void inlet_left_list(t_inout *x, t_symbol *s, long l, t_atom *av) {
    outlet_list(x->outlet4_list,s,l,av);
}


// void inlet3_list(t_inout *x, t_atom *n) {
//     post("dbg");
//     //x->s_value2 = n;
// }

// void inout_bang(t_base *x) {
    
//     t_atom argv[3];
//     atom_setlong(argv, 43);
//     atom_setsym(argv + 1, gensym("crazy")); // gen sym è il message selector
//     atom_setfloat(argv+2, 8.3);
    
//     post("value0 is %ld", x->s_value0); // left-most
//     post("value1 is %ld", x->s_value2); // centrale
//     post("value2 is %ld", x->s_value1); // right-most
    
//     x->sum = x->s_value0 + x->s_value1 + x->s_value2;
    
//     // outlet intero
//     outlet_int(x->m_outlet1, x->sum);
    
//     // outlet generico
//     // green è la "key" alla quale attacco un array di tipo t_atom di 3 elementi
//     outlet_anything(x->m_outlet2,gensym("green"), 3, argv);
// }

// void myobject_printargs(t_base *x, t_symbol *s, long argc, t_atom *argv){
//     long i;
//     t_atom *ap;
    
//     post("message selector is %s", s->s_name);
//     post("there are %ld arguments", argc);
    
//     // increment ap each time to get to the next atom
//     for(i=0, ap=argv; i<argc; i++, ap++){
//         switch(atom_gettype(ap)){
//             case A_LONG:
//                 post("%ld: %ld", i+1,atom_getlong(ap));
//                 break;
//             case A_FLOAT:
//                 post("%ld: %.2f", i+1,atom_getfloat(ap));
//                 break;
//             case A_SYM:
//                 post("%ld: %s", i+1,atom_getsym(ap)->s_name);
//                 break;
//             default:
//                 post("%ld: unknown atom type %ld", i+1,atom_gettype(ap));
//                 break;
//         }
//     }
// }


