#include <gtest/gtest.h>

#include <nael_utils/sort/lexical_sort.hh>

struct DataToSort
{
    DataToSort(int val_a, int val_b, std::string val_c) : a(val_a), b(val_b), c(val_c) {}
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

std::ostream &operator<<(std::ostream &os, DataToSort const &data)
{
    return os<<"[a="<<data.a<<", b="<<data.b<<", c="<<data.c<<"]";
}

struct CompA : LexicalComparator<DataToSort>
{
    bool is_better(DataToSort const &lhs, DataToSort const &rhs) const
    {
        return lhs.a < rhs.a;
    }
    bool is_equivalent(DataToSort const &lhs, DataToSort const &rhs) const
    {
        return lhs.a == rhs.a;
    }
};

struct CompATolerance5 : LexicalComparator<DataToSort>
{
    bool is_better(DataToSort const &lhs, DataToSort const &rhs) const
    {
        return lhs.a < rhs.a;
    }
    bool is_equivalent(DataToSort const &lhs, DataToSort const &rhs) const
    {
        return std::abs(lhs.a - rhs.a) < 5;
    }
};

struct CompB : LexicalComparator<DataToSort>
{
    bool is_better(DataToSort const &lhs, DataToSort const &rhs) const
    {
        return lhs.b < rhs.b;
    }
    bool is_equivalent(DataToSort const &lhs, DataToSort const &rhs) const
    {
        return lhs.b == rhs.b;
    }
};

struct CompC : LexicalComparator<DataToSort>
{
    bool is_better(DataToSort const &lhs, DataToSort const &rhs) const
    {
        return lhs.c.compare(rhs.c) < 0;
    }
    bool is_equivalent(DataToSort const &lhs, DataToSort const &rhs) const
    {
        return lhs.c == rhs.c;
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

    std::vector<DataToSort> expected {
        {1, 2, "abc"},
        {1, 2, "bca"},
        {2, 2, "abc"},
        {2, 2, "bca"},
        {3, 2, "abc"},
        {3, 2, "bca"},
    };

    EXPECT_EQ(expected, data);
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

    std::vector<DataToSort> expected {
        {1, 2, "abc"},
        {2, 2, "abc"},
        {3, 2, "abc"},
        {1, 2, "bca"},
        {2, 2, "bca"},
        {3, 2, "bca"},
    };

    EXPECT_EQ(expected, data);
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

    std::vector<DataToSort> expected {
        {1, 2, "abc"},
        {2, 2, "abc"},
        {3, 2, "abc"},
        {1, 2, "bca"},
        {2, 2, "bca"},
        {3, 2, "bca"},
    };

    EXPECT_EQ(expected, data);
}

TEST(lexical_sort_test, order_3_ptr)
{
    std::vector<DataToSort> data {
        {3, 2, "abc"},
        {2, 2, "abc"},
        {1, 2, "abc"},
        {3, 2, "bca"},
        {2, 2, "bca"},
        {1, 2, "bca"},
    };

    std::vector< LexicalComparator<DataToSort>* > comp {
        new CompB(),
        new CompC(),
        new CompA(),
    };

    lexical_sort(comp.begin(), comp.end(), data.begin(), data.end());

    std::vector<DataToSort> expected {
        {1, 2, "abc"},
        {2, 2, "abc"},
        {3, 2, "abc"},
        {1, 2, "bca"},
        {2, 2, "bca"},
        {3, 2, "bca"},
    };

    EXPECT_EQ(expected, data);

    for (auto &&ptr : comp)
    {
        delete ptr;
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

    std::vector<DataToSort> expected {
        {2, 2, "bca"},
        {6, 2, "bca"},
        {7, 2, "bca"},
        {10, 2, "abc"},
        {12, 2, "abc"},
        {13, 2, "abc"},
    };

    EXPECT_EQ(expected, data);
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
    std::vector<DataToSort> expected {
        {6, 2, "abc"},
        {2, 2, "bca"},
        {10, 2, "abc"},
        {7, 2, "bca"},
        {12, 2, "abc"},
        {13, 2, "abc"},
    };

    EXPECT_EQ(expected, data);
}
