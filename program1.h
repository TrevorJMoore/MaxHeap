#ifndef PROGRAM1_H
#define PROGRAM1_H

// Constants that should be used when implementing your code.
#define NO_DEPTH	-1	// No depth
#define NO_JOB		-1	// No job
#define NO_PRIORITY	-1	// No priority

/*** Below is the list of functions that must be implemented by the program. ***/

// Resets the heap to be empty.
void EmptyHeap();

// Adds a job to the heap with the specified priority.
void AddJob(int priority);

// Remove the job at the top of the heap and returns its priority.
int GetJob();

// Returns the depth of the first job found with the specified priority.
int GetDepth(int priority);

// Returns the priority of the job with the specified index.
int GetPriority(int index);

#endif
