#ifndef DSA_STRUCTURES_H
#define DSA_STRUCTURES_H

#include <QString>
#include <QVector>
#include <stdexcept>

//  1. LINKED LIST — Voter Registry

struct VoterNode {
    int        voterId;
    QString    name;
    bool       hasVoted;
    int        votedForId;
    VoterNode* next;
    VoterNode(int id, const QString& n)
        : voterId(id), name(n), hasVoted(false), votedForId(-1), next(nullptr) {}
};

class VoterLinkedList {
public:
    VoterNode* head;
    int        size;

    VoterLinkedList() : head(nullptr), size(0) {}
    ~VoterLinkedList() { clear(); }

    void insert(int id, const QString& name) {
        VoterNode* node = new VoterNode(id, name);
        if (!head) { head = node; }
        else {
            VoterNode* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++size;
    }

    VoterNode* findById(int id) {
        for (VoterNode* cur = head; cur; cur = cur->next)
            if (cur->voterId == id) return cur;
        return nullptr;
    }

    bool deleteById(int id) {
        if (!head) return false;
        if (head->voterId == id) {
            VoterNode* tmp = head; head = head->next; delete tmp; --size; return true;
        }
        for (VoterNode* cur = head; cur->next; cur = cur->next) {
            if (cur->next->voterId == id) {
                VoterNode* tmp = cur->next; cur->next = tmp->next; delete tmp; --size; return true;
            }
        }
        return false;
    }

    QVector<VoterNode*> toVector() const {
        QVector<VoterNode*> v;
        for (VoterNode* cur = head; cur; cur = cur->next) v.append(cur);
        return v;
    }

    void clear() {
        while (head) { VoterNode* t = head; head = head->next; delete t; }
        size = 0;
    }
};
//  2. QUEUE — Voting Line (FIFO)

struct QueueNode {
    int        voterId;
    QueueNode* next;
    QueueNode(int id) : voterId(id), next(nullptr) {}
};

class VotingQueue {
public:
    QueueNode* front;
    QueueNode* rear;
    int        count;

    VotingQueue() : front(nullptr), rear(nullptr), count(0) {}
    ~VotingQueue() { while (!isEmpty()) dequeue(); }

    bool isEmpty() const { return front == nullptr; }

    void enqueue(int id) {
        QueueNode* node = new QueueNode(id);
        if (!rear) front = rear = node;
        else { rear->next = node; rear = node; }
        ++count;
    }

    int dequeue() {
        if (isEmpty()) throw std::runtime_error("Queue empty");
        QueueNode* tmp = front;
        int id = tmp->voterId;
        front = front->next;
        if (!front) rear = nullptr;
        delete tmp; --count;
        return id;
    }

    int peek() const { return isEmpty() ? -1 : front->voterId; }

    QVector<int> toVector() const {
        QVector<int> v;
        for (QueueNode* cur = front; cur; cur = cur->next) v.append(cur->voterId);
        return v;
    }
};


//  3. BINARY SEARCH TREE — Candidate Store

struct BSTNode {
    int      candidateId;
    QString  name;
    QString  party;
    int      voteCount;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int id, const QString& n, const QString& p)
        : candidateId(id), name(n), party(p), voteCount(0), left(nullptr), right(nullptr) {}
};

class CandidateBST {
public:
    BSTNode* root;
    CandidateBST() : root(nullptr) {}
    ~CandidateBST() { destroy(root); }

    void     insert(int id, const QString& n, const QString& p) { root = ins(root, id, n, p); }
    BSTNode* search(int id) { return srch(root, id); }
    bool     addVote(int id) { BSTNode* n = search(id); if (n) { ++n->voteCount; return true; } return false; }
    void     remove(int id)  { root = del(root, id); }
    bool     isEmpty() const { return root == nullptr; }

    QVector<BSTNode*> inOrder() const {
        QVector<BSTNode*> v; inOrd(root, v); return v;
    }

private:
    BSTNode* ins(BSTNode* n, int id, const QString& nm, const QString& p) {
        if (!n) return new BSTNode(id, nm, p);
        if (id < n->candidateId) n->left  = ins(n->left,  id, nm, p);
        else if (id > n->candidateId) n->right = ins(n->right, id, nm, p);
        return n;
    }
    BSTNode* srch(BSTNode* n, int id) const {
        if (!n || n->candidateId == id) return n;
        return id < n->candidateId ? srch(n->left, id) : srch(n->right, id);
    }
    BSTNode* minNode(BSTNode* n) { while (n->left) n = n->left; return n; }
    BSTNode* del(BSTNode* n, int id) {
        if (!n) return nullptr;
        if (id < n->candidateId) n->left  = del(n->left,  id);
        else if (id > n->candidateId) n->right = del(n->right, id);
        else {
            if (!n->left) { BSTNode* t = n->right; delete n; return t; }
            if (!n->right){ BSTNode* t = n->left;  delete n; return t; }
            BSTNode* s = minNode(n->right);
            n->candidateId = s->candidateId; n->name = s->name;
            n->party = s->party; n->voteCount = s->voteCount;
            n->right = del(n->right, s->candidateId);
        }
        return n;
    }
    void inOrd(BSTNode* n, QVector<BSTNode*>& v) const {
        if (!n) return; inOrd(n->left, v); v.append(n); inOrd(n->right, v);
    }
    void destroy(BSTNode* n) {
        if (!n) return; destroy(n->left); destroy(n->right); delete n;
    }
};


//  4. SORTING ALGORITHMS — Results Ranking

struct CandidateResult {
    int     id;
    QString name;
    QString party;
    int     votes;
};

class SortingAlgorithms {
public:
    static void bubbleSort(QVector<CandidateResult>& a) //Bubble sort
    {
        int n = a.size();
        for (int i = 0; i < n-1; i++) {
            bool swapped = false;
            for (int j = 0; j < n-i-1; j++)
                if (a[j].votes < a[j+1].votes) { qSwap(a[j], a[j+1]); swapped = true; }
            if (!swapped) break;
        }
    }

    static void selectionSort(QVector<CandidateResult>& a) //selection sort
    {
        int n = a.size();
        for (int i = 0; i < n-1; i++) {
            int mx = i;
            for (int j = i+1; j < n; j++) if (a[j].votes > a[mx].votes) mx = j;
            if (mx != i) qSwap(a[i], a[mx]);
        }
    }

    static void mergeSort(QVector<CandidateResult>& a, int l, int r) //merge sort
    {
        if (l >= r) return;
        int m = l + (r-l)/2;
        mergeSort(a, l, m); mergeSort(a, m+1, r); merge(a, l, m, r);
    }

private:
    static void merge(QVector<CandidateResult>& a, int l, int m, int r) {
        QVector<CandidateResult> tmp;
        int i = l, j = m+1;
        while (i <= m && j <= r)
            tmp.append(a[i].votes >= a[j].votes ? a[i++] : a[j++]);
        while (i <= m) tmp.append(a[i++]);
        while (j <= r)  tmp.append(a[j++]);
        for (int k = l; k <= r; k++) a[k] = tmp[k-l];
    }
};

#endif // DSA_STRUCTURES_H
