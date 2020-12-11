// Main console for Dynamic Programming loop

#include "stdafx.h"
#include "iostream"
#include <windows.h>
#include "Agent.h"
#include "Environment.h"
#include <fstream>

using namespace std;


int main()
{
	Agent agent;
	cout << "Agent Instantiated" << endl;
	double precision = 10;
	while (true) {
		cout << "Evaluating..." << endl;
		agent.policy_eval(precision);
		precision /= 2;
		cout << "improving..." << endl;
		bool policy_stable = agent.policy_iter();
		if (policy_stable)
			break;
	}
	ofstream File;
	File.open("D:\\result.txt");
	if (File.is_open())
	{
		for (std::vector<std::vector<int>>::size_type i = 0; i < agent.policy.size(); i++)
		{
			for (std::vector<int>::size_type j = 0; j < agent.policy[i].size(); j++)
			{
				std::cout << agent.policy[i][j] << ' ';
			}
			std::cout << std::endl;
		}
		File.close();
	}
	else
		cout << "Cannot write to file" << endl;
	system("pause");
	return 0;
}

