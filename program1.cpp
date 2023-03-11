#include "program1.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Creates a vector to represent the priority integer heap
vector<int> jobs;

// Creates a vector to represent the number of GetJob() calls
vector<int> calls;



//Simple heap print to observe changes.
void PrintHeap()
{
	for (int i = 0; i < jobs.size(); i++)
		cout << "Element " << i << ":\t" << jobs.at(i) << "\n";
	cout << "\n";
}

// Resets the heap to be empty.
void EmptyHeap() {
	jobs.clear();
	calls.clear();
}

//Given the depth, determine how many elements occupy said depth.
int ElementsInDepth(int depth) {
	int n = 0;
	for (int idx = 0; idx <= depth; idx++)
		n += pow(4, idx);
	return n;
}



//Returns the depth of the job with the specified index
//Root node has depth of 0
int GetDepthFromIndex(int index) {
	int previousPowers = 1;
	//If the index is within 0 and the size of heap
	if (index >= 0 && index <= jobs.size()) {
		//Check to see if our index is between the ranges of 4^n-1 and 4^n where n is the depth that index occurs in
		for (int j = 0; index >= pow(4, j); j++)
		{
			if (index <= pow(4, j + 1) + previousPowers)
				return j + 1;
			previousPowers += pow(4, j+1);
		}
		return 0;
	}
	return NO_DEPTH;
}

//Given 0-based index, determine the node's inline index appropriate for the depth
int GetInlineIndex(int index) {
	if (index > 0)
		return index - ElementsInDepth(GetDepthFromIndex(index)-1);
	else
		return 0;
}

//Find the index of the parent node provided the index of child (index starts at 0 for this function) returns 0 based index
// Given k is the child index
//  k=0	-> No parent node (calling function should know this already)
//  k=[1,5] -> Root node is parent
//	Otherwise, Nth node is parent
int FindParent(int index) {
	if (index > 5) {
		//Add the elements in the depths, previous of the parent's depth, and then add how many nodes to the right the parent is in its current depth.
		return ElementsInDepth(GetDepthFromIndex(index) - 2) + (ceil(((index + 1) - ElementsInDepth(GetDepthFromIndex(index) - 1.0)) / 4.0)) - 1;
	}
	else { return 0; }
}

// Adds a job to the heap while also swapping parent nodes if they are less than the specified priority.
void AddJob(int priority) {	
	//If priority is a non-negative integer
	if (priority > 0) {
		//Add job with specified priority to the end of the heap
		jobs.push_back(priority);
		//Initialize newly added job to have 0 GetJob() calls
		calls.push_back(0);

		//Store the last index of our heap with the newly added job
		int childIndex = jobs.size()-1;
		
		//Check whether the parent nodes' priorities are less than our added priority
		for (int idx = GetDepthFromIndex(jobs.size()); idx > 0; idx--) {
			//If the parent node is less than child node
			if (jobs.at(FindParent(childIndex)) < priority) {
				//Swap the parent and the child node's priorities
				jobs.at(childIndex) = jobs.at(FindParent(childIndex));
				jobs.at(FindParent(childIndex)) = priority;
				
				//Swap the parent and the child node's calls values
				int childCallVal = calls.at(childIndex);
				calls.at(childIndex) = calls.at(FindParent(childIndex));
				calls.at(FindParent(childIndex)) = childCallVal;

				//Change the childIndex to our child's new index
				childIndex = FindParent(childIndex);
			}
			else { break; }
		}
	}
	//Else, priority is negative integer therefore no job is added.
}

//Finds the 0-based index of the largest child node provided the index of parent (0 based)
int FindLargestChild(int parentIndex) {
	//First, find the range of the children nodes
	//Compare children nodes and return largest one
	int childrenIndex = ElementsInDepth(GetDepthFromIndex(parentIndex)) + (4 * GetInlineIndex(parentIndex));
	int largestIndex = parentIndex;

	
	for (int childIndex = childrenIndex; childIndex < childrenIndex+4 && childIndex < jobs.size(); childIndex++)
		if (jobs.at(childIndex) > jobs.at(largestIndex))
			largestIndex = childIndex;
	
	return largestIndex;
}


// Removes the job at the top of the heap and returns its priority.
int GetJob() {
	//If there are jobs in the heap
	if (jobs.size() > 0)
	{
		//Get the priority of the job at the top of the heap
		int jobPriority = jobs.at(0);

		//Swap the top node with the last node in the heap
		int topNodeIndex = 0;
		jobs.at(0) = jobs.at(jobs.size() - 1);
		int topNodePriority = jobs.at(0);

		//Remove last node in heap (Originally top node)
		jobs.erase(jobs.end()-1);
		calls.erase(calls.end() - 1);

		//Push down new top node if topNode < any child node

		//For each time the topNode moves down, compare all children
		//Swap with the child that holds the greatest difference
		for (int j = 0; j < GetDepthFromIndex(jobs.size()); j++) {
			if (topNodePriority < jobs.at(FindLargestChild(topNodeIndex))) {
				int largestIndex = FindLargestChild(topNodeIndex);
				//Swap priorities
				jobs.at(topNodeIndex) = jobs.at(largestIndex);
				jobs.at(largestIndex) = topNodePriority;

				//Swap call counts
				int topCallVal = calls.at(largestIndex);
				calls.at(topNodeIndex) = calls.at(largestIndex);
				calls.at(largestIndex) = calls.at(topNodeIndex);

				//Change topNodeIndex
				topNodeIndex = largestIndex;
			}
		}

		//Lastly, increase priority of jobs by 2 that have a corresponding calls values >5
		int callIndex = 0;
		for (int i = 0; i < GetDepthFromIndex(jobs.size())+1; i++) {
			for (int j = 0; j < pow(4, i); j++) {
				if (callIndex < jobs.size()) {
					if (calls.at(callIndex) >= 4) {
						jobs.at(callIndex) += 2;
					}
					calls.at(callIndex) += 1;
					callIndex++;
				}
			}
		}

		return jobPriority;
	}
	//Else, no jobs in the heap therefore return NO_JOB
	return NO_JOB;
}

// Returns the depth of the first job found with the specified priority.
int GetDepth(int priority) {
	//Find the index of priority
	for (int idx = 0; idx < jobs.size(); idx++) {
		//We found our priority
		if (jobs.at(idx) == priority) {
			//Find the depth of the job
			for (int j = 0; idx >= pow(4, j); j++) {
				if (idx < ElementsInDepth(j) + pow(4,j+1))
					return j+1;
			}
			return 0;
		}
	}
	return NO_DEPTH;
}

// Returns the priority of the job with the specified index.
int GetPriority(int index) {
	//Check to make sure there is a valid priority with the given index
	if (index > 0 && index <= jobs.size()) {
		//Index starts at 1 so offset our vector index by 1
		return jobs.at(index - 1);
	}
	return NO_PRIORITY;
}
