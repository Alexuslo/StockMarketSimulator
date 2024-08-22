#include "StockMarketView.h"
#include "Stock.h"
#include "deque"
#include "HelperFuncs.h"

CStockMarketView::CStockMarketView(const CStockMarketModel &StockMarketModel) :
	m_StockMarketModel(StockMarketModel)
{
	HideCursor();
}

CStockMarketView::~CStockMarketView()
{
	ShowCursor();
}

void CStockMarketView::HideCursor() const
{
	std::cout << "\033[?25l";
}

void CStockMarketView::ShowCursor() const
{
	std::cout << "\033[?25h";
}

void CStockMarketView::SetPosition(int x, int y) const
{
	printf("\033[%d;%dH", y, x);
}

void CStockMarketView::Run()
{
	CommonDraw(EDrawFlagFlags::DrawAllFlag);
}

void CStockMarketView::DrawStockMarket()
{
	int PosY = MARKET_Y_COORD;
	int Num = 1;

	for (const auto &StockPackage : m_StockMarketModel.GetStockPackages())
	{
		SetPosition(MARKET_X_COORD, PosY++);

		std::string StrategyName = ::GetNameStringByStrategyTypeEnum(StockPackage.m_pStock->GetPriceStrategyType());

		std::cout << Num++ << ". ";
		std::cout << GreenColor << StockPackage.m_pStock->GetName() << ResetColor << ", ";
		std::cout << YellowColor << StockPackage.m_Quantity << ResetColor << " Pcs., ";
		std::cout << " Strategy - " << YellowColor << StrategyName << ResetColor << ", ";
		std::cout << "Price - " << YellowColor << StockPackage.m_pStock->GetPrice() << ResetColor << ", ";
		std::cout << std::string(SPACE_COUNT, ' ') << std::endl;
	}
}

void CStockMarketView::DrawConsole()
{
	int PosY = LOG_Y_COORD;

	SetPosition(LOG_X_COORD, PosY++);

	std::cout << "Console logs:";

	for (const auto &LogString : m_StockMarketModel.GetLogStrings())
	{
		SetPosition(LOG_X_COORD, PosY++);

		std::cout << "> " << LogString;
		
		if (LogString.size() < CONSOLE_WIDTH)
			std::cout << std::string(CONSOLE_WIDTH - static_cast<int>(LogString.size()), ' ') << std::endl;
	}
}

void CStockMarketView::DrawHint()
{
	SetPosition(HINT_X_COORD, HINT_Y_COORD);

	std::cout << std::string(CONSOLE_WIDTH, ' ');

	SetPosition(HINT_X_COORD, HINT_Y_COORD);

	std::cout << m_StockMarketModel.GetInputHint();
}

void CStockMarketView::DrawInput()
{
	SetPosition(INPUT_X_COORD, INPUT_Y_COORD);

	std::cout << ">>> " << m_StockMarketModel.GetInputText() << "_" << std::string(INPUT_MAX_SYMBOL, ' ');
}

void CStockMarketView::CommonDraw(EDrawFlagFlags Flags)
{
	std::lock_guard<std::mutex> Lock(m_Mutex);

	if ((Flags & EDrawFlagFlags::DrawStockFlag) != EDrawFlagFlags::None)
		DrawStockMarket();

	if ((Flags & EDrawFlagFlags::DrawLogFlag) != EDrawFlagFlags::None)
		DrawConsole();

	if ((Flags & EDrawFlagFlags::DrawHintFlag) != EDrawFlagFlags::None)
		DrawHint();

	if ((Flags & EDrawFlagFlags::DrawInputFlag) != EDrawFlagFlags::None)
		DrawInput();
}

void CStockMarketView::Notify(EDrawFlagFlags Flags)
{
	CommonDraw(Flags);
}
