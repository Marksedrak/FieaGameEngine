#pragma once
#include <vector>
#include "Event.h"

namespace Fiea::GameEngine {
	class EventQueue {
	public:

		//static EventQueue& Instance();

		template <typename T>
		static void Enqueue(Event<T>* event, const GameTime& currentTime, const GameTime& delay) {
			Queue<T>.push_back(event); //Issue with this is that there are no different Queues, try to create a queues unordered_map using type and event.
			// For now this will create memory leaks. Do fix when you can!!
			// Think of a GetQueue<T> method that returns the correct vector depending on the Event's type
			event->TimeEnqueued() = currentTime;
			event->Delay() = delay;
		}

		template <typename T>
		static void Update(GameTime time) {
			for (auto event : Queue<T>) {
				if (event->IsExpired(time)) 
				{
					event->Deliver();
				}
			}
		}

		template <typename T>
		static void Clear() {
			//Update(GameClock::Current());
			for (Event<T>* event : Queue<T>)
			{
				delete event;
			}

			Queue<T>.clear();
		}

		template <typename T>
		static bool IsEmpty() {
			return Queue.empty();
		}

		template <typename T>
		static int Size() {
			return Queue.size();
		}

	private:
		
		template <typename T>
		static std::vector<Event<T>*> Queue;
	};

	template <typename T>
	std::vector<Event<T>*> EventQueue::Queue;
}