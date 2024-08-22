#include "Application.h"
#include <cstdlib>
#include <iostream>

CApplication &CApplication::GetInstance()
{
	static CApplication m_Instance;
	return m_Instance;
}

void CApplication::Init()
{
	m_PortfolioInfos.LoadFromJSON("Data/Portolio.json");
	m_StockInfos.LoadFromJSON("Data/Stocks.json");
	m_TutorialInfos.LoadFromJSON("Data/Tutorial.json");
}

void::CApplication::AttachDependencies(CStockMarketModel &m, CStockMarketView &v, CStockMarketController &c)
{
	m.GetLogicUpdater().Attach(&v);
	m.GetLogicUpdater().Attach(&m.GetPortfolio());

	m.Attach(&v);
}

void CApplication::Run()
{
	system("cls");

	CStockMarketModel Model;
	CStockMarketView View(Model);
	CStockMarketController Controller(Model);

	AttachDependencies(Model, View, Controller);

	View.Run();
	Controller.Run();

	system("cls");
	std::cout << "GOD BYY!";
}

const std::vector<CPortfolioStockInfo> &CApplication::GetPortfolioInfos() const
{
	return	m_PortfolioInfos.GetInfos();
}

const std::vector<CStockInfo> &CApplication::GetStockInfos() const
{
	return m_StockInfos.GetInfos();
}
const std::vector<CTutorialInfo> &CApplication::GetTutorialInfos() const
{
	return m_TutorialInfos.GetInfos();
}

void CApplication::SetTutorialPaused(bool bTutorialPaused)
{
	m_bTutorialStarted.store(bTutorialPaused);
}

bool CApplication::IsTutorialPaused() const
{
	return m_bTutorialStarted.load();
}
