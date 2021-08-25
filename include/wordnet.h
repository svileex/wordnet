#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Digraph
{
    struct Node
    {
        std::unordered_set<size_t> parents;
        std::string gloss;

        Node() = default;

        Node(const std::string & gloss)
            : gloss(gloss)
        {
        }
    };

    std::map<size_t, Node> graph;
    std::unordered_map<std::string, std::set<int>> words;

    void add_words(const std::vector<std::string> & words_list, size_t parent);

public:
    class iterator
    {
        std::unordered_map<std::string, std::set<int>>::const_iterator it;

        iterator(const std::unordered_map<std::string, std::set<int>> & words, bool isEnd)
            : it(isEnd ? words.end() : words.begin())
        {
        }

    public:
        friend Digraph;
        using difference_type = std::ptrdiff_t;
        using value_type = std::string;
        using pointer = std::string const *;
        using reference = const value_type &;
        using iterator_category = std::forward_iterator_tag;

        iterator(){};

        reference operator*() const
        {
            return it->first;
        }

        pointer operator->() const
        {
            return &it->first;
        }

        iterator & operator++()
        {
            it++;
            return *this;
        }

        iterator operator++(int)
        {
            auto temp = *this;
            operator++();
            return temp;
        }

        friend bool operator==(const iterator & lhs, const iterator & rhs)
        {
            return (lhs.it == rhs.it);
        }

        friend bool operator!=(const iterator & lhs, const iterator & rhs)
        {
            return !(lhs == rhs);
        }
    };

    iterator begin() const;

    iterator end() const;

    void add(size_t index, const std::vector<std::string> & wordsa, const std::string & gloss);

    void set_parents(size_t id, std::unordered_set<size_t> parents);

    const std::unordered_set<size_t> & get_vertices_parents(size_t id) const;

    bool contains_word(const std::string & word) const;

    const std::set<int> & get_word_parents(const std::string & word) const;

    const std::string & get_gloss(size_t id) const;

    size_t size() const;

    friend std::ostream & operator<<(std::ostream & os, const Digraph & dg);
};

class ShortestCommonAncestor
{
private:
    Digraph * graph;

    std::pair<size_t, size_t> colored_search(const std::set<int> & subset_a, const std::set<int> & subset_b) const;

    struct Marked
    {
        size_t red_dist;
        size_t green_dist;

        Marked()
            : red_dist(std::numeric_limits<size_t>().max())
            , green_dist(std::numeric_limits<size_t>().max())
        {
        }
    };

public:
    ShortestCommonAncestor(Digraph & dg);

    //calculates length of shortest common ancestor path from node with id 'v' to node with id 'w'
    int length(int v, int w);

    //returns node id of shortest common ancestor of nodes v and w
    int ancestor(int v, int w);

    //calculates length of shortest common ancestor path from node subset 'subset_a' to node subset 'subset_b'
    int length_subset(const std::set<int> & subset_a, const std::set<int> & subset_b);

    //returns node id of shortest common ancestor of node subset 'subset_a' and node subset 'subset_b'
    int ancestor_subset(const std::set<int> & subset_a, const std::set<int> & subset_b);
};

class WordNet
{
    Digraph graph;
    ShortestCommonAncestor graph_sca;

public:
    using iterator = Digraph::iterator;

    WordNet(const std::string & synsets_fn, const std::string & hypernyms_fn);

    size_t size() const;

    Digraph::iterator begin() const;

    // get iterator to list all nouns stored in WordNet
    Digraph::iterator nouns() const;

    Digraph::iterator end() const;

    // returns 'true' iff 'word' is stored in WordNet
    bool is_noun(const std::string & word) const;

    // returns gloss of "shortest common ancestor" of noun1 and noun2
    std::string sca(const std::string & noun1, const std::string & noun2);

    // calculates distance between noun1 and noun2
    int distance(const std::string & noun1, const std::string & noun2);
};

class Outcast
{
    WordNet & wordnet;

public:
    Outcast(WordNet & wordnet);

    // returns outcast word
    std::string outcast(const std::vector<std::string> & nouns);
};
