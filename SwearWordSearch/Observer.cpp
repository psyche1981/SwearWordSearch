#include "Observer.h"

Observer::Observer()
{
	
}

Observer::~Observer()
{
	
}

Observable::Observable()
{
	
}

Observable::~Observable()
{
	
}

bool Observable::AddObserver(Observer * obs)
{
	auto temp = std::find(_observers.begin(), _observers.end(), obs);
	if (temp != _observers.end())
	{
		//dont add duplicates
		return false;
	}
	_observers.push_back(obs);
	return true;
}

bool Observable::RemoveObserver(Observer* obs)
{
	auto temp = std::find(_observers.begin(), _observers.end(), obs);
	if (temp == _observers.end())
	{
		//not in there, so cant remove
		return false;
	}
	_observers.erase(std::remove(_observers.begin(), _observers.end(), obs));
	return true;
}

bool Observable::NotifyObservers()
{
	std::for_each(_observers.begin(), _observers.end(), Notifier(this));
	return _observers.size() > 0;
}