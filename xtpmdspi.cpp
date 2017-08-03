#include "xtpmdspi.h"
#include "timing.hpp"
#include <cstdio>
#include <utility>

#include <sys/stat.h>
#include <unistd.h>

XtpMdSpi::XtpMdSpi(QuoteApi *api, const std::string &dataDir) : api_(api) {
  if (access(dataDir.c_str(), F_OK && W_OK) < 0) {
    mkdir(dataDir.c_str(), 0751);
  }
  market_data_dir_ = std::move(dataDir);
  const size_t BUFFER_SIZE = 1 << 22;
  ring_buffer_.setSize(BUFFER_SIZE);
  printf("QuoteSpi created...\n");
}

XtpMdSpi::~XtpMdSpi() { printf("QuoteSpi deleted...\n"); }

void XtpMdSpi::OnDisconnected(int reason) { printf("Disconnected...\n"); }

void XtpMdSpi::OnError(XTPRI *error_info) {
  printf("Server Error: %s (errno: %d)\n", error_info->error_msg,
         error_info->error_id);
}

void XtpMdSpi::OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) {
  if (error_info == nullptr || error_info->error_id == 0) {
    if (is_last) {
      printf("Subscribe succeed\n");
    }
  } else {
    printf("Subscribe Error: %s (errno: %d)\n", error_info->error_msg,
           error_info->error_id);
  }
}

void XtpMdSpi::OnUnSubMarketData(XTPST *ticker, XTPRI *error_info,
                                 bool is_last) {
  if (error_info == nullptr || error_info->error_id == 0) {
    if (is_last) {
      printf("Unsubscribe succeed\n");
    }
  } else {
    printf("Unsubscribe Error: %s (errno: %d)\n", error_info->error_msg,
           error_info->error_id);
  }
}

void XtpMdSpi::OnMarketData(XTPMD *market_data) {
  // size of XTPMD: 736 bytes
  // printf("Put Data in thread: %x\n", std::this_thread::get_id());
  common::tick_header h;
  h.init();
  h.body_size = 736;
  // printf("ticker header size = %d\n", sizeof(h)); // 24
  ring_buffer_.putData((char *)&h, 24);
  ring_buffer_.putData((char *)market_data, h.body_size);
}

void XtpMdSpi::OnQueryAllTickers(XTPQSI *ticker_info, XTPRI *error_info,
                                 bool is_last) {
  /*
  ///股票行情静态信息
  typedef struct XTPQuoteStaticInfo {
      ///交易所代码
      XTP_EXCHANGE_TYPE exchange_id;
      ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
      char    ticker[XTP_TICKER_LEN];
      /// 合约名称
      char    ticker_name[XTP_TICKER_NAME_LEN];
      /// 合约类型
    XTP_TICKER_TYPE ticker_type;
      ///昨收盘
      double  pre_close_price;
      ///涨停板价
      double  upper_limit_price;
      ///跌停板价
      double  lower_limit_price;
    ///最小变动价位
    double  price_tick;
      /// 合约最小交易量(买)
      int32_t  buy_qty_unit;
      /// 合约最小交易量(卖)
    int32_t sell_qty_unit;
  } XTPQSI;
  */
  if (error_info == nullptr || error_info->error_id == 0) {
    std::string ticker_str(ticker_info->ticker);
    subscriptions_.insert(std::move(ticker_str));
    if (is_last) {
      int count;
      char **tickers;
      count = subscriptions_.size();
      tickers = new char *[count];
      int idx = 0;
      for (auto &str : subscriptions_) {
        tickers[idx++] =
            (char *)str.c_str(); // c_str() 返回底层 char 数组，不需要 copy
      }
      XTP_EXCHANGE_TYPE exchange_id = ticker_info->exchange_id;
      std::string filename;
      if (exchange_id == XTP_EXCHANGE_SZ) {
        filename = "SZ";
      } else if (exchange_id == XTP_EXCHANGE_SH) {
        filename = "SH";
      } else {
        filename = "Unknown";
      }
      ring_buffer_.setSavePath(market_data_dir_ + filename);
      api_->SubscribeMarketData(tickers, count, exchange_id);

      // clear tickers
      delete[] tickers;
    }
  } else {
    printf("Query Error: %s (errno: %d)\n", error_info->error_msg,
           error_info->error_id);
  }
}
