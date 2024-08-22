#include "PricingStrategy.h"

CRandomPricingStrategy::CRandomPricingStrategy(float Factor) :
	CPricingStrategyBase(Factor),
	m_Generator(std::random_device{}()),
	m_Distribution(-Factor, Factor)
{

}

float CRandomPricingStrategy::CalculateNewPrice(float CurrentPrice)
{
	return CurrentPrice * (1.0f + static_cast<float>(m_Distribution(m_Generator)));
}

EStrategyType CRandomPricingStrategy::GetPriceStrategyType() const
{
	return EStrategyType::Random;
}

CTrendPricingStrategy::CTrendPricingStrategy(float Factor) :
	CPricingStrategyBase(Factor),
	m_StartTime(std::chrono::steady_clock::now())
{

}

float CTrendPricingStrategy::CalculateNewPrice(float CurrentPrice)
{
	auto Now = std::chrono::steady_clock::now();
	auto Duration = std::chrono::duration_cast<std::chrono::seconds>(Now - m_StartTime).count();
	return CurrentPrice * (1.0f + m_Factor * std::sin(Duration / 10.0f));
}

EStrategyType CTrendPricingStrategy::GetPriceStrategyType() const
{
	return EStrategyType::Trend;
}

CPricingStrategyBase::CPricingStrategyBase(float Factor) :
	m_Factor(Factor)
{

}
