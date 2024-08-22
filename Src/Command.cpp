#include "Command.h"
#include "HelperFuncs.h"

void CCommandManager::RegisterCommands()
{
	m_CommandMap["buy"] = [this]() { return std::make_unique<CBuyCommand>(this); };
	m_CommandMap["sell"] = [this]() { return std::make_unique<CSellCommand>(this); };
	m_CommandMap["new"] = [this]() { return std::make_unique<CNewCommand>(this); };
	m_CommandMap["add"] = [this]() { return std::make_unique<CAddCommand>(this); };
	m_CommandMap["del"] = [this]() { return std::make_unique<CDelCommand>(this); };
	m_CommandMap["info"] = [this]() { return std::make_unique<CInfoCommand>(this); };
	m_CommandMap["report"] = [this]() { return std::make_unique<CReportCommand>(this); };
	m_CommandMap["exit"] = [this]() { return std::make_unique<CExitCommand>(this); };

	m_Hint = std::string("Enter command (buy, sell, new, add, del, info, report, exit): ");
}

std::unique_ptr<CCommandBase> CCommandManager::CreateCommand(const std::string &Command)
{
	auto It = m_CommandMap.find(Command);
	if (It != m_CommandMap.end())
		return It->second();

	return nullptr;
}

CCommandManager::CCommandManager(CStockMarketController &StockMarketController) :
	m_StockMarketController(StockMarketController)
{
	RegisterCommands();
}

void CCommandManager::Run()
{
	while (true)
	{
		m_StockMarketController.SetInputHint(m_Hint);

		std::string Command = Input(std::string(), std::string(), false);

		if (auto pCommand = CreateCommand(Command))
		{
			m_StockMarketController.AddLogMessage(std::string("Select ") + Command);

			CCommandResult Result = pCommand->CommandInput();

			if (Result.m_Command == ECommandType::Exit)
				break;

			m_StockMarketController.OnCommand(Result);
		}
		else
		{
			m_StockMarketController.AddLogMessage("Invalid command.");
		}
	}
}

std::string CCommandManager::Input(const std::string &InputHint, const std::string &Log, bool bNumber)
{
	if (!InputHint.empty())
		m_StockMarketController.SetInputHint(InputHint);

	auto Value = InputUpdater(bNumber);

	if (!Log.empty())
		m_StockMarketController.AddLogMessage(std::string(Log + "\"" + Value + "\""));

	return Value;
}

std::string CCommandManager::InputUpdater(bool bNumber)
{
	std::string InputString;
	bool HasDecimalPoint = false;

	while (true)
	{
		if (_kbhit())
		{
			const auto Code = _getch();

			if (CApplication::GetInstance().IsTutorialPaused())
				continue;

			if (Code == '\b' || static_cast<int>(Code) == 83 || static_cast<int>(Code) == 127)
			{
				if (!InputString.empty())
				{
					InputString.pop_back();

					m_StockMarketController.SetInputText(InputString);
				}
			}
			else
			if (Code == '\r')
			{
				if (bNumber)
				{
					if (InputString.empty())
						InputString.push_back('0');
				}
				else
				{
					std::transform(InputString.begin(), InputString.end(), InputString.begin(), [](const auto &Symbol) {

						return std::tolower(Symbol);
					});
				}

				std::string ReturnInputString = InputString;
				InputString.clear();

				m_StockMarketController.SetInputText(InputString);

				return ReturnInputString;
			}
			else
			if (InputString.size() < INPUT_MAX_SYMBOL)
			{
				if ((bNumber && (isdigit(Code) || (Code == '.' && !HasDecimalPoint))) || (!bNumber && isalpha(Code)))
				{
					if (Code == '.')
						HasDecimalPoint = true;

					InputString += Code;

					m_StockMarketController.SetInputText(InputString);
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

CCommandBase::CCommandBase(CCommandManager *pCommandManager)
{
	m_pCommandManager = pCommandManager;
}

CBuyCommand::CBuyCommand(CCommandManager *pCommandManager) :
	CCommandBase(pCommandManager)
{

}

CCommandResult CBuyCommand::CommandInput()
{
	CCommandResult result;
	result.m_Command = ECommandType::Buy;

	result.m_Name = m_pCommandManager->Input("[buy]Enter product name: ", "Product name: ", false);
	result.m_Quantity = ValidateInputNumber<int>("[buy]Enter quantity: ", "Quantity: ");
	result.m_Price = ValidateInputNumber<float>("[buy]Enter price: ", "Price: ");

	return result;
}

CSellCommand::CSellCommand(CCommandManager *pCommandManager) :
	CCommandBase(pCommandManager)
{

}

CCommandResult CSellCommand::CommandInput()
{
	CCommandResult result;
	result.m_Command = ECommandType::Sell;

	result.m_Name = m_pCommandManager->Input("[sell]Enter product name: ", "Product name: ", false);
	result.m_Quantity = ValidateInputNumber<int>("[sell]Enter quantity: ", "Quantity: ");
	result.m_Price = ValidateInputNumber<float>("[sell]Enter price: ", "Price: ");

	return result;
}

CNewCommand::CNewCommand(CCommandManager *pCommandManager) :
	CCommandBase(pCommandManager)
{

}

CCommandResult CNewCommand::CommandInput()
{
	CCommandResult result;
	result.m_Command = ECommandType::New;

	return result;
}

CAddCommand::CAddCommand(CCommandManager *pCommandManager) :
	CCommandBase(pCommandManager)
{

}

CCommandResult CAddCommand::CommandInput()
{
	CCommandResult result;
	result.m_Command = ECommandType::Add;

	result.m_Name = m_pCommandManager->Input("[add to portfolio free]Enter product name: ", "Product name: ", false);
	result.m_Quantity = ValidateInputNumber<int>("[add to portfolio free]Enter quantity: ", "Quantity: ");

	return result;
}

CDelCommand::CDelCommand(CCommandManager *pCommandManager) :
	CCommandBase(pCommandManager)
{

}

CCommandResult CDelCommand::CommandInput()
{
	CCommandResult result;
	result.m_Command = ECommandType::Del;

	result.m_Name = m_pCommandManager->Input("[del from portfolio free]Enter product name: ", "Product name: ", false);
	result.m_Quantity = ValidateInputNumber<int>("[del from portfolio free]Enter quantity: ", "Quantity: ");

	return result;
}

CInfoCommand::CInfoCommand(CCommandManager *pCommandManager) :
	CCommandBase(pCommandManager)
{

}

CCommandResult CInfoCommand::CommandInput()
{
	CCommandResult result;
	result.m_Command = ECommandType::Info;

	return result;
}

CReportCommand::CReportCommand(CCommandManager *pCommandManager) :
	CCommandBase(pCommandManager)
{

}

CCommandResult CReportCommand::CommandInput()
{
	CCommandResult result;
	result.m_Command = ECommandType::Report;

	return result;
}

CExitCommand::CExitCommand(CCommandManager *pCommandManager) :
	CCommandBase(pCommandManager)
{

}

CCommandResult CExitCommand::CommandInput()
{
	CCommandResult result;
	result.m_Command = ECommandType::Exit;

	return result;
}