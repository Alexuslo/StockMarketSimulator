#ifndef STOCK_MARKET_CONTROLLER_H
#define STOCK_MARKET_CONTROLLER_H

#include "Portfolio.h"
#include "StockMarketModel.h"
#include <string>

class CStockMarketController 
{
	CStockMarketModel &m_StockMarketModel;

	std::string m_InputString;
	
public:

	explicit CStockMarketController(CStockMarketModel &m_StockMarketModel);

	void Run();

	void BuyStock(const std::string &Name, int Quantity, float Price, bool bFree) const;
	void SellStock(const std::string &Name, int Quantity, float Price, bool bFree) const;

	void NewPortfolio() const;

	void GenerateInfo() const;
	void GenerateReport() const;

	void AddLogMessage(const std::string &LogMessage) const;
	void SetInputHint(const std::string &InputHint) const;
	void SetInputText(const std::string &InputText) const;

	void OnCommand(const CCommandResult &CommandResult) const;
};

#endif // STOCK_MARKET_CONTROLLER_H
