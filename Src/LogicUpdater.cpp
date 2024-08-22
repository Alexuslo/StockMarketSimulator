#include "LogicUpdater.h"
#include "StockMarketModel.h"
#include "Definitions.h"
#include "Application.h"

CLogicUpdater::CLogicUpdater(CStockMarketModel &StockMarketModel) : m_StockMarketModel(StockMarketModel),
m_bRunningUpdateLogicThread(false)
{

}

CLogicUpdater::~CLogicUpdater()
{
	Stop();
}

/*

This code was placed in a separate thread not to break
interconnections by ripping out the update function and thus creating 
a competition between threads for the objects of the “Model” component
and the “View” object (especially since the update function fits well 
into the main thread), but to show the possibility of autonomous and 
parallel operation of the logic in the form of a “market simulator” 
as a separate, independent entity.

Данный код был вынесен в отдельный поток не для того, чтоб нарушить 
взаимосвязи, вырвав функцию обновления и тем самым создав конкурентную 
борьбу потоков за объекты компонента "Model" и объект "View" (тем более, 
что функция обновления хорошо вписывается в основную нить), а для того, 
чтоб показать возможность автономной и прарралельной работы логики в виде 
"симулятора рынка" как отдельной, независимой сущности.

*/

void CLogicUpdater::Run()
{
	m_bRunningUpdateLogicThread.store(true);

	m_UpdateLogicThread = std::thread([this]() {
		
		auto NextTutorialUpdateTime = std::chrono::steady_clock::now();
		auto NextPriceUpdateTime = NextTutorialUpdateTime + std::chrono::seconds(MAX_SECOND_FOR_UPDATE_PRICE);
		auto NextStrategyUpdateTime = NextTutorialUpdateTime + std::chrono::seconds(MAX_SECOND_FOR_UPDATE_STRATEGY);

		size_t TutorialStep = 0;

		const auto &TutorialInfo = CApplication::GetInstance().GetTutorialInfos();
		bool TutorialStarted = !TutorialInfo.empty();
		CApplication::GetInstance().SetTutorialPaused(TutorialStarted);

		while (m_bRunningUpdateLogicThread.load())
		{
			auto Now = std::chrono::steady_clock::now();

			// Price update
			if (Now >= NextPriceUpdateTime)
			{
				m_StockMarketModel.UpdatePrices();
				Notify(EDrawFlagFlags::DrawStockFlag);
				Notify();
				NextPriceUpdateTime = Now + std::chrono::seconds(MAX_SECOND_FOR_UPDATE_PRICE);
			}

			// Strategies update
			if (Now >= NextStrategyUpdateTime)
			{
				m_StockMarketModel.UpdateStrategy();
				Notify(EDrawFlagFlags::DrawStockFlag);
				Notify();
				NextStrategyUpdateTime = Now + std::chrono::seconds(MAX_SECOND_FOR_UPDATE_STRATEGY);
			}

			// Tutorials update
			if (TutorialStarted && (Now >= NextTutorialUpdateTime))
			{
				if (TutorialStep >= TutorialInfo.size())
				{
					TutorialStarted = false;
					CApplication::GetInstance().SetTutorialPaused(TutorialStarted);
				}
				else
				{
					NextTutorialUpdateTime = Now + std::chrono::milliseconds(TutorialInfo[TutorialStep].m_Delay);

					m_StockMarketModel.AddLogMessage(TutorialInfo[TutorialStep].m_Text);

					TutorialStep++;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	});
}

void CLogicUpdater::Stop()
{
	m_bRunningUpdateLogicThread.store(false);

	if (m_UpdateLogicThread.joinable())
		m_UpdateLogicThread.join();
}
