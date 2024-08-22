#ifndef STOCK_MARKET_MODEL_H
#define STOCK_MARKET_MODEL_H

#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <algorithm>
#include <deque>
#include <exception>

#include "Portfolio.h"
#include "LogicUpdater.h"
#include "Observer.h"

class CApplication;
class CStock;

struct CStockPackage
{
	std::shared_ptr<CStock> m_pStock;

	int m_Quantity = 0;

	void ChangeQuantity(int Quantity);
	bool IsPresent() const;
};

class CStockMarketModel : public CSender
{
	std::vector<CStockPackage> m_StockPackages;
	
	CLogicUpdater m_LogicUpdater;
	CPortfolio m_Portfolio;

	std::deque<std::string> m_LogQueue;

	std::string m_InputHint;
	std::string m_InputText;

public:

	CStockMarketModel();
	~CStockMarketModel();

	CStockMarketModel(const CStockMarketModel &) = delete;
	CStockMarketModel &operator=(const CStockMarketModel &) = delete;

	void UpdatePrices();
	void UpdateStrategy();

	const std::vector<CStockPackage> &GetStockPackages() const;

	CPortfolio &GetPortfolio();
	const CPortfolio &GetPortfolio() const;

	CLogicUpdater &GetLogicUpdater();

	std::tuple<std::shared_ptr<CStock>, int, float> Buy(const std::string &Name, int Quantity, float Price, float &Cash, bool bFree);
	void Sell(const std::string &Name, int Quantity, float Price, float &Cash, bool bFree);

	void AddLogMessage(const std::string &LogMessage);
	const std::deque<std::string> &GetLogStrings() const;

	void SetInputHint(const std::string &InputHint);
	const std::string &GetInputHint() const;

	void SetInputText(const std::string &InputText);
	const std::string &GetInputText() const;
};
#endif // STOCK_MARKET_MODEL_H
