
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
    // istanzio un oggetto della classe
    t_inout *x = (t_inout *)object_alloc(inout_class);
    
    // first inlet is created by default
    // creo il secondo inlet che accetta float
    // floatin(x,1);

    // l'ordine con cui vengono qui creati definisce l'ordine degli outlet dell'oggetto in Max
    // il "più in alto" è il "più a destra"
    x->outlet5_list = outlet_new((t_object *)x,NULL);
    x->outlet4_string = outlet_new((t_object *)x,NULL);
    x->outlet3_bang = bangout((t_object *)x);
    x->outlet2_double = floatout((t_object *)x); // equivale a: outlet_new((t_object *)x, "float");
    x->outlet1_int = intout((t_object *)x); // equivale a: outlet_new((t_object *)x, "int");
    
    // per generico outlet: x->m_outlet2 = outlet_new((t_object *)x, NULL); // con NULL possiamo mandare fuori qualsiasi tipo
    return (x);
}

void assist(t_inout *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) { //inlet
        sprintf(s, "(int/float/bang/sym) I am inlet 1 ");
    }
    else {    // outlet
        switch(a){
            case 0: sprintf(s, "I am outlet int"); break;
            case 1: sprintf(s, "I am outlet float"); break;
            case 2: sprintf(s, "I am outlet bang"); break;
            case 3: sprintf(s, "I am outlet sym"); break;
            case 4: sprintf(s, "I am outlet list"); break;
        }
        
    }
}

void inlet_int(t_inout *x, long n){
    outlet_int(x->outlet1_int, n);
}

void inlet_float(t_inout *x, double n) {
    outlet_float(x->outlet2_double,n);
}

void inlet_bang(t_inout *x) {
    outlet_bang(x->outlet3_bang);
}

void inlet_sym(t_inout *x, t_symbol *s) {
    outlet_anything(x->outlet4_string,s,0,NIL);
}

void inlet_list(t_inout *x, t_symbol *s, short ac, t_atom *av){
    
    for(int i=0; i<ac; i++){
        outlet_anything(x->outlet5_list,atom_getsym(av+i),0,NIL);
    }
    
}

