#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <algorithm>

#include "Definitions.h"

class CSender;

class CObserver
{
	friend class CSender;

	std::vector<CSender *> m_Senders;
	virtual void AddSender(CSender *pSender);

public:
	
	virtual ~CObserver();

	virtual void Notify() {}
	virtual void Notify(EDrawFlagFlags Flags) {}
};

class CSender 
{
	std::vector<CObserver *> m_Observers;
	
public:
	
	void Attach(CObserver *pObserver);
	void Detach(CObserver *pObserver);

	void Notify();
	void Notify(EDrawFlagFlags Flags);
};

#endif // OBSERVER_H
