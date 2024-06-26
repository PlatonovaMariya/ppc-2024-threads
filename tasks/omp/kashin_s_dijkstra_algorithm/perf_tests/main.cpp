// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>

#include <numeric>
#include <random>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "omp/kashin_s_dijkstra_algorithm/include/Dijkstra.hpp"

TEST(KashinDijkstraOmpTest, test_pipeline_run) {
  const int vertexCount = 5000;
  const int edgeWeight = 100;
  const int start = 0;
  const int seed = 42;
  // создаем повторяющийся рандом
  std::mt19937 rng(seed);
  std::uniform_int_distribution<int> dist(1, edgeWeight);
  std::uniform_int_distribution<int> isEdge(0, 2);

  // Create data
  std::vector<int> in;
  in.reserve(vertexCount * vertexCount);
  std::vector<int> out(vertexCount);
  for (int i = 0; i < vertexCount * vertexCount; i++) {
    int weight = isEdge(rng) == 0 ? -1 : dist(rng);
    in.push_back(weight);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOmp->inputs_count.emplace_back(in.size());
  taskDataOmp->inputs_count.emplace_back(start);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOmp->outputs_count.emplace_back(out.size());

  // Create Task
  auto testDijkstraOmpuential = std::make_shared<KashinDijkstraOmp::Dijkstra>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testDijkstraOmpuential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(13716, std::accumulate(out.begin(), out.end(), 0));
}

TEST(KashinDijkstraOmpTest, test_task_run) {
  const int vertexCount = 5000;
  const int edgeWeight = 100;
  const int start = 0;
  const int seed = 42;
  // создаем повторяющийся рандом
  std::mt19937 rng(seed);
  std::uniform_int_distribution<int> dist(1, edgeWeight);
  std::uniform_int_distribution<int> isEdge(0, 2);

  // Create data
  std::vector<int> in;
  in.reserve(vertexCount * vertexCount);
  std::vector<int> out(vertexCount);
  for (int i = 0; i < vertexCount * vertexCount; i++) {
    int weight = isEdge(rng) == 0 ? -1 : dist(rng);
    in.push_back(weight);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataOmp->inputs_count.emplace_back(in.size());
  taskDataOmp->inputs_count.emplace_back(start);
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataOmp->outputs_count.emplace_back(out.size());

  // Create Task
  auto testDijkstraOmpuential = std::make_shared<KashinDijkstraOmp::Dijkstra>(taskDataOmp);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testDijkstraOmpuential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_EQ(13716, std::accumulate(out.begin(), out.end(), 0));
}
