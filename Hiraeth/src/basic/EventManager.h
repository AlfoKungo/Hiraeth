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
	template <typename... Ts>
	class Event : public BaseEvent {
		// To store all listeners of the event
		std::vector<std::function<void(Ts...)>> handlers;
	public:

		void addListener(std::function<void(Ts...)> func) {
			handlers.push_back(func);
		}

		void execute(Ts... ts) {
			for (auto & handler : handlers) {
				handler(ts...);
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

		template <typename Class, typename... Ts>
		//bool subscribe(EventList name, std::function<void(Ts...)>& func) {
		void subscribe(EventList name, Class * object, void (Class::*func)(Ts...)) {
			if (m_Events.find(name) == m_Events.end())
				m_Events[name] = new Event<Ts...>();
			Event<Ts...> * sEvent = dynamic_cast<Event<Ts...> *>(m_Events[name]);
			if (sEvent == nullptr)
				throw std::invalid_argument("argument 3 is unfit");

			sEvent->addListener(std::function<void(Ts...)>([object, func](Ts... ts) {return (object->*func)(ts...); }));
		}

		template <typename... Ts>
		void execute(EventList name, Ts... ts) {
			if (m_Events.find(name) != m_Events.end())
				static_cast<Event<Ts...> *>(m_Events[name])->execute(ts...);
		}
	};
}