#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <google/protobuf/util/time_util.h>

#include "stock_service.grpc.pb.h"
#include "stockclient.h"
#include "threadmanager.h"


int main(int argc, char **argv) {

    std::string host = "localhost";
    std::string port = "54321";

    auto channel = grpc::CreateChannel(host+":"+port, grpc::InsecureChannelCredentials());
    StockClient theClient = StockClient(channel);
}