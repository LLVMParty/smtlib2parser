

#include "smtlib2cpp.h"

#include <string>
#include <vector>
#include <unordered_map>

struct Sort
{
    virtual ~Sort() = default;
};

struct NamedSort : Sort
{
    std::string name;

    explicit NamedSort(std::string name)
        : name(std::move(name))
    {
    }
};

struct FunctionSort : Sort
{
    std::vector<Sort*> params;
    Sort* result = nullptr;

    FunctionSort(std::vector<Sort*> params, Sort* result)
        : params(std::move(params))
        , result(result)
    {
    }
};

struct ParametricSort : Sort
{
    std::string name;
    std::vector<Sort*> params;

    ParametricSort(std::string name, std::vector<Sort*> params)
        : name(std::move(name))
        , params(std::move(params))
    {
    }
};

struct BitVecSort : Sort
{
    uint32_t length;

    explicit BitVecSort(uint32_t length)
        : length(length)
    {
    }
};

struct Function
{
    std::string name;
    Sort* sort = nullptr;

    Function(std::string name, Sort* sort)
        : name(std::move(name)), sort(sort)
    {
    }
};

struct Term
{
    virtual ~Term() = default;
};

struct NumberTerm : Term
{
    std::string numval;
    int width;
    int base;

    NumberTerm(std::string numval, int width, int base)
        : numval(std::move(numval)), width(width), base(base)
    {
    }
};

struct VariableTerm : Term
{
    std::string name;

    explicit VariableTerm(std::string name)
        : name(std::move(name))
    {
    }
};

struct FunctionTerm : Term
{
    std::string name;
    std::vector<Term*> args;

    FunctionTerm(std::string name, std::vector<Term*> args)
        : name(std::move(name)), args(std::move(args))
    {
    }
};

struct test_parser : smtlib2_cpp_parser
{
    // TODO: implement scope push/pop
    std::unordered_map<std::string, Function*> m_functions;
    std::unordered_map<std::string, int> m_declared_sorts;
    std::unordered_map<std::string, NamedSort*> m_named_sorts;
    std::unordered_map<size_t, BitVecSort*> m_bitvec_sorts;

    test_parser()
    {
        m_named_sorts.emplace("Bool", new NamedSort("Bool"));
        m_named_sorts.emplace("Int", new NamedSort("Int"));
        m_named_sorts.emplace("Real", new NamedSort("Real"));
    }

    void declare_function(const char* name, smtlib2_sort sort) override
    {
        auto func = new Function(name, (Sort*)sort);

        printf("declare_function %s S:%p -> F:%p\n", name, sort, func);

        if (!m_functions.emplace(name, func).second)
        {
            set_error("failed to declare function %s", name);
        }
    }

    void define_function(const char* name, smtlib2_vector* params, smtlib2_sort sort, smtlib2_term term) override
    {
        printf("define_function %s %p %p %p\n", name, params, sort, term);
        // TODO: implement
    }

    void declare_sort(const char* sortname, int arity) override
    {
        // TODO: set parser error here correctly
        printf("declare_sort %s %d\n", sortname, arity);
        if (m_named_sorts.count(sortname) > 0)
        {
            printf("sort already declared: %s\n", sortname);
        }

        if (!m_declared_sorts.emplace(sortname, arity).second)
        {
            set_error("failed to declare sort %s %d", sortname, arity);
        }
    }

    void define_sort(const char* sortname, smtlib2_vector* params, smtlib2_sort sort) override
    {
        printf("define_sort %s %p %p\n", sortname, params, sort);
        // TODO: implement
    }

    smtlib2_sort make_sort(const char* sortname, smtlib2_vector* index) override
    {
        if (index != nullptr)
        {
            if (strcmp(sortname, "BitVec") == 0)
            {
                if (smtlib2_vector_size(index) != 1)
                {
                    set_error("BitVec sort must have one parameter, got %zu", smtlib2_vector_size(index));
                    return nullptr;
                }
                auto size = smtlib2_vector_at(index, 0);
                auto itr = m_bitvec_sorts.find(size);
                if (itr == m_bitvec_sorts.end())
                {
                    auto bvs = new BitVecSort(size);
                    itr = m_bitvec_sorts.emplace(size, bvs).first;
                }
                printf("BitVec %d -> S:%p\n", (int)size, itr->second);
                return itr->second;
            }
            else
            {
                set_error("unknown sort %s", sortname);
                return nullptr;
            }
        }

        std::string name = sortname;
        auto named_itr = m_named_sorts.find(name);
        if (named_itr == m_named_sorts.end())
        {
            auto declared_itr = m_declared_sorts.find(name);
            if (declared_itr == m_declared_sorts.end())
            {
                set_error("unknown sort %s", sortname);
                return nullptr;
            }

            auto arity = declared_itr->second;
            if (arity > 0)
            {
                set_error("illegal sort creation, expected arity 0, got %d", arity);
            }
            named_itr = m_named_sorts.emplace(name, new NamedSort(name)).first;
        }

        printf("make_sort(%s) %p -> S:%p\n", sortname, index, named_itr->second);

        return named_itr->second;
    }

    smtlib2_sort make_function_sort(smtlib2_vector* tps) override
    {
        auto tps_size = smtlib2_vector_size(tps);
        assert(tps_size > 0);

        printf("make_function_sort (");
        std::vector<Sort*> params;
        for (size_t i = 0; i < smtlib2_vector_size(tps) - 1; i++)
        {
            auto param_sort = (Sort*)smtlib2_vector_at(tps, i);
            printf(" S:%p", param_sort);
            params.push_back(param_sort);
        }

        // TODO: make sure types are unique
        auto result = (Sort*)smtlib2_vector_at(tps, tps_size - 1);
        auto fn_sort = new FunctionSort(std::move(params), result);

        printf(" ) S:%p -> S:%p\n", result, fn_sort);

        return fn_sort;
    }

    smtlib2_sort make_parametric_sort(const char* name, smtlib2_vector* tps) override
    {
        auto declared_itr = m_declared_sorts.find(name);
        if (declared_itr == m_declared_sorts.end())
        {
            printf("Unknown parametric sort '%s'\n", name);
            return 0;
        }
        auto tps_size = smtlib2_vector_size(tps);
        if (tps_size != declared_itr->second)
        {
            printf("Parametric sort '%s' expected arity %d, got %zu\n", name, declared_itr->second, tps_size);
            return 0;
        }
        printf("make_parametric_sort (");
        std::vector<Sort*> params;
        for (size_t i = 0; i < tps_size; i++)
        {
            auto sort = (Sort*)smtlib2_vector_at(tps, i);
            printf(" S:%p", sort);
            params.push_back(sort);
        }
        // TODO: make sure type pointers are unique
        auto param_sort = new ParametricSort(name, params);
        printf(" ) -> %p\n", param_sort);
        return param_sort;
    }

    smtlib2_term make_term(const char* symbol, smtlib2_sort sort, smtlib2_vector* index, smtlib2_vector* args) override
    {
        if (sort != nullptr)
        {
            // TODO: this happens for the "as term sort" construct
            set_error("make_term called with non-null sort");
            return nullptr;
        }

        if(index != nullptr)
        {
            // TODO: this happens for the (_ extract 3 1) construct
            set_error("make_term called with non-null index");
            return nullptr;
        }

        if(args == nullptr)
        {
            if(m_functions.count(symbol) == 0)
            {
                set_error("unknown variable %s", symbol);
                return nullptr;
            }
            auto var_term = new VariableTerm(symbol);
            printf("make_term(%s) [variable] -> T:%p\n", symbol, var_term);
            return var_term;
        }
        else
        {
            printf("make_term(%s", symbol);
            std::vector<Term*> vargs;
            for(size_t i = 0; i < smtlib2_vector_size(args); i++)
            {
                auto arg = (Term*)smtlib2_vector_at(args, i);
                printf(" T:%p", arg);
                vargs.push_back(arg);
            }
            auto fn_term = new FunctionTerm(symbol, std::move(vargs));
            printf(") -> T:%p\n", fn_term);
            return fn_term;
        }

        printf("make_term(%s, %p, %p, %p)\n", symbol, sort, index, args);
        return nullptr;
    }

    smtlib2_term make_number_term(const char* numval, int width, int base) override
    {
        auto num_term = new NumberTerm(numval, width, base);
        printf("make_number_term('%s', %d, %d) -> T:%p\n", numval, width, base, num_term);
        return num_term;
    }
};

int main(int argc, char** argv)
{
    test_parser cp;
    cp.parse(stdin);
}