#include "pch.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "Hero.h"
#include "Factory.h"
#include "TableHelper.h"
#include "ParseCoordinator.h"
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace GameObjectTest
{
	TEST_CLASS(GameObjectTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::add(GameObject::TypeIdClass(), GameObject::Signatures());
			TypeManager::add(Hero::TypeIdClass(), Hero::Signatures());
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
			TypeManager::Clear();
		}

		TEST_METHOD(Constructor) {
			GameObject* Monster = new GameObject();
			Monster->Name = "Beast";
			Monster->ObjTransform.Position = Vec4(2.4f, 3.6f, 2.2f, 9.8f);
			Monster->ObjTransform.Rotation = Vec4(38.5f, 48.5f, 22.8f, 10.0f);
			Monster->ObjTransform.Scale = Vec4(10.0f, 10.0f, 33.3f, 0.0f);

			Assert::AreEqual(Monster->Find("Name")->Get<string>(), string("Beast"));
			Assert::AreEqual(Monster->Find("Position")->Get<Vec4>(), Vec4(2.4f, 3.6f, 2.2f, 9.8f));
			Assert::AreEqual(Monster->Find("Rotation")->Get<Vec4>(), Vec4(38.5f, 48.5f, 22.8f, 10.0f));
			Assert::AreEqual(Monster->Find("Scale")->Get<Vec4>(), Vec4(10.0f, 10.0f, 33.3f, 0.0f));
			delete Monster;

			Hero* Paladin = new Hero();
			Paladin->Name = "Varolik";
			Paladin->ObjTransform.Position = Vec4(2.4f, 3.6f, 2.2f, 9.8f);
			Paladin->ObjTransform.Rotation = Vec4(38.5f, 48.5f, 22.8f, 10.0f);
			Paladin->ObjTransform.Scale = Vec4(10.0f, 10.0f, 33.3f, 0.0f);
			Paladin->PassiveName = "Divine Sense";
			Paladin->HeroName = "White Knight";

			Assert::AreEqual(Paladin->Find("Name")->Get<string>(), string("Varolik"));
			Assert::AreEqual(Paladin->Find("PassiveName")->Get<string>(), string("Divine Sense"));
			Assert::AreEqual(Paladin->Find("HeroName")->Get<string>(), string("White Knight"));
			Assert::AreEqual(Paladin->Find("Position")->Get<Vec4>(), Vec4(2.4f, 3.6f, 2.2f, 9.8f));
			Assert::AreEqual(Paladin->Find("Rotation")->Get<Vec4>(), Vec4(38.5f, 48.5f, 22.8f, 10.0f));
			Assert::AreEqual(Paladin->Find("Scale")->Get<Vec4>(), Vec4(10.0f, 10.0f, 33.3f, 0.0f));

			delete Paladin;
		}

		// Cause why not
		TEST_METHOD(Clone) {
			GameObject* Monster = new GameObject();
			Monster->Name = "Friend";

			GameObject* otherMonster = Monster->Clone();

			Assert::AreEqual(Monster->Name, otherMonster->Name);
			delete Monster;
			delete otherMonster;
		}

		TEST_METHOD(AddChild) {
			GameObject* Hero = new GameObject();
			GameObject* Player = new GameObject();
			Hero->Name = "Hero";
			Hero->AddChild(Player);

			Scope* notObject = new Scope();
			Assert::IsFalse(Player->AddChild(notObject));

			delete notObject;

			delete Player;
			delete Hero;
		}

		TEST_METHOD(RemoveChild) {
			GameObject* Player = new GameObject();
			GameObject* Sword = new GameObject();

			Sword->Name = "Sword";

			Player->Name = "Gandalf";
			Player->AddChild(Sword);

			// Drop Sword
			Assert::IsTrue(Player->RemoveChild(Sword));

			Player->AddChild(Sword);

			Assert::IsTrue(Player->RemoveChild(Sword));

			delete Sword;
			delete Player;
		}

		TEST_METHOD(Update) {
			Hero* Batman = new Hero();
			Hero* Robin = new Hero();
			GameObject* Player = new GameObject();
			GameClock clock;
			GameTime time = clock.Current();
			Batman->Name = "Batman";
			Player->AddChild(Batman);
			Batman->AddChild(Robin);
			Player->Update(time);


			Assert::IsTrue(Player->Updated);
			Assert::IsTrue(Batman->Updated);
			Assert::IsTrue(Robin->Updated);

			delete Player;
		}

		TEST_METHOD(ParsingFromJson) {
			Scope MainChar;
			TableHelper::TableWrapper Twrapper(MainChar);
			ParseCoordinator parser(Twrapper);
			TableHelper* tHandler = new(TableHelper);
			parser.AddHandler(tHandler);
			ConcreteFactory(Scope, Hero);
			ConcreteFactory(Scope, GameObject);
			bool deserializedScope = parser.DeserializeObjectFromFile("Hero.txt");
			Assert::IsTrue(deserializedScope);

			Assert::AreEqual(MainChar["MainCharacter"].GetScope()->Find("Name")->Get<string>(), string("Barry Allen"));
			Hero* MainCharacter = MainChar["MainCharacter"].GetScope()->As<Hero>();

			//Checking casting was successful
			Assert::IsTrue(MainCharacter != nullptr);

			Assert::AreEqual(MainCharacter->Name, std::string("Barry Allen"));

			Assert::AreEqual(MainCharacter->Find("PassiveName")->Get<string>(), string("Super Speed"));
			Assert::AreEqual(MainCharacter->Find("HeroName")->Get<string>(), string("The Flash"));

			Scope* MainCharScope = MainChar["MainCharacter"].GetScope();
			Assert::AreEqual(MainCharScope->Find("Class")->Get<string>(), string("Speedster"));

			GameObject* Sword = MainCharacter->Find("Children")->GetScope()->Find("Sword")->GetScope()->As<GameObject>();
			Assert::IsFalse(Sword == nullptr);

			Sword->ObjTransform.Position = Vec4(0, 0, 0, 1);
			Sword->ObjTransform.Rotation = Vec4(1, 1, 1, 1);
			Sword->ObjTransform.Scale = Vec4(1, 1, 1, 1);


			Assert::AreEqual(Sword->Find("Name")->Get<string>(), string("Excalibur"));
			Assert::AreEqual(Sword->Find("Position")->Get<Vec4>(), Vec4(0, 0, 0, 1));
			Assert::AreEqual(Sword->Find("Rotation")->Get<Vec4>(), Vec4(1, 1, 1, 1));
			Assert::AreEqual(Sword->Find("Scale")->Get<Vec4>(), Vec4(1, 1, 1, 1));
			Assert::AreEqual(Sword->Find("Damage")->Get<int>(), 25);
			Assert::AreEqual(Sword->Find("Range")->Get<int>(), 5);
			Assert::AreEqual(Sword->Find("Durability")->Get<float>(), 99.9f);
			Assert::AreEqual(Sword->Find("Subclass")->Get<string>(), string("Longsword"));

			FactoryManager<Scope>::Remove("GameObject");
			FactoryManager<Scope>::Remove("Hero");
		}



	private:
		inline static _CrtMemState _startMemState;
	};
}