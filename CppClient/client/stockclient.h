#pragma once

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <google/protobuf/util/time_util.h>

#include "stock_service.grpc.pb.h"
#include "threadmanager.h"

using namespace std;
using namespace Stock;

class StockClient {

private:
	std::shared_ptr<grpc::Channel> m_channel;
	std::unique_ptr<StockService::Stub> m_stub;
	string m_host;
	string m_port;
	std::vector<std::vector<std::string>> m_volumes;

public:
	
	// Constructors
	StockClient() = default;
	StockClient(std::shared_ptr<grpc::Channel> m_channel) : m_stub(StockService::NewStub(m_channel)) { };

	// Accessors (getters)
	const string& getHost() const;
	const string& getPort() const;

	// Modifiers (setters)
	void setHost(const string host);
	void setPort(const string port);

	// Functions
	void GetValue(const StockByProperty* val_ptr);
	void SimpleReadCSV();
	void StartStreaming(SimpleThreadMgr* threadMgr, vector<string> tickers);

	// Functions GRPC implementation
	void CallGetStockValueAtTime(string ticker, string timestamp, Stock::StockProperty required_property);
	void CallGetStockValuesAtTime(string ticker, string timestamp);
	void CallGetStockValueAtAllTimes(string ticker, Stock::StockProperty required_property);
	void CallSendStockVolume(std::atomic<bool>* kill_flag, string company_name);

};
