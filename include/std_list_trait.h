#ifndef STD_LIST_NODE_TRAIT_H
#define STD_LIST_NODE_TRAIT_H

#include "list_node_trait.h"

template<>
struct ListNodeTrait<std::list<Node*>>
{
    typedef Node* node;
    static void appendElement(Node* node, bool& firstElement, std::wstring& result)
    {
        if (!firstElement) result.append(L".");
        result.append(reinterpret_cast<Ident*>(node)->name);
        firstElement = false;
    }
};

#endif
