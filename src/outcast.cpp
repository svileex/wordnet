#include <wordnet.h>

std::string Outcast::outcast(const std::vector<std::string> & nouns)
{
    std::vector<std::vector<size_t>> distances(nouns.size(), std::vector<size_t>(nouns.size()));
    size_t max = 0;
    size_t count = 0;
    size_t index;

    for (size_t i = 0; i < nouns.size(); ++i) {
        size_t current = 0;

        for (size_t j = 0; j < nouns.size(); ++j) {
            if (i != j) {
                if (distances[i][j] == 0) {
                    distances[i][j] = wordnet.distance(nouns[i], nouns[j]);
                    distances[j][i] = distances[i][j];
                }
                current += distances[i][j];
            }
        }

        if (current > max) {
            max = current;
            index = i;
            count = 1;
        }
        else if (current == max) {
            ++count;
        }
    }
    if (nouns.size() == 1 || count != 1) {
        return "";
    }

    return nouns[index];
}

Outcast::Outcast(WordNet & wordnet)
    : wordnet(wordnet)
{
}