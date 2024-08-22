#ifndef LOGIC_UPDATER_H
#define LOGIC_UPDATER_H

#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include "Observer.h"

class CStockMarketModel;

class CLogicUpdater : public CSender
{
	CStockMarketModel &m_StockMarketModel;
	std::atomic<bool> m_bRunningUpdateLogicThread;
	std::thread m_UpdateLogicThread;

public:

	explicit CLogicUpdater(CStockMarketModel &StockMarketModel);
	~CLogicUpdater();

	void Run();
	void Stop();
};
#endif // LOGIC_UPDATER_H