#include <cstddef>
#include <cctype>
#include <cstring>
#include <iostream>

namespace belykov {

  void dgtSnd(const char* input, const char* second, char* output) noexcept
  {
    std::strcpy(output, input);
    size_t i = std::strlen(output);
    size_t j = 0;
    while (second[j] != '\0') {
      if (std::isdigit(static_cast< unsigned char >(second[j]))) {
        output[i] = second[j];
        i++;
      }
      j++;
    }
    output[i] = '\0';
  }

  void uppLow(const char* input, char* output) noexcept
  {
    size_t i = 0;
    while (input[i] != '\0') {
      if (std::isupper(static_cast< unsigned char >(input[i]))) {
        output[i] = static_cast< char >(std::tolower(static_cast< unsigned char >(input[i])));
      } else {
        output[i] = input[i];
      }
      i++;
    }
    output[i] = '\0';
  }
}

int main()
{
  const size_t max_len = 10000;
  const size_t max_len_output = 20000;
  const char* const second = "g1h2k";

  char* const input = new char[max_len];
  char* const output = new char[max_len_output];

  std::cin.getline(input, max_len, '\n');

  if (!std::cin) {
    std::cerr << "error: string too long\n";
    delete[] input;
    delete[] output;
    return 1;
  }

  belykov::dgtSnd(input, second, output);
  std::cout << output << '\n';

  belykov::uppLow(input, output);
  std::cout << output << '\n';

  delete[] input;
  delete[] output;

  return 0;
}
