#ifndef APPLICATION_H
#define APPLICATION_H

#include "Info.h"
#include "StockMarketModel.h"
#include "StockMarketView.h"
#include <atomic>
#include <vector>

#include "StockMarketController.h"

class CApplication 
{
	std::atomic<bool> m_bTutorialStarted;

	CSettings<CPortfolioStockInfo> m_PortfolioInfos;
	CSettings<CStockInfo> m_StockInfos;
	CSettings<CTutorialInfo> m_TutorialInfos;

	CApplication() { }

	CApplication(const CApplication &) = delete;
	CApplication &operator=(const CApplication &) = delete;

public:

	virtual ~CApplication() = default;

	static CApplication &GetInstance();

	void Init();
	void AttachDependencies(CStockMarketModel &m, CStockMarketView &v, CStockMarketController &c);
	void Run();

	const std::vector<CPortfolioStockInfo> &GetPortfolioInfos() const;
	const std::vector<CStockInfo> &GetStockInfos() const;
	const std::vector<CTutorialInfo> &GetTutorialInfos() const;

	void SetTutorialPaused(bool bTutorialPaused);
	bool IsTutorialPaused() const;
};
#endif // APPLICATION_H
