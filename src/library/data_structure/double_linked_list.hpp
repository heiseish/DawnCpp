#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <memory>

namespace Dawn::Library {

template <typename value_type>
class DoublyLinkedListNode {
public:
    DoublyLinkedListNode() {}
    DoublyLinkedListNode(value_type value) : _value(value) {}
    void Set(value_type);
    value_type Get();
    const value_type& Get();
    void SetNext(std::shared_ptr<DoublyLinkedListNode<value_type>>);
    void SetPrev(std::shared_ptr<DoublyLinkedListNode<value_type>>);

private:
    value_type _value;
    std::shared_ptr<DoublyLinkedListNode<value_type>> _next, _prev;
};

template <typename value_type>
class DoubleLinkedList {
public:
    DoubleLinkedList() {}
    ~DoubleLinkedList();
    void PushBack(value_type);
    void PushBack(DoublyLinkedListNode<value_type>);
    void PushBack(std::shared_ptr<DoublyLinkedListNode<value_type>>);
    void PopBack();
    std::shared_ptr<DoublyLinkedListNode<value_type>> GetBack();

    void PushFront(value_type);
    void PushFront(DoublyLinkedListNode<value_type>);
    void PushFront(std::shared_ptr<DoublyLinkedListNode<value_type>>);
    void PopFront();
    std::shared_ptr<DoublyLinkedListNode<value_type>> GetFront();

private:
    std::shared_ptr<DoublyLinkedListNode<value_type>> _front, _back;
};

}  // namespace Dawn::Library

#endif