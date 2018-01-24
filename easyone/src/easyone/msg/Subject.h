#pragma once

#include "msg/MessageID.h"

#include <memory>
#include <set>

namespace eone
{

class VariantSet;
class Observer;

class Subject
{
public:
	Subject(MessageID id);

	void RegisterObserver(Observer* o);
	void UnregisterObserver(Observer* o);

	void NotifyObservers(const VariantSet& variants);

private:
	MessageID m_id;

	// todo: Observer usually is wxWindow which is raw pointer.
	std::set<Observer*> m_observers;

}; // Subject

}