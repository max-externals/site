/**
   CODICE BASE
   Give the same name to the project, the max object and the .c file; prepend "_" to the struct
*/

// standard Max LIBRARY to include, always required
#include "ext.h"

// required for new style Max object
#include "ext_obex.h"

/* 1
====================================================================
DEFINE A STRUCTURE
Insert here the global variables of your object, including the object itself.
The outlets of the object have to be included here as pointers.
====================================================================
*/

typedef struct _base
{
    // the object itself (always need, first);
    // t_object is the type for the max objects
    t_object s_ob;
    
    // global variables you need
    long s_value0;
    long s_value1;
    long sum;
    
    // outlets (as void pointers)
    void *m_outlet1;
    void *m_outlet2;

} t_base; // with this you instantiate the structure; this is the name of the instance of the structure _base

/* 2
====================================================================
FUNCTION PROTOTYPES
Definition of the methods of your max object
====================================================================
*/

// constructor
void *base_new();

// metodi che rispondono ai messaggi mandati all'oggetto attraverso gli inlets
void inout_int(t_base *x, long n); // inlet left-most
void inout_in1(t_base *x, long n); // inlet right-most
void inout_in2(t_base *x, long n); // in questo caso il centrale

// bang
void inout_bang(t_base *x);

// metodo che usa A_GIMME
// t_symbol è il "selettore"
// argc è il numero di elementi nell'argv (che è l'array di atom)
void myobject_printargs(t_base *x, t_symbol *s, long argc, t_atom *argv);

/* 3
    ====================================================================
    GLOBAL CLASS POINTER VARIABLE
    Definisco una variabile puntatore di tipo t_class
    ====================================================================
*/

static t_class *inout_class;

/* 4
    ====================================================================
    INITIALIZATION ROUTINE (ext_main)
    Funzione che viene chiamata da Max quando creo un nuovo oggetto
    ====================================================================
*/

void ext_main(void *r)
{
    t_class *c;
    
    // creo la classe ("inlets" è il nome dell'oggetto in Max)
    // 1. nome; 2. the new instance routine (chiamo il costruttore); 3. una free function;
    // 4. la size della struttura; 5. un argomento non più usato;
    // 6. descrizione degli argomenti da inserire quando creimo l'oggetto
    c = class_new("inout", (method)base_new, (method)NULL,  (long)sizeof(t_base), 0L , A_GIMME, 0);
    
    // aggiungo alla classe i metodi
    // associamo un metodo in base al tipo di messaggio che l'oggetto riceve e in base all'inlet
    // i selettori dei messaggi non sono stringhe, ma speciali strutture dati chiamati symbol (t_symbol)
    // ogni simbolo in max è unico e contiene una stringa e un valore. Il dato del messaggio è passato come array di atoms (t_atmo), una struttura che può contenere interi, float, o anche pointers, identificati da un tag.
    
    
    // i tipi possibili sono: A_LONG - A_FLOAT - A_SYM - A_GIMME; quest'ultimo passa la lista ti atoms
    // i TIPED METHOD sono quelli in cui Max controlla il tipo di ogni atom in un messaggio per controllare la consistenza con ciò che si aspetta di ottenere
    class_addmethod(c, (method)inout_int, "int", A_LONG, 0); // left most inlet - A_LONG dice che la funzione accetta un intero long e lo zero successivo termina la lista degli argomenti.
    
    class_addmethod(c, (method)inout_in1, "in1", A_LONG, 0); // right most inlet
    class_addmethod(c, (method)inout_in2, "in2", A_LONG, 0); // ...

    class_addmethod(c, (method)inout_bang, "bang", 0);
    
    class_addmethod(c, (method)myobject_printargs, "printargs", A_GIMME, 0);
    
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

void *base_new()
{
    // istanzio un oggetto della classe
    t_base *x = (t_base *)object_alloc(inout_class);
    
    // creo gli inlets
    intin(x, 1); // right-most
    intin(x, 2);
    
    // creo un outlet di tipo intero - right-most outlet
    x->m_outlet1 = intout((t_object *)x); // equivale a: outlet_new((t_object *)x, "int");
    
    // creo un generico outlet
    x->m_outlet2 = outlet_new((t_object *)x, NULL); // NULL indica che possiamo mandare fuori dall'outlet qualsiasi tipo di messaggio
    
    return (x);
}

/* 5
    ====================================================================
    MESSAGE HANDLERS
    Definizione dei metodi che trattano i messaggi ricevuti dall'oggetto
    ====================================================================
*/

void inout_int(t_base *x, long n) {
    x->s_value0 = n;
}

void inout_in1(t_base *x, long n) {
    x->s_value1 = n;
}

void inout_in2(t_base *x, long n) {
    x->s_value2 = n;
}

void inout_bang(t_base *x) {
    
    t_atom argv[3];
    atom_setlong(argv, 43);
    atom_setsym(argv + 1, gensym("crazy")); // gen sym è il message selector
    atom_setfloat(argv+2, 8.3);
    
    post("value0 is %ld", x->s_value0); // left-most
    post("value1 is %ld", x->s_value2); // centrale
    post("value2 is %ld", x->s_value1); // right-most
    
    x->sum = x->s_value0 + x->s_value1 + x->s_value2;
    
    // outlet intero
    outlet_int(x->m_outlet1, x->sum);
    
    // outlet generico
    // green è la "key" alla quale attacco un array di tipo t_atom di 3 elementi
    outlet_anything(x->m_outlet2,gensym("green"), 3, argv);
}

void myobject_printargs(t_base *x, t_symbol *s, long argc, t_atom *argv){
    long i;
    t_atom *ap;
    
    post("message selector is %s", s->s_name);
    post("there are %ld arguments", argc);
    
    // increment ap each time to get to the next atom
    for(i=0, ap=argv; i<argc; i++, ap++){
        switch(atom_gettype(ap)){
            case A_LONG:
                post("%ld: %ld", i+1,atom_getlong(ap));
                break;
            case A_FLOAT:
                post("%ld: %.2f", i+1,atom_getfloat(ap));
                break;
            case A_SYM:
                post("%ld: %s", i+1,atom_getsym(ap)->s_name);
                break;
            default:
                post("%ld: unknown atom type %ld", i+1,atom_gettype(ap));
                break;
        }
    }
}


