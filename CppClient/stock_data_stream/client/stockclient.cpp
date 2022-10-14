#pragma once

#include "stockclient.h"
#include "fmt/format.h"

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
void StockClient::GetValueAndVolume(const StockValueAndVolume* valPtr) {
	auto val = 0.0;
	switch (valPtr->info_case()) {
	case StockValueAndVolume::InfoCase::kValueOpen:
		if (valPtr->has_value_open()) {
			val = valPtr->value_open().value();
		}
		else {
			std::string msg = fmt::format("Error: Value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockValueAndVolume::InfoCase::kValueHigh:
		if (valPtr->has_value_high()) {
			val = valPtr->value_high().value();
		}
		else {
			std::string msg = fmt::format("Error: Value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockValueAndVolume::InfoCase::kValueLow:
		if (valPtr->has_value_low()) {
			val = valPtr->value_low().value();
		}
		else {
			std::string msg = fmt::format("Error: Value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockValueAndVolume::InfoCase::kValueClose:
		if (valPtr->has_value_close()) {
			val = valPtr->value_close().value();
		}
		else {
			std::string msg = fmt::format("Error: Value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockValueAndVolume::InfoCase::kValueAdjClose:
		if (valPtr->has_value_adj_close()) {
			val = valPtr->value_adj_close().value();
		}
		else {
			std::string msg = fmt::format("Error: Value not found.");
			std::cout << msg << std::endl;
		}
		break;
	case StockValueAndVolume::InfoCase::kVolume:
		if (valPtr->has_volume()) {
			val = valPtr->volume().volume();
		}
		else {
			std::string msg = fmt::format("Error: Value not found.");
			std::cout << msg << std::endl;
		}
		break;
	}
}


// Functions GRPC Implementation
void StockClient::CallGetStockValueAtTime() {

	grpc::ClientContext context;

	std::string ticker = "GOLD";
	std::string timestamp = "2020-03-10";

	// Make request
	auto request = StockAtTime();
	auto stkDataPtr = request.mutable_stock_data();
	stkDataPtr->set_info(OPEN);
	stkDataPtr->mutable_company_name()->set_name(ticker);
	request.set_timestamp(timestamp);

	// Make response
	StockValue response;
	grpc::Status status = m_stub->GetStockValueAtTime(&context, request, &response);
	if (status.ok()) {
		auto val = response.value();
		std::string msg = fmt::format("The value for ticker '{}' at {} is {}", ticker, timestamp, val);
		std::cout << msg << std::endl;
	}
	else {
		std::string msg = fmt::format("Error {}: {}", to_string(status.error_code()), to_string(status.error_code()));
		std::cout << msg << std::endl;
	}

};

void StockClient::CallGetStockValueAndVolumeAtTime() {
	
	grpc::ClientContext context;

	std::string ticker = "XOM";
	std::string timestamp = "2020-01-21";

	// Make request
	auto request = StockAtTime();
	auto stkDataPtr = request.mutable_stock_data();
	stkDataPtr->set_info(OPEN);
	stkDataPtr->mutable_company_name()->set_name(ticker);
	request.set_timestamp(timestamp);

	// Make response
	auto response = StockValueAndVolume();
	grpc::Status status = m_stub->GetStockValueAndVolumeAtTime(&context, request, &response);
	if (status.ok()) {
		GetValueAndVolume(&response);
	}
	else {
		std::string msg = fmt::format("Error {}: {}", to_string(status.error_code()), to_string(status.error_code()));
		std::cout << msg << std::endl;
	}
};
void StockClient::CallGetStockValueAtAllTimes() {

	grpc::ClientContext context;
	auto request = StockData();


};
void StockClient::CallSendStockVolume() {

	grpc::ClientContext context;
	auto request = StockValuesAndVolumes();

};