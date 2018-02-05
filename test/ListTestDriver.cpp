//
// This file contains tests regarding
// reverse implementations as well as generic List handling
// To work with List structure in more convenient way
// new API was provided that allows to use it in similar way as
// other STL containers. For instance, List can be used with STL algorithms
// There are possible further improvements, for instance, building a container similar to STL containers on top 
// of provided API
//

#include <functional>
#include "list_tools.h"
#include "std_list_trait.h"
#include "reverse_impl.h"

#include "gtest/gtest.h"

// Converts vector of wstrings to List of Idents*
List buildList(const std::vector<std::wstring>& arg)
{
    List list = makeList();
    for (auto elem : arg) {
        push_back(list, makeIdent(elem.c_str()));
    }
    return list;
}

// Converts vector of wstrings to std::list of Node*
std::list<Node*> buildStdList(const std::vector<std::wstring>& arg)
{
    std::list<Node*> stdlist;
    for (auto elem : arg) {
        stdlist.push_back(makeIdent(elem.c_str()));
    }
    return stdlist;
}

// Contains input and result set for reverse implementations
std::vector<std::pair<std::vector<std::wstring>, std::wstring>> reverseInputOutput = {
    {{ L"" }, L"" },
    {{ L"delak" }, L"delak" },
    {{ L"delak", L"bolek" }, L"bolek.delak" },
    {{ L"delak", L"bolek", L"patryk", L"monika", L"milosz" }, L"milosz.monika.patryk.bolek.delak" }
};

void cleanNodes(const List& list)
{
    std::for_each(begin(list), end(list), [&](const ListCell* cell) {
        delete castNode<Node>(cell);
    });
}

// Helper method that takes function as parameter
// and goes through  whole input set checking result
// works on List container
template<typename T>
void reverseTestHelper(const T& fun)
{
    for (auto rio : reverseInputOutput) {
        List list = buildList(rio.first);
        EXPECT_EQ(list_length(&list), rio.first.size());
        const auto& expectedResult = rio.second;
        auto result = fun(list);
        EXPECT_EQ(result, expectedResult);
        cleanNodes(list);
        clean(list);
        EXPECT_EQ(list_length(&list), 0);
    }
}

// The same as above but works for std::list
template<typename T>
void reverseStdListTestHelper(const T& fun)
{
    for (auto rio : reverseInputOutput) {
        std::list<Node*> stdlist = buildStdList(rio.first);
        EXPECT_EQ(stdlist.size(), rio.first.size());
        const auto& expectedResult = rio.second;
        auto result = fun(stdlist);
        EXPECT_EQ(result, expectedResult);
        std::for_each(stdlist.begin(), stdlist.end(), [](const Node* n) { delete n;});
    }
}

// Helper function that takes two List(s) and 
// checks them against equality
void CheckEQList(const List& list1, const List& list2)
{
    EXPECT_EQ(list_length(&list1), list_length(&list2));
    
    auto list1_begin = begin(list1);
    auto list1_end = end(list1);
    auto list2_begin = begin(list2);

    std::for_each(list1_begin, list1_end, [&](const ListCell* cell) {
        auto l1Value = std::wstring(castNode<Ident>(cell)->name);
        auto l2Value = std::wstring(castNode<Ident>(*list2_begin)->name);
        ++list2_begin;
        EXPECT_EQ(l1Value, l2Value);
    });    
}

// Reverse test against all reverse implementations
// works on List type
TEST(ListTest, test_reverse)
{
    reverseTestHelper([](const List& list) { return reverse_impl_1(list);});
    reverseTestHelper([](const List& list) { return reverse_impl_2(list);});
    reverseTestHelper([](const List& list) { return reverse_impl_3(list);});
    reverseTestHelper([](List& list) { return reverse_impl_4(list);});

}

// Reverse test against all reverse implementations
// works on std::list type
TEST(ListTest, test_reverse_std_list)
{
    reverseStdListTestHelper([](const std::list<Node*>& list) { return reverse_impl_1(list);});
    reverseStdListTestHelper([](const std::list<Node*>& list) { return reverse_impl_2(list);});
    reverseStdListTestHelper([](const std::list<Node*>& list) { return reverse_impl_3(list);});
    reverseStdListTestHelper([](std::list<Node*>& stdlist) { return reverse_impl_4(stdlist);});
}

// Test std::find_if with List container
TEST(ListTest, test_find)
{
    List list = buildList({L"delak", L"bolek", L"patryk"});

    auto b = begin(list);
    auto e = end(list);
   
    auto it = std::find_if(b, e, [](const ListCell* cell) {
        const wchar_t *buff = castNode<Ident>(cell)->name;
        std::wstring str(buff);
        return str == L"bolek";
    });
    EXPECT_EQ(castNode<Ident>(*it)->name, std::wstring(L"bolek"));
    EXPECT_EQ(list_length(&list) , 3);
    cleanNodes(list);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

// Tests removing middle element from List container
TEST(ListTest, test_erase1)
{
    List list = buildList({ L"delak", L"bolek", L"patryk" });
    List resultList = buildList({ L"delak", L"patryk" });

    auto b = begin(list);
    auto e = end(list);
    
    auto it = std::find_if(b, e, [](const ListCell* cell) {
        const wchar_t *buff = castNode<Ident>(cell)->name;
        std::wstring str(buff);
        return str == L"bolek";
    });
    delete castNode<Node>(*it);
    erase(it);
    EXPECT_EQ(list_length(&list) , 2);
    CheckEQList(list, resultList);
    cleanNodes(list);
    clean(list);
    cleanNodes(resultList);
    clean(resultList);
    EXPECT_EQ(list_length(&list) , 0);
}

// Tests removing last element from 2-elements List container
TEST(ListTest, test_erase2)
{
    List list = buildList({ L"delak", L"bolek" });
    List resultList = buildList({ L"delak" });
    auto b = begin(list);
    auto e = end(list);

    auto it = std::find_if(b, e, [](const ListCell* cell) {
        const wchar_t *buff = castNode<Ident>(cell)->name;
        std::wstring str(buff);
        return str == L"bolek";
    });
    delete castNode<Node>(*it);
    erase(it);
    EXPECT_EQ(list_length(&list) , 1);
    CheckEQList(list, resultList);
    cleanNodes(list);
    clean(list);
    cleanNodes(resultList);
    clean(resultList);
    EXPECT_EQ(list_length(&list) , 0);
}

// Tests removing first element from 2-elements List container
TEST(ListTest, test_erase3)
{
    List list = buildList({ L"delak", L"bolek" });
    List resultList = buildList({ L"bolek" });
    auto b = begin(list);
    auto e = end(list);

    auto it = std::find_if(b, e, [](const ListCell* cell) {
        return std::wstring(castNode<Ident>(cell)->name) == L"delak";
    });
    BasicListIterator itSecond = it;
    ++itSecond;
    delete castNode<Node>(*it);
    erase(it);
    EXPECT_EQ(list_length(&list) , 1);
    CheckEQList(list, resultList);
    cleanNodes(list);
    cleanNodes(resultList);
    clean(list);
    clean(resultList);
    EXPECT_EQ(list_length(&list) , 0);
}

// Tests removing element from 1-element List
TEST(ListTest, test_erase4)
{
    List list = buildList({ L"delak" });

    auto b = begin(list);
    EXPECT_EQ(list_length(&list) , 1);
    delete castNode<Node>(*b);
    erase(b);
    EXPECT_EQ(list_length(&list) , 0);
}

// Tests ListHolder against releasing resources
TEST(ListTest, test_ListHolder)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });
    {
        ListHolder listHolder(list);
        cleanNodes(list);
    }

    EXPECT_EQ(list_length(&list) , 0);

}

TEST(ListTest, test_copy_node)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });
    auto cloneF = [](const ListCell* cell) {
        return makeIdent(castNode<Ident>(cell)->name);
    };
    auto listCopy = copy(list, cloneF);
    CheckEQList(list, listCopy);
    cleanNodes(list);
    cleanNodes(listCopy);
    clean(list);
    clean(listCopy);
}

TEST(ListTest, test_copy_int)
{
    List list = makeList();
    push_back(list, 1);
    push_back(list, 2);
    push_back(list, 3);
    auto cloneF = [](const ListCell* cell) {
        return cell->data.int_value;
    };
    auto listCopy = copy(list, cloneF);
    auto it = begin(listCopy);
    EXPECT_EQ(list_length(&list), list_length(&listCopy));

    std::for_each(begin(list), end(list), [&](const ListCell* cell) {
        EXPECT_EQ(cell->data.int_value, (*it)->data.int_value);
        ++it;
    });
    clean(list);
    clean(listCopy);
}

TEST(ListTest, test_autolist_int)
{
    AutoList<int> alist;
    alist.push_back(1);
    alist.push_back(2);
    alist.push_back(3);
    EXPECT_EQ(alist.size(), 3);
    AutoList<int> alistCopy = alist;
    EXPECT_EQ(alistCopy.size(), 3);
}

TEST(ListTest, test_autolist_node)
{
    auto cloneF = [](const ListCell* cell) {
        return makeIdent(castNode<Ident>(cell)->name);
    };

    AutoList<Node> alist(cloneF);
    alist.push_back(makeIdent(L"delak"));
    alist.push_back(makeIdent(L"bolek"));
    alist.push_back(makeIdent(L"monika"));
    EXPECT_EQ(alist.size(), 3);
    AutoList<Node> alistCopy = alist;
    EXPECT_EQ(alistCopy.size(), 3);
}


int main(int argc, char* argv[]) 
{    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    
}
