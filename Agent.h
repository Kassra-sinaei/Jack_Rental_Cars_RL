// Implmentation of Policy Evaluation and Policy Improvement

#pragma once
#include "Environment.h"
#include <vector>

using namespace std;

class Agent{
	friend int main();
public:
	Agent() {
		int lambdas[4] = { 3,3,4,2 };
		Environment env;
		vector<int> temp(env._max_customer + 1, 0);
		for (int i = 0; i < env._max_customer + 1; ++i)
			policy.push_back(temp);
	}
	void policy_eval(double theta = 0.5) {
		while(true) {
			double delta = 0;

			for(int i = 0; i < env._max_customer + 1 ; ++i)
				for (int j = 0; j < env._max_customer + 1; ++j)
				{
					double old_val = env.get_v(i, j);
					int state[2] = { i,j };
					env.set_v(i, j, env.expected_reward(state, policy[i][j]));
					delta = max(delta, abs(old_val - env.get_v(i, j)));
				}
			cout << delta << endl;
			if (theta > delta)
				break;
		}
	}

	bool policy_iter() {
		bool stable = true;
		for (int i = 0; i < env._max_customer + 1; ++i)
			for (int j = 0; j < env._max_customer + 1; ++j)
			{
				int old_action = policy[i][j];

				int a2b = min(i, 5);
				int b2a = -min(j, 5);
				double max_value = -1000.0f;
				int max_action = -20;
				for (int action = b2a; action <= a2b; ++action)
				{
					double sigma;
					int state[2] = { i,j };
					sigma = env.expected_reward(state, action);
					if (max_value < sigma) {
						max_action = action;
						max_value = sigma;
					}
				}
				policy[i][j] = max_action;
				if (policy[i][j] == old_action)
					stable = false;
			}

		return stable;
	}

private:
	Environment env;
	vector<vector<int>> policy;
};