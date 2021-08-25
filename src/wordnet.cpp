#include <regex>
#include <unordered_map>
#include <vector>
#include <wordnet.h>

WordNet::WordNet(const std::string & synsets_fn, const std::string & hypernyms_fn)
    : graph_sca(graph)
{
    std::ifstream synsets(synsets_fn);
    if (!synsets.is_open()) {
        throw std::ios_base::failure("synsets file not found");
    }
    std::string line;

    while (std::getline(synsets, line)) {
        std::string id_str = line.substr(0, line.find(','));
        std::string temp = line.substr(line.find(',') + 1, line.size() - line.find(',')); //line without id
        std::vector<std::string> syns;

        //NOTE: режу строку на три куска, т.к потенциально regex_token_iterator разобьет сразу по всем ',',
        // а меня интересуют только первые две
        size_t id = stoi(id_str);
        std::string syns_list = temp.substr(0, temp.find(','));
        std::regex syns_del{' '};
        std::sregex_token_iterator synsIterator{syns_list.cbegin(), syns_list.cend(), syns_del, -1};

        while (synsIterator != std::sregex_token_iterator{}) {
            syns.push_back(*synsIterator);
            ++synsIterator;
        }

        graph.add(id, syns, temp.substr(temp.find(',') + 1, temp.size() - temp.find(',') - 1));
    }

    synsets.close();
    synsets.open(hypernyms_fn);
    if (!synsets.is_open()) {
        throw std::ios_base::failure("hypernyms file not found");
    }

    while (std::getline(synsets, line)) {
        std::regex delimiter{','};
        std::sregex_token_iterator tokenIterator{line.cbegin(), line.cend(), delimiter, -1};
        std::unordered_set<size_t> parents;

        size_t id = stoi(*tokenIterator);
        ++tokenIterator;

        while (tokenIterator != std::sregex_token_iterator{}) {
            parents.insert(stoi(*tokenIterator));
            ++tokenIterator;
        }

        graph.set_parents(id, std::move(parents));
    }
}

size_t WordNet::size() const
{
    return graph.size();
}

Digraph::iterator WordNet::begin() const
{
    return nouns();
}

Digraph::iterator WordNet::nouns() const
{
    return graph.begin();
}

Digraph::iterator WordNet::end() const
{
    return graph.end();
}

bool WordNet::is_noun(const std::string & word) const
{
    return graph.contains_word(word);
}

std::string WordNet::sca(const std::string & noun1, const std::string & noun2)
{
    return graph.get_gloss(graph_sca.ancestor_subset(graph.get_word_parents(noun1), graph.get_word_parents(noun2)));
}

int WordNet::distance(const std::string & noun1, const std::string & noun2)
{
    return graph_sca.length_subset(graph.get_word_parents(noun1), graph.get_word_parents(noun2));
}

std::ostream & operator<<(std::ostream & os, const Digraph & dg)
{
    for (const auto & i : dg.graph) {
        os << "vert: " << i.first << " parents:";
        for (const auto & j : i.second.parents) {
            os << " " << j;
        }
        os << " gloss: " << i.second.gloss << '\n';
    }

    return os;
}