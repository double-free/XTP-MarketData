#include "../../XTP_API/xquote_api_struct.h"
#include "../../timing.hpp"
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>

#define MIN(A, B) ((A) < (B) ? (A) : (B))
struct Frame {
  common::tick_header header;
  XTPMD market_data;
};

int readBinaryToBuff(char **buf, const std::string &binary_path) {
  const int MAX_SIZE = 10000 * sizeof(Frame);

  std::ifstream inFile(binary_path, std::ifstream::binary);
  inFile.seekg(0, inFile.end);  // 设置指向文件末尾
  int fileLen = inFile.tellg(); // 返回位置，即长度
  inFile.seekg(0, inFile.beg);  // 设置指文件头以便开始读取

  // 作校验用，如果文件太大则只校验前 10000 个
  int ret = MIN(MAX_SIZE, fileLen);
  *buf = new char[ret];
  inFile.read(*buf, ret);
  return ret;
}

void printBinaryFile(const std::string &binary_path) {
  char *buf;
  int fileLen = readBinaryToBuff(&buf, binary_path);

  int nCount = fileLen / sizeof(Frame);
  Frame *dataArr = (Frame *)buf;
  for (int i = 0; i < nCount; i++) {
    // common::tick_header h = dataArr[i].header;
    // XTPMD md = dataArr[i].market_data;
    std::cout << "Time stamp header: " << dataArr[i].header.timestamp;
    std::cout << '\t' << "交易所:" << dataArr[i].market_data.exchange_id << "\n"
              << '\t' << "合约代码:" << dataArr[i].market_data.ticker << "\n"
              << '\t' << "最新价:" << dataArr[i].market_data.last_price << "\n"
              << '\t' << "昨收盘:" << dataArr[i].market_data.pre_close_price
              << "\n"
              << '\t' << "今开盘:" << dataArr[i].market_data.open_price << "\n"
              << '\t' << "最高价:" << dataArr[i].market_data.high_price << "\n"
              << '\t' << "最低价:" << dataArr[i].market_data.low_price << "\n"
              << '\t' << "今收盘:" << dataArr[i].market_data.close_price
              << std::endl;
  }
  printf("fileLen = %d, elemLen = %d, total %d elements\n", fileLen,
         sizeof(Frame), nCount);
  delete[] buf;
}

void binary2csv(const char *binary_path, const char *csv_path) {
  printf("Not Implemented yet...\n");
}

int main(int argc, const char *argv[]) {
  if (argc == 1 || argc > 3) {
    printf("Usage: %s [<binary file>] (print)\n", argv[0]);
    printf("       %s [<binary file>] [<csv file>] (to csv)\n", argv[0]);
    return -1;
  }
  if (argc == 2) {
    const char *fileName = argv[1];
    printBinaryFile(fileName);
    return 0;
  }
  if (argc == 3) {
    const char *binary_path = argv[1];
    const char *csv_path = argv[2];
    binary2csv(binary_path, csv_path);
    return 0;
  }
}
