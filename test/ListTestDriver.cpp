#include <iostream>
#include "list_tools.h"
#include "reverse_impl.h"

#include "gtest/gtest.h"

TEST(ListTest, dummy) {}

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
    std::wcout << castNode<Ident>(*it)->name << std::endl;
    EXPECT_EQ(list_length(&list) , 3);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_erase1)
{
    List list = buildList({ L"delak", L"bolek", L"patryk" });
    
    auto b = begin(list);
    auto e = end(list);
    
    auto it = std::find_if(b, e, [](const ListCell* cell) {
        const wchar_t *buff = castNode<Ident>(cell)->name;
        std::wstring str(buff);
        return str == L"bolek";
    });
    erase<Ident>(it);
    EXPECT_EQ(list_length(&list) , 2);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}


TEST(ListTest, test_erase2)
{
    List list = buildList({ L"delak", L"bolek" });

    auto b = begin(list);
    auto e = end(list);

    auto it = std::find_if(b, e, [](const ListCell* cell) {
        const wchar_t *buff = castNode<Ident>(cell)->name;
        std::wstring str(buff);
        return str == L"bolek";
    });
    erase<Ident>(it);
    EXPECT_EQ(list_length(&list) , 1);
    std::wcout << castNode<Ident>(*b)->name << std::endl;
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_erase3)
{
    List list = buildList({ L"delak", L"bolek" });

    auto b = begin(list);
    auto e = end(list);

    auto it = std::find_if(b, e, [](const ListCell* cell) {
        return std::wstring(castNode<Ident>(cell)->name) == L"delak";
    });
    BasicListIterator itSecond = it;
    ++itSecond;
    erase<Ident>(it);
    EXPECT_EQ(list_length(&list) , 1);
    std::wcout << castNode<Ident>(*itSecond)->name << std::endl;
    clean(list);
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
        printIdent(castNode<Ident>(*it_begin));
        ++it_begin;
    }
    std::cout << list_length(&list) << std::endl;
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse1)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });
    
    auto result = reverse_impl_1(list);

    std::wcout << result.c_str() << std::endl;
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse2)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });

    auto result = reverse_impl_2(list);
    std::wcout << result.c_str() << std::endl;
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse3)
{
    List list = buildList({ L"delak", L"bolek", L"patryk", L"monika", L"milosz" });

    auto result = reverse_impl_3(list);
    std::wcout << result.c_str() << std::endl;
    EXPECT_EQ(list_length(&list) , 5);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse4)
{
    List list = buildList({ L"delak" });

    auto result = reverse_impl_3(list);
    std::wcout << result.c_str() << std::endl;
    EXPECT_EQ(list_length(&list) , 1);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse5)
{
    List list = makeList();
    auto result = reverse_impl_3(list);
    std::wcout << result.c_str() << std::endl;
    EXPECT_EQ(list_length(&list) , 0);
    clean(list);
    EXPECT_EQ(list_length(&list) , 0);
}

TEST(ListTest, test_reverse6)
{
    List list = buildList({ L"delak", L"bolek" });

    auto result = reverse_impl_3(list);
    std::wcout << result.c_str() << std::endl;
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
