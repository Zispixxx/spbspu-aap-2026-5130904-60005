#include <iostream>

namespace belykov
{
  const int exit_success_code = 0;
  const int exit_invalid_input = 1;
  const int exit_empty_sequence = 2;
  void monDec(int current, int &prev, int &cur_len, int &max_len, bool &first)
  {
    if (first)
    {
      cur_len = 1;
      max_len = 1;
      first = false;
    }
    else
    {
      if (current <= prev)
      {
        cur_len++;
      }
      else
      {
        if (cur_len > max_len)
        {
          max_len = cur_len;
        }
        cur_len = 1;
      }
    }
    prev = current;
  }
  void aftMax(int current, int &max_val, int &count_after, bool &first)
  {
    if (first)
    {
      max_val = current;
      count_after = 0;
      first = false;
    }
    else
    {
      if (current > max_val)
      {
        max_val = current;
        count_after = 0;
      }
      else
      {
        count_after++;
      }
    }
  }
  int run()
  {
    int current = 0;
    int count = 0;
    int prev = 0;
    int cur_dec_len = 0;
    int max_dec_len = 0;
    bool first_dec = true;
    int max_val = 0;
    int count_after_max = 0;
    bool first_aft = true;
    while (std::cin >> current && current != 0)
    {
      count++;
      monDec(current, prev, cur_dec_len, max_dec_len, first_dec);
      aftMax(current, max_val, count_after_max, first_aft);
    }
    if (!std::cin)
    {
      std::cerr << "Error: Invalid input data.\n";
      return exit_invalid_input;
    }
    if (count == 0)
    {
      std::cout << 0 << "\n";
      std::cerr << "Error: Cannot calculate AFT-MAX.\n";
      return exit_empty_sequence;
    }
    if (cur_dec_len > max_dec_len)
    {
      max_dec_len = cur_dec_len;
    }
    std::cout << max_dec_len << "\n";
    std::cout << count_after_max << "\n";
    return exit_success_code;
  }
}
int main()
{
  return belykov::run();
}
