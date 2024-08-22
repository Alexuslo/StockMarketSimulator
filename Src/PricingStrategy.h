#ifndef PRICING_STRATEGY_H
#define PRICING_STRATEGY_H

#include <random>
#include <chrono>
#include "Definitions.h"

class CPricingStrategyBase abstract
{

protected:

	float m_Factor;

public:

	explicit CPricingStrategyBase(float Factor);
	virtual ~CPricingStrategyBase() = default;

	virtual float CalculateNewPrice(float CurrentPrice) = 0;
	virtual EStrategyType GetPriceStrategyType() const { return EStrategyType::None; }
};

class CRandomPricingStrategy : public CPricingStrategyBase 
{
	std::mt19937 m_Generator;
	std::uniform_real_distribution<> m_Distribution;

public:
	
	explicit CRandomPricingStrategy(float Factor);
	
	virtual float CalculateNewPrice(float CurrentPrice) override;
	virtual EStrategyType GetPriceStrategyType() const override;
};

class CTrendPricingStrategy : public CPricingStrategyBase
{

	std::chrono::steady_clock::time_point m_StartTime;

public:

	explicit CTrendPricingStrategy(float Factor);

	virtual float CalculateNewPrice(float CurrentPrice) override;
	virtual EStrategyType GetPriceStrategyType() const override;
};
#endif // PRICING_STRATEGY_H
