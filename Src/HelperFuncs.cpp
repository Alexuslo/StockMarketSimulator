#include "HelperFuncs.h"

std::unique_ptr<CPricingStrategyBase> CreateStrategy(const std::string &StrategyName)
{
	if (StrategyName == "random")
		return std::make_unique<CRandomPricingStrategy>(0.05f);

	else
	if (StrategyName == "trend")
		return std::make_unique<CTrendPricingStrategy>(0.01f);

	throw std::runtime_error("CreateStrategy - unknown strategy: " + StrategyName);
}

const std::string &GetNameStringByStrategyTypeEnum(EStrategyType StrategyType)
{
	const auto It = g_StrategyTypeEnamToNameString.find(StrategyType);
	if (It != g_StrategyTypeEnamToNameString.end())
		return It->second;

	return g_StrategyTypeEnamToNameString[EStrategyType::None];
}

EStrategyType GetStrategyTypeEnumByNameString(const std::string &Name)
{
	const auto It = g_NameStringToStrategyTypeEnam.find(Name);
	if (It != g_NameStringToStrategyTypeEnam.end())
		return It->second;

	return EStrategyType::None;
}
