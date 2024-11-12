

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

struct Function
{
    std::string name;
    Sort* sort = nullptr;

    Function(std::string name, Sort* sort)
        : name(std::move(name)), sort(sort)
    {
    }
};

struct test_parser : smtlib2_cpp_parser
{
    // TODO: implement scope push/pop
    std::unordered_map<std::string, Function*> m_functions;
    std::unordered_map<std::string, int> m_declared_sorts;
    std::unordered_map<std::string, NamedSort*> m_named_sorts;

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
            puts("Failed to insert function!");
        }
    }

    void declare_sort(const char* sortname, int arity) override
    {
        // TODO: set parser error here correctly
        printf("declare_sort %s %d\n", sortname, arity);
        if (m_named_sorts.count(sortname) > 0)
        {
            printf("Sort already declared: %s\n", sortname);
        }

        if (!m_declared_sorts.emplace(sortname, arity).second)
        {
            printf("Failed to insert sort %s %d", sortname, arity);
        }
    }

    smtlib2_sort make_sort(const char* sortname, smtlib2_vector* index) override
    {
        if (index != nullptr)
        {
            // BitVec support
            printf("TODO: implement %s", sortname);
            return smtlib2_cpp_parser::make_sort(sortname, index);
        }

        std::string name = sortname;
        auto named_itr = m_named_sorts.find(name);
        if (named_itr == m_named_sorts.end())
        {
            auto declared_itr = m_declared_sorts.find(name);
            if (declared_itr == m_declared_sorts.end())
            {
                printf("Unknown sort '%s'\n", sortname);
                return nullptr;
            }

            auto arity = declared_itr->second;
            if (arity > 0)
            {
                printf("illegal sort creation, expected arity 0, got %d\n", arity);
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
};

int main(int argc, char** argv)
{
    test_parser cp;
    cp.parse(stdin);
}