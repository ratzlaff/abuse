/*
 *  Abuse — dark 2D side-scrolling platform game
 *  Copyright © 1995 Crack dot Com
 *  Copyright © 2005—2016 Sam Hocevar <sam@hocevar.net>
 *
 *  This software was released into the Public Domain. As with most public
 *  domain software, no warranty is made or implied by Crack dot Com, by
 *  Jonathan Clark, or by Sam Hocevar.
 */

#pragma once

#include <cstdlib>
#include <stdint.h>

#define Cell void
#define MAX_LISP_TOKEN_LEN 200

#define NILP(x) ((x) == nullptr)
#define DEFINEDP(x) ((x) != lisp::obj::undefined)

class bFILE;
extern bFILE *current_print_file;

enum
{
    L_BAD_CELL, // error catching type
    L_CONS_CELL,
    L_NUMBER,
    L_SYMBOL,
    L_SYS_FUNCTION,
    L_USER_FUNCTION,
    L_STRING,
    L_CHARACTER,
    L_C_FUNCTION,
    L_C_BOOL,
    L_L_FUNCTION,
    L_POINTER,
    L_OBJECT_VAR,
    L_1D_ARRAY,
    L_FIXED_POINT,
    L_COLLECTED_OBJECT,
};

// FIXME: switch this to uint8_t one day? it still breaks stuff
typedef uint8_t ltype;

struct LSpace
{
    size_t GetFree();
    void *Alloc(size_t size);

    void *Mark();
    void Restore(void *val);
    void Clear();

    static LSpace Tmp, Perm, Gc;
    static LSpace *Current;

    uint8_t *m_data;
    uint8_t *m_free;
    char const *m_name;
    size_t m_size;
};

struct LObject
{
    /* Members */
    ltype m_type;
};

struct LObjectVar : LObject
{
    /* Factories */
    static LObjectVar *Create(int index);

    /* Members */
    int m_index;
};

struct LList : LObject
{
    /* Factories */
    static LList *Create();

    /* Methods */
    size_t GetLength();
    LList *Assoc(LObject *item);

    /* Members */
    LObject *m_cdr, *m_car;
};

struct LNumber : LObject
{
    /* Factories */
    static LNumber *Create(long num);

    /* Members */
    long m_num;
};

struct LRedirect : LObject
{
    /* Members */
    LObject *m_ref;
};

struct LString : LObject
{
    /* Factories */
    static LString *Create(char const *string);
    static LString *Create(char const *string, int length);
    static LString *Create(int length);

    /* Methods */
    char *GetString();

    /* Members */
private:
    char m_str[1]; /* Can be allocated much larger than 1 */
};

struct LSymbol : LObject
{
    /* Methods */
    LObject *EvalFunction(void *arg_list);
    LObject *EvalUserFunction(LList *arg_list);

    LString *GetName();
    LObject *GetFunction();
    LObject *GetValue();

    void SetFunction(LObject *fun);
    void SetValue(LObject *value);
    void SetNumber(long num);

    /* Members */
#ifdef L_PROFILE
    float time_taken;
#endif
    LObject *m_value;
    LObject *m_function;
    LString *m_name;
    LSymbol *m_left, *m_right; // tree structure
};

struct LSysFunction : LObject
{
    /* Methods */
    LObject *EvalFunction(LList *arg_list);

    /* Members */
    short min_args, max_args;
    short fun_number;
};

struct LUserFunction : LObject
{
    LList *arg_list, *block_list;
};

struct LArray : LObject
{
    /* Factories */
    static LArray *Create(size_t len, void *rest);

    /* Methods */
    inline LObject **GetData() { return m_data; }
    LObject *Get(int x);

    /* Members */
    size_t m_len;

private:
    LObject *m_data[1]; /* Can be allocated much larger than 1 */
};

struct LChar : LObject
{
    /* Factories */
    static LChar *Create(uint16_t ch);

    /* Methods */
    uint16_t GetValue();

    /* Members */
    uint16_t m_ch;
};

struct LPointer : LObject
{
    /* Factories */
    static LPointer *Create(void *addr);

    /* Members */
    void *m_addr;
};

struct LFixedPoint : LObject
{
    /* Factories */
    static LFixedPoint *Create(int32_t x);

    /* Members */
    int32_t m_fixed;
};

static inline ltype item_type(void const *x)
{
    return x ? *static_cast<ltype const *>(x) : ltype(L_CONS_CELL);
}

struct lisp
{
    static void init();
    static void uninit();
    static void init_constants();

    static LSymbol *find_sym(char const *name, bool create = false);
    static LSymbol *make_sym(char const *name);

    static LObject *compile(char const *&s);
    static LObject *eval(LObject *);
    static void print(LObject *);

    // Maths functions
    static int32_t atan2(int32_t dy, int32_t dx);
    static int32_t sin(int32_t x);
    static int32_t cos(int32_t x);

    // Garbage collect temporary or permanent spaces
    static void collect_space(LSpace *which_space, int grow);

    // Use this object when a reference to null has to be provided
    // (the caller must not try to write to it)
    static LObject *null;

    // List getters
    static LObject *nth(int num, void *list);

    static inline LObject *&cdr(void *c)
    {
        if (!c || item_type(c) != ltype(L_CONS_CELL))
            return lisp::null;
        return ((LList *)c)->m_cdr;
    }

    static inline LObject *&car(void *c)
    {
        if (!c || item_type(c) != ltype(L_CONS_CELL))
            return lisp::null;
        return ((LList *)c)->m_car;
    }

    static inline LObject *&caar(void *c) { return car(car(c)); }
    static inline LObject *&cadr(void *c) { return car(cdr(c)); }
    static inline LObject *&cdar(void *c) { return cdr(car(c)); }
    static inline LObject *&cddr(void *c) { return cdr(cdr(c)); }

    static inline LObject *&caaar(void *c) { return car(caar(c)); }
    static inline LObject *&caadr(void *c) { return car(cadr(c)); }
    static inline LObject *&cadar(void *c) { return car(cdar(c)); }
    static inline LObject *&caddr(void *c) { return car(cddr(c)); }
    static inline LObject *&cdaar(void *c) { return cdr(caar(c)); }
    static inline LObject *&cdadr(void *c) { return cdr(cadr(c)); }
    static inline LObject *&cddar(void *c) { return cdr(cdar(c)); }
    static inline LObject *&cdddr(void *c) { return cdr(cddr(c)); }

    static inline LObject *&caaaar(void *c) { return car(caaar(c)); }
    static inline LObject *&caaadr(void *c) { return car(caadr(c)); }
    static inline LObject *&caadar(void *c) { return car(cadar(c)); }
    static inline LObject *&caaddr(void *c) { return car(caddr(c)); }
    static inline LObject *&cadaar(void *c) { return car(cdaar(c)); }
    static inline LObject *&cadadr(void *c) { return car(cdadr(c)); }
    static inline LObject *&caddar(void *c) { return car(cddar(c)); }
    static inline LObject *&cadddr(void *c) { return car(cdddr(c)); }
    static inline LObject *&cdaaar(void *c) { return cdr(caaar(c)); }
    static inline LObject *&cdaadr(void *c) { return cdr(caadr(c)); }
    static inline LObject *&cdadar(void *c) { return cdr(cadar(c)); }
    static inline LObject *&cdaddr(void *c) { return cdr(caddr(c)); }
    static inline LObject *&cddaar(void *c) { return cdr(cdaar(c)); }
    static inline LObject *&cddadr(void *c) { return cdr(cdadr(c)); }
    static inline LObject *&cdddar(void *c) { return cdr(cddar(c)); }
    static inline LObject *&cddddr(void *c) { return cdr(cdddr(c)); }

    // Predefined objects
    struct obj
    {
        static LObject *undefined;
    };

    // Predefined symbols
    struct sym
    {
        static LSymbol *true_;
        static LSymbol *list;
        static LSymbol *string;
        static LSymbol *quote;
        static LSymbol *backquote;
        static LSymbol *comma;
        static LSymbol *do_;
        static LSymbol *in;
        static LSymbol *aref;
        static LSymbol *if_;
        static LSymbol *progn;
        static LSymbol *car;
        static LSymbol *cdr;

        static LSymbol *eq;
        static LSymbol *zero;
        static LSymbol *eq0;

        static LSymbol *if_1progn;
        static LSymbol *if_2progn;
        static LSymbol *if_12progn;
        static LSymbol *not_;
    };

private:
    // Symbol tree
    static LSymbol *symbol_root;
    static size_t symbol_count;

    // Garbage collector
    static LArray *collect_array(LArray *x);
    static LList *collect_list(LList *x);
    static LObject *collect_object(LObject *x);
    static void collect_symbols(LSymbol *root);
    static void collect_stacks();
};

void perm_space();
void tmp_space();
void *lpointer_value(void *lpointer);
int32_t lnumber_value(void *lnumber);
long lfixed_point_value(void *c);
void *lisp_atom(void *i);
void *lisp_eq(void *n1, void *n2);
void *lisp_equal(void *n1, void *n2);
void *eval_block(void *list);
void resize_tmp(size_t new_size);
void resize_perm(size_t new_size);

void push_onto_list(void *object, void *&list);
LSymbol *add_c_object(void *symbol, int index);
LSymbol *add_c_function(char const *name, short min_args, short max_args, short number);
LSymbol *add_c_bool_fun(char const *name, short min_args, short max_args, short number);
LSymbol *add_lisp_function(char const *name, short min_args, short max_args, short number);
int read_ltoken(char *&s, char *buffer);
void print_trace_stack(int max_levels);


LSysFunction *new_lisp_sys_function(int min_args, int max_args, int fun_number);
LSysFunction *new_lisp_c_function(int min_args, int max_args, int fun_number);
LSysFunction *new_lisp_c_bool(int min_args, int max_args, int fun_number);

LUserFunction *new_lisp_user_function(LList *arg_list, LList *block_list);

LSysFunction *new_user_lisp_function(int min_args, int max_args, int fun_number);

int end_of_program(char *s);

extern "C" {
void lbreak(const char *format, ...);
};

extern void clisp_init();                      // external initalizer call by lisp_init()
extern long c_caller(long number, void *arg);  // exten c function switches on number
extern void *l_caller(long number, void *arg);  // exten lisp function switches on number

extern void *l_obj_get(long number);  // exten lisp function switches on number
extern void l_obj_set(long number, void *arg);  // exten lisp function switches on number
extern void l_obj_print(long number);  // exten lisp function switches on number

// FIXME: get rid of this later
static inline LObject *symbol_value(void *sym) { return ((LSymbol *)sym)->GetValue(); }
static inline char *lstring_value(void *str) { return ((LString *)str)->GetString(); }

