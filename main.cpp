#include "xtpmdspi.h"
#include <cstdio>
#include <chrono>
#include <thread>
#include <cstring>

using namespace XTP::API;

int main(int argc, const char* argv[]) {
  if (argc != 2 || (strcmp(argv[1], "SH") && strcmp(argv[1], "SZ"))) {
    printf("Usage: %s SH\n\tor: %s SZ\n", argv[0], argv[0]);
    exit(-1);
  }

  // client_id 为 0 时创建的是空指针
  // 为 SH 和 SZ 安排不同的 client_id，允许同时开启
  uint8_t client_id = 0;
  if (strcmp(argv[1], "SH") == 0) {
    client_id = 22;
  } else if (strcmp(argv[1], "SZ") == 0) {
    client_id = 23;
  } else {
    printf("Unknown Exchange: %s\n", argv[1]);
    exit(-1);
  }
  QuoteApi *api = QuoteApi::CreateQuoteApi(client_id, "Data/");
  XtpMdSpi spi(api, "Data/MarketData/");
  api->RegisterSpi(&spi);
  const char* ip = "120.27.164.138";
  int port = 6002;
  const char* user = "username";
  const char* password = "password";

  /*
  enum XTP_PROTOCOL_TYPE
  {
  	XTP_PROTOCOL_TCP = 1,	///<采用TCP方式传输
  	XTP_PROTOCOL_UDP		///<采用UDP方式传输（目前暂未支持）
  };
  enum XTP_EXCHANGE_TYPE
  {
  	XTP_EXCHANGE_SH = 1,	///<上证
  	XTP_EXCHANGE_SZ,		///<深证
      XTP_EXCHANGE_UNKNOWN	///<不存在的交易所类型
  };
  */
  int login_result = api->Login(ip, port, user, password, XTP_PROTOCOL_TCP);
  if (login_result == 0) {
    printf("Login succeed...\n");
    if (strcmp(argv[1], "SH") == 0) {
      api->QueryAllTickers(XTP_EXCHANGE_SH);
    } else if (strcmp(argv[1],"SZ") == 0) {
      api->QueryAllTickers(XTP_EXCHANGE_SZ);
    } else {
      printf("Unknown Exchange: %s\n", argv[1]);
      exit(-1);
    }
  } else {
    printf("Login failed...\n");
  }
  printf("Main thread: %08x\n", std::this_thread::get_id());
  std::this_thread::sleep_for(std::chrono::hours(24));
  return 0;
}
