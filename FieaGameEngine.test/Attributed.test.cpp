#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace AttributedTest
{
	TEST_CLASS(AttributedTest)
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
#endif
			TypeManager::Clear();
		}

		TEST_METHOD(Constructor) {
			AttributedFoo aFoo;

			Assert::AreEqual(aFoo["externalInteger"].Get<int>(), 4);
			Assert::AreEqual(aFoo["externalFloat"].Get<float>(), 3.6f);
			Assert::AreEqual(aFoo["externalString"].Get<string>(), std::string("Hi"));
			Assert::AreEqual(aFoo["externalVector"].Get<glm::vec4>(), glm::vec4(2, 4, 1, 5));
			Assert::AreEqual(aFoo["externalMatrix"].Get<glm::mat4>(), glm::mat4(2.54f));
		}

		TEST_METHOD(CopyConstructor) {
			AttributedFoo Foo1;
			AttributedFoo Foo2(Foo1);

			Assert::AreEqual(Foo1["externalInteger"].Get<int>(), Foo2["externalInteger"].Get<int>());
			Assert::AreEqual(Foo1["externalFloat"].Get<float>(), Foo2["externalFloat"].Get<float>());
			Assert::AreEqual(Foo1["externalString"].Get<string>(), Foo2["externalString"].Get<string>());

			// Confirm they are not pointing to the same addresses
			Foo1.externalInteger = 200;
			Assert::AreNotEqual(Foo1["externalInteger"].Get<int>(), Foo2["externalInteger"].Get<int>());
			Foo2["externalInteger"].Get<int>() = 400;
			Assert::AreEqual(Foo1["externalInteger"].Get<int>(), 200);
			Assert::AreEqual(Foo2["externalInteger"].Get<int>(), 400);

			// Should change the value of Datum under the key "externalInteger"
			Foo2.externalInteger = 65;

			AttributedFoo Foo3 = Foo2;

			Assert::AreEqual(Foo3["externalInteger"].Get<int>(), Foo2["externalInteger"].Get<int>());
			Assert::AreEqual(Foo3["externalFloat"].Get<float>(), Foo2["externalFloat"].Get<float>());
			Assert::AreEqual(Foo3["externalString"].Get<string>(), Foo2["externalString"].Get<string>());

			float newFloat = 2.2f;
			Foo3["externalFloat"].Get<float>() = newFloat;
			
			Assert::AreNotEqual(Foo1["externalInteger"].Get<int>(), Foo2["externalInteger"].Get<int>());
			Assert::AreNotEqual(Foo3["externalFloat"].Get<float>(), Foo2["externalFloat"].Get<float>());
		}

		TEST_METHOD(MoveSemantics) {

			AttributedFoo F1;

			F1.externalInteger = 98;
			F1.externalFloat = 5.7f;
			F1.externalString = string("Luffy");

			AttributedFoo F2(std::move(F1));

			// Testing integrity of non-array elements

			Assert::AreEqual(F2["externalInteger"].Get<int>(), 98); // Int
			Assert::AreEqual(F2["externalFloat"].Get<float>(), 5.7f); // Float
			Assert::AreEqual(F2["externalString"].Get<string>(), string("Luffy")); // String
			Assert::AreEqual(F2["externalVector"].Get<glm::vec4>(), glm::vec4(2, 4, 1, 5)); // Vector
			Assert::AreEqual(F2["externalMatrix"].Get<glm::mat4>(), glm::mat4(2.54f)); // Matrix

			//Testing integrity of array elements

			// Int Array
			Assert::AreEqual(F2["externalIntegerArray"].Get<int>(), 1);
			Assert::AreEqual(F2["externalIntegerArray"].Get<int>(1), 4);
			Assert::AreEqual(F2["externalIntegerArray"].Get<int>(2), 6);

			// Float Array
			Assert::AreEqual(F2["externalFloatArray"].Get<float>(), 2.4f);
			Assert::AreEqual(F2["externalFloatArray"].Get<float>(1), 4.3f);
			Assert::AreEqual(F2["externalFloatArray"].Get<float>(2), 3.6f);

			// String Array
			Assert::AreEqual(F2["externalStringArray"].Get<std::string>(), std::string("Hello"));
			Assert::AreEqual(F2["externalStringArray"].Get<std::string>(1), std::string("It's"));
			Assert::AreEqual(F2["externalStringArray"].Get<std::string>(2), std::string("Me"));

			// Vector Array
			Assert::AreEqual(F2["externalVectorArray"].Get<glm::vec4>(), glm::vec4(2, 4, 1, 5));
			Assert::AreEqual(F2["externalVectorArray"].Get<glm::vec4>(1), glm::vec4(4, 2, 1, 5));

			// Matrix Array
			Assert::AreEqual(F2["externalMatrixArray"].Get<glm::mat4>(), glm::mat4(3.6f));
			Assert::AreEqual(F2["externalMatrixArray"].Get<glm::mat4>(1), glm::mat4(2.2f));
		}

		TEST_METHOD(IsAttribute) {
			AttributedFoo Foo;
			Assert::IsTrue(Foo.IsAttribute("externalInteger"));
			Assert::IsTrue(Foo.IsAttribute("externalFloat"));
			Assert::IsTrue(Foo.IsAttribute("externalString"));
			
			Assert::IsFalse(Foo.IsAttribute("NotAnAttribute"));
		}

		TEST_METHOD(IsPrescribedAttribute) {
			AttributedFoo Foo;
			Assert::IsTrue(Foo.IsPrescribedAttribute("externalInteger"));
			Assert::IsTrue(Foo.IsPrescribedAttribute("externalFloat"));
			Assert::IsTrue(Foo.IsPrescribedAttribute("externalString"));
			Assert::IsFalse(Foo.IsPrescribedAttribute("NotAnAttribute"));
		}

		TEST_METHOD(IsAuxiliaryAttribute) {
			AttributedFoo Foo;
			Assert::IsFalse(Foo.IsAuxiliaryAttribute("externalInteger"));
			// Also Testing AppendAuxiliaryAttribute
			Foo.AppendAuxiliaryAttribute("AuxAttribute");
			Assert::IsTrue(Foo.IsAuxiliaryAttribute("AuxAttribute"));
		}


		
	private:
		inline static _CrtMemState _startMemState;
	};
}