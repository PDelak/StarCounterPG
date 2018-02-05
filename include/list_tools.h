#ifndef LIST_TOOLS_H
#define LIST_TOOLS_H

#include <iostream>
#include <memory>
#include <algorithm>
#include <wchar.h>
#include <cstring>
#include "list_node_trait.h"
#include <list>

#include "pg/pg_list.h"

// Casts ListCell to specific Node* type
// example cast to Ident : castNode<Ident>(cell)
template<typename DEST>
DEST* castNode(const ListCell* cell)
{
    return reinterpret_cast<DEST*>(cell->data.ptr_value);
}

// IdentExt structure is an extension 
// to Ident type to provide destructor
// that will release name 
struct IdentExt : public Ident
{
    ~IdentExt() { delete[] name; }
};

// List can contain void* or int values
// To choose to which union field it should be assigned to
// below type has been provided 
// Specific specializations do the work
// It will not compile if there will be choosen different type
template<typename T>
struct AssignemntTypeChooser
{
    static T* assign(T* type, int value)
    {
        static_assert(std::is_integral<T>::value || std::is_same<T, Node>::value, "Unsupported type");
    }
};

// Provides choosing data.int_value and assignment to int value
template<>
struct AssignemntTypeChooser<int>
{
    static ListCell* assign(ListCell* listCell, int value)
    {
        listCell->data.int_value = value;
        return listCell;
    }
};

// Provides choosing data.ptr_value and assignment to Node* 
template<>
struct AssignemntTypeChooser<Node*>
{
    static ListCell* assign(ListCell* listCell, Node* value)
    {
        listCell->data.ptr_value = value;
        return listCell;
    }
};

// Constructor for List container
List makeList()
{
    List list;
    list.head = nullptr;
    list.length = 0;
    list.tail = nullptr;
    list.type = T_List;
    return list;
}

// Returns list length
int getListSize(const List& list)
{
    return const_cast<List&>(list).length;
}

// Ident(Ext) constructor 
Node* makeIdent(const std::basic_string<wchar_t>& name)
{
    auto node = std::make_unique<IdentExt>();
    node->type = T_Ident;
    std::unique_ptr<wchar_t[]> wchar_buf (new wchar_t[name.size() + 1]);
    memset(wchar_buf.get(), 0, sizeof(wchar_t) * (name.size()+1));
    node->name = wchar_buf.release();
    #ifdef __GNUC__
    wcsncpy(const_cast<wchar_t*>(node->name), name.c_str(), name.size());    
    #else
    wcsncpy_s(const_cast<wchar_t*>(node->name), name.size() + 1, name.c_str(), name.size());
    #endif
    return node.release();

}

// Insert element to List at the end
template<typename ValueType>
void push_back(List& list, ValueType value)
{    
    // for now type choosing is based on SFINAE
    auto cellPtr = AssignemntTypeChooser<ValueType>::assign(std::make_unique<ListCell>().release(), value);

    auto tail = list.tail;

    if (tail) tail->next = cellPtr;    
    else list.head = cellPtr;
    
    list.tail = cellPtr;
    list.length = list.length + 1;
}

// Insert element to List at the beginning
template<typename ValueType>
void push_front(List& list, ValueType value)
{
    // for now type choosing is based on SFINAE
    auto cellPtr = AssignemntTypeChooser<ValueType>::assign(std::make_unique<ListCell>().release(), value);

    auto head = list.head;
    if (head) {
        cellPtr->next = head;
        list.tail = head;
    }
    list.head = cellPtr;
    list.length = list.length + 1;    
}

// ForwardIterator for List type
struct BasicListIterator
    :public std::iterator<std::forward_iterator_tag, ListCell*>
{
    BasicListIterator(const List& l) :list(l), nodePtr(l.head){}
    ListCell* operator*() const { return nodePtr; }
    bool operator != (const BasicListIterator& rhs) const { return nodePtr != rhs.nodePtr; }
    bool operator == (const BasicListIterator& rhs) const { return nodePtr == rhs.nodePtr; }
    
    BasicListIterator& operator=(BasicListIterator rhs)
    {
        std::swap(nodePtr, rhs.nodePtr);
        return *this;
    }
    
    BasicListIterator& operator++()
    {
        nodePtr = nodePtr->next;
        return *this;
    }
    
    BasicListIterator operator++(int)
    {
        BasicListIterator tmp = *this;
        nodePtr = nodePtr->next;
        return tmp;
    }
    
private:
    BasicListIterator(const List& l, ListCell* cell) :list(l), nodePtr(cell) {}
    const List& list;
    ListCell* nodePtr;
    ListCell* getPrevNodePtr(const ListCell* node) {
        auto current = list.head;
        ListCell* prev = nullptr;
        while (current) {
            if (current == node) break;
            prev = current;
            current = current->next;
        }
        return prev;
    }
    friend BasicListIterator begin(const List& list);
    friend BasicListIterator end(const List& list);
    friend BasicListIterator erase(BasicListIterator& iter);
};

// Returns List head
BasicListIterator begin(const List& list) { return BasicListIterator(list, list.head);}
// Returns List end
BasicListIterator end(const List& list)   { return BasicListIterator(list, nullptr);}

// Removes element pointer by iterator
BasicListIterator erase(BasicListIterator& iter)
{
    auto toDelete = iter.nodePtr;
    auto prev = iter.getPrevNodePtr(iter.nodePtr);
    List& tmpList = const_cast<List&>(iter.list);
    if (toDelete == iter.list.head) {
        tmpList.head = iter.nodePtr->next;
    }
    if (toDelete == iter.list.tail) {
        tmpList.tail = prev;
    }
    if (prev) prev->next = iter.nodePtr->next;      
    --const_cast<List&>(iter.list).length;
    BasicListIterator i(iter.list, iter.nodePtr->next);
    delete toDelete;
    return i;
}

// Remove all List elements
void clean(const List& list)
{
    auto current = begin(list);
    auto e = end(list);
    while (current != e) {
        auto i = current++;        
        erase(i);
    }    
}

// Returns a copy of List
// take a list and clone function as parameter
template<typename NodeCloneFunction>
List copy(const List& list, NodeCloneFunction cloneF)
{
    List newList = makeList();
    auto b = begin(list);
    auto e = end(list);

    std::for_each(b, e, [&](const ListCell* cell) {
        push_back(newList, cloneF(cell));
    });
    return newList;
}

// Reverse list inplace by direct pointer manipulation
void reverse(List& list)
{
    ListCell* head = list.head;
    ListCell* tail = list.head;
    ListCell *prev = nullptr;
    ListCell *next;

    while (head) {
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }

    list.head = prev;
    list.tail = tail;
}

typedef std::unique_ptr<List> UniqueListPtr;

// Create a List in head and returns a std::unique_ptr to it
UniqueListPtr makeListHeap()
{
    auto list = UniqueListPtr(new List());
    list->head = nullptr;
    list->length = 0;
    list->tail = nullptr;
    list->type = T_List;
    return list;
}

// Basic RAII List holder
// cleans List during destruction
struct ListHolder
{
    ListHolder(List& l):list(l) {}
    ~ListHolder() { clean(list); }
    List& getList() { return list; }
private:
    ListHolder(const ListHolder&);
    ListHolder& operator=(const ListHolder&);
    List& list;
};


// ListNodeTrait implementation for List type
// Reverse implementations can work with any type
// that provides few additional information about type
template<>
struct ListNodeTrait<List>
{
    typedef ListCell* node;
    typedef BasicListIterator iterator;

    static iterator begin(const List& list) { return ::begin(list); }
    static iterator end(const List& list) { return ::end(list); }
    static void reverse(List& list) { ::reverse(list); }

    static void appendElement(ListCell* node, bool& firstElement, std::wstring& result)
    {
        if (!firstElement) result.append(L".");
        result.append(castNode<Ident>(node)->name);
        firstElement = false;
    }
};

#endif
