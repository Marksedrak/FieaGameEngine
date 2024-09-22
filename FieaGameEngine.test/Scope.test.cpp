#include "pch.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace ScopeTest
{
	TEST_CLASS(ScopeTest)
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

		TEST_METHOD(Default_Constructr)
		{
			Scope s1;
			Assert::IsTrue(s1.isEmpty());
		}

		TEST_METHOD(Capacity_Constructor) {
			Scope s(4);
			Assert::AreEqual(s.GetCapacity(), (size_t)4);
		}

		TEST_METHOD(CopyConstructor) {
			Scope s1(3);
			Scope& s2 = s1.AppendScope("Kid");
			Datum& dInt = s2.Append("Hoy");
			dInt = 5;
			Scope s3(s1);

			Assert::IsTrue(s1 == s3);
			Assert::AreEqual(s1, s3);
			Assert::AreEqual(s1["Kid"].GetScope()->Find("Hoy")->Get<int>(), s3["Kid"].GetScope()->Find("Hoy")->Get<int>());
			
			Datum* dInt2 = s3.Find("Kid")->GetScope()->Find("Hoy");
			Assert::AreEqual(dInt2->Get<int>(), 5);
		}

		TEST_METHOD(Assignment_Operator) {
			Scope s1(4);
			Scope& s2 = s1.AppendScope("ToCopy");
			Datum& dt1 = s1.Append("InnerDatum");
			dt1.Push(44);

			Scope s3; 
			s3 = s1;

			Assert::IsTrue(s1 == s3);
			Assert::AreEqual(s1["InnerDatum"].Get<int>(), s3["InnerDatum"].Get<int>());

			s3 = s3;
			Assert::IsTrue(s3 == s3);
		}

		TEST_METHOD(Move_Semantics) {
			Scope s1(6);
			Scope& sEmbedded = s1.AppendScope("Embedded");
			Datum& dInt = s1.Append("Moving");
			dInt.Push(4);
			dInt.Push(6);

			Scope s2(std::move(s1));
			Assert::AreEqual(s2["Moving"].Get<int>(), 4);
			Assert::AreEqual(s2["Moving"].Get<int>(1), 6);


			Scope s3(2);
			s3 = std::move(s2);

			Assert::AreEqual(s3["Moving"].Get<int>(), 4);
			Assert::AreEqual(s3["Moving"].Get<int>(1), 6);
		}

		TEST_METHOD(Append){
			// Constructs Scope with capacity 3
			Scope s(3);
			// Use append to create a new Datum in s and put a reference in dt
			Datum& dt = s.Append("keys");
			// Push something to dt
			dt.Push("Hello");
			
			Datum* dt2;
			dt2 = s.Find("keys");
			// dt2 should point to dt
			Assert::AreEqual(dt2->Get<std::string>(), std::string("Hello"));
		}

		TEST_METHOD(Find) {
			Scope s;

			//Find will return a nullptr
			Datum* dt = s.Find("Something");
			Assert::IsTrue(dt == nullptr);
			
			s.Append("Here");
			Datum* d2 = s.Find("Here");
			Assert::IsFalse(d2 == nullptr);
		}

		TEST_METHOD(AppendScope) {
			Scope sParent(2);
			Scope& sChild = sParent.AppendScope("MyChild");
			Assert::AreEqual(sChild.GetParent(), &sParent);
			Datum* dt = sParent.Find("MyChild");
			Assert::AreEqual(sChild, *dt->GetScope());
		}

		TEST_METHOD(Search) {
			Scope s1;

			Scope& s2 = s1.AppendScope("Child");
			Scope& s3 = s2.AppendScope("GrandChild");

			Scope* sL;
			Datum* d = s3.Search("Child", &sL);

			// sL should point to s1 now and d should be the Datum in s1 associated with the key "Child"
			Assert::AreEqual(*sL, s1);
			Datum* d2 = s1.Find("Child");
			Assert::AreEqual(*d, *d2);
		}

		TEST_METHOD(Orphan) {
			Scope s1;

			Scope& s2 = s1.AppendScope("C");
			Scope& s3 = s1.AppendScope("C");

			Scope* ss = s2.Orphan();
			Assert::AreEqual(ss, &s2);
			Assert::IsTrue(s2.GetParent() == nullptr);
			// Since s2 is orphaned it does not destruct itself
			delete ss;
		}

		TEST_METHOD(Adopt) {
			Scope s1;

			Scope& s2 = s1.AppendScope("A");
			Scope& s3 = s1.AppendScope("A");

			Scope s4;
			s1.Adopt(s4, "A");

			Datum* d =  s1.Find("A");
			Assert::IsFalse(d == nullptr);
			Assert::AreEqual(*d->GetScope(), s2);
			Assert::AreEqual(*d->GetScope(1), s3);
			Assert::AreEqual(*d->GetScope(2), s4);

			Assert::IsTrue(s2.GetParent() == &s1);

			const Scope s5;
			Assert::IsTrue(s5.GetParent() == nullptr);
			Assert::IsTrue(s5.GetParent() == nullptr);

			s1.Adopt(const_cast<Scope&>(s5), "A");
			Assert::IsTrue(s5.GetParent() == &s1);

		}

		TEST_METHOD(FindConatinedScope) {
			Scope s1;
			Scope& s2 = s1.AppendScope("Help");

			uint32_t t;
			Datum* ds2 = s1.FindContainedScope(&s2, t);
			Assert::AreEqual(ds2->GetScope(t), &s2);
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}