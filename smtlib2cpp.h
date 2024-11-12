#pragma once

#include <cstdio>
#include <span>

#include "smtlib2types.h"
#include "smtlib2vector.h"

class smtlib2_cpp_parser
{
    struct smtlib2_cpp_inner* inner = nullptr;

public:
    smtlib2_cpp_parser();
    virtual ~smtlib2_cpp_parser();
    smtlib2_cpp_parser(const smtlib2_cpp_parser&) = delete;
    smtlib2_cpp_parser& operator=(const smtlib2_cpp_parser&) = delete;
    void parse(FILE* src);

protected:
    /**
     * callback for a "set-logic" command
     */
    virtual void set_logic(const char* logic);

    /**
     * callback for a "declare-sort" command
     */
    virtual void declare_sort(const char* sortname, int arity);

    /**
     * callback for a "define-sort" command
     * "params" is a vector of smtlib2_sort for the parameters,
     *          NULL when no parameters are given
     * "sort" is the "template" sort when parameters are used,
     *        otherwise it is a concrete sort
     */
    virtual void define_sort(const char* sortname, smtlib2_vector* params, smtlib2_sort sort);

    /**
     * callback for a "declare-fun" command
     */
    virtual void declare_function(const char* name, smtlib2_sort sort);

    /**
     * callback for declaring a quantified variable
     * This gets called for variables declared by an "exists" or "forall"
     * quantification, but also for parameters of a "define-fun" command
     */
    virtual void declare_variable(const char* name, smtlib2_sort sort);

    /**
     * callback for a "define-fun" command
     * "params" is a vector of smtlib2_term returned by make_term (see below),
     *          declared by declare_variable (see above) in the current scope
     *          (see push_quantifier_scope below),
     *          NULL when no parameters are given
     * "sort" is the sort of the definition
     * "term" is the term "template" term when parameters are used,
     *        or the actual definition when no parameters are given
     */
    virtual void define_function(const char* name, smtlib2_vector* params, smtlib2_sort sort, smtlib2_term term);

    /**
     * callback for a "push" command
     */
    virtual void push(int n);

    /**
     * callback for a "pop" command
     */
    virtual void pop(int n);

    /**
     * callback for an "assert" command
     */
    virtual void assert_formula(smtlib2_term term);

    /**
     * callback for a "check-sat" command
     */
    virtual void check_sat();

    /**
     * callback for a "get-assignment" command
     */
    virtual void get_assignment();

    /**
     * callback for a "get-assertions" command
     */
    virtual void get_assertions();

    /**
     * callback for a "get-unsat-core" command
     */
    virtual void get_unsat_core();

    /**
     * callback for a "get-proof" command
     */
    virtual void get_proof();

    /**
     * callback for a "set-option" command with string value
     */
    virtual void set_str_option(const char* keyword, const char* value);

    /**
     * callback for a "set-option" command with integer value
     * also options with true/false values will trigger this callback
     */
    virtual void set_int_option(const char* keyword, int value);

    /**
     * callback for a "set-option" command with a rational value (e.g. timeout)
     */
    virtual void set_rat_option(const char* keyword, double value);

    /**
     * callback for a "get-info" command
     */
    virtual void get_info(const char* keyword);

    /**
     * callback for a "set-info" command
     */
    virtual void set_info(const char* keyword, const char* value);

    /**
     * callback for a "get-value" command
     * "terms" is a vector of *string representations* of the terms for which
     *         the model value is requested. In order to get the actual terms,
     *         such strings should be parsed. For this, the (non-standard)
     *         ".internal-parse-terms" command can be used.
     *
     *         The reason for this choice is that the SMT-LIB v2 language
     *         mandates that responses to a get-value command return the same
     *         string representation as the input: by explicitly passing the
     *         strings themselves, it is actually possible to perform
     *         arbitrary simplifications to terms (including e.g. expansion of
     *         let bindings) directly at parsing time
     *
     *         See smtlib2yices.c for an example of use of
     *         ".internal-parse-terms" from within a "get-value" callback
     */
    virtual void get_value(smtlib2_vector* terms);

    /**
     * callback for a "get-model" command
     */
    virtual void get_model();

    /**
     * callback for a "exit" command
     */
    virtual void exit();

    /**
     * callback for handling parse errors
     */
    virtual void handle_error(const char* msg);

    /**
     * callback for the ".internal-parse-terms" command (see above)
     */
    virtual void set_internal_parsed_terms(smtlib2_vector* terms);

    /**
     * push a scope for let bindings. called every time a "let" is parsed
     */
    virtual void push_let_scope();

    /**
     * pop a scope for let bindings. called every time the closing parenthesis
     * for a "let" is parsed
     */
    virtual smtlib2_term pop_let_scope();

    /**
     * push a scope for quantified variables. called every time an "exist" or
     * "forall" is parsed, and also when a "define-fun" with parameters
     * is parsed
     */
    virtual smtlib2_term push_quantifier_scope();

    /**
     * pop a scope for quantified variabled. called when the closing
     * parenthesis for an "exists", "forall" or "define-fun" is parsed
     */
    virtual smtlib2_term pop_quantifier_scope();

    /**
     * push a scope for sort parameters. called when a "define-sort" is parsed
     */
    virtual void push_sort_param_scope();

    /**
     * pop a scope for sort parameters. called when the closing parenthesis
     * for a "define-sort" is parsed
     */
    virtual void pop_sort_param_scope();

    /**
     * callback for creating terms
     * "symbol" is the identifier associated to this term,
     *          which must have been declared or defined before by a
     *          "declare-fun", "define-fun", "declare-variable"
     *          command or by a let binding
     * "sort" is the requested sort for this term. it is not NULL only when
     *        an "(as term sort)" construct is used
     * "index" is the index for the identifier (a vector of integers),
     *         used in bit-vector terms.
     *         example: in "(_ extract 3 1)" "extract" is the symbol,
     *         and {3, 1} is the index
     * "args" is the vector of arguments for this term
     *        (a vector of smtlib2_term)
     */
    virtual smtlib2_term make_term(const char* symbol, smtlib2_sort sort, smtlib2_vector* index, smtlib2_vector* args);

    /**
     * callback for creating numbers
     * "numval" is the string representation of the number, in the given base
     *         (see below). notice that the prefix which identifies the base
     *         (e.g. "#b" for base 2) is not included in "numval"
     *         (so for instance when parsing "#b011" "numval" will be "011")
     * "width" is the bit-width of the number. this is zero if the number
     *         is not a bit-vector
     * "base" is the base used for the representation "numval".
     *        can be 2, 10 or 16
     */
    virtual smtlib2_term make_number_term(const char* numval, int width, int base);

    /**
     * callback for creating universally-quantified terms
     */
    virtual smtlib2_term make_forall_term(smtlib2_term term);

    /**
     * callback for creating existentially-quantified terms
     */
    virtual smtlib2_term make_exists_term(smtlib2_term term);

    /**
     * callback for attaching annotations to terms
     * "term" is the term to annotate
     * "annotations" is a vector of size-2 arrays of strings. each element
     *               represents a pair <keyword, value>. the parser checks
     *               that values are valid sexps, but doesn't interpret
     *               them in any way
     *
     * example: parsing "(! x :named pippo)" will result in a call to
     *          "annotate_term" with "term" set to "x" and annotations to
     *          { { ":named", "pippo" } }
     */
    virtual void annotate_term(smtlib2_term term, smtlib2_vector* annotations);

    /**
     * callback for defining let bindings
     * "symbol" is the name of the binding
     * "term" is the definition
     */
    virtual void define_let_binding(const char* symbol, smtlib2_term term);

    /**
     * callback for creating sorts
     * "sortname" is the name of the sort. it must have been declared with
     *            "declare-sort" or defined with "define-sort"
     * "index" is the sort index, a vector of integers
     *         (e.g. when parsing "(_ BitVec 32)"
     *         "sortname" is "BitVec" and "index" is { 32 })
     */
    virtual smtlib2_sort make_sort(const char* sortname, smtlib2_vector* index);

    /**
     * callback for instantiating parametric sorts
     * "name" is the name of the parametric sort. it must have been declared
     *        with "declare-sort" or defined with "define-sort"
     * "tps" is a vector of smtlib2_sort corresponding to the actual parameters
     *       for the parametric sort "name"
     */
    virtual smtlib2_sort make_parametric_sort(const char* name, smtlib2_vector* tps);

    /**
     * callback for creating function sorts
     * "tps" is a vector of size N of smtlib2_sort.
     *       the first N-1 elements are the sorts of the function domain,
     *       and the last one is the sort of the codomain
     */
    virtual smtlib2_sort make_function_sort(smtlib2_vector* tps);
};
