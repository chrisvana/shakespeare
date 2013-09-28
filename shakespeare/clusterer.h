// Copyright 2013
// Author: Christopher Van Arsdale

#ifndef _SHAKESPEARE_CLUSTERER_H__
#define _SHAKESPEARE_CLUSTERER_H__

#include <memory>
#include <string>
#include <vector>

namespace shakespeare {

class Clusterer {
 public:
  Clusterer();
  ~Clusterer();

  void AddPair(const std::string& a, const std::string& b, double distance);
  void Cluster(int min_clusters,
               double max_cluster_merge,
               std::vector<std::vector<std::string> >* clusters) const;

 private:
  struct PairMap;
  std::unique_ptr<PairMap> distances_;
};

}

#endif  // _SHAKESPEARE_CLUSTERER_H__
