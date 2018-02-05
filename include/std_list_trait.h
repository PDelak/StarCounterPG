#ifndef STD_LIST_NODE_TRAIT_H
#define STD_LIST_NODE_TRAIT_H

#include "list_node_trait.h"

template<>
struct ListNodeTrait<std::list<Node*>>
{
    typedef Node* node;
    typedef std::list<Node*>::const_iterator iterator;
    static iterator begin(const std::list<Node*>& stdlist) { return stdlist.begin(); }
    static iterator end(const std::list<Node*>& stdlist) { return stdlist.end(); }
    static void reverse(std::list<Node*>& stdlist) { stdlist.reverse(); }

    static void appendElement(Node* node, bool& firstElement, std::wstring& result)
    {
        if (!firstElement) result.append(L".");
        result.append(reinterpret_cast<Ident*>(node)->name);
        firstElement = false;
    }
};

#endif
