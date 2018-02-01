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

template<typename DEST>
DEST* castNode(const ListCell* cell)
{
    return reinterpret_cast<DEST*>(cell->data.ptr_value);
}

template<>
struct ListNodeTrait<List>
{
    typedef ListCell* node;

    static void appendElement(ListCell* node, bool& firstElement, std::wstring& result)
    {
        if (!firstElement) result.append(L".");
        result.append(castNode<Ident>(node)->name);
        firstElement = false;
    }
};


struct IdentExt : public Ident
{
    ~IdentExt() { delete[] name; }
};


template<typename T>
struct AssignemntTypeChooser
{
    static T* assign(T* type, int value)
    {
	#ifdef __GNUC__
        #else
        static_assert("Unsupported type");
	#endif
    }
};

template<>
struct AssignemntTypeChooser<int>
{
    static ListCell* assign(ListCell* listCell, int value)
    {
        listCell->data.int_value = value;
        return listCell;
    }
};


template<>
struct AssignemntTypeChooser<Node*>
{
    static ListCell* assign(ListCell* listCell, Node* value)
    {
        listCell->data.ptr_value = value;
        return listCell;
    }
};

List makeList()
{
    List list;
    list.head = nullptr;
    list.length = 0;
    list.tail = nullptr;
    list.type = T_List;
    return list;
}


void decListSize(const List& list)
{
    --const_cast<List&>(list).length;
}

int getListSize(const List& list)
{
    return const_cast<List&>(list).length;
}

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

void cleanIdent(const Ident* ident)
{
    delete [] ident->name;
}

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
    template<typename ValueType>
    friend BasicListIterator erase(BasicListIterator& iter);
};
   
BasicListIterator begin(const List& list) { return BasicListIterator(list, list.head);}
BasicListIterator end(const List& list)   { return BasicListIterator(list, nullptr);}

template<typename ValueType>
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
    decListSize(iter.list);    
    BasicListIterator i(iter.list, iter.nodePtr->next);
    // TODO below delete depends on ValueType
    delete reinterpret_cast<ValueType*>(toDelete->data.ptr_value);
    delete toDelete;
    return i;
}

void clean(const List& list)
{
    auto current = begin(list);
    auto e = end(list);
    while (current != e) {
        auto i = current++;        
        erase<Node>(i);
    }    
}

typedef std::unique_ptr<List> UniqueListPtr;

UniqueListPtr makeListHeap()
{
    auto list = UniqueListPtr(new List());
    list->head = nullptr;
    list->length = 0;
    list->tail = nullptr;
    list->type = T_List;
    return list;
}


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

void traverseNodes(const List& list)
{
    auto current = list.head;
    while (current) {
        std::wcout << castNode<Ident>(current)->name << std::endl;
        current = current->next;
    }
}
void traverseInts(const List& list)
{
    auto current = list.head;
    while (current) {
        std::wcout << current->data.int_value << std::endl;
        current = current->next;
    }
}

void printIdent(Ident* ident)
{
    std::wcout << ident->name << std::endl;
}


#endif
