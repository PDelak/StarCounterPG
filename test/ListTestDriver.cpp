#include <iostream>
#include "list_tools.h"
#include "reverse_impl.h"

#include "gtest/gtest.h"

TEST(ListTest, dummy) {}

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

List buildList(const std::initializer_list<std::wstring>& arg)
{
    List list = makeList();
    for (auto elem : arg) {
        push_back(list, makeIdent(elem.c_str()));
    }
    return list;
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

TEST(ListTest, test_reverse1)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });
    auto expectedResult = std::wstring(L"milosz.monika.patryk.bolek.delak");
    auto result = reverse_impl_1(list);
    EXPECT_EQ(result, expectedResult);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse2)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });
    auto expectedResult = std::wstring(L"milosz.monika.patryk.bolek.delak");

    auto result = reverse_impl_2(list);
    EXPECT_EQ(result, expectedResult);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse3)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });
    auto expectedResult = std::wstring(L"milosz.monika.patryk.bolek.delak");

    auto result = reverse_impl_3(list);
    EXPECT_EQ(result, expectedResult);
    EXPECT_EQ(list_length(&list) , 5);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse4)
{
    List list = buildList({ L"delak" });
    auto expectedResult = std::wstring(L"delak");

    auto result = reverse_impl_3(list);
    EXPECT_EQ(result, expectedResult);
    EXPECT_EQ(list_length(&list) , 1);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse5)
{
    List list = makeList();
    auto result = reverse_impl_3(list);
    auto expectedResult = std::wstring(L"");
    EXPECT_EQ(result, expectedResult);
    EXPECT_EQ(list_length(&list) , 0);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse6)
{
    List list = buildList({ L"delak", L"bolek" });
    auto expectedResult = std::wstring(L"bolek.delak");
    auto result = reverse_impl_3(list);
    EXPECT_EQ(result, expectedResult);
    EXPECT_EQ(list_length(&list) , 2);
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

int main(int argc, char* argv[]) 
{    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    
}
