#ifndef REVERSE_IMPL_H
#define REVERSE_IMPL_H

#include <algorithm>
#include <stack>
#include <list>

// Reverse algorithms will work with any
// data structure that provides forward iterator 
// and specialization for ListNodeTrait<>
// As an example for that look at std_list_trait.h 
// or trait for List type defined in list_tools.h


// All reverse implementations take one parameter
// a container that support forward iterator

// implementation with explicit stack
// O(n) time complexity, O(n) space complexity
template<typename T>
std::wstring reverse_impl_1(const T& stream)
{
    auto b = begin(stream);
    auto e = end(stream);

    std::stack<typename ListNodeTrait<T>::node> elems;

    std::for_each(b, e, [&](typename ListNodeTrait<T>::node element) {
        elems.push(element);
    });
    
    std::wstring result;
    bool first = true;
    while (!elems.empty()) {
        ListNodeTrait<T>::appendElement(elems.top(), first, result);
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

    std::list<typename ListNodeTrait<T>::node> elems;

    std::for_each(b, e, [&](typename ListNodeTrait<T>::node element) {
        elems.push_front(element);
    });

    std::wstring result;

    bool first = true;
    std::for_each(elems.begin(), elems.end(), [&](typename ListNodeTrait<T>::node element) {
        ListNodeTrait<T>::appendElement(element, first, result);
    });

    return result;
}

template<typename T>
void recursive_reverse_impl_helper(const T& list, bool& first, typename ListNodeTrait<T>::iterator i, std::wstring& result)
{
    auto e = ListNodeTrait<T>::end(list);
    if (i == e) return;
    typename ListNodeTrait<T>::iterator j = i;
    ++j;        
    recursive_reverse_impl_helper(list, first, j, result);
    ListNodeTrait<T>::appendElement(*i, first, result);
}

// recursive implementation with implicit stack
// O(n) time complexity
template<typename T>
std::wstring reverse_impl_3(const T& stream)
{
    auto b = ListNodeTrait<T>::begin(stream);

    std::wstring result;
    bool first = true;
    recursive_reverse_impl_helper(stream, first, b, result);

    return result;
}

// O(n) time complexity O(1) space
// based on T::reverse()
template<typename T>
std::wstring reverse_impl_4(T& stream)
{
    ListNodeTrait<T>::reverse(stream);

    std::wstring result;

    bool first = true;

    auto b = begin(stream);
    auto e = end(stream);

    std::for_each(b, e, [&](typename ListNodeTrait<T>::node element) {
        ListNodeTrait<T>::appendElement(element, first, result);
    });

    return result;
}

#endif
