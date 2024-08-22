#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>
#include <map>

constexpr float START_PORTFOLIO_CASH = 10'000.0f;

constexpr int MAX_MARKET_STOCK_QUANTITY = 10'000;
constexpr int MAX_SECOND_FOR_UPDATE_PRICE = 5;
constexpr int MAX_SECOND_FOR_UPDATE_STRATEGY = 42;

constexpr int MARKET_X_COORD = 1;
constexpr int MARKET_Y_COORD = 1;

constexpr int HINT_X_COORD = 1;
constexpr int HINT_Y_COORD = 26;

constexpr int INPUT_X_COORD = 1;
constexpr int INPUT_Y_COORD = 27;
constexpr int INPUT_MAX_SYMBOL = 20;

constexpr int CONSOLE_HEDER_WIDTH = 2;
constexpr int LOG_H = 10;
constexpr int LOG_X_COORD = 1;
constexpr int LOG_Y_COORD = HINT_Y_COORD - LOG_H - CONSOLE_HEDER_WIDTH;

constexpr int CONSOLE_WIDTH = 90;

constexpr int SPACE_COUNT = 10;

constexpr int PERCENT_TO_CHANGE_STRATEGY = 50;

enum class EDrawFlagFlags
{
	None = 0,
	DrawStockFlag = 1,
	DrawLogFlag = 1 << 1,
	DrawHintFlag = 1 << 2,		
	DrawInputFlag = 1 << 3,

	//^^^ Please add new draw flag before ^^^ vvv and add to "DrawAllFlag" vvv
	DrawAllFlag = DrawStockFlag | DrawLogFlag | DrawHintFlag | DrawInputFlag
};

inline EDrawFlagFlags operator&(EDrawFlagFlags lhs, EDrawFlagFlags rhs)
{
	return static_cast<EDrawFlagFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline EDrawFlagFlags operator|(EDrawFlagFlags lhs, EDrawFlagFlags rhs)
{
	return static_cast<EDrawFlagFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

enum class ECommandType
{
	None,
	Buy,
	Sell,
	New,
	Add,
	Del,
	Info,
	Report,
	Exit
};

struct CCommandResult
{
	ECommandType m_Command = ECommandType::None;
	std::string m_Name;
	int m_Quantity = 0;
	float m_Price = 0.0f;
};

enum class EStrategyType
{
	None = -1,
	Random = 0,
	Trend = 1,

	//^^^Please add before^^^
	TotalStrategy
};

EStrategyType &operator++(EStrategyType &Type);
EStrategyType operator++(EStrategyType &Type, int);

extern const std::string ResetColor;
extern const std::string RedColor;
extern const std::string GreenColor;
extern const std::string YellowColor;
extern const std::string BlueColor;

#endif // DEFINITIONS_H