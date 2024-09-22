#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "TableHelper.h"
#include "AttributedFoo.h"
#include "ParseCoordinator.h"
#include "TestTypes.h"
#include <iostream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace FactoryTest
{
	TEST_CLASS(FactoryTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::add(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
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
			TypeManager::Clear();
#endif
		}

		TEST_METHOD(BasicTest) {
			// Simple test to see if basic functionality of Factory works

			ConcreteFactory(Scope, AttributedFoo); // My Macro creates the factory class and adds the factory to the FactoryManager at the same time

			const Scope* FooCreated = FactoryManager<Scope>::Create("AttributedFoo");
			Assert::AreEqual(FooCreated->Find("externalInteger")->Get<int>(), 4);
			
			// Destructing FooCreated
			FactoryManager<Scope>::Clear();
			delete FooCreated;
		}

		TEST_METHOD(TestWithParser) {
			Scope Foo;
			TableHelper::TableWrapper Twrapper(Foo);
			ParseCoordinator parser(Twrapper);
			TableHelper* tHandler = new(TableHelper);
			parser.AddHandler(tHandler);
			ConcreteFactory(Scope, AttributedFoo);
			bool deserializedScope = parser.DeserializeObjectFromFile("TestFactory.txt");
			Assert::IsTrue(deserializedScope);

			FactoryManager<Scope>::Remove("AttributedFoo");
			parser.RemoveHandler(tHandler);

			AttributedFoo* Foo1 = Foo["Foo1"].GetScope()->As<AttributedFoo>();

			// Prescribed Attributes do change
			Assert::AreEqual(Foo1->externalInteger, 2048);
			Assert::AreEqual(Foo1->externalFloat, 5.4f);
			Assert::AreEqual(Foo1->externalString, std::string("Hello"));

			// Testing Values including Auxiliary attributes added in json
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("HelperInt")->Get<int>(), 24);
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("TestFloat")->Get<float>(), 3.5f);
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("NewString")->Get<std::string>(), std::string("Factory F'in Works"));
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("NewVec")->Get<glm::vec4>(), glm::vec4(2.4f, 3.5f, 4.5f, 2.2f));
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("NewMatrix")->Get<glm::mat4>(), glm::mat4(2.5f, 1.8f, 4.4f, 3.2f, 2.5f, 1.8f, 4.4f, 3.2f, 2.5f, 1.8f, 4.4f, 3.2f, 2.5f, 1.8f, 4.4f, 3.2f));

			// Testing Array attributes
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("ObjArray")->GetScope()->Find("NestedArrayInt")->Get<int>(), 1);
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("ObjArray")->GetScope()->Find("NestedArrayInt")->Get<int>(1), 2);
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("ObjArray")->GetScope()->Find("NestedArrayInt")->Get<int>(2), 3);
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("ObjArray")->GetScope()->Find("NestedArrayInt")->Get<int>(3), 4);

			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("ObjArray")->GetScope()->Find("NestedArrayFloat")->Get<float>(), 2.3f);
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("ObjArray")->GetScope()->Find("NestedArrayFloat")->Get<float>(1), 4.5f);
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("ObjArray")->GetScope()->Find("NestedArrayFloat")->Get<float>(2), 2.4f);
			Assert::AreEqual(Foo["Foo1"].GetScope()->Find("ObjArray")->GetScope()->Find("NestedArrayFloat")->Get<float>(3), 1.5f);

			FactoryManager<Scope>::Clear();
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}