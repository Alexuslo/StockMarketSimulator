#include "StockMarketController.h"
#include "Definitions.h"
#include "Command.h"

CStockMarketController::CStockMarketController(CStockMarketModel &m_StockMarketModel) :
	m_StockMarketModel(m_StockMarketModel)
{

}

void CStockMarketController::Run()
{
	CCommandManager CommandManager(*this);

	CommandManager.Run();
}

void CStockMarketController::BuyStock(const std::string &Name, int Quantity, float Price, bool bFree) const
{
	m_StockMarketModel.GetPortfolio().Buy(Name, Quantity, Price, bFree);
}

void CStockMarketController::SellStock(const std::string &Name, int Quantity, float Price, bool bFree)  const
{
	m_StockMarketModel.GetPortfolio().Sell(Name, Quantity, Price, bFree);
}

void CStockMarketController::NewPortfolio()  const
{
	m_StockMarketModel.GetPortfolio().NewPortfolio();
}

void CStockMarketController::GenerateInfo()  const
{
	m_StockMarketModel.GetPortfolio().GenerateInfo();
}

void CStockMarketController::GenerateReport()  const
{
	m_StockMarketModel.GetPortfolio().GenerateReport();
}

void CStockMarketController::AddLogMessage(const std::string &LogMessage) const
{
	m_StockMarketModel.AddLogMessage(LogMessage);
}

void CStockMarketController::SetInputHint(const std::string &InputHint) const
{
	m_StockMarketModel.SetInputHint(InputHint);
}

void CStockMarketController::SetInputText(const std::string &InputText) const
{
	m_StockMarketModel.SetInputText(InputText);
}

void CStockMarketController::OnCommand(const CCommandResult &CommandResult) const
{
	const auto [Command, Name, Quantity, Price] = CommandResult;
	switch (Command)
	{
	case ECommandType::Buy:

		BuyStock(Name, Quantity, Price, false);
		break;

	case ECommandType::Sell:

		SellStock(Name, Quantity, Price, false);
		break;

	case ECommandType::New:

		NewPortfolio();
		break;

	case ECommandType::Add:

		BuyStock(Name, Quantity, Price, true);
		break;

	case ECommandType::Del:

		SellStock(Name, Quantity, Price, true);
		break;

	case ECommandType::Info:

		GenerateInfo();
		break;
	
	case ECommandType::Report:

		GenerateReport();
		break;

	default:
		break;
	}
}
