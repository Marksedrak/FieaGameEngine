#include "pch.h"
#include "CppUnitTest.h"
#include "RTTI.h"
#include "Foo.h"
#include "FooChild.h"
#include "Empty.h"
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace RTTITest
{
	TEST_CLASS(RTTITest)
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

		TEST_METHOD(As) {
			// FooChild can be converted to Foo since Foo is part of it
			FooChild child;
			Foo* unchild = child.As<Foo>();
			Assert::IsTrue(unchild == &child);

			// Empty can't be converted to a Foo so it should return nullptr
			Empty empty;
			Foo* emptyFoo = empty.As<Foo>();
			Assert::IsTrue(emptyFoo == nullptr);
		}

		TEST_METHOD(Is) {
			// FooChild is a Foo
			FooChild child;
			Foo foo;
			Assert::IsTrue(child.Is(foo.TypeIdClass()));

			// Foo is not a FooChild
			Assert::IsFalse(foo.Is(child.TypeIdClass()));

			// Empty is neither Foo nor FooChild and viceversa
			Empty empt;
			Assert::IsFalse(empt.Is(foo.TypeIdClass()));
			Assert::IsFalse(empt.Is(child.TypeIdClass()));
			Assert::IsFalse(foo.Is(empt.TypeIdClass()));
			Assert::IsFalse(child.Is(empt.TypeIdClass()));

		}

	private:
		inline static _CrtMemState _startMemState;
	};
}