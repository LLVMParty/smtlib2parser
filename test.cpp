extern "C"
{
#include "smtlib2abstractparser_private.h"
}

struct smtlib2_example_parser
{
    smtlib2_abstract_parser parent_;
    bool test = false;
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

int main(int argc, char** argv)
{
    smtlib2_example_parser ep = {};
    printf("ep: %p\n", &ep);
    smtlib2_abstract_parser_init(&ep.parent_, &ep);

    /* initialize the term parser and override virtual methods */
    auto pi = &ep.parent_.parent_;
    pi->set_logic = smtlib2_abstract_parser_set_logic;
    pi->declare_sort = smtlib2_abstract_parser_declare_sort;
    pi->define_sort = smtlib2_abstract_parser_define_sort;
    pi->push_sort_param_scope = smtlib2_abstract_parser_push_sort_param_scope;
    pi->pop_sort_param_scope = smtlib2_abstract_parser_pop_sort_param_scope;
    pi->declare_function = smtlib2_abstract_parser_declare_function;
    pi->declare_variable = smtlib2_abstract_parser_declare_variable;
    pi->define_function = smtlib2_abstract_parser_define_function;
    pi->push = smtlib2_abstract_parser_push;
    pi->pop = smtlib2_abstract_parser_pop;
    pi->assert_formula = smtlib2_abstract_parser_assert_formula;
    pi->check_sat = smtlib2_abstract_parser_check_sat;
    pi->get_unsat_core = smtlib2_abstract_parser_get_unsat_core;
    pi->get_proof = smtlib2_abstract_parser_get_proof;
    pi->set_str_option = smtlib2_abstract_parser_set_str_option;
    pi->set_int_option = smtlib2_abstract_parser_set_int_option;
    pi->set_rat_option = smtlib2_abstract_parser_set_rat_option;
    pi->get_info = smtlib2_abstract_parser_get_info;
    pi->set_info = smtlib2_abstract_parser_set_info;
    pi->get_assignment = smtlib2_abstract_parser_get_assignment;
    pi->get_value = smtlib2_abstract_parser_get_value;
    pi->get_model = smtlib2_abstract_parser_get_model;
    pi->exit = smtlib2_abstract_parser_exit;
    pi->handle_error = smtlib2_abstract_parser_handle_error;
    pi->push_let_scope = smtlib2_abstract_parser_push_let_scope;
    pi->pop_let_scope = smtlib2_abstract_parser_pop_let_scope;
    pi->push_quantifier_scope = smtlib2_abstract_parser_push_quantifier_scope;
    pi->pop_quantifier_scope = smtlib2_abstract_parser_pop_quantifier_scope;
    pi->make_term = smtlib2_abstract_parser_make_term;
    pi->make_number_term = smtlib2_abstract_parser_make_number_term;
    pi->annotate_term = smtlib2_abstract_parser_annotate_term;
    pi->define_let_binding = smtlib2_abstract_parser_define_let_binding;
    pi->make_sort = smtlib2_abstract_parser_make_sort;
    pi->make_parametric_sort = smtlib2_abstract_parser_make_parametric_sort;
    pi->make_function_sort = smtlib2_abstract_parser_make_function_sort;
    pi->set_internal_parsed_terms = smtlib2_abstract_parser_set_internal_parsed_terms;

    auto tp = ep.parent_.termparser_;
    smtlib2_term_parser_set_function_handler(tp, smtlib2_example_term_parser_functionhandler);
    smtlib2_term_parser_set_number_handler(tp, smtlib2_example_term_parser_numberhandler);

    // smtlib2_term_parser_set_handler()

    smtlib2_abstract_parser_parse(&ep.parent_, stdin);

    printf("response: %d\n", ep.parent_.response_);

    smtlib2_abstract_parser_deinit(&ep.parent_);
}