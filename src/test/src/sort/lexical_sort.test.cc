#include <gtest/gtest.h>

#include <nael_utils/sort/lexical_sort.hh>

struct DataToSort
{
    DataToSort(int a_p, int b_p, std::string c_p) : a(a_p), b(b_p), c(c_p) {}
    int a = 0;
    int b = 0;
    std::string c = "";

    bool operator==(DataToSort const &other_p) const
    {
        return a==other_p.a
            && b==other_p.b
            && c==other_p.c;
    }
};

std::ostream &operator<<(std::ostream &os_p, DataToSort const &data_p)
{
    return os_p<<"[a="<<data_p.a<<", b="<<data_p.b<<", c="<<data_p.c<<"]";
}

struct CompA : LexicalComparator<DataToSort>
{
    bool is_better(DataToSort const &lhs_p, DataToSort const &rhs_p) const
    {
        return lhs_p.a < rhs_p.a;
    }
    bool is_equivalent(DataToSort const &lhs_p, DataToSort const &rhs_p) const
    {
        return lhs_p.a == rhs_p.a;
    }
};

struct CompATolerance5 : LexicalComparator<DataToSort>
{
    bool is_better(DataToSort const &lhs_p, DataToSort const &rhs_p) const
    {
        return lhs_p.a < rhs_p.a;
    }
    bool is_equivalent(DataToSort const &lhs_p, DataToSort const &rhs_p) const
    {
        return std::abs(lhs_p.a - rhs_p.a) < 5;
    }
};

struct CompB : LexicalComparator<DataToSort>
{
    bool is_better(DataToSort const &lhs_p, DataToSort const &rhs_p) const
    {
        return lhs_p.b < rhs_p.b;
    }
    bool is_equivalent(DataToSort const &lhs_p, DataToSort const &rhs_p) const
    {
        return lhs_p.b == rhs_p.b;
    }
};

struct CompC : LexicalComparator<DataToSort>
{
    bool is_better(DataToSort const &lhs_p, DataToSort const &rhs_p) const
    {
        return lhs_p.c.compare(rhs_p.c) < 0;
    }
    bool is_equivalent(DataToSort const &lhs_p, DataToSort const &rhs_p) const
    {
        return lhs_p.c == rhs_p.c;
    }
};

TEST(lexical_sort_test, order_1)
{
    std::vector<DataToSort> data {
        {3, 2, "abc"},
        {2, 2, "abc"},
        {1, 2, "abc"},
        {3, 2, "bca"},
        {2, 2, "bca"},
        {1, 2, "bca"},
    };

    std::vector<std::unique_ptr<LexicalComparator<DataToSort>> > comp;
    comp.push_back(std::make_unique<CompA>());
    comp.push_back(std::make_unique<CompB>());
    comp.push_back(std::make_unique<CompC>());

    lexical_sort(comp.begin(), comp.end(), data.begin(), data.end());

    std::vector<DataToSort> expected_l {
        {1, 2, "abc"},
        {1, 2, "bca"},
        {2, 2, "abc"},
        {2, 2, "bca"},
        {3, 2, "abc"},
        {3, 2, "bca"},
    };

    EXPECT_EQ(expected_l, data);
}

TEST(lexical_sort_test, order_2)
{
    std::vector<DataToSort> data {
        {3, 2, "abc"},
        {2, 2, "abc"},
        {1, 2, "abc"},
        {3, 2, "bca"},
        {2, 2, "bca"},
        {1, 2, "bca"},
    };

    std::vector<std::unique_ptr<LexicalComparator<DataToSort>> > comp;
    comp.push_back(std::make_unique<CompC>());
    comp.push_back(std::make_unique<CompA>());
    comp.push_back(std::make_unique<CompB>());

    lexical_sort(comp.begin(), comp.end(), data.begin(), data.end());

    std::vector<DataToSort> expected_l {
        {1, 2, "abc"},
        {2, 2, "abc"},
        {3, 2, "abc"},
        {1, 2, "bca"},
        {2, 2, "bca"},
        {3, 2, "bca"},
    };

    EXPECT_EQ(expected_l, data);
}

TEST(lexical_sort_test, order_3)
{
    std::vector<DataToSort> data {
        {3, 2, "abc"},
        {2, 2, "abc"},
        {1, 2, "abc"},
        {3, 2, "bca"},
        {2, 2, "bca"},
        {1, 2, "bca"},
    };

    std::vector<std::unique_ptr<LexicalComparator<DataToSort>> > comp;
    comp.push_back(std::make_unique<CompB>());
    comp.push_back(std::make_unique<CompC>());
    comp.push_back(std::make_unique<CompA>());

    lexical_sort(comp.begin(), comp.end(), data.begin(), data.end());

    std::vector<DataToSort> expected_l {
        {1, 2, "abc"},
        {2, 2, "abc"},
        {3, 2, "abc"},
        {1, 2, "bca"},
        {2, 2, "bca"},
        {3, 2, "bca"},
    };

    EXPECT_EQ(expected_l, data);
}

TEST(lexical_sort_test, order_3_ptr)
{
    std::vector<DataToSort> data_l {
        {3, 2, "abc"},
        {2, 2, "abc"},
        {1, 2, "abc"},
        {3, 2, "bca"},
        {2, 2, "bca"},
        {1, 2, "bca"},
    };

    std::vector< LexicalComparator<DataToSort>* > comp_l {
        new CompB(),
        new CompC(),
        new CompA(),
    };

    lexical_sort(comp_l.begin(), comp_l.end(), data_l.begin(), data_l.end());

    std::vector<DataToSort> expected_l {
        {1, 2, "abc"},
        {2, 2, "abc"},
        {3, 2, "abc"},
        {1, 2, "bca"},
        {2, 2, "bca"},
        {3, 2, "bca"},
    };

    EXPECT_EQ(expected_l, data_l);

    for (auto &&ptr_l : comp_l)
    {
        delete ptr_l;
    }
}

/// @brief Si l'on compare des valeurs en utilisant des tolérances il est primordial d'effectuer
/// en amont un tri sans tolérance afin d'éviter des effets d'égalité qui se propageraient
/// et entrainerait un tri incohérent.
TEST(lexical_sort_test, tolerance)
{
    std::vector<DataToSort> data {
        {10, 2, "abc"},
        {12, 2, "abc"},
        {13, 2, "abc"},
        {7, 2, "bca"},
        {6, 2, "bca"},
        {2, 2, "bca"},
    };

    std::vector<std::unique_ptr<LexicalComparator<DataToSort>> > comp;
    comp.push_back(std::make_unique<CompATolerance5>());

    lexical_sort(comp.begin(), comp.end(), data.begin(), data.end());

    std::vector<DataToSort> expected_l {
        {2, 2, "bca"},
        {6, 2, "bca"},
        {7, 2, "bca"},
        {10, 2, "abc"},
        {12, 2, "abc"},
        {13, 2, "abc"},
    };

    EXPECT_EQ(expected_l, data);
}

TEST(lexical_sort_test, tolerance_2criterion)
{
    ///
    /// Dans ce test les deux critères de tri sont (dans l'ordre) :
    /// - a : on considère que deux valeurs sont égales tant que la différence est inférieure à 5
    /// - c : ordre alphabétique
    ///
    std::vector<DataToSort> data {
        {10, 2, "abc"},
        {12, 2, "abc"},
        {13, 2, "abc"},
        {7, 2, "bca"},
        {6, 2, "abc"},
        {2, 2, "bca"},
    };

    std::vector<std::unique_ptr<LexicalComparator<DataToSort>> > comp;
    comp.push_back(std::make_unique<CompATolerance5>());
    comp.push_back(std::make_unique<CompC>());

    lexical_sort(comp.begin(), comp.end(), data.begin(), data.end());

    /// Les valeurs de a sont triés ainsi
    /// 2, 6, 7, 10, 12, 13
    /// les groupes d'équivalences sont définis ainsi
    /// [2, 6], [7, 10], [12, 13]
    /// un groupe d'équivalence est défini tel que toutes les valeurs sont égales entre elles selon l'opérateur de comparaison
    /// Au sein de chaque groupe d'équivalence on tri ensuite dans l'ordre alphabétique.
    /// Ce qui donne la solution suivante :
    std::vector<DataToSort> expected_l {
        {6, 2, "abc"},
        {2, 2, "bca"},
        {10, 2, "abc"},
        {7, 2, "bca"},
        {12, 2, "abc"},
        {13, 2, "abc"},
    };

    EXPECT_EQ(expected_l, data);
}
