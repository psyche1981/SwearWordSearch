#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>


class Observable;
class Notifier;

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void Notify(Observable* _obs) = 0;
};

class Observable
{
public:
	Observable();
	virtual ~Observable();

	bool AddObserver(Observer* obs);
	bool RemoveObserver(Observer* obs);
	bool NotifyObservers();

protected:
	std::vector<Observer*> _observers;
};

class Notifier
{
public:
	Notifier(Observable* obs) : _observable(obs) {}

	void operator()(Observer* obs)
	{
		obs->Notify(_observable);
	}

private:
	Observable* _observable;
};