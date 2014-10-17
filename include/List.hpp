//!
//! @file				List.hpp
//! @author				Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//! @created			2014-10-16
//! @last-modified		2014-10-17
//! @brief
//! @details
//!						See README.rst in repo root dir for more info.

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

//===============================================================================================//
//======================================== HEADER GUARD =========================================//
//===============================================================================================//

#ifndef M_LIST_LIST_H
#define M_LIST_LIST_H

//===============================================================================================//
//==================================== FORWARD DECLARATION ======================================//
//===============================================================================================//

namespace MbeddedNinja
{
	template < typename dataT >
	class List;
}

//===============================================================================================//
//========================================== INCLUDES ===========================================//
//===============================================================================================//

//===== SYSTEM LIBRARIES =====//
#include <stdint.h>		// uint8_t, uint32_t, e.t.c
#include <iostream>

//===== USER LIBRARIES =====//
#include "MAssert/api/MAssertApi.hpp"

//===== USER SOURCE =====//
#include "ListNode.hpp"

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace MbeddedNinja
{

	//! @brief
	//! @details
	template < typename dataT >
	class List
	{	
		
		public:

		class Iterator
		{
			public:

			ListNode<dataT> * currListNode;
			List<dataT> * list;

			//! @brief		Postfix increment.
			Iterator & operator++(int)
			{
				// Makes sure we are not trying to increment if we are at List::End().
				M_ASSERT(this->currListNode);

				this->currListNode = this->currListNode->nextListNode;
				return *this;
			}

			bool operator!=(const Iterator & it)
			{
				if(this->currListNode != it.currListNode)
					return true;
				else
					return false;
			}

			/*bool operator<=(const Iterator & it)
			{
				if(this->currListElement == nullptr)
					return false;
				else
					return true;
			}*/

			dataT operator*() const
			{
				// Makes sure we are not trying to increment if we are at List::End().
				M_ASSERT(this->currListNode);
				return this->currListNode->data;
			}

		}; // class Iterator

		//======================================================================================//
		//==================================== PUBLIC METHODS ==================================//
		//======================================================================================//

		List() :
			numNodes(0),
			firstNode(nullptr),
			lastNode(nullptr)
		{

		}

		static void StaticInit(std::ostream * ostream, bool isDebugPrintingEnabled)
		{
			List::ostream = ostream;
			List::isDebugPrintingEnabled = isDebugPrintingEnabled;
		}

		static std::ostream * ostream;
		static bool isDebugPrintingEnabled;

		uint32_t NumNodes() const
		{
			return this->numNodes;
		}

		//! @brief		Deletes the list node pointed to by the iterator it.
		void Delete(Iterator it)
		{
			if(List::isDebugPrintingEnabled)
				std::cerr << __PRETTY_FUNCTION__ << " called." << std::endl;

			// Make sure Delete() wasn't called when there were no elements to delete
			if(this->NumNodes() == 0)
			{
				M_ASSERT_FAIL("%s", "Error: List::Delete() called when there were no nodes to delete.");
				return;
			}

			// There must be at least one element to delete now!

			ListNode<dataT> * prevListNode = it.currListNode->prevListNode;
			ListNode<dataT> * nextListNode = it.currListNode->nextListNode;

			if(prevListNode == nullptr && nextListNode == nullptr)
			{
				// There must be only one node in the list
				if(List::isDebugPrintingEnabled)
					std::cerr << "We are deleting the only node in the list." << std::endl;

				// No nodes will be left in list
				this->firstNode = nullptr;
				this->lastNode = nullptr;
			}
			else if(prevListNode == nullptr)
			{
				// We must be at the start of the list!
				if(List::isDebugPrintingEnabled)
					std::cerr << "We are deleting from the start of the list." << std::endl;


				// We need to delete the first node, and change the firstNode variable
				nextListNode->prevListNode = nullptr;
				this->firstNode = nextListNode;
			}
			else if(nextListNode == nullptr)
			{
				// We must be at the end of the list!
				if(List::isDebugPrintingEnabled)
					std::cerr << "We are deleting from the end of the list." << std::endl;

				//
				prevListNode->nextListNode = nullptr;
				this->lastNode = prevListNode;
			}
			else
			{
				// We must be somewhere in the middle of the list
				if(List::isDebugPrintingEnabled)
					std::cerr << "We are deleting from somewhere in the middle of the list." << std::endl;

				// Stitch up the previous and next list nodes together, removing the current node from the list
				prevListNode->nextListNode = nextListNode;
				nextListNode->prevListNode = prevListNode;
			}

			if(List::isDebugPrintingEnabled)
				std::cerr << "Deleting current node..." << std::endl;
			// Now that we have stitched up the list nodes before and after the one we wish to delete, we
			// can delete the current list node from memory.
			delete it.currListNode;

			// Decrement the node count
			this->numNodes--;

			if(List::isDebugPrintingEnabled)
				std::cerr << "Returning..." << std::endl;
		}

		//! @brief		Inserts data before the position specified by the iterator, unless there are currently no nodes in list, and in that case, inserts at start.
		void Insert(Iterator it, dataT & data)
		{
			if(List::isDebugPrintingEnabled)
				std::cerr << __PRETTY_FUNCTION__ << " called." << std::endl;

			// Make sure insert position is valid
			//M_ASSERT(pos <= numElements);

			ListNode<dataT> * listNode = new ListNode<dataT>();

			// Copy data to element
			listNode->data = data;


			// Now we need to insert it at the correct place
			if(numNodes == 0)
			{
				if(List::isDebugPrintingEnabled)
					std::cout << "List is empty, inserting first node." << std::endl;
				this->firstNode = listNode;
				this->lastNode = listNode;
				listNode->prevListNode = nullptr;
				listNode->nextListNode = nullptr;
			}
			else
			{
				if(it.currListNode == nullptr)
				{
					// We must be past the end of the list!
					if(List::isDebugPrintingEnabled)
						std::cout << "We are past the end of the list!" << std::endl;


					ListNode<dataT> * prevListNode = this->lastNode;
					if(List::isDebugPrintingEnabled)
						std::cout << "prevListNode = '" << prevListNode << "'." << std::endl;

					if(List::isDebugPrintingEnabled)
						std::cout << "prevListNode->data = '"<< prevListNode->data << "'." << std::endl;

					if(List::isDebugPrintingEnabled)
						std::cout << "Adjusting neighbouring list node pointers..." << std::endl;
					// Point the old list node last element to this new one
					prevListNode->nextListNode = listNode;
					listNode->prevListNode = prevListNode;

					// We are at the last node in the list
					listNode->nextListNode = nullptr;

					this->lastNode = listNode;
				}
				else
				{
					if(List::isDebugPrintingEnabled)
						std::cout << "We are inserting before a existing and valid node!" << std::endl;

					//===== PREV NODE =====//
					ListNode<dataT> * prevListNode = it.currListNode->prevListNode;
					if(List::isDebugPrintingEnabled)
						std::cout << "prevListNode = '" << prevListNode << "'." << std::endl;

					if(List::isDebugPrintingEnabled)
						std::cout << "prevListNode->data = '"<< prevListNode->data << "'." << std::endl;

					if(List::isDebugPrintingEnabled)
						std::cout << "Adjusting neighbouring list node pointers..." << std::endl;
					// Point the old list node last element to this new one
					prevListNode->nextListNode = listNode;
					listNode->prevListNode = prevListNode;

					//===== NEXT NODE =====//
					ListNode<dataT> * nextListNode = it.currListNode;
					if(List::isDebugPrintingEnabled)
						std::cout << "currListNode = '" << nextListNode << "'." << std::endl;

					if(List::isDebugPrintingEnabled)
						std::cout << "currListNode->data = '"<< nextListNode->data << "'." << std::endl;

					if(List::isDebugPrintingEnabled)
						std::cout << "Adjusting neighbouring list node pointers..." << std::endl;
					// Point the old list node last element to this new one
					nextListNode->prevListNode = listNode;
					listNode->nextListNode = nextListNode;

				}
			}

			// Increment element count
			numNodes++;
		}

		//! @brief		Returns an iterator to the first node in the list.
		//! @details	Mimics the behaviour of std::begin().
		Iterator Start()
		{
			if(List::isDebugPrintingEnabled)
				*List::ostream << __PRETTY_FUNCTION__ << " called." << std::endl;
			Iterator it;
			it.list = this;
			// This could be nullptr if there are no nodes in the list
			it.currListNode = this->firstNode;
			return it;
		}

		//! @brief		Returns the 'past-the-end' node of the list.
		//! @details	This behaviour is so that you can move through an iterator inside a for loop and use the it <! List::End() guard to stop the for loop. Mimics the behaviour of std::end().
		Iterator End()
		{
			if(List::isDebugPrintingEnabled)
				std::cout << __PRETTY_FUNCTION__ << " called." << std::endl;
			Iterator it;
			it.list = this;
			// Return the 'past-the-end' node, not the last node.
			it.currListNode = nullptr;
			return it;
		}

		//======================================================================================//
		//============================ PUBLIC OPERATOR OVERLOAD DECLARATIONS ===================//
		//======================================================================================//



		//======================================================================================//
		//================================= PUBLIC VARIABLES ===================================//
		//======================================================================================//
				

				
		private:
						
		//======================================================================================//
		//=================================== PRIVATE METHODS ==================================//
		//======================================================================================//

		// none

		//======================================================================================//
		//================================== PRIVATE VARIABLES =================================//
		//======================================================================================//

		uint32_t numNodes;

		//! @brief		Pointer to the first node in the list.
		ListNode<dataT> * firstNode;
		ListNode<dataT> * lastNode;

		
	}; // class List

	template < typename dataT >
	std::ostream * List<dataT>::ostream = nullptr;

	template < typename dataT >
	bool List<dataT>::isDebugPrintingEnabled = false;


} // namespace MbeddedNinja

#endif	// #ifndef M_LIST_LIST_H

// EOF
