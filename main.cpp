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
  QuoteApi *api = QuoteApi::CreateQuoteApi(22, "Data/");
  XtpMdSpi spi(api, "Data/MarketData/");
  api->RegisterSpi(&spi);
  const char* ip = "120.27.164.138";
  int port = 6002;
  const char* user = "15030398";
  const char* password = "I6k8ysSR";

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
      exit(0);
    }
  } else {
    printf("Login failed...\n");
  }
  printf("Main thread: %lu\n", std::this_thread::get_id());
  std::this_thread::sleep_for(std::chrono::hours(24));
  return 0;
}
