#include <wordnet.h>

std::pair<size_t, size_t> ShortestCommonAncestor::colored_search(const std::set<int> & subset_a, const std::set<int> & subset_b) const
{
    std::vector<Marked> tree(graph->size());
    std::unordered_set<size_t> curr_a;
    std::unordered_set<size_t> curr_b;

    for (const auto & i : subset_a) {
        tree[i].red_dist = 0;
        curr_a.insert(i);
    }

    for (const auto & i : subset_b) {
        tree[i].green_dist = 0;
        if (tree[i].red_dist != std::numeric_limits<size_t>().max()) {
            return {i, 0};
        }
        curr_b.insert(i);
    }

    std::pair<size_t, size_t> answ{0, std::numeric_limits<size_t>().max()};
    bool notAll = true;

    while (notAll) {
        notAll = false;
        std::unordered_set<size_t> new_a;
        std::unordered_set<size_t> new_b;

        for (const auto & i : curr_a) {
            const auto & parents = graph->get_vertices_parents(i);

            for (const auto & j : parents) {
                if (tree[j].red_dist == std::numeric_limits<size_t>().max()) {
                    tree[j].red_dist = tree[i].red_dist + 1;
                    notAll = true;
                    new_a.insert(j);
                }
                if (tree[j].green_dist != std::numeric_limits<size_t>().max()) {
                    if (answ.second > tree[j].green_dist + tree[j].red_dist) {
                        answ = {j, tree[j].green_dist + tree[j].red_dist};
                    }
                }
            }
        }

        curr_a = new_a;

        for (const auto & i : curr_b) {
            const auto & parents = graph->get_vertices_parents(i);

            for (const auto & j : parents) {
                if (tree[j].green_dist == std::numeric_limits<size_t>().max()) {
                    tree[j].green_dist = tree[i].green_dist + 1;
                    notAll = true;
                    new_b.insert(j);
                }
                if (tree[j].red_dist != std::numeric_limits<size_t>().max()) {
                    if (answ.second > tree[j].green_dist + tree[j].red_dist) {
                        answ = {j, tree[j].green_dist + tree[j].red_dist};
                    }
                }
            }
        }

        curr_b = new_b;
    }

    return answ;
}

ShortestCommonAncestor::ShortestCommonAncestor(Digraph & dg)
    : graph(&dg)
{
}

int ShortestCommonAncestor::length(int v, int w)
{
    return colored_search({v}, {w}).second;
}

int ShortestCommonAncestor::ancestor(int v, int w)
{
    return colored_search({v}, {w}).first;
}

int ShortestCommonAncestor::length_subset(const std::set<int> & subset_a, const std::set<int> & subset_b)
{
    return colored_search(subset_a, subset_b).second;
}

int ShortestCommonAncestor::ancestor_subset(const std::set<int> & subset_a, const std::set<int> & subset_b)
{
    return colored_search(subset_a, subset_b).first;
}