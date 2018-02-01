#include <functional>
#include "list_tools.h"
#include "std_list_trait.h"
#include "reverse_impl.h"

#include "gtest/gtest.h"

List buildList(const std::vector<std::wstring>& arg)
{
    List list = makeList();
    for (auto elem : arg) {
        push_back(list, makeIdent(elem.c_str()));
    }
    return list;
}

std::vector<std::pair<std::vector<std::wstring>, std::wstring>> reverseInputOutput = {
    {{ L"" }, L"" },
    {{ L"delak" }, L"delak" },
    {{ L"delak", L"bolek" }, L"bolek.delak" },
    {{ L"delak", L"bolek", L"patryk", L"monika", L"milosz" }, L"milosz.monika.patryk.bolek.delak" }
};

template<typename T>
void reverseTestHelper(const T& fun)
{
    for (auto rio : reverseInputOutput) {
        List list = buildList(rio.first);
        EXPECT_EQ(list_length(&list), rio.first.size());
        const auto& expectedResult = rio.second;
        auto result = fun(list);
        EXPECT_EQ(result, expectedResult);
        clean(list);
        EXPECT_EQ(list_length(&list), 0);
    }
}

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
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

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
    erase<Ident>(it);
    EXPECT_EQ(list_length(&list) , 2);
    CheckEQList(list, resultList);
    clean(list);
    clean(resultList);
    EXPECT_EQ(list_length(&list) , 0);
}


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
    erase<Ident>(it);
    EXPECT_EQ(list_length(&list) , 1);
    CheckEQList(list, resultList);
    clean(list);
    clean(resultList);
    EXPECT_EQ(list_length(&list) , 0);
}

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
    erase<Ident>(it);
    EXPECT_EQ(list_length(&list) , 1);
    CheckEQList(list, resultList);
    clean(list);
    clean(resultList);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_erase4)
{
    List list = buildList({ L"delak" });

    auto b = begin(list);
    EXPECT_EQ(list_length(&list) , 1);
    erase<Ident>(b);
    EXPECT_EQ(list_length(&list) , 0);
}


TEST(ListTest, test_iterate)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });

    auto it_begin = begin(list);
    
    while (it_begin != end(list)) {
        ++it_begin;
    }
    EXPECT_EQ(list_length(&list), 5);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}



TEST(ListTest, test_ListHolder)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });
    {
        ListHolder listHolder(list);
    }
    EXPECT_EQ(list_length(&list) , 0);

}

TEST(ListTest, test_reverse)
{
    reverseTestHelper([](const List& list) { return reverse_impl_1(list);});
    reverseTestHelper([](const List& list) { return reverse_impl_2(list);});
    reverseTestHelper([](const List& list) { return reverse_impl_3(list);});    
}

TEST(ListTest, test_reverse_std_list)
{
    std::list<Node*> l;

    l.push_back(makeIdent(L"delak"));
    l.push_back(makeIdent(L"bolek"));

    auto result = reverse_impl_1(l);
    auto expectedResult = L"bolek.delak";
    EXPECT_EQ(result, expectedResult);
    std::for_each(l.begin(), l.end(), [](const Node* n) { delete n;});
}

int main(int argc, char* argv[]) 
{    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    
}
