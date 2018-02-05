#ifndef LIST_NODE_TRAIT_H
#define LIST_NODE_TRAIT_H

// Trait should contain few things
// node type definition -> typedef Node* node
// iterator type definition ->  typedef Container::iterator iterator
// begin function -> static iterator begin(const Container& container)
// end function -> static iterator end(const Container& container)
// appendElement -> static void appendElement(Node* node, bool& firstElement, std::wstring& result)

template<typename T>
struct ListNodeTrait;

#endif
