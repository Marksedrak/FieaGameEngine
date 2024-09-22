#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include "TestTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

typedef glm::vec4 Vec4;
typedef glm::mat4 Mat4;
typedef std::string string;

namespace DatumTest
{
	TEST_CLASS(DatumTest)
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
				_CrtMemDumpStatistics(&diffMemState);
				_CrtDumpMemoryLeaks();
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
		TEST_METHOD(DefaultConstructors)
		{
			// Default Constructor
			{
				Datum d1;
				Assert::IsTrue(d1.Empty());
				Assert::AreEqual(d1.Size(), (size_t)0);
				Assert::AreEqual(d1.GetType(), std::string("Unknown"));
			}
		}
		TEST_METHOD(ParameterizedConstructor)// Parameterized Constructors
		{
			// Int
			Datum d1(7);
			Assert::AreEqual(d1.GetType(), std::string("Int"));
			Assert::AreEqual(d1.GetInt(), 7);

			// String
			Datum dString("Kaloob");
			Assert::AreEqual(dString.GetType(), std::string("String"));
			Assert::AreEqual(dString.GetString(), std::string("Kaloob"));

			// Float
			Datum dFloat(4.5f);
			Assert::AreEqual(dFloat.GetType(), std::string("Float"));
			Assert::AreEqual(dFloat.GetFloat(), 4.5f);

			// Vector
			Datum dVect(glm::vec4(1, 2, 3, 4));
			Assert::AreEqual(dVect.GetType(), std::string("Vector"));
			Assert::AreEqual(dVect.GetVector(), glm::vec4(1, 2, 3, 4));

			//Matrix
			Datum dMat(glm::mat4(1.0));
			Assert::AreEqual(dMat.GetType(), std::string("Matrix"));
			Assert::AreEqual(dMat.GetMatrix(), glm::mat4(1.0));

			// Throws Exception since type Double is not supported
			Assert::ExpectException<std::runtime_error>([&] { Datum dDouble(4.5); });

			//Attempt to change Datum Type
			Assert::ExpectException<std::runtime_error>([&dFloat] { dFloat.SetType(1); });
		}

		TEST_METHOD(ParameterizedConstructorWithSize)// Parameterized with size
		{
			// Int
			Datum dInt5(2, 5);
			Assert::AreEqual(dInt5.Size(), (size_t)1);
			Assert::AreEqual(dInt5.Capacity(), (size_t)5);
			Assert::AreEqual(dInt5.GetInt(), 2);

			// Float
			Datum dFloat4(1.2f, 4);
			Assert::AreEqual(dFloat4.Size(), (size_t)1);
			Assert::AreEqual(dFloat4.Capacity(), (size_t)4);
			Assert::AreEqual(dFloat4.GetFloat(), 1.2f);

			// String
			Datum dString2("Hello", 2);
			Assert::AreEqual(dString2.Size(), (size_t)1);
			Assert::AreEqual(dString2.Capacity(), (size_t)2);
			Assert::AreEqual(dString2.GetString(), std::string("Hello"));

			// Vector
			Datum dVec3(glm::vec4(1, 5, 9, 7), 3);
			Assert::AreEqual(dVec3.Size(), (size_t)1);
			Assert::AreEqual(dVec3.Capacity(), (size_t)3);
			Assert::AreEqual(dVec3.GetVector(), glm::vec4(1, 5, 9, 7));

			// Matrix
			Datum dMat6(glm::mat4(2.5), 5);
			Assert::AreEqual(dMat6.Size(), (size_t)1);
			Assert::AreEqual(dMat6.Capacity(), (size_t)5);
			Assert::AreEqual(dMat6.GetMatrix(), glm::mat4(2.5));
		}
		TEST_METHOD(CopyConstructor)
		{
			// Copy Constructor

			// Int
			Datum dInt4(4, 4);
			Datum dIntE(dInt4);
			Assert::AreEqual(dIntE.Size(), (size_t)1);
			Assert::AreEqual(dIntE.Capacity(), (size_t)4);
			Assert::AreEqual(dIntE.GetInt(), 4);

			// Float
			Datum dFloat(2.4f, 3);
			Datum dFloatE(dFloat);
			Assert::AreEqual(dFloatE.Size(), dFloat.Size());
			Assert::AreEqual(dFloatE.Capacity(), dFloat.Capacity());
			Assert::AreEqual(dFloatE.GetFloat(), dFloat.GetFloat());

			// String
			Datum dString1("Hello Darkness My Old Friend");
			Datum dString2(dString1);
			Assert::AreEqual(dString2.Size(), dString1.Size());
			Assert::AreEqual(dString2.Capacity(), dString1.Capacity());
			Assert::AreEqual(dString2.GetString(), dString1.GetString());

			// Vector
			Datum dVec1(glm::vec4(1, 4, 5, 7));
			Datum dVec2(dVec1);
			Assert::AreEqual(dVec2.Size(), dVec1.Size());
			Assert::AreEqual(dVec2.Capacity(), dVec1.Capacity());
			Assert::AreEqual(dVec2.GetVector(), dVec1.GetVector());

			// Matrix
			Datum dMat1(glm::mat4(1.2f));
			Datum dMat2(dMat1);
			Assert::AreEqual(dMat1.Size(), dMat2.Size());
			Assert::AreEqual(dMat1.Capacity(), dMat2.Capacity());
			Assert::AreEqual(dMat1.GetMatrix(), dMat2.GetMatrix());

			// Scope
			Scope* s1 = new Scope(3);
			Datum dScope1(s1);
			Datum dScope2(dScope1);
			Assert::AreEqual(dScope1.Size(), dScope2.Size());
			Assert::AreEqual(dScope1.Capacity(), dScope2.Capacity());
			Assert::AreEqual(dScope1.GetScope(), dScope2.GetScope());
			delete s1;

			// RTTI
			Scope* s2 = new Scope(4);
			RTTI* r2 = s2;
			Datum dRTTI1(r2);
			Datum dRTTI2(dRTTI1);
			Assert::AreEqual(dRTTI1.Size(), dRTTI2.Size());
			Assert::AreEqual(dRTTI1.Capacity(), dRTTI2.Capacity());
			delete s2;

		}
		TEST_METHOD(MoveConstructor)
		{
			Datum dInt1(1, 2);
			Datum dInt2(std::move(dInt1));

			Assert::AreEqual(dInt2.GetInt(), 1);
			Assert::AreEqual(dInt2.Size(), (size_t)1);
			Assert::AreEqual(dInt2.Capacity(), (size_t)2);

			Assert::AreEqual(dInt1.Size(), (size_t)0);
			Assert::AreEqual(dInt1.Capacity(), (size_t)0);
			Assert::ExpectException<std::runtime_error>([&dInt1] { dInt1.GetInt(); });
		}
		TEST_METHOD(AssignmentOperator)
		{
			// Assignment Operator
			{
				// Int
				Datum intD1(5);
				Datum emptyD;
				emptyD = intD1;

				Assert::AreEqual(emptyD.Size(), (size_t)1);
				Assert::AreEqual(emptyD.Capacity(), (size_t)1);
				Assert::AreEqual(emptyD.GetInt(), 5);

				// Float
				Datum dFloat(2.5f);
				Datum dFloatless;
				dFloatless = dFloat;

				Assert::AreEqual(dFloatless.Size(), (size_t)1);
				Assert::AreEqual(dFloatless.Capacity(), (size_t)1);
				Assert::AreEqual(dFloatless.GetFloat(), 2.5f);

				// String
				Datum dString("Really Hope This Works");
				Datum dEmptyString;
				dEmptyString = dString;

				Assert::AreEqual(dEmptyString.Size(), (size_t)1);
				Assert::AreEqual(dEmptyString.Capacity(), (size_t)1);
				Assert::AreEqual(dEmptyString.GetString(), std::string("Really Hope This Works"));

				// Vector
				Datum dVec(glm::vec4(8, 4, 7, 6));
				Datum dVecE;
				dVecE = dVec;

				Assert::AreEqual(dVecE.Size(), (size_t)1);
				Assert::AreEqual(dVecE.Capacity(), (size_t)1);
				Assert::AreEqual(dVecE.GetVector(), glm::vec4(8, 4, 7, 6));

				// Matrix
				Datum dMat(glm::mat4(2.5f));
				Datum dMatE;
				dMatE = dMat;

				Assert::AreEqual(dMatE.Size(), (size_t)1);
				Assert::AreEqual(dMatE.Capacity(), (size_t)1);
				Assert::AreEqual(dMatE.GetMatrix(), glm::mat4(2.5f));
			}
		}

		TEST_METHOD(Assignment_Operator_Overloads) {
			// Int
			Datum dInt;
			dInt = 4;
			Assert::AreEqual(dInt.GetInt(), 4);
			Assert::AreEqual(dInt.Size(), (size_t)1);
			Assert::AreEqual(dInt.Capacity(), (size_t)1);
			
			dInt = 5;
			Assert::AreEqual(dInt.GetInt(), 5);
			Assert::AreEqual(dInt.Size(), (size_t)1);
			Assert::AreEqual(dInt.Capacity(), (size_t)1);

			Assert::ExpectException<std::runtime_error>([&dInt] { dInt = 3.5f; });

			// Float
			Datum dFloat;
			dFloat = 2.5f;

			Assert::AreEqual(dFloat.GetFloat(), 2.5f);
			Assert::AreEqual(dFloat.Size(), (size_t)1);
			Assert::AreEqual(dFloat.Capacity(), (size_t)1);

			dFloat = 3.5f;
			Assert::AreEqual(dFloat.GetFloat(), 3.5f);
			Assert::AreEqual(dFloat.Size(), (size_t)1);
			Assert::AreEqual(dFloat.Capacity(), (size_t)1);

			Assert::ExpectException<std::runtime_error>([&dFloat] { dFloat = 2; });

			// String
			Datum dString;
			dString = std::string("Halloo");

			Assert::AreEqual(dString.GetString(), std::string("Halloo"));
			Assert::AreEqual(dString.Size(), (size_t)1);
			Assert::AreEqual(dString.Capacity(), (size_t)1);

			dString = std::string("Byyyeeee!");

			Assert::AreEqual(dString.GetString(), std::string("Byyyeeee!"));
			Assert::AreEqual(dString.Size(), (size_t)1);
			Assert::AreEqual(dString.Capacity(), (size_t)1);

			Assert::ExpectException<std::runtime_error>([&dString] { dString = glm::vec4(1,5,8,6);  });
		
			// Vector
			Datum dVec;
			dVec = glm::vec4(1, 5, 3, 2);

			Assert::AreEqual(dVec.GetVector(), glm::vec4(1, 5, 3, 2));
			Assert::AreEqual(dVec.Size(), (size_t)1);
			Assert::AreEqual(dVec.Capacity(), (size_t)1);

			dVec = glm::vec4(2, 4, 6, 8);
			Assert::AreEqual(dVec.GetVector(), glm::vec4(2, 4, 6, 8));
			Assert::AreEqual(dVec.Size(), (size_t)1);
			Assert::AreEqual(dVec.Capacity(), (size_t)1);

			Assert::ExpectException<std::runtime_error>([&dVec] { dVec = std::string("Not A Vector");  });
			Assert::ExpectException<std::runtime_error>([&dVec] { dVec = glm::mat4(1.6f);  });

			// Matrix
			Datum dMat;
			dMat = glm::mat4(1.6f);

			Assert::AreEqual(dMat.GetMatrix(), glm::mat4(1.6f));
			Assert::AreEqual(dMat.Size(), (size_t)1);
			Assert::AreEqual(dMat.Capacity(), (size_t)1);

			dMat = glm::mat4(8.4f);

			Assert::AreEqual(dMat.GetMatrix(), glm::mat4(8.4f));
			Assert::AreEqual(dMat.Size(), (size_t)1);
			Assert::AreEqual(dMat.Capacity(), (size_t)1);

			Assert::ExpectException<std::runtime_error>([&dMat] { dMat = 7;  });

		}

		TEST_METHOD(Move_Assignment) {
			// Testing Move Assignment
			
			// Testing for Int type
			Datum dInt1(5, 7);
			Datum dInt2;

			dInt2 = std::move(dInt1);
			
			// Testing that dInt1 is emptied out
			Assert::AreEqual(dInt1.Size(), (size_t)0);
			Assert::AreEqual(dInt1.Capacity(), (size_t)0);
			Assert::AreEqual(dInt1.GetType(), std::string("Unknown"));
			Assert::ExpectException<std::runtime_error>([&dInt1] { dInt1.GetInt(); });

			// Testing that dInt2 contains what dInt1 had
			Assert::AreEqual(dInt2.Size(), (size_t)1);
			Assert::AreEqual(dInt2.Capacity(), (size_t)7);
			Assert::AreEqual(dInt2.GetInt(), 5);

			// Testing moving into an already initialized Datum of same type
			Datum dInt3(2, 5);
			dInt3 = std::move(dInt2);

			Assert::AreEqual(dInt3.Size(), (size_t)1);
			Assert::AreEqual(dInt3.Capacity(), (size_t)7);
			Assert::AreEqual(dInt3.GetInt(), 5);


			// Testing for Float
			Datum dFloat1(1.5f, 2);
			Datum dFloat2;

			dFloat2 = std::move(dFloat1);

			// Testing that dFloat1 is emptied out
			Assert::AreEqual(dFloat1.Size(), (size_t)0);
			Assert::AreEqual(dFloat1.Capacity(), (size_t)0);
			Assert::AreEqual(dFloat1.GetType(), std::string("Unknown"));
			Assert::ExpectException<std::runtime_error>([&dFloat1] { dFloat1.GetFloat(); });

			// Testing that dFloat2 contains what dInt1 had
			Assert::AreEqual(dFloat2.Size(), (size_t)1);
			Assert::AreEqual(dFloat2.Capacity(), (size_t)2);
			Assert::AreEqual(dFloat2.GetFloat(), 1.5f);

			// Testing moving into an already initialized Datum of same type
			Datum dFloat3(2.5f, 5);
			dFloat3 = std::move(dFloat2);

			Assert::AreEqual(dFloat3.Size(), (size_t)1);
			Assert::AreEqual(dFloat3.Capacity(), (size_t)2);
			Assert::AreEqual(dFloat3.GetFloat(), 1.5f);


			// Testing for String
			Datum dString1("Plz Don't Crash", 3);
			Datum dString2;

			dString2 = std::move(dString1);

			// Testing that dString1 is emptied out
			Assert::AreEqual(dString1.Size(), (size_t)0);
			Assert::AreEqual(dString1.Capacity(), (size_t)0);
			Assert::AreEqual(dString1.GetType(), std::string("Unknown"));
			Assert::ExpectException<std::runtime_error>([&dString1] { dString1.GetString(); });

			// Testing that dString2 contains what dInt1 had
			Assert::AreEqual(dString2.Size(), (size_t)1);
			Assert::AreEqual(dString2.Capacity(), (size_t)3);
			Assert::AreEqual(dString2.GetString(), std::string("Plz Don't Crash"));

			// Testing moving into an already initialized Datum of same type
			Datum dString3("Unnecessary", 2);
			dString3 = std::move(dString2);

			Assert::AreEqual(dString3.Size(), (size_t)1);
			Assert::AreEqual(dString3.Capacity(), (size_t)3);
			Assert::AreEqual(dString3.GetString(), string("Plz Don't Crash"));


			// Testing for Vector
			Datum dVec1(Vec4(4,8,9,9), 8);
			Datum dVec2;

			dVec2 = std::move(dVec1);

			// Testing that dVec1 is emptied out
			Assert::AreEqual(dVec1.Size(), (size_t)0);
			Assert::AreEqual(dVec1.Capacity(), (size_t)0);
			Assert::AreEqual(dVec1.GetType(), std::string("Unknown"));
			Assert::ExpectException<std::runtime_error>([&dVec1] { dVec1.GetVector(); });

			// Testing that dVec2 contains what dInt1 had
			Assert::AreEqual(dVec2.Size(), (size_t)1);
			Assert::AreEqual(dVec2.Capacity(), (size_t)8);
			Assert::AreEqual(dVec2.GetVector(), Vec4(4, 8, 9, 9));

			// Testing moving into an already initialized Datum of same type
			Datum dVec3(Vec4(4,5,1,2), 2);
			dVec3 = std::move(dVec2);

			Assert::AreEqual(dVec3.Size(), (size_t)1);
			Assert::AreEqual(dVec3.Capacity(), (size_t)8);
			Assert::AreEqual(dVec3.GetVector(), Vec4(4,8,9,9));


			//Testing for Matrix
			Datum dMat1(Mat4(5.5f), 7);
			Datum dMat2;

			dMat2 = std::move(dMat1);

			// Testing that dMat1 is emptied out
			Assert::AreEqual(dMat1.Size(), (size_t)0);
			Assert::AreEqual(dMat1.Capacity(), (size_t)0);
			Assert::AreEqual(dMat1.GetType(), std::string("Unknown"));
			Assert::ExpectException<std::runtime_error>([&dMat1] { dMat1.GetMatrix(); });

			// Testing that dMat2 contains what dInt1 had
			Assert::AreEqual(dMat2.Size(), (size_t)1);
			Assert::AreEqual(dMat2.Capacity(), (size_t)7);
			Assert::AreEqual(dMat2.GetMatrix(), Mat4(5.5f));

			// Testing moving into an already initialized Datum of same type
			Datum dMat3(Mat4(4.3f), 4);
			dMat3 = std::move(dMat2);

			Assert::AreEqual(dMat3.Size(), (size_t)1);
			Assert::AreEqual(dMat3.Capacity(), (size_t)7);
			Assert::AreEqual(dMat3.GetMatrix(), Mat4(5.5f));
		}

		// Need Revamp once Push is added
		TEST_METHOD(Get) {
			// Get and const Get
			// Demonstrating all Get methods would throw an exception if Datum is not initialized;
			Datum dEmpty;
			Assert::ExpectException<std::runtime_error>([&dEmpty] {dEmpty.GetInt(); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] {dEmpty.GetFloat(); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] {dEmpty.GetString(); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] {dEmpty.GetVector(); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] {dEmpty.GetMatrix(); });

			// Demonstrating all const Get methods would throw an exception if Datum is not initialized;
			const Datum dEmptyC;
			Assert::ExpectException<std::runtime_error>([&dEmptyC] {dEmptyC.GetInt(); });
			Assert::ExpectException<std::runtime_error>([&dEmptyC] {dEmptyC.GetFloat(); });
			Assert::ExpectException<std::runtime_error>([&dEmptyC] {dEmptyC.GetString(); });
			Assert::ExpectException<std::runtime_error>([&dEmptyC] {dEmptyC.GetVector(); });
			Assert::ExpectException<std::runtime_error>([&dEmptyC] {dEmptyC.GetMatrix(); });

			// GetInt
			Datum dInt(6);
			Assert::ExpectException<std::out_of_range>([&dInt] {dInt.GetInt(2); });
			Assert::AreEqual(dInt.GetInt(), 6);

			// GetInt const
			const Datum dIntC(16);
			Assert::ExpectException<std::out_of_range>([&dIntC] {dIntC.GetInt(3); });
			Assert::AreEqual(dIntC.GetInt(), 16);

			// GetFloat
			Datum dFloat(7.8f);
			Assert::AreEqual(dFloat.GetFloat(), 7.8f);
			Assert::ExpectException<std::out_of_range>([&dFloat] {dFloat.GetFloat(5); });
				
			// const Get Float
			const Datum dFloatC(4.5f);
			Assert::AreEqual(dFloatC.GetFloat(), 4.5f);
			Assert::ExpectException<std::out_of_range>([&dFloatC] {dFloatC.GetFloat(5); });

			//GetString
			Datum dString("Undefined");
			Assert::AreEqual(dString.GetString(), std::string("Undefined"));
			Assert::ExpectException<std::out_of_range>([&dString] {dString.GetString(4); });

			//GetString const
			const Datum dStringC("StringHere");
			Assert::AreEqual(dStringC.GetString(), std::string("StringHere"));
			Assert::ExpectException<std::out_of_range>([&dStringC] {dStringC.GetString(4); });

			// GetVector
			Datum dVec(glm::vec4(2,4,6,8));
			Assert::AreEqual(dVec.GetVector(), glm::vec4(2, 4, 6, 8));
			Assert::ExpectException<std::out_of_range>([&dVec] {dVec.GetVector(4); });

			// GetVector const
			const Datum dVecC(glm::vec4(2, 5, 10, 8), 3);
			Assert::AreEqual(dVecC.GetVector(), glm::vec4(2, 5, 10, 8));
			Assert::ExpectException<std::out_of_range>([&dVecC] {dVecC.GetVector(4); });

			// GetMatrix
			Datum dMat(glm::mat4(3.8f), 6);
			Assert::AreEqual(dMat.GetMatrix(), glm::mat4(3.8f));
			Assert::ExpectException<std::out_of_range>([&dMat] {dMat.GetMatrix(8); });

			// const GetMatrix
			const Datum dMatC(glm::mat4(4.5f), 2);
			Assert::AreEqual(dMatC.GetMatrix(), glm::mat4(4.5f));
			Assert::ExpectException<std::out_of_range>([&dMatC] { dMatC.GetMatrix(5); });
			
		}

		TEST_METHOD(Push) {

			// Testing Push with Int

			Datum dInt(2);
			//// Pushing where reallocating is required
			dInt.Push(1);
			Assert::AreEqual(dInt.Get<int>(), 2);
			Assert::AreEqual(dInt.Get<int>(1), 1);
			Assert::AreEqual(dInt.Size(), (size_t)2);
			//// Pushing where no reallocation is required
			dInt.Push(5);
			Assert::AreEqual(dInt.Get<int>(2), 5);
			Assert::AreEqual(dInt.Size(), (size_t)3);
			//// Try Pushing an invalid value (of different type than Datum)
			Assert::ExpectException<std::runtime_error>([&dInt] {dInt.Push(3.5f); });


			//// Testing Push with Float

			Datum dFloat(2.5f);
			//// Pushing where reallocating is required
			dFloat.Push(1.8f);
			Assert::AreEqual(dFloat.Get<float>(), 2.5f);
			Assert::AreEqual(dFloat.Get<float>(1), 1.8f);
			Assert::AreEqual(dFloat.Size(), (size_t)2);
			//// Pushing where no reallocation is required
			dFloat.Push(5.8f);
			Assert::AreEqual(dFloat.Get<float>(2), 5.8f);
			Assert::AreEqual(dFloat.Size(), (size_t)3);
			//// Try Pushing an invalid value (of different type than Datum)
			Assert::ExpectException<std::runtime_error>([&dFloat] {dFloat.Push(6); });


			// Testing Push with String

			Datum dString("Hello", 2);
			//// Pushing where reallocating is required
			dString.Push("It's");
			Assert::AreEqual(dString.Get<string>(), string("Hello"));
			Assert::AreEqual(dString.Get<string>(1), string("It's"));
			Assert::AreEqual(dString.Size(), (size_t)2);
			// Pushing where no reallocation is required
			dString.Push(string("Me"));
			Assert::AreEqual(dString.Get<string>(2), string("Me"));
			Assert::AreEqual(dString.Size(), (size_t)3);
			// Push using string instead of const char*
			// Try Pushing an invalid value (of different type than Datum)
			Assert::ExpectException<std::runtime_error>([&dString] {dString.Push(Vec4(2,4,6,8)); });

			// Testing Push with Vector

			Datum dVec(Vec4(1,2,5,4));
			//// Pushing where reallocating is required
			dVec.Push(Vec4(8,0,0,8));
			Assert::AreEqual(dVec.Get<Vec4>(), Vec4(1, 2, 5, 4));
			Assert::AreEqual(dVec.Get<Vec4>(1), Vec4(8, 0, 0, 8));
			Assert::AreEqual(dVec.Size(), (size_t)2);
			//// Pushing where no reallocation is required
			dVec.Push(Vec4(3, 0, 5, 1));
			Assert::AreEqual(dVec.Get<Vec4>(2), Vec4(3, 0, 5, 1));
			Assert::AreEqual(dVec.Size(), (size_t)3);
			//// Try Pushing an invalid value (of different type than Datum)
			Assert::ExpectException<std::runtime_error>([&dVec] {dVec.Push(Mat4(2.3f)); });


			////// Testing Push with Matrix

			Datum dMat(Mat4(1.3f));
			//// Pushing where reallocating is required
			dMat.Push(Mat4(5.5f));
			Assert::AreEqual(dMat.Get<Mat4>(), Mat4(1.3f));
			Assert::AreEqual(dMat.Get<Mat4>(1), Mat4(5.5f));
			Assert::AreEqual(dMat.Size(), (size_t)2);
			//// Pushing where no reallocation is required
			dMat.Push(Mat4(2.2f));
			Assert::AreEqual(dMat.Get<Mat4>(2), Mat4(2.2f));
			Assert::AreEqual(dMat.Size(), (size_t)3);
			//// Try Pushing an invalid value (of different type than Datum)
			Assert::ExpectException<std::runtime_error>([&dMat] {dMat.Push(Vec4(3, 0, 5, 1)); });


			// Pushing to an empty Datum
			Datum EmptyInt;
			EmptyInt.Push(2);

			//Pushing to an empty string Datum
			Datum dStringE;
			dStringE.Push("Empty String");
			Assert::AreEqual(dStringE.Get<string>(), string("Empty String"));
			Assert::AreEqual(dStringE.Size(), (size_t)1);
			dStringE.Push("Not so empty anymore");
			Assert::AreEqual(dStringE.Get<string>(1), string("Not so empty anymore"));
			Assert::AreEqual(dStringE.Size(), (size_t)2);

			//Test Exception thrown with Push(const char*) if Datum is not of String type
			Assert::ExpectException<std::runtime_error>([&dInt] { dInt.Push("Not an Int"); });

			// Testing External Storage not being able to manipulate through Push

				// Int
			int exampleInt = 5;
			int* testInt = &exampleInt;
			Datum dIntExternal;
			dIntExternal.SetStorage(testInt, 1);
			Assert::ExpectException<std::runtime_error>([&dIntExternal] { dIntExternal.Push(5); });

				// Float
			float exampleFloat = 2.4f;
			float* testFloat = &exampleFloat;
			Datum dFloatExternal;
			dFloatExternal.SetStorage(testFloat, 1);
			Assert::ExpectException<std::runtime_error>([&dFloatExternal] { dFloatExternal.Push(2.7f); });

				// Strings
			string exampleString = string("External");
			string* testString = &exampleString;
			Datum dStringExternal;
			dStringExternal.SetStorage(testString, 1);
			Assert::ExpectException<std::runtime_error>([&dStringExternal] { dStringExternal.Push("Hello There"); });

				// Vectors
			Vec4 exampleVec = Vec4(2, 5, 3, 1);
			Vec4* testVec = &exampleVec;
			Datum dVecExt;
			dVecExt.SetStorage(testVec, 1);
			Assert::ExpectException<std::runtime_error>([&dVecExt] { dVecExt.Push(Vec4(2,2,1,5)); });

				// Matrix
			Mat4 exampleMat = Mat4(1, 2, 5, 5, 2, 5, 3, 2, 4, 6, 2, 5, 3, 6,4,2);
			Mat4* testMat = &exampleMat;
			Datum dMatExt;
			dMatExt.SetStorage(testMat, 1);
			Assert::ExpectException<std::runtime_error>([&dMatExt] { dMatExt.Push(Vec4(2, 2, 1, 5)); });

				// Scope
			Scope* s = new Scope();
			Scope* t = new Scope();
			Datum dScopExt;
			dScopExt.SetStorage(&s, 1);
			Assert::ExpectException<std::runtime_error>([&dScopExt, &t] { dScopExt.Push(t); });
		

				// RTTI
			RTTI* r = &*s;
			RTTI* test = &*t;
			Datum dRTTIExt;
			dRTTIExt.SetStorage(&r, 1);
			Assert::ExpectException<std::runtime_error>([&dRTTIExt, &test] { dRTTIExt.Push(test); });
			delete s;
			delete t;
		}

		TEST_METHOD(Pop) {

			// Testing for Int
			Datum dInt(5, 3);
			dInt.Push(4);
			dInt.Push(8);
			dInt.Pop();
			// dInt should now have {5,4}
			Assert::AreEqual(dInt.Size(), (size_t)2);
			Assert::AreEqual(dInt.Get<int>(), 5);
			Assert::AreEqual(dInt.Get<int>(1), 4);
			Assert::ExpectException<std::out_of_range>([&dInt] { dInt.Get<int>(2); });

			//Testing for Float
			Datum dFloat(1.5f, 2);
			dFloat.Push(2.5f);
			dFloat.Push(3.8f);
			dFloat.Pop();
			// dFloat should now have {1.5f,2.5f}
			Assert::AreEqual(dFloat.Size(), (size_t)2);
			Assert::AreEqual(dFloat.Get<float>(), 1.5f);
			Assert::AreEqual(dFloat.Get<float>(1), 2.5f);
			Assert::ExpectException<std::out_of_range>([&dFloat] { dFloat.Get<float>(2); });

			// Testing for String
			Datum dString("Welcome", 3);
			dString.Push("To Jurrassic");
			dString.Push("Park");
			dString.Pop();
			// dString should now have {"Welcome" , "To Jurrassic"}
			Assert::AreEqual(dString.Size(), (size_t)2);
			Assert::AreEqual(dString.Get<string>(), string("Welcome"));
			Assert::AreEqual(dString.Get<string>(1), string("To Jurrassic"));
			Assert::ExpectException<std::out_of_range>([&dString] { dString.Get<string>(2); });

			// Testing for Vector
			Datum dVec(Vec4(2.5f), 4);
			dVec.Push(Vec4(2.8f, 3.4f, 7.8f, 8.9f));
			dVec.Push(Vec4(2,4,6,8));
			dVec.Pop();
			// dVec should now have {Vec4(2.5f), Vec4(2.8f,3.4f, 7.8f, 8.9f)}
			Assert::AreEqual(dVec.Size(), (size_t)2);
			Assert::AreEqual(dVec.Get<Vec4>(), Vec4(2.5f));
			Assert::AreEqual(dVec.Get<Vec4>(1), Vec4(2.8f, 3.4f, 7.8f, 8.9f));
			Assert::ExpectException<std::out_of_range>([&dVec] { dVec.Get<Vec4>(2); });
		
			// Testing for Matrix
			Datum dMat(Mat4(4.5f));
			dMat.Push(Mat4(3.3f));
			dMat.Push(Mat4(2.8f));
			dMat.Pop();
			// dMat should now conatain {Mat4(4.5f), Mat4(3.3f)}
			Assert::AreEqual(dMat.Size(), (size_t)2);
			Assert::AreEqual(dMat.Get<Mat4>(), Mat4(4.5f));
			Assert::AreEqual(dMat.Get<Mat4>(1), Mat4(3.3f));
			Assert::ExpectException<std::out_of_range>([&dMat] { dMat.Get<Mat4>(2); });

		}

		TEST_METHOD(CheckType) {

			// Throws exception if Datum is uninitialized
			Datum dEmpty;
			Assert::ExpectException<std::runtime_error>([&dEmpty] { dEmpty.CheckType(5); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] { dEmpty.CheckType(5.4f); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] { dEmpty.CheckType(string("Throw!")); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] { dEmpty.CheckType(Vec4(2,7,6,8)); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] { dEmpty.CheckType(Mat4(2.5f)); });

			Datum dInt(2);
			Assert::IsTrue(dInt.CheckType(5));
			Assert::IsFalse(dInt.CheckType(1.5f));

			Datum dFloat(6.5f);
			Assert::IsTrue(dFloat.CheckType(4.8f));
			Assert::IsFalse(dFloat.CheckType(string("Not a String")));

			Datum dString("A string");
			Assert::IsTrue(dString.CheckType(string("is a string")));
			Assert::IsFalse(dString.CheckType(4));

			Datum dVec(Vec4(5, 5, 5, 5));
			Assert::IsTrue(dVec.CheckType(Vec4(2, 4, 6, 8)));
			Assert::IsFalse(dVec.CheckType(Mat4(5.4f)));

			Datum dMat(Mat4(6.5f));
			Assert::IsTrue(dMat.CheckType(Mat4(5.4f)));
			Assert::IsFalse(dMat.CheckType(Vec4(2, 4, 8, 1)));

			Scope* s = new Scope();
			Scope* test = new Scope();
			Datum dScope(s);
			Assert::IsTrue(dScope.CheckType(test));
			Assert::IsFalse(dScope.CheckType(3));

			RTTI* rtest1 = &*s;
			RTTI* rtest2 = &*test;
			Datum dRTTI(rtest1);
			Assert::IsTrue(dRTTI.CheckType(rtest2));
			Assert::IsFalse(dRTTI.CheckType(3.5f));
			delete s;
			delete test;
			
			// Showing it will return false if type is not even in _type
			Assert::IsFalse(dMat.CheckType(2.8));

		}

		TEST_METHOD(Set) {
			// Testing with Int
			Datum dInt(4);
			dInt.Push(8);
			// dInt should be {4, 8}
			Assert::AreEqual(dInt.Get<int>(), 4);
			Assert::AreEqual(dInt.Get<int>(1), 8);
			int a = 7;
			dInt.Set(1, a);
			// dInt should now be {4, 7}
			Assert::AreEqual(dInt.Get<int>(), 4);
			Assert::AreEqual(dInt.Get<int>(1), 7);
			// Testing Exception thrown when out of range
			int testInt = 9;
			Assert::ExpectException<std::out_of_range>([&dInt, &testInt] {dInt.Set(2, testInt); });
			// Testing Exception when CheckType fails
			string testString("Wow");
			Assert::ExpectException<std::runtime_error>([&dInt, &testString] {dInt.Set(0, testString); });

			//Testing with Float
			Datum dFloat(5.5f);
			dFloat.Push(1.3f);
			// dFloat should be {5.5f, 1.3f}
			Assert::AreEqual(dFloat.Get<float>(), 5.5f);
			Assert::AreEqual(dFloat.Get<float>(1), 1.3f);
			float f = 4.8f;
			dFloat.Set(0, f);
			// dFloat should now be {4.8f, 1.3f}
			Assert::AreEqual(dFloat.Get<float>(), 4.8f);
			Assert::AreEqual(dFloat.Get<float>(1), 1.3f);
			// Testing Exception thrown when out of range
			float testF = 5.7f;
			Assert::ExpectException<std::out_of_range>([&dFloat, &testF] {dFloat.Set(2, testF); });
			// Testing Exception when CheckType fails
			testInt = 7;
			Assert::ExpectException<std::runtime_error>([&dFloat, &testInt] {dFloat.Set(0, testInt); });

			//Testing with String
			Datum dString("Start");
			dString.Push("Middle");
			// dString should be {"Start", "Middle"}
			Assert::AreEqual(dString.Get<string>(), string("Start"));
			Assert::AreEqual(dString.Get<string>(1), string("Middle"));
			string s("End");
			dString.Set(0, s);
			// dString should now be {4.8f, 1.3f}
			Assert::AreEqual(dString.Get<string>(), string("End"));
			Assert::AreEqual(dString.Get<string>(1), string("Middle"));
			// Testing Exception thrown when out of range
			string testS("Test");
			Assert::ExpectException<std::out_of_range>([&dString, &testS] {dString.Set(2, testS); });
			// Testing Exception when CheckType fails
			testF = 0.8f;
			Assert::ExpectException<std::runtime_error>([&dString, &testF] {dString.Set(0, testF); });


			// Testing with Vector
			Datum dVec(Vec4(2, 4, 8, 6));
			dVec.Push(Vec4(5, 5, 5, 5));
			dVec.Push(Vec4(4, 8, 3, 2));
			// dVec should now contain {Vec4(2,4,8,6), Vec4(5,5,5,5), Vec4(4, 8, 3, 2)}
			Assert::AreEqual(dVec.Get<Vec4>(), Vec4(2, 4, 8, 6));
			Assert::AreEqual(dVec.Get<Vec4>(1), Vec4(5, 5, 5, 5));
			Assert::AreEqual(dVec.Get<Vec4>(2), Vec4(4, 8, 3, 2));
			Vec4 v = Vec4(8, 9, 1, 2);
			dVec.Set(1, v);
			// dVec should now contain {Vec4(2,4,8,6), Vec4(8, 9, 1, 2), Vec4(4, 8, 3, 2)}
			Assert::AreEqual(dVec.Get<Vec4>(), Vec4(2, 4, 8, 6));
			Assert::AreEqual(dVec.Get<Vec4>(1), Vec4(8, 9, 1, 2));
			Assert::AreEqual(dVec.Get<Vec4>(2), Vec4(4, 8, 3, 2));
			// Test for exception when index is out of range
			Vec4 v2 = Vec4(28, 34, 14, 24);
			Assert::ExpectException<std::out_of_range>([&dVec, &v2] { dVec.Set(3, v2); });
			// Test for exception when CheckType fails
			Mat4 testM = Mat4(4.5f);
			Assert::ExpectException<std::runtime_error>([&dVec, &testM] { dVec.Set(0, testM); });


			// Testing with Matrix
			Datum dMat(Mat4(2.5f));
			dMat.Push(Mat4(4.8f));
			dMat.Push(Mat4(8.9f));
			// dVec should now contain {Mat4(2.5f), Mat4(4.8f), Mat4(8.9f)}
			Assert::AreEqual(dMat.Get<Mat4>(), Mat4(2.5f));
			Assert::AreEqual(dMat.Get<Mat4>(1), Mat4(4.8f));
			Assert::AreEqual(dMat.Get<Mat4>(2), Mat4(8.9f));
			Mat4 m = Mat4(24.5f);
			dMat.Set(2, m);
			// dVec should now contain {Vec4(2,4,8,6), Vec4(8, 9, 1, 2), Vec4(4, 8, 3, 2)}
			Assert::AreEqual(dMat.Get<Mat4>(), Mat4(2.5f));
			Assert::AreEqual(dMat.Get<Mat4>(1), Mat4(4.8f));
			Assert::AreEqual(dMat.Get<Mat4>(2), Mat4(24.5f));
			// Test if index is out of range (index >= 3) 
			Mat4 m2 = Mat4(2.47f);
			Assert::ExpectException<std::out_of_range>([&dMat, &m2] { dMat.Set(3, m2); });
			// Test for exception when checktype fails
			Vec4 testV(2, 4, 6, 6);
			Assert::ExpectException<std::runtime_error>([&dMat, &testV] { dMat.Set(2, testV); });

		}

		TEST_METHOD(SetFromString) {

			// Int
			Datum dInt(1);
			dInt.SetFromString(0, "24");
			Assert::AreEqual(dInt.Get<int>(), 24);
			Assert::ExpectException<std::runtime_error>([&dInt] {dInt.SetFromString(0, "NotInt"); });

			//Float
			Datum dFloat(3.5f);
			dFloat.SetFromString(0, "4.5");
			Assert::AreEqual(dFloat.Get<float>(), 4.5f);
			Assert::ExpectException<std::runtime_error>([&dFloat] {dFloat.SetFromString(0, "NotFloat"); });

			// String
			Datum dString("Thats easy");
			dString.SetFromString(0, "Yes");
			Assert::AreEqual(dString.Get<string>(), string("Yes"));


			// Vector
			Datum dVec(Vec4(1, 2, 4, 8));
			dVec.SetFromString(0, "Vec4(1, 5, 4, 8)");
			Assert::AreEqual(dVec.Get<Vec4>(), Vec4(1, 5, 4, 8));
			Assert::ExpectException<std::runtime_error>([&dVec] {dVec.SetFromString(0, "NotVector"); });

			// Matrix
			Datum dMat(Mat4(2.5f));
			Mat4 m1 = Mat4(4.5f);
			string mString = "Mat4((4.5, 0.0, 0.0, 0.0), (0.0, 4.5, 0.0, 0.0), (0.0, 0.0, 4.5, 0.0), (0.0, 0.0, 0.0, 4.5))";
			dMat.SetFromString(0, mString);
			Assert::AreEqual(dMat.Get<Mat4>(), Mat4(4.5f));
			Assert::ExpectException<std::runtime_error>([&dMat] {dMat.SetFromString(0, "NotMatrix"); });
		}

		// Testing Empty works 
		TEST_METHOD(Empty) {
			Datum dEmpty;
			Assert::IsTrue(dEmpty.Empty());
			
			// Int
			Datum dInt(5);
			Assert::IsFalse(dInt.Empty());

			// Float
			Datum dFloat(1.5f);
			Assert::IsFalse(dFloat.Empty());

			// String
			Datum dString("Mallo");
			Assert::IsFalse(dString.Empty());

			// Vector
			Datum dVec(glm::vec4(8,9,7,5));
			Assert::IsFalse(dVec.Empty());

			// Matrix
			Datum dMatrix(glm::mat4(25.5f));
			Assert::IsFalse(dMatrix.Empty());
		}

		TEST_METHOD(GetAsString) {

			// Trying to GetAsString from an empty Datum
			Datum dEmpty;
			Assert::ExpectException<std::out_of_range>([&dEmpty] { dEmpty.GetAsString(); });
			Assert::ExpectException<std::out_of_range>([&dEmpty] { dEmpty.GetAsString(1); });
			
			// Get Int as a string
			Datum dInt(4);
				// Getting first element as a string;
			string intString = dInt.GetAsString();
			Assert::AreEqual(intString, std::to_string(4));
				// Getting second element as a string;
			dInt.Push(24);
			intString = dInt.GetAsString(1);
			Assert::AreEqual(intString, std::to_string(24));
				// Exception if out of bounds
			Assert::ExpectException<std::out_of_range>([&dInt] { dInt.GetAsString(2); });


			// Get Float as a string
			Datum dFloat(2.5f);
				// Getting first element as string
			string floatString = dFloat.GetAsString();
			Assert::AreEqual(floatString, std::to_string(2.5f));
				// Getting secodn element as a string
			dFloat.Push(4.4f);
			floatString = dFloat.GetAsString(1);
			Assert::AreEqual(floatString, std::to_string(4.4f));
				// Out of Bound Exception
			Assert::ExpectException<std::out_of_range>([&dFloat] { dFloat.GetAsString(2); });


			// Get String as string (why would anyone need to do that
			Datum dString("For Science");
				// Get first element as string
			string stringStr = dString.GetAsString();
			Assert::AreEqual(stringStr, string("For Science"));
				// Get second element
			dString.Push("Another one?");
			stringStr = dString.GetAsString(1);
			Assert::AreEqual(stringStr, string("Another one?"));
				// Out of bound Exception
			Assert::ExpectException<std::out_of_range>([&dString] { dString.GetAsString(2); });


			// Get Vector as string
			Datum dVec(Vec4(1, 2, 3, 4));
				// Get first element as string
			string vecString = dVec.GetAsString();
			Assert::AreEqual(vecString, glm::to_string(Vec4(1, 2, 3, 4)));
				// Get second element as string
			dVec.Push(Vec4(3, 2, 7, 8));
			vecString = dVec.GetAsString(1);
			Assert::AreEqual(vecString, glm::to_string(Vec4(3, 2, 7, 8)));
				// Out of Bounds Exception
			Assert::ExpectException<std::out_of_range>([&dVec] { dVec.GetAsString(3); });


			// Get Matrix as String
			Datum dMat(Mat4(3.3f));
			// Get first element as string
			string matString = dMat.GetAsString();
			Assert::AreEqual(matString, glm::to_string(Mat4(3.3f)));
			// Get second element as string
			dMat.Push(Mat4(8.9f));
			matString = dMat.GetAsString(1);
			Assert::AreEqual(matString, glm::to_string(Mat4(8.9f)));
			// Out of Bounds Exception
			Assert::ExpectException<std::out_of_range>([&dMat] { dMat.GetAsString(3); });
		}

		TEST_METHOD(Clear) {
			
			// Testing for Int
			Datum dInt(5, 7);
			dInt.Push(4);
			dInt.Push(6);
			Assert::AreEqual(dInt.Size(), (size_t)3);
			Assert::AreEqual(dInt.Capacity(), (size_t)7);
			// Clearing dInt
			dInt.Clear();
			Assert::ExpectException<std::out_of_range>([&dInt] { dInt.Get<int>(); });
			Assert::ExpectException<std::out_of_range>([&dInt] { dInt.Get<int>(1); });
			Assert::ExpectException<std::out_of_range>([&dInt] { dInt.Get<int>(2); });
			// Test that capcity was not changed
			Assert::AreEqual(dInt.Size(), (size_t)0);
			Assert::AreEqual(dInt.Capacity(), (size_t)7);


			// Testing for Float
			Datum dFloat(33.5f, 4);
			dFloat.Push(4.5f);
			dFloat.Push(8.4f);
			Assert::AreEqual(dFloat.Size(), (size_t)3);
			Assert::AreEqual(dFloat.Capacity(), (size_t)4);
			// Clear dFloat
			dFloat.Clear();
			Assert::ExpectException<std::out_of_range>([&dFloat] { dFloat.Get<float>(); });
			Assert::ExpectException<std::out_of_range>([&dFloat] { dFloat.Get<float>(1); });
			Assert::ExpectException<std::out_of_range>([&dFloat] { dFloat.Get<float>(2); });
			// Test Capacity is unchanged
			Assert::AreEqual(dFloat.Size(), (size_t)0);
			Assert::AreEqual(dFloat.Capacity(), (size_t)4);

			{
				//Test for String
				Datum dString("TestClear", 4);
				dString.Push("T2");
				dString.Push("T800");
				Assert::AreEqual(dString.Size(), (size_t)3);
				Assert::AreEqual(dString.Capacity(), (size_t)4);
				// Clear dFloat
				dString.Clear();
				Assert::ExpectException<std::out_of_range>([&dString] { dString.Get<string>(); });
				Assert::ExpectException<std::out_of_range>([&dString] { dString.Get<string>(1); });
				Assert::ExpectException<std::out_of_range>([&dString] { dString.Get<string>(2); });
				// Test Capacity is unchanged
				Assert::AreEqual(dString.Size(), (size_t)0);
				Assert::AreEqual(dString.Capacity(), (size_t)4);
			}


			// Test for Vector
			Datum dVec(Vec4(2.5f), 3);
			dVec.Push(Vec4(2, 7, 9, 1));
			dVec.Push(Vec4(4, 8, 6, 3));
			Assert::AreEqual(dVec.Size(), (size_t)3);
			Assert::AreEqual(dVec.Capacity(), (size_t)3);
			// Clear dVec
			dVec.Clear();
			Assert::ExpectException<std::out_of_range>([&dVec] { dVec.Get<Vec4>(); });
			Assert::ExpectException<std::out_of_range>([&dVec] { dVec.Get<Vec4>(1); });
			Assert::ExpectException<std::out_of_range>([&dVec] { dVec.Get<Vec4>(2); });
			// Test Capacity is unchanged
			Assert::AreEqual(dVec.Size(), (size_t)0);
			Assert::AreEqual(dVec.Capacity(), (size_t)3);


			// Test for Matrix
			Datum dMat(Mat4(3.4f), 9);
			dMat.Push(Mat4(1.4f));
			dMat.Push(Mat4(4.5f));
			Assert::AreEqual(dMat.Size(), (size_t)3);
			Assert::AreEqual(dMat.Capacity(), (size_t)9);
			// Clear dMat
			dMat.Clear();
			Assert::ExpectException<std::out_of_range>([&dMat] { dMat.Get<Mat4>(); });
			Assert::ExpectException<std::out_of_range>([&dMat] { dMat.Get<Mat4>(1); });
			Assert::ExpectException<std::out_of_range>([&dMat] { dMat.Get<Mat4>(2); });
			// Test Capactiy is unchanged
			Assert::AreEqual(dMat.Size(), (size_t)0);
			Assert::AreEqual(dMat.Capacity(), (size_t)9);
		}

		TEST_METHOD(Resize) {
			// Int
				// newSize < size
			Datum dInt(5, 5);
			dInt.Push(4);
			dInt.Push(5);
			dInt.Push(6);
			dInt.Resize(3);
			// Verifying Size and Capactiy are now equal to newSize 
			Assert::AreEqual(dInt.Size(), dInt.Capacity());
			// Making sure data was correctly transfered
			Assert::AreEqual(dInt.Get<int>(), 5);
			Assert::AreEqual(dInt.Get<int>(1), 4);
			Assert::AreEqual(dInt.Get<int>(2), 5);
				// newSize == size
			dInt.Push(8);
			dInt.Push(9);
			dInt.Resize(5);
			// Verify Size and Capacity are now the same
			Assert::AreEqual(dInt.Size(), (size_t)5);
			Assert::AreEqual(dInt.Size(), dInt.Capacity());
			// Verify data integrity
			Assert::AreEqual(dInt.Get<int>(3), 8);
			Assert::AreEqual(dInt.Get<int>(4), 9);
				// newSize > size (contains default constructed items)
			dInt.Resize(7);
			Assert::AreEqual(dInt.Size(), (size_t)5);
			Assert::AreEqual(dInt.Capacity(), (size_t)7);
			// Since they are outside of the bounds of the size you can't access the,
			// but they are default contsructed
			Assert::ExpectException<std::out_of_range>([&dInt] { dInt.Get<int>(5); });
			Assert::ExpectException<std::out_of_range>([&dInt] { dInt.Get<int>(6); });
			dInt.Push(10);
			dInt.Resize(dInt.Capacity());


			// Float
				// newSize < size
			Datum dFloat(5.1f, 2);
			dFloat.Push(7.4f);
			dFloat.Push(1.4f);
			dFloat.Resize(2);
			// Verify Size and Capacity = newSize
			Assert::AreEqual(dFloat.Capacity(), dFloat.Size());
			// Verifying Data was successfully transfered
			Assert::AreEqual(dFloat.Get<float>(), 5.1f);
			Assert::AreEqual(dFloat.Get<float>(1), 7.4f);
				// newSize == size
			// Capacity is increased with push as not to reallocate on each push
			// In other words after these two Push methods, capacity is more than 4 
			dFloat.Push(24.f);
			dFloat.Push(7.48f);
			dFloat.Resize(4);
			// Verify Size and Capacity are now the same
			Assert::AreEqual(dFloat.Size(), dFloat.Capacity());
			// Verify data integrity
			Assert::AreEqual(dFloat.Get<float>(2), 24.f);
			Assert::AreEqual(dFloat.Get<float>(3), 7.48f);
				// newSize > size (contains default constructed items)
			dFloat.Resize(7);
			Assert::AreEqual(dFloat.Size(), (size_t)4);
			Assert::AreEqual(dFloat.Capacity(), (size_t)7);
			Assert::ExpectException<std::out_of_range>([&dFloat] { dFloat.Get<float>(5); });
			Assert::ExpectException<std::out_of_range>([&dFloat] { dFloat.Get<float>(6); });
			dFloat.Push(2.6f);
			dFloat.Resize(dFloat.Capacity());


			// String
				// newSize < size
			Datum dString("Stop!", 5);
			dString.Push("Wait");
			dString.Push("A");
			dString.Push("Minute");
			// Resize
			dString.Resize(1);
			// Verify Size and Capacity = newSize
			Assert::AreEqual(dString.Capacity(), dString.Size());
			// Verify Data was transfered
			Assert::AreEqual(dString.Get<string>(), string("Stop!"));
				// newSize == size
			dString.Push("Another");
			dString.Push("One");
			dString.Resize(3);
			// Verify Size and Capacity are now the same
			Assert::AreEqual(dString.Size(), (size_t)3);
			Assert::AreEqual(dString.Size(), dString.Capacity());
			// Verify data integrity
			Assert::AreEqual(dString.Get<string>(1), string("Another"));
			Assert::AreEqual(dString.Get<string>(2), string("One"));
				// newSize > size (contains default constructed items)
			dString.Resize(5);
			Assert::AreEqual(dString.Capacity(), (size_t)5);
			Assert::AreEqual(dString.Size(), (size_t)3);
			Assert::ExpectException<std::out_of_range>([&dString] { dString.Get<string>(3); });
			Assert::ExpectException<std::out_of_range>([&dString] { dString.Get<string>(4); });
			dString.Push("Hello");
			dString.Resize(dString.Capacity());


			// Vector
				// newSize < size
			Datum dVec(Vec4(2, 4, 7, 5));
			dVec.Push(Vec4(8, 4, 6, 2));
			dVec.Push(Vec4(9, 9, 4, 2));
			// Resize
			dVec.Resize(2);
			// Verify Size and Capacity = newSize
			Assert::AreEqual(dVec.Size(), (size_t)2);
			Assert::AreEqual(dVec.Capacity(), dVec.Size());
			// Verify Data was transfered
			Assert::AreEqual(dVec.Get<Vec4>(), Vec4(2, 4, 7, 5));
			Assert::AreEqual(dVec.Get<Vec4>(1), Vec4(8, 4, 6, 2));
				// newSize == size
			dVec.Push(Vec4(5,5,6,7));
			dVec.Push(Vec4(8,6,2,5));
			dVec.Resize(4);
			// Verify Size and Capacity are now the same
			Assert::AreEqual(dVec.Size(), dVec.Capacity());
			// Verify data integrity
			Assert::AreEqual(dVec.Get<Vec4>(2), Vec4(5, 5, 6, 7));
			Assert::AreEqual(dVec.Get<Vec4>(3), Vec4(8, 6, 2, 5));
				// newSize > size (contains default constructed items)
			dVec.Resize(6);
			Assert::AreEqual(dVec.Capacity(), (size_t)6);
			Assert::AreEqual(dVec.Size(), (size_t)4);
			Assert::ExpectException<std::out_of_range>([&dVec] { dVec.Get<Vec4>(4); });
			dVec.Push(Vec4(4, 4, 5, 2));
			dVec.Resize(dVec.Capacity());


			// Matrix
				// newSize < size
			Datum dMat(Mat4(3.3f));
			dMat.Push(Mat4(4.8f));
			dMat.Push(Mat4(2.1f));
			dMat.Push(Mat4(1.9f));
			dMat.Push(Mat4(7.4f));
			// Resize
			dMat.Resize(4);
			// Verify Size and Capacity = newSize
			Assert::AreEqual(dMat.Size(), (size_t)4);
			Assert::AreEqual(dMat.Capacity(), (size_t)4);
			// Verify Data was transfered
			Assert::AreEqual(dMat.Get<Mat4>(), Mat4(3.3f));
			Assert::AreEqual(dMat.Get<Mat4>(1), Mat4(4.8f));
			Assert::AreEqual(dMat.Get<Mat4>(2), Mat4(2.1f));
			Assert::AreEqual(dMat.Get<Mat4>(3), Mat4(1.9f));
				// newSize == size
			dMat.Push(Mat4(1.4f));
			dMat.Push(Mat4(8.1f));
			dMat.Resize(6);
			// Verify Size and Capacity are now the same
			Assert::AreEqual(dMat.Size(), dMat.Capacity());
			// Verify data integrity
			Assert::AreEqual(dMat.Get<Mat4>(4), Mat4(1.4f));
			Assert::AreEqual(dMat.Get<Mat4>(5), Mat4(8.1f));
				// newSize > size (contains default constructed items)
			dMat.Resize(7);
			Assert::AreEqual(dMat.Capacity(),(size_t)7);
			Assert::AreEqual(dMat.Size(),(size_t)6);
			Assert::ExpectException<std::out_of_range>([&dMat] { dMat.Get<Mat4>(6); });
				// Test when newSize == Capacity
			dMat.Push(Mat4(2.2f));
			dMat.Resize(dMat.Capacity());
			Assert::AreEqual(dMat.Capacity(), dMat.Size());

			// When type is unsupported
			Datum dEmpty;
			Assert::ExpectException<std::runtime_error>([&dEmpty] { dEmpty.Resize(0); });
			Assert::ExpectException<std::runtime_error>([&dEmpty] { dEmpty.Resize(2); });
		}

		TEST_METHOD(SetStorage) {
			// Make sure Set Storage works and the Datum is now an external storage
			Datum dInt;
			int a[5] = {4,5,6,3,2};
			dInt.SetStorage(a, 5);
			int testI = 7;
			// Set should be able to change values in external data
			dInt.Set(0, testI);
			Assert::AreEqual(a[0], testI);
			Assert::ExpectException<std::runtime_error>([&dInt, &a] { dInt.SetStorage(a, 6); });

			Datum dFloat;
			float f[3] = { 3.5f, 1.6f, 8.4f };
			dFloat.SetStorage(f, 3);
			float testF = 7.1f;
			dFloat.Set(2, testF);
			Assert::AreEqual(f[2], testF);
			Assert::ExpectException<std::runtime_error>([&dFloat, &f] { dFloat.SetStorage(f, 6); });

			Datum dString;
			string s[2] = { "Hey", "There" };
			dString.SetStorage(s, 2);
			string testS = string("Wait");
			dString.Set(1, testS);
			Assert::AreEqual(s[1], testS);
			Assert::ExpectException<std::runtime_error>([&dString, &s] { dString.SetStorage(s, 1); });

			Datum dVec;
			Vec4 v[1] = { Vec4(2,2,4,4) };
			dVec.SetStorage(v, 1);
			Vec4 testV = Vec4(5, 1, 2, 2);
			dVec.Set(0, testV);
			Assert::AreEqual(v[0], testV);
			Assert::ExpectException<std::runtime_error>([&dVec, &v] { dVec.SetStorage(v, 1); });

			Datum dMat;
			Mat4 m[2] = { Mat4(4.4f), Mat4(6.4f) };
			dMat.SetStorage(m, 2);
			Mat4 testM = Mat4(6.1f);
			dMat.Set(0, testM);
			Assert::AreEqual(m[0], testM);
			Assert::ExpectException<std::runtime_error>([&dMat, &m] { dMat.SetStorage(m, 1); });
		}

		TEST_METHOD(Equality_Operator) {
			// Int
			Datum dInt1(4);
			Datum dInt2(4);
			Assert::IsTrue(dInt1 == dInt2);
			Assert::IsFalse(dInt1 != dInt2);

			dInt1.Push(3);
			Assert::IsFalse(dInt1 == dInt2);

			dInt2.Push(4);
			Assert::IsFalse(dInt1 == dInt2);

			// Float
			Datum dFloat1(4.5f);
			Datum dFloat2(4.5f);
			Assert::IsTrue(dFloat1 == dFloat2);
			Assert::IsFalse(dFloat1 != dFloat2);

			dFloat1.Push(1.2f);
			Assert::IsFalse(dFloat1 == dFloat2);

			dFloat2.Push(2.4f);
			Assert::IsFalse(dFloat1 == dFloat2);

			// String
			Datum dString1("Hello");
			Datum dString2(std::string("Hello"));
			Assert::IsTrue(dString1 == dString2);

			dString2.Push("Hey");
			Assert::IsFalse(dString1 == dString2);
			Assert::IsTrue(dString1 != dString2);
		}

		TEST_METHOD(RemoveAt) {
			Datum dInt(4);
			dInt.Push(6);
			dInt.Push(8);
			dInt.RemoveAt(1);

			Assert::AreEqual(dInt.Get<int>(), 4);
			Assert::AreEqual(dInt.Get<int>(1), 8);
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}