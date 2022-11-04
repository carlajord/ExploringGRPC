#pragma once

#include "stockclient.h"
#include "fmt/format.h"
#include <algorithm>
#include <fstream>
#include <sstream>

// Globals



// Accessors (getters)
const string& StockClient::getHost() const
{
	return this->m_host;
}
const string& StockClient::getPort() const
{
	return this->m_port;
}

// Modifiers (setters)
void StockClient::setHost(const string host)
{
	this->m_host = host;
}
void StockClient::setPort(const string port)
{
	this->m_port = port;
}


// Functions
void StockClient::GetValue(const StockByProperty* valPtr) const {

	auto val = 0.0;
	switch (valPtr->info_case()) {
	case StockByProperty::InfoCase::kValueOpen:
		if (valPtr->has_value_open()) {
			val = valPtr->value_open().value();
			string msg = fmt::format("  The OPEN value is {}.", to_string(val));
			std::cout << msg << std::endl;
		}
		else {
			string msg = fmt::format("  Error: OPEN value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockByProperty::InfoCase::kValueHigh:
		if (valPtr->has_value_high()) {
			val = valPtr->value_high().value();
			string msg = fmt::format("  The HIGH value is {}.", to_string(val));
			std::cout << msg << std::endl;
		}
		else {
			string msg = fmt::format("  Error: HIGH value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockByProperty::InfoCase::kValueLow:
		if (valPtr->has_value_low()) {
			val = valPtr->value_low().value();
			string msg = fmt::format("  The LOW value is {}.", to_string(val));
			std::cout << msg << std::endl;
		}
		else {
			string msg = fmt::format("  Error: LOW value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockByProperty::InfoCase::kValueClose:
		if (valPtr->has_value_close()) {
			val = valPtr->value_close().value();
			string msg = fmt::format("  The CLOSE value is {}.", to_string(val));
			std::cout << msg << std::endl;
		}
		else {
			string msg = fmt::format("  Error: CLOSE value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockByProperty::InfoCase::kValueAdjClose:
		if (valPtr->has_value_adj_close()) {
			val = valPtr->value_adj_close().value();
			string msg = fmt::format("  The ADJ_CLOSE value is {}.", to_string(val));
			std::cout << msg << std::endl;
		}
		else {
			string msg = fmt::format("  Error: ADJ_CLOSE value not found.");
			std::cout << msg << std::endl;
		}
		break;
	}
}


// Functions GRPC Implementation
void StockClient::CallGetStockValueAtTime(string ticker, string timestamp, Stock::StockProperty required_property) {

	/* Given a stock ticker, a time stamp and the required property type,
	this function returns the price of the stock for the required property. */
	
	grpc::ClientContext context;

	// Make request
	auto request = StockValueAtTime();
	request.mutable_stock_data()->set_property(required_property);
	request.mutable_stock_data()->mutable_company_name()->set_name(ticker);
	request.set_timestamp(timestamp);

	// Make response
	StockValue response;
	grpc::Status status = m_stub->GetStockValueAtTime(&context, request, &response);
	if (status.ok()) {
		auto val = response.value();
		string msg = fmt::format("The value for ticker '{}' at {} is {}", ticker, timestamp, val);
		std::cout << msg << std::endl;
	}
	else {
		string msg = fmt::format("Error {}: {}", to_string(status.error_code()), status.error_message());
		std::cout << msg << std::endl;
	}
	
};

void StockClient::CallGetStockValuesAtTime(string ticker, string timestamp) {
	
	/* Given a stock ticker and a time stamp, this function returns the 
	price of the stock for all properties. */
	
	grpc::ClientContext context;

	// Make request
	auto request = StockValuesAtTime();
	request.mutable_company_name()->set_name(ticker);
	request.set_timestamp(timestamp);

	// Make response
	auto response = StockValues();
	grpc::Status status = m_stub->GetStockValuesAtTime(&context, request, &response);
	if (status.ok()) {

		string msg = fmt::format("Stock values for company: {}", ticker);
		std::cout << msg << std::endl;

		for (const StockByProperty& r : response.value_by_property()) {
			GetValue(&r);
		}
	}
	else {
		string msg = fmt::format("Error {}: {}", to_string(status.error_code()), status.error_message());
		std::cout << msg << std::endl;
	}
	
};

void StockClient::CallGetStockValueAtAllTimes(string ticker, Stock::StockProperty required_property) {

	/* Given a stock ticker and the required property type,
	this function returns the price of the stock for the required property
	at all times from the data source. It prints to console the maximum stock price. */
	
	grpc::ClientContext context;

	// Make request
	auto request = StockData();
	request.mutable_company_name()->set_name(ticker);
	request.set_property(required_property);

	// Make response
	auto response = StockValue();
	double max_value = 0.0;

	unique_ptr<grpc::ClientReader<StockValue>>
		reader(m_stub->GetStockValueAtAllTimes(&context, request));
	while (reader->Read(&response)) { max_value = max(max_value, response.value()); }

	// Get name of the required property
	const google::protobuf::EnumDescriptor* descriptor = StockProperty_descriptor();
	StockProperty property_name = StockProperty(required_property);
	string name = descriptor->FindValueByNumber(required_property)->name();

	string msg = fmt::format("The maximum {} value of stock {} is USD {}.",
		name, ticker, to_string(max_value));
	std::cout << msg << std::endl;
	
};
void StockClient::CallSendStockVolume(std::atomic<bool>* kill_flag, string ticker) {

	/* Sends to the server the volume of a stock
	at all times from the data source. */
	
	grpc::ClientContext context;
	auto request = StockVolumes();

	auto response = Empty();

	std::unique_ptr<grpc::ClientWriter<StockVolumes> > writer(
	m_stub->SendStockVolume(&context, &response));

	for (size_t i = 0; i < m_volumes.size(); i++) {

		if (*kill_flag) {
			std::cout << "Kill request received" << std::endl;
			break;
		}

		if (m_volumes[i][1] == ticker) {
			request.mutable_company_name()->set_name(ticker);
			request.set_timestamp(m_volumes[i][0]);
			request.mutable_stock_volume()->set_volume(stod(m_volumes[i][2]));

			if (!writer->Write(request)) {
				// Broken stream.
				break;
			}

		}

	}

	writer->WritesDone();
	grpc::Status status = writer->Finish();

};

void StockClient::SimpleReadCSV() {

	string datadir = DATA_DIR;
	string filepath = datadir + "MarketVolume.csv";

	ifstream infile{ filepath.c_str() };

	string line;
	
	while (std::getline(infile, line))
	{
		std::string line_value;
		std::vector<std::string> line_values;
		std::istringstream s(line);
		while (std::getline(s, line_value, ','))
		{
			line_values.push_back(line_value);
		}
		m_volumes.emplace_back(line_values);
	}
}

void StockClient::StreamStockVolume(SimpleThreadMgr* threadMgr, vector<string> tickers) {
	
	/* Uploads and sends volume of required stocks to the server.
	Each stock ticker volume is sent in a different thread. */

	vector<thread> threads;
	SimpleReadCSV();

	for (size_t i = 0; i < tickers.size(); i++) {

		string tName = tickers[i];
		threadMgr->SetThreadConfig(tName);
		threads.push_back(thread(&StockClient::CallSendStockVolume, this,
			&(threadMgr->m_killmap[tName]), tName));
		threadMgr->RegisterThread(threads.back(), tName);
	}

	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
	
}
