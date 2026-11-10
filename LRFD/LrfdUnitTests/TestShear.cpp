#include "pch.h"
#include <stdexcept>
#include <cmath>
#include <LRFD/Shear.h>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WBFL::LRFD;

namespace LrfdUnitTests
{
	TEST_CLASS(TestShear)
	{
	public:
		

      TEST_METHOD(LongitudinalShearDemand)
      {
         Float64 theta = std::acos(-1.0) / 4.0;
         Assert::AreEqual(90.0, Shear::LongitudinalShearDemand(100, 20, 40, 2, 1, 1, 1, 0, 20, theta), 1e-10);
         // Negative moment/shear, axial compression and the stirrup cap.
         Assert::AreEqual(60.0, Shear::LongitudinalShearDemand(-100, -20, -40, 2, 1, 1, 1, 0, 200, theta), 1e-10);
         Assert::ExpectException<std::invalid_argument>([theta] {
            Shear::LongitudinalShearDemand(1, 0, 1, 0, 1, 1, 1, 0, 0, theta);
         });
      }
		TEST_METHOD(ShearStress)
		{
			BDSAutoVersion av;
			BDSManager::SetEdition(BDSManager::Edition::ThirdEditionWith2006Interims); // changed in 2007
			Assert::AreEqual(0.29320987654320985, Shear::ComputeShearStress(100, 90, 0.9, 6, 12), 0.001);
			Assert::AreEqual(-1.0956790123456788, Shear::ComputeShearStress(10, 90, 0.9, 6, 12), 0.001);

			BDSManager::SetEdition(BDSManager::Edition::FourthEdition2007);
			Assert::AreEqual(0.29320987654320985, Shear::ComputeShearStress(100, 90, 0.9, 6, 12), 0.001);
			Assert::AreEqual(1.0956790123456788, Shear::ComputeShearStress(10, 90, 0.9, 6, 12), 0.001);
		}

		TEST_METHOD(VciVcw)
		{
			// US units
			WBFL::Units::AutoSystem au;
			WBFL::Units::System::SetMassUnit(WBFL::Units::Measure::_12KSlug);
			WBFL::Units::System::SetLengthUnit(WBFL::Units::Measure::Inch);
			WBFL::Units::System::SetAngleUnit(WBFL::Units::Measure::Radian);

			BDSAutoVersion av;

			ShearData data;
			// This method is only valid between 3rd Edition 2006 and was removed in 8th Edition 2017
			BDSManager::SetEdition(BDSManager::Edition::ThirdEditionWith2006Interims);
			Assert::ExpectException<XCodeVersion>([&]() {Shear::ComputeVciVcw(&data); });

			BDSManager::SetEdition(BDSManager::Edition::EighthEdition2017);
			Assert::ExpectException<XCodeVersion>([&]() {Shear::ComputeVciVcw(&data); });

			BDSManager::SetEdition(BDSManager::Edition::FourthEdition2007);

			data.Mu = 500;
			data.Nu = 100;
			data.Vu = 50;
			data.phi = 0.85;
			data.Vp = 20;
			data.dv = 48;
			data.bv = 6;
			data.Es = 28000;
			data.As = 1.25;
			data.Eps = 28500;
			data.Aps = 6.27;
			data.EptSegment = 28500;
			data.AptSegment = 1.29;
			data.EptGirder = 28500;
			data.AptGirder = 1.88;
			data.Ec = 4000;
			data.Ac = 500;
			data.fpops = 200;
			data.fpoptSegment = 200;
			data.fpoptGirder = 200;
			data.fc = 7;
			data.Vd = 45;
			data.Vi = 65;
			data.Mcre = 235;
			data.fpc = 0.55;
			data.lambda = 1.0;

			Shear::ComputeVciVcw(&data);

			Assert::AreEqual(45.718582655252860, data.VciMin, 0.0001);
			Assert::AreEqual(90.789527551750950, data.VciCalc, 0.0001);
			Assert::AreEqual(90.789527551750950, data.Vci, 0.0001);
			Assert::AreEqual(113.23858265524154, data.Vcw, 0.0001);
		}

		TEST_METHOD(BetaTheta_Details)
		{
			// US units
			WBFL::Units::AutoSystem au;
			WBFL::Units::System::SetMassUnit(WBFL::Units::Measure::_12KSlug);
			WBFL::Units::System::SetLengthUnit(WBFL::Units::Measure::Inch);
			WBFL::Units::System::SetAngleUnit(WBFL::Units::Measure::Radian);

			BDSAutoVersion av;

			ShearData data;
			data.Mu = 500;
			data.Nu = 100;
			data.Vu = 50;
			data.phi = 0.85;
			data.Vp = 20;
			data.dv = 48;
			data.bv = 6;
			data.Es = 28000;
			data.As = 1.25;
			data.Eps = 28500;
			data.Aps = 6.27;
			data.EptSegment = 28500;
			data.AptSegment = 1.29;
			data.EptGirder = 28500;
			data.AptGirder = 1.88;
			data.Ec = 4000;
			data.Ac = 500;
			data.fpops = 200;
			data.fpoptSegment = 200;
			data.fpoptGirder = 200;
			data.fc = 7;
			data.Vd = 45;
			data.Vi = 65;
			data.Mcre = 235;
			data.fpc = 0.55;
			data.lambda = 1.0;
			data.fy = 60.0;
			data.AvS = 0.9;
			data.sx = 2;
			data.ag = 0.1;

			Shear::ComputeThetaAndBeta(&data, Shear::Method::Tables);
			Assert::AreEqual(6.32, data.Beta);
			Assert::AreEqual(0.38920842319377003, data.Theta);
			Assert::AreEqual((Int16)1, data.BetaTheta_tbl);
			Assert::AreEqual(0.019257703081232497, data.vufc);
			Assert::AreEqual(0.075, data.vufc_tbl, 0.001);
			Assert::AreEqual(0.0, data.ex);
			Assert::AreEqual(-0.0002, data.ex_tbl);
			Assert::AreEqual(-1.0, data.Fe);


			Shear::ComputeThetaAndBeta(&data, Shear::Method::Equations);
			Assert::AreEqual(6.8571428571428577, data.Beta);
			Assert::AreEqual(0.48171087354924003, data.Theta);
			Assert::AreEqual((Int16)1, data.BetaTheta_tbl);
			Assert::AreEqual(0.019257703081232497, data.vufc);
			Assert::AreEqual(-1.0, data.vufc_tbl, 0.001);
			Assert::AreEqual(-0.0004, data.ex);
			Assert::AreEqual(-0.0002, data.ex_tbl);
			Assert::AreEqual(-1.0, data.Fe);
		}


      TEST_METHOD(MBE4RatingAllowances)
      {
         WBFL::Units::AutoSystem au;
         WBFL::Units::System::SetMassUnit(WBFL::Units::Measure::_12KSlug);
         WBFL::Units::System::SetLengthUnit(WBFL::Units::Measure::Inch);
         WBFL::Units::System::SetAngleUnit(WBFL::Units::Measure::Radian);
         BDSAutoVersion av;
         MBEAutoVersion mv;
         BDSManager::SetEdition(BDSManager::Edition::TenthEdition2024);
         MBEManager::SetEdition(MBEManager::Edition::FourthEdition2026);
         ShearData data;
         data.Mu = 500; data.Vu = 50; data.phi = 0.9;
         data.dv = 48; data.bv = 6; data.Es = 29000; data.As = 2;
         data.Eps = 28500; data.Aps = 6; data.fc = 7; data.fy = 60;
         data.sx = 48; data.ag = 0.75; data.AvS = 0;
         data.fpc = 0.02 * data.fc;
         Shear::ComputeRatingThetaAndBeta(&data, true);
         Assert::AreEqual(0.0, data.ex);
         Assert::AreEqual(4.8, data.Beta, 1e-10);
         Assert::AreEqual(WBFL::Units::ConvertToSysUnits(29.0, WBFL::Units::Measure::Degree), data.Theta, 1e-10);
         ShearData noAllowance(data);
         ShearData ordinary(data);
         Shear::ComputeRatingThetaAndBeta(&noAllowance, true, false, false);
         Shear::ComputeThetaAndBeta(&ordinary, Shear::Method::Equations);
         Assert::AreEqual(ordinary.ex, noAllowance.ex);
         Assert::AreEqual(ordinary.Theta, noAllowance.Theta);
         Assert::AreEqual(ordinary.Beta, noAllowance.Beta);
         ShearData sizeOnly(data);
         Shear::ComputeRatingThetaAndBeta(&sizeOnly, true, false, true);
         Assert::AreEqual(ordinary.ex, sizeOnly.ex);
         Assert::AreEqual((Int16)1, sizeOnly.BetaEqn);
         ShearData strainOnly(data);
         Shear::ComputeRatingThetaAndBeta(&strainOnly, true, true, false);
         Assert::AreEqual(0.0, strainOnly.ex);
         Assert::IsTrue(strainOnly.Beta < 4.8);
         data.fpc = 0.0199 * data.fc;
         Shear::ComputeRatingThetaAndBeta(&data, true);
         Assert::IsTrue(data.Beta < 4.8);
         data.fpc = 0.02 * data.fc;
         Shear::ComputeRatingThetaAndBeta(&data, false);
         Assert::IsTrue(0.0 < data.ex); // cracked sections retain the computed strain
         Assert::AreEqual((Int16)1, data.BetaEqn); // prestress exception also applies when cracked
         ShearData crackedDesign(data);
         Shear::ComputeThetaAndBeta(&crackedDesign, Shear::Method::Equations);
         Assert::IsTrue(crackedDesign.Beta < data.Beta); // design retains the size effect
         data.Aps = 0; data.AptGirder = 6; data.EptGirder = 28500;
         Shear::ComputeRatingThetaAndBeta(&data, true);
         Assert::AreEqual(4.8, data.Beta, 1e-10); // post-tensioning also qualifies
         data.AptGirder = 0;
         Shear::ComputeRatingThetaAndBeta(&data, true);
         Assert::IsTrue(data.Beta < 4.8); // prestress exception cannot apply to RC
         MBEManager::SetEdition(MBEManager::Edition::ThirdEditionWith2020Interims);
         ShearData design(data);
         Shear::ComputeThetaAndBeta(&design, Shear::Method::Equations);
         Shear::ComputeRatingThetaAndBeta(&data, true);
         Assert::AreEqual(design.ex, data.ex);
         Assert::AreEqual(design.Beta, data.Beta);
         Assert::AreEqual(design.Theta, data.Theta);
      }

		TEST_METHOD(BetaTheta_WSDOT)
		{
			// LRFD Shear 2001 Interims, modified by WSDOT Design Memo 7-2001... Dated June 18, 2001

			// US units
			WBFL::Units::AutoSystem au;
			WBFL::Units::System::SetMassUnit(WBFL::Units::Measure::_12KSlug);
			WBFL::Units::System::SetLengthUnit(WBFL::Units::Measure::Inch);
			WBFL::Units::System::SetAngleUnit(WBFL::Units::Measure::Radian);

			BDSAutoVersion av;

			ShearData data;
			data.Mu = 500;
			data.Nu = 100;
			data.Vu = 50;
			data.phi = 0.85;
			data.Vp = 20;
			data.dv = 48;
			data.bv = 6;
			data.Es = 28000;
			data.As = 1.25;
			data.Eps = 28500;
			data.Aps = 6.27;
			data.EptSegment = 28500;
			data.AptSegment = 1.29;
			data.EptGirder = 28500;
			data.AptGirder = 1.88;
			data.Ec = 4000;
			data.Ac = 500;
			data.fpops = 200;
			data.fpoptSegment = 200;
			data.fpoptGirder = 200;
			data.fc = 7;
			data.Vd = 45;
			data.Vi = 65;
			data.Mcre = 235;
			data.fpc = 0.55;
			data.lambda = 1.0;
			data.fy = 60.0;
			data.AvS = 0.9;
			data.sx = 2;
			data.ag = 0.1;

			WsdotShear::ComputeThetaAndBeta(&data, true);
			Assert::AreEqual(6.32, data.Beta);
			Assert::AreEqual(0.38920842319377003, data.Theta);
			Assert::AreEqual((Int16)0, data.BetaTheta_tbl);
			Assert::AreEqual(0.019257703081232497, data.vufc);
			Assert::AreEqual(0.075, data.vufc_tbl, 0.001);
			Assert::AreEqual(-0.00039009377730710691, data.ex);
			Assert::AreEqual(-0.0002, data.ex_tbl);
			Assert::AreEqual(-1.0, data.Fe);


			WsdotShear::ComputeThetaAndBeta(&data, false);
			Assert::AreEqual(6.32, data.Beta);
			Assert::AreEqual(0.38920842319377003, data.Theta);
			Assert::AreEqual((Int16)0, data.BetaTheta_tbl);
			Assert::AreEqual(0.019257703081232497, data.vufc);
			Assert::AreEqual(0.075, data.vufc_tbl, 0.001);
			Assert::AreEqual(-0.00039009377730710691, data.ex);
			Assert::AreEqual(-0.0002, data.ex_tbl);
			Assert::AreEqual(-1.0, data.Fe);
		}

		TEST_METHOD(BetaTheta_AllSpecs)
		{
			// US units
			WBFL::Units::AutoSystem au;
			WBFL::Units::System::SetMassUnit(WBFL::Units::Measure::_12KSlug);
			WBFL::Units::System::SetLengthUnit(WBFL::Units::Measure::Inch);
			WBFL::Units::System::SetAngleUnit(WBFL::Units::Measure::Radian);

			BDSAutoVersion av;

			ShearData data;
			data.Mu = 500;
			data.Nu = 100;
			data.Vu = 50;
			data.phi = 0.85;
			data.Vp = 20;
			data.dv = 48;
			data.bv = 6;
			data.Es = 28000;
			data.As = 1.25;
			data.Eps = 28500;
			data.Aps = 6.27;
			data.EptSegment = 28500;
			data.AptSegment = 1.29;
			data.EptGirder = 28500;
			data.AptGirder = 1.88;
			data.Ec = 4000;
			data.Ac = 500;
			data.fpops = 200;
			data.fpoptSegment = 200;
			data.fpoptGirder = 200;
			data.fc = 7;
			data.Vd = 45;
			data.Vi = 65;
			data.Mcre = 235;
			data.fpc = 0.55;
			data.lambda = 1.0;
			data.fy = 60.0;
			data.AvS = 0.9;
			data.sx = 2;
			data.ag = 0.1;

			std::vector<BDSManager::Edition> specs
			{
				BDSManager::Edition::FirstEdition1994,
				BDSManager::Edition::FirstEditionWith1997Interims,
				BDSManager::Edition::SecondEditionWith1999Interims,
				BDSManager::Edition::SecondEditionWith2000Interims,
				BDSManager::Edition::SecondEditionWith2002Interims,
				BDSManager::Edition::SecondEditionWith2003Interims,
				BDSManager::Edition::ThirdEditionWith2005Interims,
				BDSManager::Edition::FourthEditionWith2008Interims,
			};

			std::vector<std::pair<Float64, Float64>> expected_tables
			{
				{ 6.78, 0.47123889803730001 }, // 1st Edition 1994
				{ 6.78, 0.47123889803730001 }, // 1st Edition 1997
				{ 6.78, 0.47123889803730001 }, // 2nd Edition 1999
				{ 6.32, 0.38920842319377003 }, // 2nd Edition 2000
				{ 6.32, 0.38920842319377003 }, // 2nd Edition 2002
				{ 6.32, 0.38920842319377003 }, // 2nd Edition 2003
				{ 6.32, 0.38920842319377003 }, // 3rd Edition 2005
				{ 6.32, 0.38920842319377003 }, // 4th Edition 2008
			};

			std::vector<std::pair<Float64, Float64>> expected_equations
			{
				{ -1.0, -1.0 },// 1st Edition 1994
				{ -1.0, -1.0 },// 1st Edition 1997
				{ -1.0, -1.0 },// 2nd Edition 1999
				{ -1.0, -1.0 },// 2nd Edition 2000
				{ -1.0, -1.0 },// 2nd Edition 2002
				{ -1.0, -1.0 },// 2nd Edition 2003
				{ -1.0, -1.0 },// 3rd Edition 2005
				{ 6.8571428571428577, 0.48171087354924003 },// 4th Edition 2008
			};

			int i = 0;
			for(const auto& spec : specs)
			{
				BDSManager::SetEdition(spec);

				Shear::ComputeThetaAndBeta(&data, Shear::Method::Tables);
				Assert::AreEqual(expected_tables[i].first, data.Beta, 0.0001);
				Assert::AreEqual(expected_tables[i].second, data.Theta, 0.0001);

				if (BDSManager::Edition::FourthEditionWith2008Interims <= BDSManager::GetEdition())
				{
					// equations where introduced in 2008 interims to the 4th edition
					Shear::ComputeThetaAndBeta(&data, Shear::Method::Equations);
					Assert::AreEqual(expected_equations[i].first, data.Beta, 0.0001);
					Assert::AreEqual(expected_equations[i].second, data.Theta, 0.0001);
				}

				i++;
			}
		}
	};
}
