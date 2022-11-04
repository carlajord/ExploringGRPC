#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <google/protobuf/util/time_util.h>

#include "stock_service.grpc.pb.h"
#include "stockclient.h"
#include "threadmanager.h"


int main(int argc, char **argv) {

    std::string host = "localhost";
    std::string port = "54321";

    auto channel = grpc::CreateChannel(host + ":" + port,
        grpc::InsecureChannelCredentials());

    auto theClient = new StockClient(channel);
    
    string ticker = "GOLD";
    string timestamp = "2022-01-13";
    auto required_property = CLOSE;
    theClient->CallGetStockValueAtTime(ticker, timestamp, required_property);

    ticker = "XOM";
    timestamp = "2022-03-14";
    theClient->CallGetStockValuesAtTime(ticker, timestamp);

    ticker = "EQR";
    required_property = OPEN;
    theClient->CallGetStockValueAtAllTimes(ticker, required_property);

    vector<string> tickers = { "GOLD" , "XOM", "EQR", "CVX", "TTE", "COP"};
    timestamp = "2022-02-09";
    auto threadMgr = SimpleThreadMgr();
    theClient->StreamStockVolume(&threadMgr, tickers);
}