#include "Definitions.h"

EStrategyType &operator++(EStrategyType &Type)
{
	int NewValue = static_cast<int>(Type) + 1;

	if (NewValue >= static_cast<int>(EStrategyType::TotalStrategy))
	{
		Type = EStrategyType::Random;
	}
	else
	{
		Type = static_cast<EStrategyType>(NewValue);
	}

	return Type;
}

EStrategyType operator++(EStrategyType &Type, int)
{
	EStrategyType OldValue = Type;
	++Type;
	return OldValue;
}

std::map<EStrategyType, std::string> g_StrategyTypeEnamToNameString =
{
	{EStrategyType::None, "none"},
	{EStrategyType::Random, "random"},
	{EStrategyType::Trend, "trend"}
};

std::map<std::string, EStrategyType> g_NameStringToStrategyTypeEnam =
{
	{"none", EStrategyType::None},
	{"random", EStrategyType::Random},
	{"trend", EStrategyType::Trend}
};

const std::string ResetColor = "\033[0m";
const std::string RedColor = "\033[31m";
const std::string GreenColor = "\033[32m";
const std::string YellowColor = "\033[33m";
const std::string BlueColor = "\033[34m";