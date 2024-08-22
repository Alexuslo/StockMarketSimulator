#ifndef INFO_H
#define INFO_H

#include "../libs/nlohmann/json.hpp"
#include <fstream>
#include <optional>
#include <iostream>

using json = nlohmann::json;

struct CInfoBase
{
	std::string m_Text = "Nameless";

	template <typename T>
	std::optional<T> CheckAndAssign(const json &Json, const std::string &key)
	{
		if (Json.contains(key))
		{
			if constexpr (std::is_same_v<T, int>)
			{
				if (Json[key].is_number_integer())
					return Json[key].get<int>();
			}
			else 
			if constexpr (std::is_same_v<T, float>)
			{
				if (Json[key].is_number_float())
					return Json[key].get<float>();
			}
			else 
			if constexpr (std::is_same_v<T, std::string>)
			{
				if (Json[key].is_string())
					return Json[key].get<std::string>();
			}
		}

		return std::nullopt; // We return nullopt if the key is missing or the type does not match
	}

	explicit CInfoBase(const json &Json);
};

struct CInfoMarketBase : public CInfoBase
{
	float m_Price = 0.0f;

	explicit CInfoMarketBase(const json &Json);
};

struct CPortfolioStockInfo : public CInfoMarketBase
{
	int m_Quantity = 0;

	explicit CPortfolioStockInfo(const json &Json);
};

struct CStockInfo : public CInfoMarketBase
{
	std::string m_StartStrategy = "None";

	explicit CStockInfo(const json &Json);
};

struct CTutorialInfo : public CInfoBase
{
	int m_Delay = 0;

	explicit CTutorialInfo(const json &Json);

};

template <class T>
class CSettings
{
	std::vector<T> m_Infos;

public:

	void LoadFromJSON(const std::string &JSONName)
	{
		std::ifstream file(JSONName.data());

		if (!file.is_open())
			throw std::runtime_error("Failed to open file: " + std::string(JSONName));
		
		try
		{
			const auto Json = json::parse(file);

			for (const auto &Item : Json)
				m_Infos.emplace_back(Item);
		}
		catch (const json::parse_error &e)
		{
			throw std::runtime_error("JSON parse error: " + std::string(e.what()));
		}
	}

	const std::vector<T> &GetInfos() const
	{
		return m_Infos;
	}
};

#endif // INFO_H