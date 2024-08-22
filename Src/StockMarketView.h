#ifndef STOCK_MARKET_VIEW_H
#define STOCK_MARKET_VIEW_H

#include <iostream>

#include "Observer.h"
#include "Definitions.h"
#include <mutex>

#include "StockMarketModel.h"

class CStockMarketView : public CObserver
{
	const CStockMarketModel &m_StockMarketModel;

	std::mutex m_Mutex;

public:

	explicit CStockMarketView(const CStockMarketModel &StockMarketModel);
	~CStockMarketView();

	void ShowCursor() const;
	void HideCursor() const;

	void SetPosition(int x, int y) const;

	void Run();

	void CommonDraw(EDrawFlagFlags Flags);

	void DrawStockMarket();
	void DrawConsole();
	void DrawHint();
	void DrawInput();

	virtual void Notify(EDrawFlagFlags Flags) override;
};

#endif // STOCK_MARKET_VIEW_H