//!
//! @file 			DeleteNodeTests.cpp
//! @author 		Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//! @created		2014-10-17
//! @last-modified 	2014-10-19
//! @brief 			Contains tests which make sure nodes can be deleted from lists correctly.
//! @details
//!					See README.rst in root dir for more info.

//===== SYSTEM LIBRARIES =====//
#include <cstdint>	// uint32_t, e.t.c

//====== USER LIBRARIES =====//
#include "MUnitTest/api/MUnitTestApi.hpp"

//===== USER SOURCE =====//
#include "../api/MListApi.hpp"

using namespace MbeddedNinja;

namespace MListTestsNs
{

	MTEST(DeleteOneNodeTest)
	{
		//List<double>::isDebugPrintingEnabled = true;

		List<double> list;

		double myDouble = 5.5;
		list.Insert(list.Start(), myDouble);

		List<double>::Iterator it;

		// Lets delete all nodes
		for(it = list.Start(); it != list.End(); it++)
		{
			// Delete all nodes
			list.Delete(it);
		}

		for(it = list.Start(); it != list.End(); it++)
		{
			// There should be no nodes, so if we get here, something went wrong!
			CHECK(false);
		}


	}

	MTEST(DeleteFromEndTest)
	{
		//List<double>::isDebugPrintingEnabled = true;

		List<double> list;

		const uint32_t sizeOfArray = 10;

		double myDoubleA[sizeOfArray];
		myDoubleA[0] = 5.5;
		myDoubleA[1] = -3.4;
		myDoubleA[2] = 2.2;
		myDoubleA[3] = 1.1;
		myDoubleA[4] = 100.2;
		myDoubleA[5] = -34.2;
		myDoubleA[6] = 21.2;
		myDoubleA[7] = 0.6;
		myDoubleA[8] = -0.6;
		myDoubleA[9] = 1.345;

		List<double>::Iterator it;
		it = list.Start();

		for(uint32_t x = 0; x < sizeOfArray; x++)
		{
			list.Insert(it, myDoubleA[x]);
		}

		uint32_t count = 0;
		for(it = list.Start(); it != list.End(); it++)
		{
			//std::cout << "*it = " << *it << std::endl;
			// Check capacity is calculated correctly
			CHECK_EQUAL(*it, myDoubleA[count]);

			count++;
		}

		// Goto the end of the list
		it = list.End();
		it--;

		// Delete the last node
		list.Delete(it);

		count = 0;
		for(it = list.Start(); it != list.End(); it++)
		{
			CHECK_EQUAL(*it, myDoubleA[count]);

			count++;
		}

		CHECK_EQUAL(list.NumNodes(), 9);

		//List<double>::isDebugPrintingEnabled = false;

	}

	MTEST(DeleteInMiddleTest)
	{
		List<double> list;

		const uint32_t sizeOfArray = 10;

		double myDoubleA[sizeOfArray];
		myDoubleA[0] = 5.5;
		myDoubleA[1] = -3.4;
		myDoubleA[2] = 2.2;
		myDoubleA[3] = 1.1;
		myDoubleA[4] = 100.2;
		myDoubleA[5] = -34.2;
		myDoubleA[6] = 21.2;
		myDoubleA[7] = 0.6;
		myDoubleA[8] = -0.6;
		myDoubleA[9] = 1.345;

		List<double>::Iterator it;
		it = list.Start();

		for(uint32_t x = 0; x < sizeOfArray; x++)
		{
			list.Insert(it, myDoubleA[x]);
		}

		uint32_t count = 0;
		for(it = list.Start(); it != list.End(); it++)
		{
			//std::cout << "*it = " << *it << std::endl;
			// Check capacity is calculated correctly
			CHECK_EQUAL(*it, myDoubleA[count]);

			count++;
		}

		// Now lets delete one from the middle
		//List<double>::isDebugPrintingEnabled = true;

		it = list.Start();
		it++;
		it++;

		// Insert double into the third position
		list.Delete(it);

		count = 0;
		for(it = list.Start(); it != list.End(); it++)
		{
			//std::cout << "*it = " << *it << std::endl;

			if(count < 2)
			{
				CHECK_EQUAL(*it, myDoubleA[count]);
			}
			else if(count == 2)
			{
				//CHECK_EQUAL(*it, insertedDouble);
			}
			else
			{
				CHECK_EQUAL(*it, myDoubleA[count+1]);
			}

			count++;
		}

	}

} // namespace MListTestsNs
