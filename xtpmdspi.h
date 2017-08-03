#pragma once
#include "XTP_API/xtp_quote_api.h"
#include "ringbuffer.h"
#include <set>
#include <string>

using namespace XTP::API;
class XtpMdSpi : public QuoteSpi {
public:
  /* 用到的数据结构
  ///交易所类型
  enum XTP_EXCHANGE_TYPE
  {
    XTP_EXCHANGE_SH = 1,	///<上证
    XTP_EXCHANGE_SZ,		///<深证
      XTP_EXCHANGE_UNKNOWN	///<不存在的交易所类型
  };

  ///指定的合约
  typedef struct XTPSpecificTickerStruct
  {
      ///交易所代码
      XTP_EXCHANGE_TYPE exchange_id;
      ///合约代码（不包含交易所信息）例如"600000"，不带空格，以'\0'结尾
    char	ticker[XTP_TICKER_LEN];
  } XTPST;

  ///返回类型
  typedef struct XTPRspInfoStruct
  {
    ///错误代码
    int32_t	error_id;
    ///错误信息
    char	error_msg[XTP_ERR_MSG_LEN];
  } XTPRI;
  */

  XtpMdSpi(QuoteApi *api, const std::string &dataDir);
  ~XtpMdSpi();
  // 与行情后台通信断开时调用
  virtual void OnDisconnected(int reason);
  // 服务器发生故障时调用，无需处理
  virtual void OnError(XTPRI *error_info);

  // 订阅行情应答
  virtual void OnSubMarketData(XTPST *tiker, XTPRI *error_info, bool is_last);
  // 退订行情应答
  virtual void OnUnSubMarketData(XTPST *tiker, XTPRI *error_info, bool is_last);

  // 行情通知
  virtual void OnMarketData(XTPMD *market_data);

  // 查询所有可交易合约结果
  virtual void OnQueryAllTickers(XTPQSI *tiker_info, XTPRI *error_info,
                                 bool is_last);

private:
  QuoteApi *api_;
  std::set<std::string> subscriptions_;
  std::string market_data_dir_;
  RingBuffer ring_buffer_;
};
