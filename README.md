# XTP-MarketData
下载相关的 api 可以到[中泰官网](https://xtp.zts.com.cn/)。

# 需求
接收行情的回调函数需要以最快速度返回。
```c++
			///行情通知
			///@param market_data 行情数据，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnMarketData(XTPMD *market_data) {}
```
# 主要思路
经查看指针地址可知， market_data 分配在栈上。因此不能直接在这里进行异步操作，因为不保证这个地址此后还能有效。<br/>
所以避免不开两个问题，一个是内存拷贝，一个是写文件。<br/>
为达到最快的存储速度，采用以下两个优化：<br/>
1. **使用 Ringbuffer 存储**<br/>
避免每次接收到行情时进行内存分配。
参考内核的 kfifo 编写。
2. **异步写文件**<br/>
io 操作是最费时的。必须异步进行。
