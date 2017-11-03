#pragma once
//
//  EventManager.h
//  EventManager
//
//  Created by Mayank Saini on 15/12/12.
//  Copyright (c) 2012 Hashstash Studios. All rights reserved.
//

#include <functional>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>

namespace hiraeth {

	enum EventList
	{
		StatsUpdate,
		MapChanged,
		InventoryUpdated,
	};

	class BaseEvent {
	public:
		virtual ~BaseEvent() = default;
	};


	// Class to create a event
	template <typename... Args>
	class Event : public BaseEvent {

		// To store all listeners of the event
		std::vector<std::function<void(Args...)>> handlers;
	public:

		void addListener(std::function<void(Args...)> func) {
			handlers.push_back(func);
		}

		void execute(Args... args) {
			for (auto & handler : handlers) {
				handler(args...);
			}
		}
	};


	class EventManager {

	private:
		std::map<EventList, BaseEvent*> m_Events;
		static EventManager *_Instance;

		EventManager() {};
	public:
		static EventManager* Instance() {
			if (!_Instance) {
				_Instance = new EventManager();
			}
			return _Instance;
		}

		void createEvent(EventList name) {
		}

		template <typename Class, typename... Args>
		//bool subscribe(EventList name, std::function<void(Ts...)>& func) {
		void subscribe(EventList name, Class * object, void (Class::*func)(Args...)) {
			if (m_Events.find(name) == m_Events.end())
				m_Events[name] = new Event<Args...>();
			Event<Args...> * sEvent = dynamic_cast<Event<Args...> *>(m_Events[name]);
			if (sEvent == nullptr)
				throw std::invalid_argument("argument 3 is unfit");

			sEvent->addListener(std::function<void(Args...)>([object, func](Args... args) {return (object->*func)(args...); }));
		}

		template <typename... Args>
		void execute(EventList name, Args... args) {
			if (m_Events.find(name) != m_Events.end())
				static_cast<Event<Args...> *>(m_Events[name])->execute(args...);
		}
	};
}