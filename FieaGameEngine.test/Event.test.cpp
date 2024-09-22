#include "pch.h"
#include "CppUnitTest.h"
#include "EventSubscriber.h"
#include "EventPublisher.h"
#include "EventQueue.h"
#include "EventApplyPoison.h"
#include "Event.h"
#include "GameClock.h"
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace EventTest
{
	TEST_CLASS(EventTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtDumpMemoryLeaks();
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Event) {
			// Creating ApplyPoison Event
			StatusEffect effect;
			effect.StatusName = "Poison";
			effect.Damage = 10;
			GameClock clock;
			GameTime time = clock.Current();

			// Constructing an ApplyPoison (Currently throws linker errors)
			//ApplyPoison Poison(effect, true); // This is the that line throws a linker error for not finding a constructor for Event<StatusEffect>
			// EventQueue::Enqueue<ApplyPoison>(&Poison, time, time);
		}


	private:
		inline static _CrtMemState _startMemState;
	};
}