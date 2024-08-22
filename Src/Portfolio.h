#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <numeric>
#include <memory>
#include <stdexcept>
#include <string>

#include "Observer.h"

class CStockMarketModel;
class CStock;

class CPortfolio : public CObserver
{
	friend class CStockMarketModel;

	class CPortfolioStockData 
	{
		std::shared_ptr<CStock> m_pStockRef;

		int m_Quantity = 0;
		float m_AveragePrice = 0.0f;

	public:

		CPortfolioStockData() :
			m_pStockRef(nullptr),
			m_Quantity(0),
			m_AveragePrice(0.0f)
		{

		}

		CPortfolioStockData(std::shared_ptr<CStock> pStockRef, int Quantity, float Price) :
			m_pStockRef(std::move(pStockRef)),
			m_Quantity(Quantity),
			m_AveragePrice(Price)
		{

		}

		void AddSomeStocks(int PurchaseQuantity, float PurchasePrice, bool bFree);
		void RemoveSomeStocks(int PurchaseQuantity);

		void Reset();

		int GetQuantity() const { return m_Quantity; }
		float GetAveragePrice() const { return m_AveragePrice; }
		float GetMarketPrice() const;

		const std::shared_ptr<CStock> &GetStockRef() const { return m_pStockRef; }
	};

	CStockMarketModel &m_StockMarketModel;

	std::vector<CPortfolioStockData> m_Stocks;

	float m_Cash = START_PORTFOLIO_CASH;
	float m_CashDelta = 0.0f;
	float m_MarketValue = 0.0f;

	void InitStartStock(std::shared_ptr<CStock> pStockRef, int Quantity, float Price);

public:

	explicit CPortfolio(CStockMarketModel &StockMarketModel);
	
	void Buy(const std::string &Name, int Quantity, float Price, bool bFree);
	void Sell(const std::string &Name, int Quantity, float Price, bool bFree);

	float GetCurrentValue() const;

	const std::vector<CPortfolioStockData> &GetHoldings() const;

	void CalculateCashDeltaAndValue();

	virtual void Notify() override;

	float GetCashDelta() const;
	float GetMarketValue() const;

	void Reset();

	void NewPortfolio();

	void GenerateInfo() const;
	void GenerateReport() const;
};
#endif // PORTFOLIO_H