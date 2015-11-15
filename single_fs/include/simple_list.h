//
//
//
//
#ifndef SIMPLE_LIST
#define SIMPLE_LIST
#include <stdio.h>
template<typename T>
struct Node
{
    Node():next_(NULL), pre_(NULL){}
    Node(T data):next_(NULL), pre_(NULL), data_(data) {}
    Node* next_;
    Node* pre_;
    T data_;
};

template<typename T>
class IteratorFun
{
    public:
        virtual void action(Node<T>* node) = 0;
};
template<typename T>
class SimpleList
{
    public:
        SimpleList();
        ~SimpleList();
        Node<T>* push_back(T t);
        void remove_node(Node<T>* node_);
        void HandleEveryNode(IteratorFun<T>* iter);
    private:
        Node<T> head_;
        Node<T> tail_;
        template<typename T1>
        class IteratorDel: public IteratorFun<T1>
        {
            virtual void action(Node<T1>* node)
            {
                printf("deling\n");
                node->next_->pre_ = node->pre_;
                node->pre_->next_ = node->next_;
                delete node;
            }
        };
};

template<typename T>
SimpleList<T>::SimpleList()
{
}
template<typename T>
SimpleList<T>::~SimpleList()
{
    IteratorFun<T>* fun = new IteratorDel<T>();
    HandleEveryNode(fun);
    delete fun;
}
template<typename T>
Node<T>* SimpleList<T>::push_back(T t)
{
    Node<T>* node = new Node<T>(t);
    if (head_.next_ == NULL || head_.next_ == &tail_) {
        printf("push first node\n");
        head_.next_ = node;
        tail_.pre_ = node;
        node->next_ = &tail_;
        node->pre_ = &head_;
        return node;
    }
    node->pre_ = tail_.pre_;
    node->next_ = &tail_;
    tail_.pre_ = node;
    node->pre_->next_ = node;
    return node;
}
template<typename T>
void SimpleList<T>::remove_node(Node<T>* node)
{
    if (node == &head_ || node == &tail_) return;
    node->next_->pre_ = node->pre_;
    node->pre_->next_ = node->next_;
    delete node;
}
template<typename T>
void SimpleList<T>::HandleEveryNode(IteratorFun<T>* iter)
{
    
    if (head_.next_ == NULL || head_.next_ == &tail_) return;
    Node<T>* node = head_.next_;
    while (node != &tail_) {
        Node<T>* tmp = node->next_;
        iter->action(node);
        node = tmp;
    }
}
#endif
