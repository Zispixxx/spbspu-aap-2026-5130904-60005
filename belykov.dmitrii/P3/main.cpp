#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstddef>
#include <new>
#include <limits>

namespace belykov {
  const int input_error_code = 1;
  const int calculation_error_code = 2;
  const int mode_static = 1;
  const int mode_dynamic = 2;
  const size_t max_static_size = 10000;
  const long long diagonal_offset = 2;
  int numColLsr(const int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    int max_series_len = 0;
    int best_col_index = 1;
    for (size_t c = 0; c < cols; ++c) {
      int current_series_len = 1;
      int max_in_this_col = 1;
      for (size_t r = 1; r < rows; ++r) {
        if (matrix[r * cols + c] == matrix[(r - 1) * cols + c]) {
          current_series_len++;
        } else {
          if (current_series_len > max_in_this_col) {
            max_in_this_col = current_series_len;
          }
          current_series_len = 1;
        }
      }
      if (current_series_len > max_in_this_col) {
        max_in_this_col = current_series_len;
      }
      if (max_in_this_col > max_series_len) {
        max_series_len = max_in_this_col;
        best_col_index = static_cast< int >(c + 1);
      }
    }
    return best_col_index;
  }
  int maxSumMdg(const int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0 || (rows == 1 && cols == 1)) {
      return 0;
    }
    int max_sum = std::numeric_limits< int >::min();
    bool has_parallel = false;
    const long long r_max = static_cast< long long >(rows);
    const long long c_max = static_cast< long long >(cols);
    for (long long k = 0; k <= r_max + c_max - diagonal_offset; ++k) {
      if (k == c_max - 1) {
        continue;
      }
      int current_diagonal_sum = 0;
      bool contains_elements = false;
      for (size_t r = 0; r < rows; ++r) {
        const long long c = k - static_cast< long long >(r);
        if (c >= 0 && c < c_max) {
          const int val = matrix[r * cols + static_cast< size_t >(c)];
          if ((val > 0 && current_diagonal_sum > std::numeric_limits< int >::max() - val)
              || (val < 0 && current_diagonal_sum < std::numeric_limits< int >::min() - val)) {
            throw std::overflow_error("MAX-SUM-MDG error");
          }
          current_diagonal_sum += val;
          contains_elements = true;
        }
      }
      if (contains_elements) {
        if (!has_parallel || current_diagonal_sum > max_sum) {
          max_sum = current_diagonal_sum;
          has_parallel = true;
        }
      }
    }

    return has_parallel ? max_sum : 0;
  }
  int run(std::istream& input, std::ostream& output, int mode)
  {
    size_t rows = 0;
    size_t cols = 0;
    if (!(input >> rows >> cols)) {
      std::cerr << "Error: Invalid matrix dimensions or empty file.\n";
      return input_error_code;
    }
    if (rows == 0 || cols == 0) {
      output << 0 << "\n" << 0 << "\n";
      return 0;
    }
    const long long total_elements = static_cast< long long >(rows) * cols;
    if (mode == mode_static && total_elements > static_cast< long long >(max_static_size)) {
      std::cerr << "Error: Matrix too large for static buffer.\n";
      return input_error_code;
    }
    int static_buffer[max_static_size] = {0};
    int* matrix = nullptr;
    if (mode == mode_static) {
      matrix = static_buffer;
    } else {
      matrix = new (std::nothrow) int[total_elements];
      if (matrix == nullptr) {
        std::cerr << "Error: Memory allocation failed.\n";
        return input_error_code;
      }
    }
    for (long long i = 0; i < total_elements; ++i) {
      if (!(input >> matrix[i])) {
        std::cerr << "Error: Failed to read matrix elements or invalid data.\n";
        if (mode == mode_dynamic) {
          delete[] matrix;
        }
        return input_error_code;
      }
    }
    int extra = 0;
    if (input >> extra) {
      std::cerr << "Error: Extra data found in file.\n";
      if (mode == mode_dynamic) {
        delete[] matrix;
      }
      return input_error_code;
    }
    const int result_11 = numColLsr(matrix, rows, cols);
    int result_13 = 0;
    bool success_13 = true;
    try {
      result_13 = maxSumMdg(matrix, rows, cols);
    } catch (const std::overflow_error& e) {
      std::cerr << e.what() << "\n";
      success_13 = false;
    }
    if (mode == mode_dynamic) {
      delete[] matrix;
    }
    output << result_11 << "\n";
    if (success_13) {
      output << result_13 << "\n";
    }
    if (!output.good()) {
      std::cerr << "Error: Failed to write to output stream.\n";
      return calculation_error_code;
    }
    return success_13 ? 0 : calculation_error_code;
  }
}
int main(int argc, char** argv)
{
  const int expected_argc = 4;
  const size_t arg_index_mode = 1;
  const size_t arg_index_input = 2;
  const size_t arg_index_output = 3;
  if (argc != expected_argc) {
    std::cerr << "Error: Wrong number of arguments.\n";
    return belykov::input_error_code;
  }
  const char* const mode_str = argv[arg_index_mode];
  if (mode_str == nullptr || mode_str[0] == '\0' || mode_str[1] != '\0' || (mode_str[0] != '1' && mode_str[0] != '2')) {
    std::cerr << "Error: Invalid first parameter (must be 1 or 2).\n";
    return belykov::input_error_code;
  }
  const int mode = mode_str[0] - '0';
  const char* const input_path = argv[arg_index_input];
  const char* const output_path = argv[arg_index_output];
  std::ifstream input{input_path};
  if (!input) {
    std::cerr << "Error: Cannot open input file.\n";
    return belykov::input_error_code;
  }
  std::ofstream output{output_path};
  if (!output) {
    std::cerr << "Error: Cannot open output file.\n";
    return belykov::input_error_code;
  }
  return belykov::run(input, output, mode);
}
