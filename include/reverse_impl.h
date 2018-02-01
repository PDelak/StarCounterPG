#ifndef REVERSE_IMPL_H
#define REVERSE_IMPL_H

#include <algorithm>
#include <stack>
#include <list>

void appendElement(const std::wstring& str, bool& firstElement, std::wstring& result)
{
    if (!firstElement) result.append(L".");
    result.append(str);
    firstElement = false;
}

// implementation with explicit stack
// O(n) time complexity, O(n) space complexity
template<typename T>
std::wstring reverse_impl_1(const T& stream)
{
    auto b = begin(stream);
    auto e = end(stream);

    std::stack<typename ListNodeTrait<T>::node*> elems;

    std::for_each(b, e, [&](typename ListNodeTrait<T>::node* element) {
        elems.push(element);
    });
    
    std::wstring result;
    bool first = true;
    while (!elems.empty()) {
        appendElement(castNode<Ident>(elems.top())->name, first, result);
        elems.pop();
    }

    return result;
}

// implementation with std::list 
// O(n) time complexity, O(n) space complexity
template<typename T>
std::wstring reverse_impl_2(const T& stream)
{
    auto b = begin(stream);
    auto e = end(stream);

    std::list<typename ListNodeTrait<T>::node*> elems;

    std::for_each(b, e, [&](typename ListNodeTrait<T>::node* element) {
        elems.push_front(element);
    });

    std::wstring result;

    bool first = true;
    std::for_each(elems.begin(), elems.end(), [&](typename ListNodeTrait<T>::node* element) {
        appendElement(castNode<Ident>(element)->name, first, result);
    });

    return result;
}

void recursive_reverse_impl_helper(const List& list, bool& first, BasicListIterator i, std::wstring& result)
{
    auto e = end(list);
    if (i == e) return;
    BasicListIterator j = i;
    ++j;        
    recursive_reverse_impl_helper(list, first, j, result);
    appendElement(castNode<Ident>(*i)->name, first, result);
}

// recursive implementation with implicit stack
// O(n) time complexity
template<typename T>
std::wstring reverse_impl_3(const T& stream)
{
    auto b = begin(stream);

    std::wstring result;
    bool first = true;
    recursive_reverse_impl_helper(stream, first, b, result);

    return result;
}


#endif
