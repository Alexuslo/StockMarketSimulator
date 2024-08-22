#include "Observer.h"

CObserver::~CObserver()
{
	for (auto pSender : m_Senders)
		if (pSender)
			pSender->Detach(this);
}

void CObserver::AddSender(CSender *pSender)
{
	if (!pSender)
		return;

	if (std::find(m_Senders.begin(), m_Senders.end(), pSender) == m_Senders.end())
	{
		m_Senders.push_back(pSender);
	}
}

void CSender::Attach(CObserver *pObserver)
{
	if (!pObserver)
		return;

	if (std::find(m_Observers.begin(), m_Observers.end(), pObserver) == m_Observers.end())
	{
		m_Observers.push_back(pObserver);

		pObserver->AddSender(this);
	}
}

void CSender::Detach(CObserver *pObserver)
{
	if (!pObserver)
		return;

	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), pObserver), m_Observers.end());
}

void CSender::Notify()
{
	for (const auto &Observer : m_Observers)
		Observer->Notify();
}

void CSender::Notify(EDrawFlagFlags Flags)
{
	for (const auto &Observer : m_Observers)
		Observer->Notify(Flags);
}
