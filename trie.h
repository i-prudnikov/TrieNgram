#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <unordered_map>

//TODO: separate includes to those that should be in main сpp & trie.cpp
//Here should be ones that, is absolutely needed.

#include <string>
#include <random>

#include <vector>
#include <deque>
#include <stdexcept>
#include <cstring>

#include <memory> //shared_ptr support
#include <fstream> // serializing/de-serializing

#include <sys/stat.h>
#include <fcntl.h>

//Working with directories
#include <dirent.h>
#include <unistd.h>

//for ICU support
//Please use this linker options "-licuuc -licuio"
#include <unicode/ustream.h>
#include <unicode/brkiter.h>
#include <unicode/ustring.h>
#include "unicode/ustdio.h"
#include "unicode/uchar.h"

using std::unordered_map;
using std::pair;
using std::string;
using std::vector;
using std::deque;
using icu::UnicodeString;
using std::shared_ptr;
using std::unique_ptr;
using std::ofstream;
using std::ifstream;
using std::ostream;

using std::cout;
using std::endl;
using std::runtime_error;
using std::logic_error;
using std::exception;

//specialize hash class for UnicodeStrings
namespace std
{
    template<>
    class hash<UnicodeString> {
    public:
        size_t operator()(const UnicodeString &s) const
        {
            return (size_t) s.hashCode();
        }
    };
};

class Trie
{
    public:
        unsigned int chainRank;
        long long nodesCount;
        struct NodeElement
        {
            unsigned short count; // how many times the node placed to this place
            unordered_map<UnicodeString, shared_ptr<NodeElement>> childNodes; // each node stores a map to its child nodes
            const pair<const UnicodeString,shared_ptr<NodeElement>> * parent; // pointer to a parent node;
            inline NodeElement(int iCount = 1): count(iCount){};
        };
        typedef pair<const UnicodeString,shared_ptr<NodeElement>> Node;
        typedef unordered_map<UnicodeString, shared_ptr<NodeElement>> Container;


        template <typename ValueType>
        class TrieIterator: public std::iterator<std::input_iterator_tag, ValueType>
        {
         friend class Trie;

         ValueType* p;
         ValueType* pLn;
         explicit TrieIterator(ValueType* ptr,ValueType* ptrLn) : p(ptr), pLn(ptrLn) {dir = direction::down;curParent = p;childProcessed=0;};

         enum class direction {down, right} dir;
         Container::iterator chNodeIteratorPos;
         const ValueType* curParent;
         const ValueType* futureParent;

         int childProcessed;
         bool isRoot(const ValueType* p);

         public:
            TrieIterator(const TrieIterator &it) : p(it.p), pLn(it.pLn) {dir = direction::down;curParent = p;childProcessed=0;};
            bool operator!=(const TrieIterator & it) const;
            bool operator==(const TrieIterator & it) const; //need for BOOST_FOREACH
            //typename TrieIterator::reference operator*() const;
            ValueType & operator*() const;
            TrieIterator& operator++();
        };

        typedef TrieIterator<Node> iterator;
        typedef TrieIterator<const Node> const_iterator;

        Trie(int nRank=0);
        Trie(const Trie &); //update to move semantics
        Trie & operator=(const Trie &); //update to move semantics
        virtual ~Trie();
        enum class Mode{MostFreq,ProbDistrib};

        //This method scans the corpus with the gliding frame of "windowWidth" width, every n-gram will be added into tire
        bool BuildTrieFromFile(const string & filename);
        bool BuildTrieFromFolder(const string & folder);

        bool CompleteText(const UnicodeString & startText,int iLength, enum Mode = Mode::MostFreq);

        bool SerializeTree(const string & filename);

        bool DeserializeTree(const string & filename);

        //Iterator methods
        iterator begin();
        iterator end();

        //const_iterator begin() const;
        //const_iterator end() const;

        friend ostream & operator<<(ostream & os, const Trie & t);
        friend ostream & operator<<(ostream & os, const Node & t);


    public: //!switch back to private

        Node rootNode;
        Node *lastNode;

        // Basic trie routines
        bool InsertNGram(const deque<UnicodeString> & nGram);

        //Tire analysis
        Node * SearchNode(const deque<UnicodeString> & nodePath);

        UnicodeString GetMostFrequentChildNode(const deque<UnicodeString> & nodePath);
        UnicodeString GetRandomChildNode(const deque<UnicodeString> & nodePath);

        //Utilities
        deque<UnicodeString> String2Deque(const UnicodeString & str);

        void DeleteNode(Node & nd);
        void CopyNodesRecursive(const Node & sourceNode,Node & parent);

        void WriteNodeToFile(ofstream & fout,const Node & node);
        void ReadNodeFromFile(ifstream & fin, Node & node);
};
#endif // TRIE_H
