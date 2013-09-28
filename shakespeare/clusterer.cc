// Copyright 2013
// Author: Christopher Van Arsdale
//
// Clusterer. Note that this is not coded optimally!


#include <memory>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "shakespeare/clusterer.h"

using std::string;
using std::vector;
using std::set;
using std::map;

namespace shakespeare {
namespace {
struct Pair {
  Pair() {}
  Pair(const string& f, const string& s) : first(f>s ? s : f),
                                           second(f>s ? f : s) {}
  string first, second;
  bool operator==(const Pair& other) const {
    return first == other.first && second == other.second;
  }
  bool operator<(const Pair& other) const {
    return (first < other.first ||
            (first == other.first && second < other.second));
  }
};

struct ComparePair {
  bool operator()(const Pair& a, const Pair& b) const {
    return a < b;
  }
};
}  // anonymous namespace

struct Clusterer::PairMap {
  map<Pair, double, ComparePair> distance;
};

Clusterer::Clusterer()
    : distances_(new PairMap) {
}

Clusterer::~Clusterer() {
}

void Clusterer::AddPair(const std::string& a,
                        const std::string& b,
                        double distance) {
  distances_->distance[Pair(a, b)] = distance;
}

void Clusterer::Cluster(int min_clusters,
                        double max_cluster_merge,
                        std::vector<std::vector<std::string> >* output) const {
  map<string, vector<string> > clusters;
  map<Pair, double, ComparePair> distance = distances_->distance;

  // Initial clusters
  for (auto it : distance) {
    const Pair& pair = it.first;
    if (clusters.find(pair.first) == clusters.end()) {
      clusters[pair.first].push_back(pair.first);
    }
    if (clusters.find(pair.second) == clusters.end()) {
      clusters[pair.second].push_back(pair.second);
    }
  }

  // Clustering time (... note that this is not coded optimally ....)!
  while (clusters.size() > min_clusters) {
    // Figure out closest distance
    double min_dist = 10000000;
    Pair min_tuple;
    for (auto it : distance) {
      if (it.second < min_dist) {
        min_dist = it.second;
        min_tuple = it.first;
      }
    }
    if (min_dist > max_cluster_merge) {
      break;
    }

    // Figure out new distances for everyone.
    int first_size =  clusters[min_tuple.first].size();
    int second_size = clusters[min_tuple.second].size();
    set<Pair> to_erase;
    for (auto& it : distance) {
      if (it.first.first == min_tuple.first ||
          it.first.second == min_tuple.first) {
        const string& other = (it.first.first == min_tuple.first ?
                               it.first.second : it.first.first);
        double other_dist = distance[Pair(min_tuple.second, other)];
        double distance = ((it.second * first_size +
                            other_dist * second_size) /
                           (first_size + second_size));
        it.second = distance;
      }
      if (it.first.first == min_tuple.second ||
          it.first.second == min_tuple.second) {
        to_erase.insert(it.first);
      }
    }

    // Erase everyone's connection to the dead cluster.
    for (auto it : to_erase) {
      distance.erase(it);
    }

    // Merge finally.
    for (auto it : clusters[min_tuple.second]) {
      clusters[min_tuple.first].push_back(it);
    }
    clusters.erase(min_tuple.second);
  }

  for (auto it : clusters) {
    output->push_back(it.second);
  }
}

}  // namespace shakespeare
