#include <wordnet.h>

void Digraph::add_words(const std::vector<std::string> & words_list, size_t parent)
{
    for (const auto & i : words_list) {
        words.try_emplace(i).first->second.insert(parent);
    }
}

void Digraph::add(size_t index, const std::vector<std::string> & wordsa, const std::string & gloss)
{
    graph[index] = Node(gloss);
    add_words(wordsa, index);
}

void Digraph::set_parents(size_t id, std::unordered_set<size_t> parents)
{
    graph[id].parents = std::move(parents);
}

const std::unordered_set<size_t> & Digraph::get_vertices_parents(size_t id) const
{
    return graph.at(id).parents;
}

bool Digraph::contains_word(const std::string & word) const
{
    return (words.find(word) != words.end());
}

const std::set<int> & Digraph::get_word_parents(const std::string & word) const
{
    return words.at(word);
}

const std::string & Digraph::get_gloss(size_t id) const
{
    return graph.at(id).gloss;
}

size_t Digraph::size() const
{
    return words.size();
}

Digraph::iterator Digraph::begin() const
{
    return iterator(words, false);
}

Digraph::iterator Digraph::end() const
{
    return iterator(words, true);
}
