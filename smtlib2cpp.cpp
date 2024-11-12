#include "smtlib2cpp.h"

extern "C"
{
#include "smtlib2abstractparser_private.h"
}

struct smtlib2_cpp_inner
{
    smtlib2_abstract_parser parent_;
    smtlib2_cpp_parser* parser;

    smtlib2_parser_interface* pi()
    {
        return &parent_.parent_;
    }
};

smtlib2_term smtlib2_example_term_parser_numberhandler(smtlib2_context ctx,
    const char* rep,
    unsigned int width,
    unsigned int base)
{
    printf("smtlib2_example_term_parser_numberhandler(%p, %s, %d, %d)\n", ctx, rep, width, base);
    ((smtlib2_abstract_parser*)ctx)->response_ = SMTLIB2_RESPONSE_ERROR;
    return NULL;
}

smtlib2_term smtlib2_example_term_parser_functionhandler(smtlib2_context ctx,
    const char* symbol,
    smtlib2_sort sort,
    smtlib2_vector* index,
    smtlib2_vector* args)
{
    printf("example_term_functionhandler(%p, %s, %p, %p, %p)\n", ctx, symbol, sort, index, args);
    ((smtlib2_abstract_parser*)ctx)->response_ = SMTLIB2_RESPONSE_ERROR;

    return NULL;
}

// References:
// - https://stackoverflow.com/a/30930497/1806760
// - https://medium.com/@matthieu.dorier/deducing-argument-and-return-types-of-a-callable-in-c-834598f6a385
template<typename FunctionType, typename = void>
struct ForwardMember;

template<class ReturnType, class... Args>
struct ForwardMember<ReturnType (smtlib2_cpp_parser::*)(Args...)>
{
    template<ReturnType (smtlib2_cpp_parser::*func)(Args...)>
    static ReturnType call(smtlib2_parser_interface* p, Args... args)
    {
        return (((smtlib2_cpp_inner*)p)->parser->*func)(args...);
    }
};

#define FORWARD(member) ForwardMember<decltype(&smtlib2_cpp_parser::member)>::call<&smtlib2_cpp_parser::member>

smtlib2_cpp_parser::smtlib2_cpp_parser()
{
    inner = new smtlib2_cpp_inner();
    inner->parent_ = {};
    inner->parser = this;
    smtlib2_abstract_parser_init(&inner->parent_, inner);
    inner->parent_.print_success_ = false;

    /* initialize the term parser and override virtual methods */
    auto pi = inner->pi();
    pi->set_logic = FORWARD(set_logic);
    pi->declare_sort = FORWARD(declare_sort);
    pi->define_sort = FORWARD(define_sort);
    pi->declare_function = FORWARD(declare_function);
    pi->declare_variable = FORWARD(declare_variable);
    pi->define_function = FORWARD(define_function);
    pi->push = FORWARD(push);
    pi->pop = FORWARD(pop);
    pi->assert_formula = FORWARD(assert_formula);
    pi->check_sat = FORWARD(check_sat);
    pi->get_assignment = FORWARD(get_assignment);
    pi->get_assertions = FORWARD(get_assertions);
    pi->get_unsat_core = FORWARD(get_unsat_core);
    pi->get_proof = FORWARD(get_proof);
    pi->set_str_option = FORWARD(set_str_option);
    pi->set_int_option = FORWARD(set_int_option);
    pi->set_rat_option = FORWARD(set_rat_option);
    pi->get_info = FORWARD(get_info);
    pi->set_info = FORWARD(set_info);
    pi->get_value = FORWARD(get_value);
    pi->get_model = FORWARD(get_model);
    pi->exit = FORWARD(exit);
    pi->handle_error = FORWARD(handle_error);
    pi->set_internal_parsed_terms = FORWARD(set_internal_parsed_terms);
    pi->push_let_scope = FORWARD(push_let_scope);
    pi->pop_let_scope = FORWARD(pop_let_scope);
    pi->push_quantifier_scope = FORWARD(push_quantifier_scope);
    pi->pop_quantifier_scope = FORWARD(pop_quantifier_scope);
    pi->push_sort_param_scope = FORWARD(push_sort_param_scope);
    pi->pop_sort_param_scope = FORWARD(pop_sort_param_scope);
    pi->make_term = FORWARD(make_term);
    pi->make_number_term = FORWARD(make_number_term);
    pi->make_forall_term = FORWARD(make_forall_term);
    pi->make_exists_term = FORWARD(make_exists_term);
    pi->annotate_term = FORWARD(annotate_term);
    pi->define_let_binding = FORWARD(define_let_binding);
    pi->make_sort = FORWARD(make_sort);
    pi->make_parametric_sort = FORWARD(make_parametric_sort);
    pi->make_function_sort = FORWARD(make_function_sort);

    auto tp = inner->parent_.termparser_;
    smtlib2_term_parser_set_function_handler(tp, smtlib2_example_term_parser_functionhandler);
    smtlib2_term_parser_set_number_handler(tp, smtlib2_example_term_parser_numberhandler);

    // TODO: add these
    // smtlib2_term_parser_set_handler()
}

smtlib2_cpp_parser::~smtlib2_cpp_parser()
{
    smtlib2_abstract_parser_deinit(&inner->parent_);
}

void smtlib2_cpp_parser::parse(FILE* src)
{
    smtlib2_abstract_parser_parse(&inner->parent_, src);
    printf("response: %d\n", inner->parent_.response_);
}

void smtlib2_cpp_parser::set_logic(const char* logic)
{
    return smtlib2_abstract_parser_set_logic(inner->pi(), logic);
}

void smtlib2_cpp_parser::declare_sort(const char* sortname, int arity)
{
    return smtlib2_abstract_parser_declare_sort(inner->pi(), sortname, arity);
}

void smtlib2_cpp_parser::define_sort(const char* sortname, smtlib2_vector* params, smtlib2_sort sort)
{
    return smtlib2_abstract_parser_define_sort(inner->pi(), sortname, params, sort);
}

void smtlib2_cpp_parser::declare_function(const char* name, smtlib2_sort sort)
{
    return smtlib2_abstract_parser_declare_function(inner->pi(), name, sort);
}

void smtlib2_cpp_parser::declare_variable(const char* name, smtlib2_sort sort)
{
    return smtlib2_abstract_parser_declare_variable(inner->pi(), name, sort);
}

void smtlib2_cpp_parser::define_function(const char* name, smtlib2_vector* params, smtlib2_sort sort, smtlib2_term term)
{
    return smtlib2_abstract_parser_define_function(inner->pi(), name, params, sort, term);
}

void smtlib2_cpp_parser::push(int n)
{
    return smtlib2_abstract_parser_push(inner->pi(), n);
}

void smtlib2_cpp_parser::pop(int n)
{
    return smtlib2_abstract_parser_pop(inner->pi(), n);
}

void smtlib2_cpp_parser::assert_formula(smtlib2_term term)
{
    return smtlib2_abstract_parser_assert_formula(inner->pi(), term);
}

void smtlib2_cpp_parser::check_sat()
{
    return smtlib2_abstract_parser_check_sat(inner->pi());
}

void smtlib2_cpp_parser::get_assignment()
{
    return smtlib2_abstract_parser_get_assignment(inner->pi());
}

void smtlib2_cpp_parser::get_assertions()
{
    return smtlib2_abstract_parser_get_assertions(inner->pi());
}

void smtlib2_cpp_parser::get_unsat_core()
{
    return smtlib2_abstract_parser_get_unsat_core(inner->pi());
}

void smtlib2_cpp_parser::get_proof()
{
    return smtlib2_abstract_parser_get_proof(inner->pi());
}

void smtlib2_cpp_parser::set_str_option(const char* keyword, const char* value)
{
    return smtlib2_abstract_parser_set_str_option(inner->pi(), keyword, value);
}

void smtlib2_cpp_parser::set_int_option(const char* keyword, int value)
{
    return smtlib2_abstract_parser_set_int_option(inner->pi(), keyword, value);
}

void smtlib2_cpp_parser::set_rat_option(const char* keyword, double value)
{
    return smtlib2_abstract_parser_set_rat_option(inner->pi(), keyword, value);
}

void smtlib2_cpp_parser::get_info(const char* keyword)
{
    return smtlib2_abstract_parser_get_info(inner->pi(), keyword);
}

void smtlib2_cpp_parser::set_info(const char* keyword, const char* value)
{
    return smtlib2_abstract_parser_set_info(inner->pi(), keyword, value);
}

void smtlib2_cpp_parser::get_value(smtlib2_vector* terms)
{
    return smtlib2_abstract_parser_get_value(inner->pi(), terms);
}

void smtlib2_cpp_parser::get_model()
{
    return smtlib2_abstract_parser_get_model(inner->pi());
}

void smtlib2_cpp_parser::exit()
{
    return smtlib2_abstract_parser_exit(inner->pi());
}

void smtlib2_cpp_parser::handle_error(const char* msg)
{
    return smtlib2_abstract_parser_handle_error(inner->pi(), msg);
}

void smtlib2_cpp_parser::set_internal_parsed_terms(smtlib2_vector* terms)
{
    return smtlib2_abstract_parser_set_internal_parsed_terms(inner->pi(), terms);
}

void smtlib2_cpp_parser::push_let_scope()
{
    return smtlib2_abstract_parser_push_let_scope(inner->pi());
}

smtlib2_term smtlib2_cpp_parser::pop_let_scope()
{
    return smtlib2_abstract_parser_pop_let_scope(inner->pi());
}

smtlib2_term smtlib2_cpp_parser::push_quantifier_scope()
{
    return smtlib2_abstract_parser_push_quantifier_scope(inner->pi());
}

smtlib2_term smtlib2_cpp_parser::pop_quantifier_scope()
{
    return smtlib2_abstract_parser_pop_quantifier_scope(inner->pi());
}

void smtlib2_cpp_parser::push_sort_param_scope()
{
    return smtlib2_abstract_parser_push_sort_param_scope(inner->pi());
}

void smtlib2_cpp_parser::pop_sort_param_scope()
{
    return smtlib2_abstract_parser_pop_sort_param_scope(inner->pi());
}

smtlib2_term smtlib2_cpp_parser::make_term(const char* symbol, smtlib2_sort sort, smtlib2_vector* index, smtlib2_vector* args)
{
    return smtlib2_abstract_parser_make_term(inner->pi(), symbol, sort, index, args);
}

smtlib2_term smtlib2_cpp_parser::make_number_term(const char* numval, int width, int base)
{
    return smtlib2_abstract_parser_make_number_term(inner->pi(), numval, width, base);
}

smtlib2_term smtlib2_cpp_parser::make_forall_term(smtlib2_term term)
{
    return smtlib2_abstract_parser_make_forall_term(inner->pi(), term);
}

smtlib2_term smtlib2_cpp_parser::make_exists_term(smtlib2_term term)
{
    return smtlib2_abstract_parser_make_exists_term(inner->pi(), term);
}

void smtlib2_cpp_parser::annotate_term(smtlib2_term term, smtlib2_vector* annotations)
{
    return smtlib2_abstract_parser_annotate_term(inner->pi(), term, annotations);
}

void smtlib2_cpp_parser::define_let_binding(const char* symbol, smtlib2_term term)
{
    return smtlib2_abstract_parser_define_let_binding(inner->pi(), symbol, term);
}

smtlib2_sort smtlib2_cpp_parser::make_sort(const char* sortname, smtlib2_vector* index)
{
    return smtlib2_abstract_parser_make_sort(inner->pi(), sortname, index);
}

smtlib2_sort smtlib2_cpp_parser::make_parametric_sort(const char* name, smtlib2_vector* tps)
{
    return smtlib2_abstract_parser_make_parametric_sort(inner->pi(), name, tps);
}

smtlib2_sort smtlib2_cpp_parser::make_function_sort(smtlib2_vector* tps)
{
    return smtlib2_abstract_parser_make_function_sort(inner->pi(), tps);
}
