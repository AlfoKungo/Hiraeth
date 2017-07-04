#pragma once
//
//  EventManager.h
//  EventManager
//
//  Created by Mayank Saini on 15/12/12.
//  Copyright (c) 2012 Hashstash Studios. All rights reserved.
//

#include <vector>
#include <map>
#include <string>

using namespace std;
namespace hiraeth {

	enum EventList
	{
		StatsUpdate,
		MapChanged,
		InventoryUpdated,
	};


#define TEMPLATE template <typename Class> 

	// Abstract Class for EventHandler to notify of a change
	class EventHandlerBase {
	public:
		virtual ~EventHandlerBase() = default;
		virtual void execute() = 0;
	};

	// Event Handler Class : Handles Callback
	template <typename Class>
	class EventHandler : public EventHandlerBase {
		// Defining type for function pointer
		typedef void (Class::*_fptr)(void);



	public:
		// Object of the Listener
		Class *object;
		// Function for callback
		_fptr function;

		EventHandler(Class *obj, _fptr func) {
			object = obj;
			function = func;
		}

		void execute() override
		{
			(object->*function)();
		}
	};

	// Class to create a event
	class Event {
		// To store all listeners of the event
		typedef std::map<int, EventHandlerBase*> EventHandlerMap;
		EventHandlerMap handlers;
		int count;
	public:

		template <typename Class>
		void addListener(Class *obj, void (Class::*func)(void)) {
			handlers[count] = new EventHandler<Class>(obj, func);
			count++;
		}

		void execute() {
			for (EventHandlerMap::iterator it = handlers.begin(); it != handlers.end(); ++it) {
				it->second->execute();
			}
		}

	};

	class EventManager {
		struct EventType {
			Event *event;
			EventList name;
		};

		std::vector<EventType> _events;

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
			for (vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
				EventType e = *it;
				if (e.name == name)
					return;
			}
			EventType e;
			e.event = new Event();
			e.name = name;
			_events.push_back(e);
		}

		template <typename Class>
		bool subscribe(EventList name, Class *obj, void (Class::*func)(void)) {
			for (vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
				EventType e = *it;
				if (e.name == name) {
					e.event->addListener(obj, func);
					return true;
				}
			}
			return false;
		}

		void execute(EventList name) {
			for (vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
				EventType e = *it;
				if (e.name == name) {
					e.event->execute();
				}
			}
		}
	};

}