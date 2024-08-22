#ifndef HELPER_FUNCS_H
#define HELPER_FUNCS_H

#include <string>
#include <conio.h>
#include <functional>
#include <thread>

#include "Definitions.h"
#include "Application.h"
#include "PricingStrategy.h"

std::unique_ptr<CPricingStrategyBase> CreateStrategy(const std::string &StrategyName);

const std::string &GetNameStringByStrategyTypeEnum(EStrategyType StrategyType);
EStrategyType GetStrategyTypeEnumByNameString(const std::string &Name);
#endif // HELPER_FUNCS_H
