#include "Stock.h"
#include "StockMarketModel.h"
#include "Application.h"
#include "PricingStrategy.h"
#include "Definitions.h"
#include "HelperFuncs.h"

CStockMarketModel::CStockMarketModel() :
	m_LogicUpdater(*this),
	m_Portfolio(*this)
{
	// filling the store with stocks
	for (const auto &StockInfo : CApplication::GetInstance().GetStockInfos())
	{
		auto pStock = std::make_shared<CStock>(StockInfo.m_Text, StockInfo.m_Price, std::move(::CreateStrategy(StockInfo.m_StartStrategy)));
		m_StockPackages.emplace_back(CStockPackage{ pStock, MAX_MARKET_STOCK_QUANTITY });
	}

	// filling a portfolio with stocks
	for (const auto &HoldingStockInfo : CApplication::GetInstance().GetPortfolioInfos()) 
	{
		auto It = std::find_if(m_StockPackages.begin(), m_StockPackages.end(), [&HoldingStockInfo](const auto &StockPackage) {

			return HoldingStockInfo.m_Text == StockPackage.m_pStock->GetName();
		});

		if (It == m_StockPackages.end())
			throw std::runtime_error("CStockMarketModel::CStockMarketModel() - The portfolio contains a starting stock that is not available on the market: " + HoldingStockInfo.m_Text);

		m_Portfolio.InitStartStock(It->m_pStock, HoldingStockInfo.m_Quantity, HoldingStockInfo.m_Price);
	}

	// Launching price change logic, price change strategies, and a tutorial
	m_LogicUpdater.Run();
}

CStockMarketModel::~CStockMarketModel()
{

}

void CStockMarketModel::UpdatePrices()
{
	for (const auto &i : m_StockPackages)
		if (i.IsPresent())
			i.m_pStock->UpdatePrice();
}

void CStockMarketModel::UpdateStrategy()
{
	bool bAnyUpdate = false;

	for (const auto &i : m_StockPackages)
	{
		if (i.IsPresent() && i.m_pStock->UpdateStrategy())
		{
			bAnyUpdate = true;

			std::string LogMessage;
			LogMessage += "\"" + i.m_pStock->GetName() + "\"" + " change strategy";

			AddLogMessage(LogMessage);
		}
	}

	if (!bAnyUpdate)
		AddLogMessage("TThe strategies have not changed");

	AddLogMessage("--------------------------------");
}

const std::vector<CStockPackage> &CStockMarketModel::GetStockPackages() const
{
	return m_StockPackages;
}

CPortfolio &CStockMarketModel::GetPortfolio()
{
	return m_Portfolio;
}

const CPortfolio &CStockMarketModel::GetPortfolio() const
{
	return m_Portfolio;
}

CLogicUpdater &CStockMarketModel::GetLogicUpdater()
{
	return m_LogicUpdater;
}

std::tuple<std::shared_ptr<CStock>, int, float> CStockMarketModel::Buy(const std::string &Name, int Quantity, float Price, float &Cash, bool bFree)
{
	const auto It = std::find_if(m_StockPackages.begin(), m_StockPackages.end(), [&Name](const auto &StockPackage) {

		return Name == StockPackage.m_pStock->GetName();
	});

	if (It == m_StockPackages.end())// An attempt to buy something that is not there.
		return { nullptr, 0, 0.0f };

	float BestPrice = std::max(Price, It->m_pStock->GetPrice());// The buyer can offer more than the market value

	int PossibleQuantity = bFree ? Quantity : std::min(Quantity, It->m_Quantity);// we take into account the quantity on the market

	if (PossibleQuantity <= 0 || BestPrice > Cash || PossibleQuantity < Quantity)
	{
		AddLogMessage("Incorrect operation.");

		return { nullptr, 0, BestPrice };
	}

	std::string LogMessage;

	if (bFree)
	{
		LogMessage += "Add \"" + Name + "\" X ";
		LogMessage += std::to_string(PossibleQuantity);
	}
	else
	{
		PossibleQuantity = std::min(PossibleQuantity, static_cast<int>(Cash / BestPrice));// we take into account the purchasing power

		Cash -= BestPrice * static_cast<float>(PossibleQuantity);

		It->ChangeQuantity(-PossibleQuantity);

		LogMessage += "Buy \"" + Name + "\" " + std::to_string(BestPrice) + " $ X ";
		LogMessage += std::to_string(PossibleQuantity);
		LogMessage += " Cash left - " + std::to_string(Cash);
	}

	AddLogMessage(LogMessage);
	Notify(EDrawFlagFlags::DrawStockFlag);

	return { It->m_pStock, PossibleQuantity, BestPrice };
}

void CStockMarketModel::Sell(const std::string &Name, int Quantity, float Price, float &Cash, bool bFree)
{
	const auto It = std::find_if(m_StockPackages.begin(), m_StockPackages.end(), [&Name](const auto &StockPackage) {

		return Name == StockPackage.m_pStock->GetName();
	});

	if (It == m_StockPackages.end())// An attempt to sell something that doesn't exist.
		return;

	float BestPrice = std::min(std::clamp(Price, 0.0f, Price), It->m_pStock->GetPrice());// The buyer can offer less than the market price

	std::string LogMessage;

	if (bFree)
	{
		LogMessage += "Del \"" + Name + "\" X ";
		LogMessage += std::to_string(Quantity);
	}
	else
	{
		Cash += BestPrice * static_cast<float>(std::clamp(Quantity, 0, Quantity));

		It->ChangeQuantity(Quantity);

		LogMessage += "Sell \"" + Name + "\" " + std::to_string(BestPrice) + " $ X ";
		LogMessage += std::to_string(Quantity);
		LogMessage += " Cash left - " + std::to_string(Cash);
	}

	AddLogMessage(LogMessage);
	Notify(EDrawFlagFlags::DrawStockFlag);
}

void CStockMarketModel::AddLogMessage(const std::string &String)
{
	m_LogQueue.push_back(String);

	while (static_cast<int>(m_LogQueue.size()) > LOG_H)
		m_LogQueue.pop_front();

	Notify(EDrawFlagFlags::DrawLogFlag);
}

const std::deque<std::string> &CStockMarketModel::GetLogStrings() const
{ 
	return m_LogQueue; 
}

const std::string &CStockMarketModel::GetInputText() const
{ 
	return m_InputText; 
}

void CStockMarketModel::SetInputHint(const std::string &InputHint)
{
	m_InputHint = InputHint;

	Notify(EDrawFlagFlags::DrawHintFlag);
}

const std::string &CStockMarketModel::GetInputHint() const
{
	return m_InputHint;
}

void CStockMarketModel::SetInputText(const std::string &InputText)
{
	m_InputText = InputText;

	Notify(EDrawFlagFlags::DrawInputFlag);
}

void CStockPackage::ChangeQuantity(int Quantity)
{
	//because selling/buying affects supply/demand
	//we change not only the number of shares on the market, but also the price, but the price depends only on the ratio of quantities (simplified)

	int ResultQuantity = m_Quantity + Quantity;

	if (ResultQuantity > 0 && m_Quantity > 0)
	{
		float NewMarketPrice = m_pStock->GetPrice() * (static_cast<float>(m_Quantity) / static_cast<float>(m_Quantity + Quantity));
		m_pStock->SetPrice(NewMarketPrice);
	}

	m_Quantity += Quantity;
}

bool CStockPackage::IsPresent() const
{
	return m_Quantity > 0;
}
