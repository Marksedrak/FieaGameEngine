#include "pch.h"
#include "CppUnitTest.h"
#include "TestParseHandler.h"
#include "TestIntHandler.h"
#include "TableHelper.h"
#include "ParseCoordinator.h"
#include "TestTypes.h"
#include <iostream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine::test;

namespace Testparser
{
	TEST_CLASS(TestParser)
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

		TEST_METHOD(IntParsing)
		{
			// Different json data entries for testing
			std::string intJson = R"({"int1":5})";
			std::string intArrayJson = R"({"intArray": [2, 4, 5, 6]})";
			std::string floatJson = R"({"float1":3.4})";
			std::string strJson = R"({"string1": "Hello"})";

			TestIntHandler::TestIntWrapper wrapper;
			ParseCoordinator parser(wrapper);
			TestIntHandler* intHandler = new(TestIntHandler);
			parser.AddHandler(intHandler);

			// This parsing should be successful
			bool deserializedInt = parser.DeserializeObject(intJson);
			
			// These should be successful
			Assert::IsTrue(deserializedInt);
			Assert::AreEqual(wrapper.GetInt(), 5);

			// Try to parse float data (should fail);
			bool deserializedFloat = parser.DeserializeObject(floatJson);

			Assert::IsFalse(deserializedFloat);
			//wrapper should stay unchanged since no new data was given
			Assert::AreEqual(wrapper.GetInt(), 5);

			//Try to parse string
			bool deserializeString = parser.DeserializeObject(strJson);

			Assert::IsFalse(deserializeString);
			Assert::AreEqual(wrapper.GetInt(), 5);

			// Testing for having an wrapper that isn't TestIntWrapper
			Wrapper testWrapper;
			ParseCoordinator failedParser(testWrapper);
			TestIntHandler* tesIntHandler = new(TestIntHandler);
			failedParser.AddHandler(tesIntHandler);

			bool deserializeFailedInt = failedParser.DeserializeObject(intJson);
			Assert::IsFalse(deserializeFailedInt);

			// Testing if it works with Int arrays
			bool deserializedIntArray = parser.DeserializeObject(intArrayJson);

			Assert::IsTrue(deserializedIntArray);
			Assert::AreEqual(wrapper.GetInt(), 2);
			Assert::AreEqual(wrapper.GetInt(1), 4);
			Assert::AreEqual(wrapper.GetInt(2), 5);
			Assert::AreEqual(wrapper.GetInt(3), 6);
			
			// Remove Handlers
			failedParser.RemoveHandler(tesIntHandler);
			parser.RemoveHandler(intHandler);
		}

		TEST_METHOD(DeserializeObject) {
			std::string intJson = R"({"int1":5})";

			TestIntHandler::TestIntWrapper wrapper;
			ParseCoordinator parser(wrapper);
			TestIntHandler* intHandler = new(TestIntHandler);
			parser.AddHandler(intHandler);

			// DeserializeObject(const std::string&)
			bool deserializedInt = parser.DeserializeObject(intJson);

			Assert::IsTrue(deserializedInt);
			Assert::AreEqual(wrapper.GetInt(), 5);

			// DeserializeObjectFromFile(const string& filename)
			bool deserializedIntFromFile = parser.DeserializeObjectFromFile("Monster.json");

			Assert::IsTrue(deserializedIntFromFile);
			Assert::AreEqual(wrapper.GetInt(), 20);

			// DeserializeObject(std::istream& jsonStream)
			std::istringstream inputStream(intJson);
			bool deserializedIntFromStream = parser.DeserializeObject(inputStream);

			Assert::IsTrue(deserializedIntFromFile);
			Assert::AreEqual(wrapper.GetInt(), 5);
		}

		TEST_METHOD(NestedObject) 
		{ 
			std::string nested = R"({
                "int":1,
                "obj":{
                    "int":1,
                    "obj":{
                        "int":1,
                        "str":"abc"
                    },
                    "str":"abc"
                },
                "obj2":{
                    "int":1,
                    "obj":{
                        "int":1,
                        "str":"abc"
                    },
                    "str":"abc"
                },
                "str":"abc"
            })";
			TestParseHandler::TestWrapper wrapper;
			ParseCoordinator parser(wrapper);
			TestParseHandler* testHandler = new(TestParseHandler);
			parser.AddHandler(testHandler);
			bool deserialized = parser.DeserializeObject(nested);
			Assert::IsTrue(deserialized);
			Assert::IsTrue(testHandler->initializeCalled);
			Assert::IsTrue(testHandler->cleanupCalled);
			Assert::AreEqual((size_t)14, testHandler->startCount);
			Assert::AreEqual((size_t)14, testHandler->endCount);
			Assert::AreEqual((size_t)3, wrapper.maxDepth);
			parser.RemoveHandler(testHandler);
		}

		TEST_METHOD(TableHandling) {
			std::string intTest = R"-({
				"Health": 500,
				"DPS": 45,
				"AoE": 23.4,
				"SpecialSkill": "Roar",
				"Velocity": "Vec4(2.3, 4.5, 3.5, 4.8)",
				"Matrix": "Mat4((1.4,2.5,3.5,4.5),(1.4,2.5,3.5,4.5),(1.4,2.5,3.5,4.5),(1.4,2.5,3.5,4.5))",
				"Numbers": [2,3,4,1],
				"Buffs": [2.4, 5.2, 4.4, 5.5],
				"Special Abilities": ["Charge", "Heavy Attack", "Solar Flare"],
				"Enemy Directions": ["Vec4(2.5, 3.5, 4.4, 2.1)", "Vec4(2.6, 4.5, 3.4, 21.3)"],
				"MatrixInfo": [
					"Mat4((1.4,2.5,3.5,4.5), 
							(1.4,2.5,3.5,4.5), 
							(1.4,2.5,3.5,4.5), 
							(1.4,2.5,3.5,4.5))",
					"Mat4((2.4,4.5,6.5,4.5), 
							(1.4,2.5,3.5,4.5), 
							(1.4,2.5,3.5,4.5), 
							(1.4,2.5,3.5,4.5))",
					"Mat4((55.5,4.5, 6.5, 4.5), 
							(1.4, 4.4, 3.5, 4.5), 
							(1.4, 3.4, 3.5, 4.5), 
							(1.4, 2.5, 0.0, 4.5))"
				],
				"Club" : {
					"Damage": 25,
					"Name": "Stone Club",
					"Range": "3.5"
				}
			})-";

			Scope Monster;
			TableHelper::TableWrapper Twrapper(Monster);
			ParseCoordinator parser(Twrapper);
			TableHelper* tHandler = new(TableHelper);
			parser.AddHandler(tHandler);
			
			bool deserializedScope = parser.DeserializeObject(intTest);
			Assert::IsTrue(deserializedScope);

			// Make sure Table Handler handles the main Datum types
			Assert::AreEqual(Monster["Health"].Get<int>(), 500);
			Assert::AreEqual(Monster["DPS"].Get<int>(), 45);
			Assert::AreEqual(Monster["AoE"].Get<float>(), 23.4f);
			Assert::AreEqual(Monster["SpecialSkill"].Get<string>(), string("Roar"));
			Assert::AreEqual(Monster["Velocity"].Get<glm::vec4>(), glm::vec4(2.3f, 4.5f, 3.5f, 4.8f));
			Assert::AreEqual(Monster["Matrix"].Get<glm::mat4>(), glm::mat4(1.4, 2.5, 3.5, 4.5, 1.4, 2.5, 3.5, 4.5, 1.4, 2.5, 3.5, 4.5, 1.4, 2.5, 3.5, 4.5));

			// Test for Arrays
			
			// IntArray
			Assert::AreEqual(Monster["Numbers"].Get<int>(), 2);
			Assert::AreEqual(Monster["Numbers"].Get<int>(1), 3);
			Assert::AreEqual(Monster["Numbers"].Get<int>(2), 4);
			Assert::AreEqual(Monster["Numbers"].Get<int>(3), 1);

			// Float Array
			Assert::AreEqual(Monster["Buffs"].Get<float>(), 2.4f);
			Assert::AreEqual(Monster["Buffs"].Get<float>(1), 5.2f);
			Assert::AreEqual(Monster["Buffs"].Get<float>(2), 4.4f);
			Assert::AreEqual(Monster["Buffs"].Get<float>(3), 5.5f);

			// String Array
			Assert::AreEqual(Monster["Special Abilities"].Get<string>(), string("Charge"));
			Assert::AreEqual(Monster["Special Abilities"].Get<string>(1), string("Heavy Attack"));
			Assert::AreEqual(Monster["Special Abilities"].Get<string>(2), string("Solar Flare"));

			// Vector Array
			Assert::AreEqual(Monster["Enemy Directions"].Get<glm::vec4>(), glm::vec4(2.5, 3.5, 4.4, 2.1));
			Assert::AreEqual(Monster["Enemy Directions"].Get<glm::vec4>(1), glm::vec4(2.6, 4.5, 3.4, 21.3));

			// Matrix Array
			Assert::AreEqual(Monster["MatrixInfo"].Get<glm::mat4>(), glm::mat4(1.4, 2.5, 3.5, 4.5,
				1.4, 2.5, 3.5, 4.5,
				1.4, 2.5, 3.5, 4.5,
				1.4, 2.5, 3.5, 4.5));
			Assert::AreEqual(Monster["MatrixInfo"].Get<glm::mat4>(1), glm::mat4(2.4, 4.5, 6.5, 4.5,
				1.4, 2.5, 3.5, 4.5,
				1.4, 2.5, 3.5, 4.5,
				1.4, 2.5, 3.5, 4.5));
			Assert::AreEqual(Monster["MatrixInfo"].Get<glm::mat4>(2), glm::mat4(55.5, 4.5, 6.5, 4.5,
				1.4, 4.4, 3.5, 4.5,
				1.4, 3.4, 3.5, 4.5,
				1.4, 2.5, 0.0, 4.5));
			
			// Test for Scopes
			Assert::AreEqual(Monster["Club"].GetScope()->Find("Damage")->Get<int>(), 25);
			Assert::AreEqual(Monster["Club"].GetScope()->Find("Name")->Get<string>(), string("Stone Club"));
			Assert::AreEqual(Monster["Club"].GetScope()->Find("Range")->Get<float>(), 3.5f);

			// Make sure depth is updated

			parser.RemoveHandler(tHandler);
		}


	private:
		inline static _CrtMemState _startMemState;
	};
}