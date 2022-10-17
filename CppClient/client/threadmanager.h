#pragma once
#include <string>
#include <unordered_map>
#include <thread>
#include <atomic>

class SimpleThreadMgr {

public:
	// Variables
	typedef std::unordered_map<std::string, std::atomic<bool>> KillThreadMap;
	KillThreadMap m_killmap;

	// Constructor
	SimpleThreadMgr() = default;

	// Functions
	void SetThreadConfig(const std::string& tname);
	void RegisterThread(std::thread& t, const std::string& tname);
	void StopThread(const std::string& tname);
	void ShowThreads();
	void KillThreads();

private:
	typedef std::unordered_map<std::string, std::thread*> ThreadMap;
	ThreadMap m_tmap;
};