#include "threadmanager.h"
#include <iostream>

// Functions

void SimpleThreadMgr::SetThreadConfig(const std::string& tname) {
	// For now, only config is to assign the kill flag
	m_killmap[tname] = false;
	m_tmap[tname] = 0; // just initializing thread placeholder
}

void SimpleThreadMgr::RegisterThread(std::thread& t, const std::string& tname) {
	m_tmap[tname] = &t;
	
};

void SimpleThreadMgr::StopThread(const std::string& tname) {
	ThreadMap::const_iterator it = m_tmap.find(tname);
	if (it != m_tmap.end()) {
		m_killmap[tname] = true;
	};
};
void SimpleThreadMgr::ShowThreads() {

	std::unordered_map<std::string, int>::iterator it;

	for (auto it = m_tmap.begin(); it != m_tmap.end(); it++)
	{
		std::cout << "Streaming " << it->first << " data" << std::endl;
	}

};

void SimpleThreadMgr::KillThreads() {
	
	std::unordered_map<std::string, int>::iterator it;

	for (auto it = m_killmap.begin(); it != m_killmap.end(); it++)
	{
		std::cout << "killing " << it->first << std::endl;
		it->second = true;
		int abc = 1;
	}
};