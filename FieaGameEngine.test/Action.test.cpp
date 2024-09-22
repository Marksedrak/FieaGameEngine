#include "pch.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "Factory.h"
#include "TableHelper.h"
#include "ParseCoordinator.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListWhile.h"
#include "ActionIncrement.h"
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace ActionTest
{
	TEST_CLASS(ActionTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::add(GameObject::TypeIdClass(), GameObject::Signatures());
			TypeManager::add(Action::TypeIdClass(), Action::Signatures());
			TypeManager::add(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeManager::add(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeManager::add(ActionListWhile::TypeIdClass(), ActionListWhile::Signatures());
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			FactoryManager<Scope>::Clear();
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
			TypeManager::Clear();
		}

		TEST_METHOD(ActionIncrementTest) {
			
			// Create Scope to hold 
			Scope MainChar;

			// Parse ActionTest.txt
			TableHelper::TableWrapper Twrapper(MainChar);
			ParseCoordinator parser(Twrapper);
			TableHelper* tHandler = new(TableHelper);
			parser.AddHandler(tHandler);

			// Creating and adding Factories
			ConcreteFactory(Scope, GameObject);
			ConcreteFactory(Scope, ActionIncrement);

			bool deserializedScope = parser.DeserializeObjectFromFile("ActionIncrementTest.txt");
			Assert::IsTrue(deserializedScope);

			// Extract Player Game Object
			GameObject* Player = MainChar.Find("Player")->GetScope()->As<GameObject>();

			Assert::IsFalse(Player == nullptr);
			
			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 0);
			
			// Create time
			GameClock clock;
			GameTime time = clock.Current();

			// Call Update which would in turn call ActionIncrement's Update;
			Assert::ExpectException<std::runtime_error>([&Player, &time] { Player->Update(time); }); // <- This will not work because IncompleteIncrement does not have a valid Datum to increment

			// Pull Incomplete increment
			ActionIncrement* AIncrIncomplete = Player->Actions(1)->GetScope()->As<ActionIncrement>();
			AIncrIncomplete->SetDatumKey("IncrementTest");
			AIncrIncomplete->SetValue(0);

			Player->Update(time);

			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 2); // Why from 0 to 2? well the earlier Update (inside of the ExpectException) still increments IncrementTest by 1 but fails

			// After this update IncrementTest should have been incremented twice by 1
			Player->Update(time);

			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 3);

			// Change ActionIncrement's value to 3
			ActionIncrement* AIncr = Player->Actions(0)->GetScope()->As<ActionIncrement>();
			AIncr->SetValue(3);

			// After this update IncrementTest should be 6
			Player->Update(time);

			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 6);

			// Changing ActionIncrement's target datum to Damage inside the Sword child
			AIncr->SetDatumKey("Sword.Damage");

			GameObject* Sword = Player->Find("Children")->GetScope()->Find("Sword")->GetScope()->As<GameObject>();

			Assert::AreEqual(Sword->Find("Damage")->Get<int>(), 12);

			// This should update Sword's Damage to 15 (12 + 3)
			Player->Update(time);

			Assert::AreEqual(Sword->Find("Damage")->Get<int>(), 15);

			// Throws exception if Datum does not exist in current 
			Assert::ExpectException<std::runtime_error>([&AIncr] { AIncr->SetDatumKey("Braveheart"); });

			// SetDatum to a float Datum
			AIncr->SetDatumKey("IncrementTestFloat");
			Assert::AreEqual(Player->Find("IncrementTestFloat")->Get<float>(), 2.4f);
			AIncr->SetValue(2.0);
			Player->Update(time);
			Assert::AreEqual(Player->Find("IncrementTestFloat")->Get<float>(), 4.4f);
			Player->Update(time);
			Assert::AreEqual(Player->Find("IncrementTestFloat")->Get<float>(), 6.4f);

			// Incrementing an element in an Array of Ints
			AIncr->SetDatumKey("IncrementArrayTest[2]");
			Assert::AreEqual(Player->Find("IncrementArrayTest")->Get<int>(2), 4);
			Player->Update(time);
			Assert::AreEqual(Player->Find("IncrementArrayTest")->Get<int>(2), 6);

			// Incrementing an element in an Array of Floats
			AIncr->SetDatumKey("IncrementArrayTestFloat[1]");
			Assert::AreEqual(Player->Find("IncrementArrayTestFloat")->Get<float>(1), 5.5f);
			AIncr->SetValue(-1.2f);
			Player->Update(time);
			Assert::AreEqual(Player->Find("IncrementArrayTestFloat")->Get<float>(1), 4.3f);

			// How to incorrectly use the [] notation
			Assert::ExpectException<std::invalid_argument>([&AIncr] { AIncr->SetDatumKey("IncrementArrayTest[gg]"); });

			// If for some reason the user decides to set parent to nullptr it throws an exception when trying to SetDatumKey
			AIncr->SetParent(nullptr);
			Assert::ExpectException<std::runtime_error>([&AIncr] { AIncr->SetDatumKey("IncrementTest"); });

			// Copying this actionIncrement into Another ActionIncrement
			ActionIncrement* AI2 = AIncr->Clone();
			Assert::IsTrue(*AI2 == AIncr);
			// To prove they are not the same ActionIncrement
			AI2->SetValue(24);
			Assert::IsFalse(*AI2 == AIncr);
			delete AI2;
		}

		TEST_METHOD(ActionListTest) {

			// Create Scope to hold 
			Scope MainChar;

			// Parse ActionTest.txt
			TableHelper::TableWrapper Twrapper(MainChar);
			ParseCoordinator parser(Twrapper);
			TableHelper* tHandler = new(TableHelper);
			parser.AddHandler(tHandler);

			// Creating and adding Factories
			ConcreteFactory(Scope, GameObject);
			ConcreteFactory(Scope, ActionIncrement);
			ConcreteFactory(Scope, ActionList);

			bool deserializedScope = parser.DeserializeObjectFromFile("ActionListTest.txt");
			Assert::IsTrue(deserializedScope);

			GameObject* Player = MainChar.Find("Player")->GetScope()->As<GameObject>();

			ActionIncrement* Increment1 = Player->Actions(0)->GetScope()->Find("Actions")->GetScope()->Find("Increment1")->GetScope()->As<ActionIncrement>();
			ActionIncrement* Increment2 = Player->Actions(0)->GetScope()->Find("Actions")->GetScope()->Find("Increment2")->GetScope()->As<ActionIncrement>();

			// Initiate Time
			GameClock clock;
			GameTime time = clock.Current();

			// ---------- Testing Update ---------------

			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 0);
			Assert::AreEqual(Player->Find("IncrementTestFloat")->Get<float>(), 2.4f);

			// Should "Call" Increment1 and Increment2
			Player->Update(time);

			// IncrementTest should now be incremented by 2
			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 2);
			// IncrementTestFloat should now be 2.5
			Assert::AreEqual(Player->Find("IncrementTestFloat")->Get<float>(), 2.5f);

			// ---------- Testing AddAction ----------------

			// Creating a Clone of Increment1 and storing it Increment3 ( a new ActionIncrement )
			ActionIncrement* Increment3 = Increment1->Clone();
			ActionList* AList = Player->Actions(0)->GetScope()->As<ActionList>();
			// Add Increment3 to AList
			AList->AddAction(Increment3);
			Increment3->SetValue(1.0);

			// So now if I call update IncrementTest should be incremented by 3 and IncrementTestFloat will be incremented by 0.1
			Player->Update(time);
			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 5);
			Assert::AreEqual(Player->Find("IncrementTestFloat")->Get<float>(), 2.6f);


			// ---------- Testing Clone ----------------
			ActionList* ALClone = AList->Clone();
			Assert::IsTrue(*ALClone == AList);
			ALClone->SetName("CloneAList");
			// Confirming that they are not both the same ActionList
			Assert::IsFalse(*ALClone == AList);
			// Illegaly add a GameObject in Actions
			GameObject* IllegalGameObject = new GameObject();
			ALClone->Find("Actions")->GetScope()->Adopt(*IllegalGameObject, "IllegalObject");
			// ExpectException when Calling update
			Assert::ExpectException<std::invalid_argument>([&ALClone, &time] { ALClone->Update(time);  });

			delete ALClone;
		}

		TEST_METHOD(ActionListWhileTest){
			// Create Scope to hold 
			Scope MainChar;

			// Parse ActionTest.txt
			TableHelper::TableWrapper Twrapper(MainChar);
			ParseCoordinator parser(Twrapper);
			TableHelper* tHandler = new(TableHelper);
			parser.AddHandler(tHandler);

			// Creating and adding Factories
			ConcreteFactory(Scope, GameObject);
			ConcreteFactory(Scope, ActionIncrement);
			ConcreteFactory(Scope, ActionListWhile);

			bool deserializedScope = parser.DeserializeObjectFromFile("ActionListWhileTest.txt");
			Assert::IsTrue(deserializedScope);

			GameObject* Player = MainChar.Find("Player")->GetScope()->As<GameObject>();

			GameClock clock;
			GameTime time = clock.Current();

			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 0);
			Player->Update(time);

			// After the ActionListWhile updates it shold have gone 5 times and each adding 5 to IncrementTest so it should be 25 now
			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 25);

			// ---------------- Testing Clone ----------------

			ActionListWhile* ALW = Player->Actions(0)->GetScope()->As<ActionListWhile>();
			ActionListWhile* CloneWhile = ALW->Clone();

			Player->Actions()->GetScope()->Adopt(*CloneWhile, "OtherALW");

			// Now if Update is called, IncrementTest should be incremented by 50 since both ActionListWhiles will update it.
			Player->Update(time);
			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 75);

			// Clear Preambble in Clone
			Scope* Reset = CloneWhile->Find("Preamble")->GetScope()->Find("Reset")->GetScope()->Orphan();
			// If Update is called now, it should only add 25 again since the preamble no longer resets condition
			Player->Update(time);
			Assert::AreEqual(Player->Find("IncrementTest")->Get<int>(), 100);
			// Let Try Adding a GameObject instead of an action in Preamble
			GameObject* newObject = new GameObject;
			CloneWhile->Find("Preamble")->GetScope()->Adopt(*newObject, "Illegal");
			// Calling Update should throw an exception because Preamble should only contain objects
			Assert::ExpectException<std::runtime_error>([&Player, &time] { Player->Update(time); });

			// Deleting Illegal object
			CloneWhile->Find("Preamble")->GetScope()->Clear();
			

			// Re-Adopting Reset
			CloneWhile->Find("Preamble")->GetScope()->Adopt(*Reset, "Reset");

			// Adding GameObject to Actions which should throw an exception
			GameObject* otherNewObject = new GameObject;
			CloneWhile->Find("Actions")->GetScope()->Adopt(*otherNewObject, "IllegalObject");
			Assert::ExpectException<std::runtime_error>([&Player, &time] { Player->Update(time); });

			CloneWhile->Find("Actions")->GetScope()->Find("IllegalObject")->GetScope()->Clear();
			

			// Setting Condition to an invalid Datum key would also result in an exception
			CloneWhile->SetCondition("YabadabaDooo");
			Assert::ExpectException<std::invalid_argument>([&Player, &time] { Player->Update(time); });
		}


	private:
		inline static _CrtMemState _startMemState;
	};
}