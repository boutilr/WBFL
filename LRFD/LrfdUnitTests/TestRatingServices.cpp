#include "pch.h"
#include "CppUnitTest.h"
#include <LRFD/MBERating.h>
#include <LRFD/RatingSolver.h>
#include <cmath>
#include <limits>
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WBFL::LRFD;

namespace LrfdUnitTests
{
   TEST_CLASS(TestRatingServices)
   {
   public:
      TEST_METHOD(ServiceIIIInventoryFactor)
      {
         // MBE 4 Table 6A.4.2.2-2: both conditions are needed.
         Assert::AreEqual(0.8, MBERating::GetServiceIIIInventoryLiveLoadFactor(false, false));
         Assert::AreEqual(0.8, MBERating::GetServiceIIIInventoryLiveLoadFactor(true, false));
         Assert::AreEqual(0.8, MBERating::GetServiceIIIInventoryLiveLoadFactor(false, true));
         Assert::AreEqual(1.0, MBERating::GetServiceIIIInventoryLiveLoadFactor(true, true));
      }

      TEST_METHOD(IteratedRatingFactor)
      {
         auto fixed = RatingSolver::Solve([](Float64 rf) { return 40.0 - 20.0 * rf; });
         Assert::AreEqual(2.0, fixed, 1e-6);
         auto variable = RatingSolver::Solve([](Float64 rf) { return 100.0 / (1.0 + 0.1 * rf) - 10.0 - 20.0 * rf; });
         Assert::AreEqual((-21.0 + std::sqrt(1161.0)) / 4.0, variable, 1e-6);
         Assert::AreEqual(0.0, RatingSolver::Solve([](Float64) { return -1.0; }));
         Assert::AreEqual((std::numeric_limits<Float64>::max)(), RatingSolver::Solve([](Float64) { return 1.0; }));
         // A force reversal can switch the tension tie between ordinary samples.
         auto reversal = RatingSolver::Solve([](Float64 rf) { return 0.51 <= rf && rf < 0.52 ? -1.0 : 1.0; }, { 0.51, 0.52 });
         Assert::AreEqual(0.51, reversal, 1e-6);
         Assert::ExpectException<std::runtime_error>([] {
            RatingSolver::Solve([](Float64) { return (std::numeric_limits<Float64>::quiet_NaN)(); });
         });
      }
   };
}
