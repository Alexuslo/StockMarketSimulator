#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>
#include <map>
#include <functional>
#include <sstream>

#include "StockMarketController.h"
#include "Definitions.h"

class CCommandManager;

class CCommandBase
{
protected:

	CCommandManager *m_pCommandManager;

public:

	explicit CCommandBase(CCommandManager *pCommandManager);

	virtual ~CCommandBase() = default;

	virtual CCommandResult CommandInput() = 0;

	template <typename T>
	T ValidateInputNumber(const std::string &InputText, const std::string &LogText);
};

class CBuyCommand : public CCommandBase
{

public:

	explicit CBuyCommand(CCommandManager *pCommandManager);

	CCommandResult CommandInput() override;
};

class CSellCommand : public CCommandBase
{

public:

	explicit CSellCommand(CCommandManager *pCommandManager);

	CCommandResult CommandInput() override;
};

class CNewCommand : public CCommandBase
{

public:

	explicit CNewCommand(CCommandManager *pCommandManager);

	CCommandResult CommandInput() override;
};

class CAddCommand : public CCommandBase
{

public:

	explicit CAddCommand(CCommandManager *pCommandManager);

	CCommandResult CommandInput() override;
};

class CDelCommand : public CCommandBase
{

public:

	explicit CDelCommand(CCommandManager *pCommandManager);

	CCommandResult CommandInput() override;
};

class CInfoCommand : public CCommandBase
{

public:

	explicit CInfoCommand(CCommandManager *pCommandManager);

	CCommandResult CommandInput() override;
};

class CReportCommand : public CCommandBase
{

public:

	explicit CReportCommand(CCommandManager *pCommandManager);

	CCommandResult CommandInput() override;
};

class CExitCommand : public CCommandBase
{

public:

	explicit CExitCommand(CCommandManager *pCommandManager);

	CCommandResult CommandInput() override;
};

class CCommandManager
{
	std::map<std::string, std::function<std::unique_ptr<CCommandBase>()>> m_CommandMap;

	CStockMarketController &m_StockMarketController;

	std::string m_Hint;
	std::string m_InputString;

	void RegisterCommands();

	std::unique_ptr<CCommandBase> CreateCommand(const std::string &Command);

	std::string InputUpdater(bool bNumber);

public:

	explicit CCommandManager(CStockMarketController &StockMarketController);

	void Run();

	std::string Input(const std::string &InputHint, const std::string &Log, bool bNumber);
};

template<typename T>
inline T CCommandBase::ValidateInputNumber(const std::string &InputText, const std::string &LogText)
{
	T Number{};
	while (true)
	{
		auto Text = m_pCommandManager->Input(InputText, LogText, true);
		std::stringstream StringStream(Text);
		StringStream >> Number;

		if (!StringStream.fail() && StringStream.eof())
			return Number;
	}
}
#endif // COMMAND_H

