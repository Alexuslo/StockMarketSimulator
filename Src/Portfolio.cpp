#include "Portfolio.h"
#include "StockMarketModel.h"
#include "Stock.h"

void CPortfolio::InitStartStock(std::shared_ptr<CStock> pStockRef, int Quantity, float Price)
{
	m_Stocks.push_back(CPortfolioStockData(std::move(pStockRef), Quantity, Price));
}

CPortfolio::CPortfolio(CStockMarketModel &StockMarketModel) :
	m_StockMarketModel(StockMarketModel)
{

}

void CPortfolio::Buy(const std::string &Name, int Quantity, float Price, bool bFree)
{
	auto[pStockRef, PurchaseQuantity, PurchasePrice] = m_StockMarketModel.Buy(Name, Quantity, Price, m_Cash, bFree);

	if (PurchaseQuantity <= 0)
		return;

	const auto StockIt = std::find_if(m_Stocks.begin(), m_Stocks.end(), [Name](const auto &Stock) {
		
			return Stock.GetStockRef() && Stock.GetStockRef()->GetName() == Name;
		});

	if (StockIt == m_Stocks.end())
		m_Stocks.push_back(CPortfolioStockData(pStockRef, PurchaseQuantity, PurchasePrice));

	else
		StockIt->AddSomeStocks(PurchaseQuantity, PurchasePrice, bFree);

	CalculateCashDeltaAndValue();
}

void CPortfolio::Sell(const std::string &Name, int Quantity, float Price, bool bFree)
{
	const auto StockIt = std::find_if(m_Stocks.begin(), m_Stocks.end(), [Name](const auto &Stock) {

		return Stock.GetStockRef() && Stock.GetStockRef()->GetName() == Name;
		});

	if (StockIt == m_Stocks.end())
	{
		m_StockMarketModel.AddLogMessage("Stock \"" + Name + "\" not found in portfolio.");
		return;
	}

	if (StockIt->GetQuantity() < Quantity)
	{
		std::string LogMessage;

		LogMessage += "Are you trying to " + std::string(bFree ? "del" : "sell");
		LogMessage += ": " + std::to_string(Quantity) + " \"" + Name;
		LogMessage += "\". The portfolio contains: " + std::to_string(StockIt->GetQuantity());

		m_StockMarketModel.AddLogMessage(LogMessage);
		return;
	}

	m_StockMarketModel.Sell(Name, Quantity, Price, m_Cash, bFree);

	StockIt->RemoveSomeStocks(Quantity);
	
	if (StockIt->GetQuantity() <= 0)
		m_Stocks.erase(StockIt);

	CalculateCashDeltaAndValue();
}

float CPortfolio::GetCurrentValue() const
{
	return std::accumulate(m_Stocks.begin(), m_Stocks.end(), 0.0f, [](float Total, const auto &Data) -> float {

		return Total + Data.GetMarketPrice() * Data.GetQuantity();
	});
}

const std::vector<CPortfolio::CPortfolioStockData> &CPortfolio::GetHoldings() const
{
	return m_Stocks;
}

void CPortfolio::CalculateCashDeltaAndValue()
{
	m_CashDelta = 0.0;
	m_MarketValue = 0.0;

	for (const auto &Stock : m_Stocks)
	{
		m_CashDelta += (Stock.GetMarketPrice() - Stock.GetAveragePrice()) * Stock.GetQuantity();
		m_MarketValue += Stock.GetMarketPrice() * Stock.GetQuantity();
	}
}

void CPortfolio::Notify()
{
	CalculateCashDeltaAndValue();
}

double CPortfolio::GetCash() const
{
	return m_Cash;
}

double CPortfolio::GetCashDelta() const
{
	return m_CashDelta;
}

double CPortfolio::GetMarketValue() const
{
	return m_MarketValue;
}

void CPortfolio::Reset()
{
	m_Cash = START_PORTFOLIO_CASH;
	m_CashDelta = 0.0;
	m_MarketValue = 0.0;

	m_Stocks.clear();
}

void CPortfolio::NewPortfolio()
{
	Reset();
}

void CPortfolio::GenerateInfo() const
{
	if (m_Stocks.empty())
	{
		m_StockMarketModel.AddLogMessage("Portfolio empty.");
	}
	else
	{
		m_StockMarketModel.AddLogMessage("You own the following shares:");

		for (const auto &Stock : m_Stocks)
		{
			std::string StockInfoBuffer;

			StockInfoBuffer += Stock.GetStockRef()->GetName() + ", ";
			StockInfoBuffer += std::to_string(Stock.GetQuantity()) + " Pcs, ";
			StockInfoBuffer += std::to_string(Stock.GetAveragePrice()) + "/";
			StockInfoBuffer += std::to_string(Stock.GetMarketPrice()) + " Avr/Market.";

			m_StockMarketModel.AddLogMessage(StockInfoBuffer);
		}
	}
}

void CPortfolio::GenerateReport() const
{
	std::string Buffer;

	Buffer += (m_CashDelta < 0.0 ? "Your loss: " : "Your Profit: ");
	Buffer += std::to_string(std::abs(m_CashDelta));
	Buffer += ". Market value: ";
	Buffer += std::to_string(m_MarketValue);
	Buffer += ". Cash: " + std::to_string(m_Cash) + ".";

	m_StockMarketModel.AddLogMessage(Buffer);
}

void CPortfolio::CPortfolioStockData::AddSomeStocks(int PurchaseQuantity, float PurchasePrice, bool bFree)
{
	if (PurchaseQuantity <= 0)
		return;

	if (!bFree)
	{
		if (m_Quantity > 0)
			m_AveragePrice = (m_AveragePrice * m_Quantity + PurchasePrice * PurchaseQuantity) / (m_Quantity + PurchaseQuantity);

		else
			m_AveragePrice = PurchasePrice;
	}

	m_Quantity += PurchaseQuantity;
}

void CPortfolio::CPortfolioStockData::RemoveSomeStocks(int PurchaseQuantity)
{
	m_Quantity -= PurchaseQuantity;
}

void CPortfolio::CPortfolioStockData::Reset()
{
	m_pStockRef.reset();
	m_Quantity = 0;
	m_AveragePrice = 0.0f;
}

float CPortfolio::CPortfolioStockData::GetMarketPrice() const
{
	if (!m_pStockRef) //Fast Fail
		throw std::runtime_error("CPortfolio::CPortfolioStockData::GetMarketPrice() - Portfolio stock pointer doesn't init.");

	return m_pStockRef->GetPrice();
}
