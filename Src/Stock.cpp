#include <array>

#include "Stock.h"
#include "Definitions.h"
#include "HelperFuncs.h"

static std::random_device RandomDevice;
static std::mt19937 Generator(RandomDevice());

CStock::CStock(const std::string &Name, float Price, std::unique_ptr<CPricingStrategyBase> &&pPricingStrategy) :
	m_Name(Name),
	m_Price(Price),
	m_pPricingStrategy(std::move(pPricingStrategy))
{

}

void CStock::SetPrice(float Price)
{
	std::lock_guard<std::mutex> Lock(m_Mutex);

	m_Price = Price;
}

float CStock::GetPrice() const
{
	std::lock_guard<std::mutex> Lock(m_Mutex);

	return m_Price;
}

const std::string &CStock::GetName() const
{
	std::lock_guard<std::mutex> Lock(m_Mutex);

	return m_Name;
}

void CStock::SetPricingStrategy(std::unique_ptr<CPricingStrategyBase> &&pStrategy)
{
	std::lock_guard<std::mutex> Lock(m_Mutex);

	m_pPricingStrategy = move(pStrategy);
}

void CStock::UpdatePrice()
{
	std::lock_guard<std::mutex> Lock(m_Mutex);

	if (m_pPricingStrategy)
		m_Price = m_pPricingStrategy->CalculateNewPrice(m_Price);
}

bool CStock::UpdateStrategy()
{
	std::lock_guard<std::mutex> Lock(m_Mutex);

	if (std::uniform_real_distribution<>(1, 100)(Generator) > PERCENT_TO_CHANGE_STRATEGY)
		return false;

	std::array<std::string, static_cast<size_t>(EStrategyType::TotalStrategy)> Names;

	EStrategyType StrategyType = m_pPricingStrategy ? m_pPricingStrategy->GetPriceStrategyType() : EStrategyType::Random;
	StrategyType++;

	std::string NewName = ::GetNameStringByStrategyTypeEnum(StrategyType);

 	m_pPricingStrategy = nullptr;
 
 	m_pPricingStrategy = std::move(::CreateStrategy(NewName));

	return true;
}

const EStrategyType CStock::GetPriceStrategyType() const
{
	std::lock_guard<std::mutex> Lock(m_Mutex);

	if (m_pPricingStrategy)
		return m_pPricingStrategy->GetPriceStrategyType();

	return EStrategyType::None;
}
