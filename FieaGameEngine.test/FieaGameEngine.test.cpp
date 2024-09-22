#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace FieaGameEnginetest
{
	TEST_CLASS(FieaGameEnginetest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Temp t;
			Assert::IsTrue(t.returnTrue());
		}
	};
}
