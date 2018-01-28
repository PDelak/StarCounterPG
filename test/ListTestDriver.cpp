#include <iostream>
#include <cassert>
#include "list_tools.h"
#include "reverse_impl.h"

#include "gtest/gtest.h"

void test_find()
{
    List list = makeList();

    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));
    push_back(list, makeIdent(L"patryk"));

    auto b = begin(list);
    auto e = end(list);
   
    auto it = std::find_if(b, e, [](const ListCell* cell) {
        const wchar_t *buff = castNode<Ident>(cell)->name;
        std::wstring str(buff);
        return str == L"bolek";
    });
    std::wcout << castNode<Ident>(*it)->name << std::endl;
    assert(list_length(&list) == 3);
    clean(list);
    assert(list_length(&list) == 0);
}

void test_erase1()
{
    List list = makeList();
    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));
    push_back(list, makeIdent(L"patryk"));
    
    auto b = begin(list);
    auto e = end(list);
    
    auto it = std::find_if(b, e, [](const ListCell* cell) {
        const wchar_t *buff = castNode<Ident>(cell)->name;
        std::wstring str(buff);
        return str == L"bolek";
    });
    erase<Ident>(it);
    assert(list_length(&list) == 2);
    clean(list);
    assert(list_length(&list) == 0);
}


void test_erase2()
{
    List list = makeList();
    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));


    auto b = begin(list);
    auto e = end(list);

    auto it = std::find_if(b, e, [](const ListCell* cell) {
        const wchar_t *buff = castNode<Ident>(cell)->name;
        std::wstring str(buff);
        return str == L"bolek";
    });
    erase<Ident>(it);
    assert(list_length(&list) == 1);
    std::wcout << castNode<Ident>(*b)->name << std::endl;
    clean(list);
    assert(list_length(&list) == 0);
}

void test_erase3()
{
    List list = makeList();
    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));


    auto b = begin(list);
    auto e = end(list);

    auto it = std::find_if(b, e, [](const ListCell* cell) {
        return std::wstring(castNode<Ident>(cell)->name) == L"delak";
    });
    BasicListIterator itSecond = it;
    ++itSecond;
    erase<Ident>(it);
    assert(list_length(&list) == 1);
    std::wcout << castNode<Ident>(*itSecond)->name << std::endl;
    clean(list);
    assert(list_length(&list) == 0);
}

void test_erase4()
{
    List list = makeList();
    push_back(list, makeIdent(L"delak"));


    auto b = begin(list);
    assert(list_length(&list) == 1);
    erase<Ident>(b);
    assert(list_length(&list) == 0);
}


void test_iterate()
{
    List list = makeList();

    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));
    push_back(list, makeIdent(L"patryk"));
    push_back(list, makeIdent(L"monika"));
    push_back(list, makeIdent(L"milosz"));

    auto it_begin = begin(list);
    auto it = begin(list);

    while (it_begin != end(list)) {
        printIdent(castNode<Ident>(*it_begin));
        ++it_begin;
    }
    std::cout << list_length(&list) << std::endl;
    clean(list);
    assert(list_length(&list) == 0);
}

void test_reverse1()
{
    List list = makeList();    
    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));
    push_back(list, makeIdent(L"patryk"));
    push_back(list, makeIdent(L"monika"));
    push_back(list, makeIdent(L"milosz"));

    auto result = reverse_impl_1(list);
    std::wcout << result.c_str() << std::endl;
    clean(list);
    assert(list_length(&list) == 0);
}

void test_reverse2()
{
    List list = makeList();
    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));
    push_back(list, makeIdent(L"patryk"));
    push_back(list, makeIdent(L"monika"));
    push_back(list, makeIdent(L"milosz"));

    auto result = reverse_impl_2(list);
    std::wcout << result.c_str() << std::endl;
    clean(list);
    assert(list_length(&list) == 0);
}

void test_reverse3()
{
    List list = makeList();
    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));
    push_back(list, makeIdent(L"patryk"));
    push_back(list, makeIdent(L"monika"));
    push_back(list, makeIdent(L"milosz"));
    auto result = reverse_impl_3(list);
    std::wcout << result.c_str() << std::endl;
    assert(list_length(&list) == 5);
    clean(list);
    assert(list_length(&list) == 0);
}

void test_reverse4()
{
    List list = makeList();
    push_back(list, makeIdent(L"delak"));
    auto result = reverse_impl_3(list);
    std::wcout << result.c_str() << std::endl;
    assert(list_length(&list) == 1);
    clean(list);
    assert(list_length(&list) == 0);
}

void test_reverse5()
{
    List list = makeList();
    auto result = reverse_impl_3(list);
    std::wcout << result.c_str() << std::endl;
    assert(list_length(&list) == 0);
    clean(list);
    assert(list_length(&list) == 0);
}

void test_reverse6()
{
    List list = makeList();
    push_back(list, makeIdent(L"delak"));
    push_back(list, makeIdent(L"bolek"));
    auto result = reverse_impl_3(list);
    std::wcout << result.c_str() << std::endl;
    assert(list_length(&list) == 2);
    clean(list);
    assert(list_length(&list) == 0);
}


void test_ListHolder()
{
    List list = makeList();
    {
        ListHolder listHolder(list);
        push_back(list, makeIdent(L"delak"));
        push_back(list, makeIdent(L"bolek"));
        push_back(list, makeIdent(L"patryk"));
        push_back(list, makeIdent(L"monika"));
        push_back(list, makeIdent(L"milosz"));
    }
    assert(list_length(&list) == 0);

}

int main() 
{
    test_find();
    test_erase1();
    test_erase2();
    test_erase3();
    test_erase4();
    test_reverse1();
    test_reverse2();
    test_reverse3();
    test_reverse4();
    test_reverse5();
    test_reverse6();
    test_ListHolder();
    return 0;
}
