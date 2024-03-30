// Copyright 2024 Borovkov Sergey
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "seq/borovkov_s_cannon/include/ops_seq.hpp"

std::vector<double> cannonMatrixMultiplication(const std::vector<double>& A, const std::vector<double>& B, int n,
                                               int m) {
  int blockSize = std::min(n, m);

  std::vector<double> C(n * m, 0.0);

  if (n == 0 || m == 0) {
    return std::vector<double>();
  }

  for (int i = 0; i < n; i += blockSize) {
    for (int j = 0; j < m; j += blockSize) {
      for (int k = 0; k < m; k += blockSize) {
        int i_end = std::min(i + blockSize, n);
        int j_end = std::min(j + blockSize, m);
        int k_end = std::min(k + blockSize, m);

        for (int ii = i; ii < i_end; ++ii) {
          for (int kk = k; kk < k_end; ++kk) {
            double A_ik = A[ii * m + kk];
            for (int jj = j; jj < j_end; ++jj) {
              C[ii * m + jj] += A_ik * B[kk * m + jj];
            }
          }
        }
      }
    }
  }

  return C;
}

std::vector<double> multiplyMatrix(const std::vector<double>& A, const std::vector<double>& B, int rows_A, int col_B) {
  int col_A = rows_A;
  std::vector<double> C(rows_A * col_B, 0.0);

  if (rows_A == 0 || col_B == 0) {
    return std::vector<double>();
  }

  for (int i = 0; i < rows_A; ++i) {
    for (int j = 0; j < col_B; ++j) {
      for (int k = 0; k < col_A; ++k) {
        C[i * col_B + j] += A[i * col_A + k] * B[k * col_B + j];
      }
    }
  }
  return C;
}

bool TestTaskSequentialBorovkovCannon::pre_processing() {
  internal_order_test();
  // Init value for input and output

  A = std::vector<double>(taskData->inputs_count[0]);
  B = std::vector<double>(taskData->inputs_count[1]);

  n = *reinterpret_cast<int*>(taskData->inputs[2]);
  m = *reinterpret_cast<int*>(taskData->inputs[3]);

  auto* tmp_ptr_A = reinterpret_cast<double*>(taskData->inputs[0]);
  for (size_t i = 0; i < taskData->inputs_count[0]; i++) {
    A[i] = tmp_ptr_A[i];
  }

  auto* tmp_ptr_B = reinterpret_cast<double*>(taskData->inputs[1]);
  for (size_t i = 0; i < taskData->inputs_count[1]; i++) {
    B[i] = tmp_ptr_B[i];
  }
  return true;
}

bool TestTaskSequentialBorovkovCannon::validation() {
  internal_order_test();
  // Check count elements of output
  return taskData->inputs_count[0] == taskData->inputs_count[1] &&
         taskData->inputs_count[0] == taskData->outputs_count[0] &&
         taskData->inputs_count[1] == taskData->outputs_count[0];
}

bool TestTaskSequentialBorovkovCannon::run() {
  internal_order_test();
  result = cannonMatrixMultiplication(A, B, n, m);
  return true;
}

bool TestTaskSequentialBorovkovCannon::post_processing() {
  internal_order_test();
  std::copy(result.begin(), result.end(), reinterpret_cast<double*>(taskData->outputs[0]));
  return true;
}
