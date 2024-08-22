#include "Info.h"

CInfoBase::CInfoBase(const json &Json) :
	m_Text(CheckAndAssign<std::string>(Json, "text_val").value_or("Nameless"))
{

}

CInfoMarketBase::CInfoMarketBase(const json &Json) :
	CInfoBase(Json),
	m_Price(CheckAndAssign<float>(Json, "price").value_or(0.0f))
{

}

CPortfolioStockInfo::CPortfolioStockInfo(const json &Json) :
	CInfoMarketBase(Json),
	m_Quantity(CheckAndAssign<int>(Json, "quantity").value_or(0))
{

}

CStockInfo::CStockInfo(const json &Json) :
	CInfoMarketBase(Json),
	m_StartStrategy(CheckAndAssign<std::string>(Json, "strategy").value_or("None"))
{

}

CTutorialInfo::CTutorialInfo(const json &Json) :
	CInfoBase(Json),
	m_Delay(CheckAndAssign<int>(Json, "delay_msec").value_or(0))
{

}
