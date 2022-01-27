/**
 * @file engToBinary.cpp
 * @author Jack (chengjunjie.jack@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-27
 *
 * @copyright Copyright (c) 2022
 */

#include <cstring>
#include <iostream>
#include <string>

std::string string_to_binary(const std::string& str) {
  std::string result;
  for (auto c : str) {
    // 8位字符
    unsigned char mask = 0b10000000;
    while (mask) {
      result.append(1, (c & mask) ? '1' : '0');
      mask >>= 1;
    }
  }
  return result;
}

std::string binary_to_string(const std::string& str) {
  std::string result;
  int str_len = str.length();
  for (int i = 0; i < str_len; i += 8) {
    char c = 0b0;
    for (int j = 0; j < 8; ++j) {
      if (str[i + j] == '1') {
        c |= 1 << (7 - j);
      }
    }
    result.append(1, c);
  }
  return result;
}

void print_help() {
  std::cout << "Usage: ascii_convert [type] [string]\n\
type:\t\n\
\t-e \t Encoding string to ascii code.\n\
\t-d \t Decoding ascii code to string."
            << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    print_help();
    return 1;
  }
  if (strlen(argv[1]) >= 2 && argv[1][0] == '-') {
    std::string result;
    switch (argv[1][1]) {
      case 'e':
        result = string_to_binary(argv[2]);
        break;
      case 'd':
        result = binary_to_string(argv[2]);
        break;
      default:
        break;
    }
    std::cout << result << std::endl;
    return 0;
  }
  print_help();
  return 1;
}
