#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <string>

std::map < std::string, std::tuple<float, float>> num2Coord;

enum move
{
	L = 0,
	R = 1,
	U = 2,
};

struct State
{
	std::string state;	// string representation of the state
	move finger;
	State* prevState;	// Previous state pointer
	float gcost;		// Move cost
	float hcost;		// Heuristic cost
	float tcost;
};

class Solver
{
	// Initializing intial and final states
	State* initState = new State();
	State* finalState = new State();

public:
	Solver() = default;

  Solver(std::string input)
  {
    finalState->state = "xx" + input;
		finalState->finger = U;
		finalState->prevState = NULL;
		finalState->gcost = 0;
		finalState->hcost = 0;
		finalState->tcost = 0;

		// Init state init
		initState->finger = U;
		initState->state = "*#";
		initState->prevState = NULL;
		initState->gcost = 0;
		initState->hcost = 0;
		initState->tcost = 0;
  }

	void GetInput()
	{
		// Final state init
    std::string inputNumber;
		std::cout << "Please enter a phone number:	";
		std::cin >> inputNumber; // Final state is input by the user
    finalState->state = "xx" + inputNumber;
		finalState->finger = U;
		finalState->prevState = NULL;
		finalState->gcost = 0;
		finalState->hcost = 0;
		finalState->tcost = 0;

		// Init state init
		initState->finger = U;
		initState->state = "*#";
		initState->prevState = NULL;
		initState->gcost = 0;
		initState->hcost = 0;
		initState->tcost = 0;
	}

	void DisplayFinalState()
	{
		std::cout << finalState->state;
	}

	bool Solve()
	{
		std::vector<State*> frontier;
		std::vector<State*> explored;
		State* currentState;

		// Pushing the intial state in frontiers
		frontier.push_back(this->initState);

    std::cout << "INIT STATE: " << initState->state;

		// Keep searching while there are nodes in frontiers
		while (frontier.size() > 0)
		{
			// std::cout << "Front Size: " << frontier.size() << std::endl;

			// Removing a node from frontiers and putting into explored
			// as we are going to expand it in the next step
			int least_cost_index = GetLeastCostStateIndex(&frontier);
			currentState = frontier[least_cost_index];
			explored.push_back(frontier[least_cost_index]);
			frontier.erase(frontier.begin() + least_cost_index);

			// Checking if the current node is the goal
			if (currentState->state.length() == finalState->state.length())
			{
        finalState = currentState;
				std::cout << "Optimal solution found" << std::endl;
				return true;
			}

			// Expanding the current node
			for (int m = L; m != U; m++)
			{
				// Creating new successor state
				State* succState = new State();
				
				// Setting successor's state [ADDING NEW NUMBER AT THE END]
				succState->state = currentState->state;
        succState->state[m] = finalState->state[succState->state.size()]; // Updating the finger pos
				succState->state += finalState->state[succState->state.size()];   // Adding the latest number in state

        // current state is the previous state of the succState
				succState->prevState = currentState;

        // Getting the "current" coordinates
				float currPos[2] = {
          std::get<0>(num2Coord[{succState->state[m]}]),
          std::get<1>(num2Coord[{succState->state[m]}])
        };

        // Finding the "previous" coordinate of the finger from the "current" state
        float prevPos[2] = {
          std::get<0>(num2Coord[{currentState->state[m]}]),
          std::get<1>(num2Coord[{currentState->state[m]}])
        };

				// Initializing from default initial location
				if (m == L)
          succState->finger = L;
				else if (m == R)
          succState->finger = R;

				// Calculating euclidian distance for the move m
				float distance = std::pow( (currPos[0] - prevPos[0]), 2) + std::pow((currPos[1] - prevPos[1]), 2);
				distance = std::pow(distance, 0.5);

				// Adding this distance to the gcost of successor node
				succState->gcost = currentState->gcost + distance;
				succState->tcost = succState->gcost;

				// std::cout << m << " " << succState->state[succState->state.size() - 1] << " " << succState->tcost << std::endl;
     
        // Finding the successor state in frontier and explored lists
        int fIndex = Where(&frontier, succState);
        int eIndex = Where(&explored, succState);

        if (fIndex != -1)
        {
          // successor is in frontier list
          if (succState->tcost < frontier[fIndex]->tcost)
          {
            // Replacing frontier node
            frontier[fIndex] = succState;
          }

          // // Deleting allocated heap
          // delete succState;
        }
        else if (eIndex != -1)
        {
          // successor is in explored list
          if (succState->tcost < explored[eIndex]->tcost)
          {
            // Removing from explored and putting in frontier
            explored[eIndex] = succState;
            frontier.push_back(explored[eIndex]);		// Putting into frontiers
            explored.erase(explored.begin() + eIndex);	// Removing from explored
          }

          // // Deleting allocated heap
          // delete succState;
        }
        else
        {
          // Adding the successor to frontier list
          frontier.push_back(succState);
        }
			}
		}

		return false;
	}

	void DisplaySolution()
	{
		// Getting the solution
		std::vector<State*> solution;
		State* state = finalState;
		while (state->prevState != NULL)
		{
			solution.push_back(state);
			state = state->prevState;
		}
    solution.push_back(initState);

    // Displaying the solution
		while (solution.size() > 0)
		{
      std::cout << "(" << solution[solution.size() - 1]->state[0] << ", " << 
        solution[solution.size() - 1]->state[1] << ")" << std::endl;

			solution.erase(solution.begin() + solution.size() - 1);
		}

		std::cout << "Cost: " << finalState->tcost;
	}

	// Returns the index of element if it exists in the List
	// Returns -1 otherwise
	int Where(std::vector<State*>* list, State* state)
	{
		/*
		This function finds a given state in the list and returns
		its index in the list
		*/

		// Checks is the list contains the same State.state in O(n)
		for (int i = 0; i < list->size(); i++)
		{
			if ((*list)[i]->state == state->state)
			{
				return i;
			}
		}

		return -1;
	}

	int GetLeastCostStateIndex(std::vector<State*>* list)
	{
		/*
		This function returns the index of element with the least cost in the list
		*/

		float min_cost = (*list)[0]->tcost;
		int least_cost_index = 0;
		for (int i = 1; i < list->size(); i++)
		{
			if ((*list)[i]->tcost < min_cost)
			{
				min_cost = (*list)[i]->tcost;
				least_cost_index = i;
			}
		}

		return least_cost_index;
	}
};

int main(int argc, char *argv[])
{
	num2Coord[{'1'}] = std::make_tuple(0, 0);
	num2Coord[{'2'}] = std::make_tuple(1, 0);
	num2Coord[{'3'}] = std::make_tuple(2, 0);
	num2Coord[{'4'}] = std::make_tuple(0, 1);
	num2Coord[{'5'}] = std::make_tuple(1, 1);
	num2Coord[{'6'}] = std::make_tuple(2, 1);
	num2Coord[{'7'}] = std::make_tuple(0, 2);
	num2Coord[{'8'}] = std::make_tuple(1, 2);
	num2Coord[{'9'}] = std::make_tuple(2, 2);
	num2Coord[{'*'}] = std::make_tuple(0, 3);
	num2Coord[{'0'}] = std::make_tuple(1, 3);
	num2Coord[{'#'}] = std::make_tuple(2, 3);

  Solver* solver;

  if (argc > 1)
  {
    std::string arg1(argv[1]);
    solver = new Solver(arg1);
  }
  else
  {
    solver = new Solver();
    solver->GetInput();
  }

	solver->Solve();
	solver->DisplaySolution();
}