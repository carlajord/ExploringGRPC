#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <google/protobuf/util/time_util.h>

#include "stock_service.grpc.pb.h"
#include "stockclient.h"
#include "threadmanager.h"


int main(int argc, char **argv) {

    std::string host = "localhost";
    std::string port = "54321";

    auto theClient = new StockClient();
    
    string ticker = "GOLD";
    string timestamp = "2020-03-10";
    auto required_property = CLOSE;
    theClient->CallGetStockValueAtTime(ticker, timestamp, required_property);

    ticker = "XOM";
    timestamp = "2020-01-21";
    theClient->CallGetStockValuesAtTime(ticker, timestamp);

    ticker = "EQR";
    required_property = OPEN;
    theClient->CallGetStockValueAtAllTimes(ticker, required_property);


}