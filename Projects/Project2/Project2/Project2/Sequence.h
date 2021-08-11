#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <string>
using ItemType = std::string;


class Sequence
{
public:
    Sequence();
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);

    ~Sequence();
    Sequence(const Sequence&);
    Sequence& operator=(const Sequence&);
private:
    struct Node 
    {
        ItemType nodeValue;
        Node* next = nullptr;
        Node* previous = nullptr;
    };
    void AddToFront(const ItemType& value);
    void AddToRear(const ItemType& value);
    Node* head = nullptr;
    Node* tail = nullptr;
    int m_size;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
#endif