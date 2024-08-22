#include "HelperFuncs.h"

std::unique_ptr<CPricingStrategyBase> CreateStrategy(const std::string &StrategyName)
{
	if (StrategyName == "random")
		return std::make_unique<CRandomPricingStrategy>(0.05f);

	else
	if (StrategyName == "trend")
		return std::make_unique<CTrendPricingStrategy>(0.01f);

	throw std::runtime_error("CStockMarketModel::CStockMarketModel() {lambda CreateStrategy(...)} - Неизвестная стратегия - " + StrategyName);
}


// You can use unordered_map to improve search performance if the table is large
const std::string &GetNameStringByStrategyTypeEnum(EStrategyType StrategyType)
{
	static std::map<EStrategyType, std::string> StrategyTypeEnamToNameString =
	{
		{EStrategyType::None, "none"},
		{EStrategyType::Random, "random"},
		{EStrategyType::Trend, "trend"}
	};

	return StrategyTypeEnamToNameString[StrategyType];
}

EStrategyType GetStrategyTypeEnumByNameString(const std::string &Name)
{
	static std::map<std::string, EStrategyType> NameStringToStrategyTypeEnam =
	{
		{"none", EStrategyType::None},
		{"random", EStrategyType::Random},
		{"trend", EStrategyType::Trend}
	};

	return NameStringToStrategyTypeEnam[Name];
}
