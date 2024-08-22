#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <memory>
#include <mutex>
#include <random>
#include <chrono>

#include "PricingStrategy.h"

class CStock 
{
	mutable std::mutex m_Mutex;

	std::string m_Name;
	float m_Price;
	std::unique_ptr<CPricingStrategyBase> m_pPricingStrategy;

public:

	CStock(const std::string &Name, float Price, std::unique_ptr<CPricingStrategyBase> &&pPricingStrategy);

	void SetPrice(float Price);
	float GetPrice() const;

	void UpdatePrice();
	bool UpdateStrategy();

	const std::string &GetName() const;

	void SetPricingStrategy(std::unique_ptr<CPricingStrategyBase> &&pStrategy);
	const EStrategyType GetPriceStrategyType() const;
};
#endif // STOCK_H
