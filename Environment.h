// Poisson distribution PMF Generator and Jack Rental  Cars Environment for
// Expected return calculation
#pragma once
#include <vector>
#include <cmath>

using namespace std;

class poisson {
public:

	poisson(double lambda =3) {	
		_lambda = lambda;
		_min_n = 0;
		bool state = true;
		double sum = 0;
		double temp;
		while (true) {
			if (state == true) {
				temp = generate(_min_n);
				if (temp <= 0.01)
					_min_n++;
				else {
					pmf.push_back(temp);
					sum += temp;
					_max_n = _min_n + 1;
					state = false;
				}
			}
			else{
				temp = generate(_max_n);
				if (temp >= 0.01) {
					pmf.push_back(temp);
					sum += temp;
					_max_n++;
				}
				else
					break;
			}
		}

		double addeval = (1 - sum) / (_max_n - _min_n);
		for (int i = 0; i < pmf.size(); i++)
			pmf[i] += addeval;
	}

	int Min() {
		return _min_n;
	 }

	int Max() {
		return _max_n;
	}
	vector<double> vals() {
		return pmf;
	}

private:
	double _lambda;
	int _max_n;
	int _min_n;
	vector<double> pmf;

	double factoriel(int n) {
		if (n == 0 || n == 1)
			return 1;
		else
			return (n * factoriel(n - 1));
	}

	double generate(int n) {
		int fact = factoriel(n);
		return ((pow(_lambda, n) * exp(-_lambda)) / fact);
	}
};


class Environment
{
	friend class Agent;

public:
	Environment( int max_customer = 20, int max_transition = 5, double discount_factor = 0.9, int penalty = 2, int income = 10) {
		vector<double> temp(max_customer + 1, 0.0f);
		for (int i = 0; i < max_customer + 1; ++i)
			_Values.push_back(temp);
		temp.clear();
		_nS[0] = max_customer + 1;
		_nS[1] = max_customer + 1;
		int lambda[4] = { 3,3,4,2 };
		a_contract = poisson(lambda[0]);
		a_termination = poisson(lambda[1]);
		b_contract = poisson(lambda[3]);
		b_termination = poisson(lambda[3]);
		_max_customer = max_customer;
		_penalty = penalty;
		_income = income;
		_discount_factor = discount_factor;
	}

	double expected_reward(int state[2], int action)
	{
		double reward = 0;
		int new_state[2] = {max(min(state[0] - action, _max_customer),0),max(min(state[1] + action, _max_customer),0)};
		reward += _penalty * abs(action);

		for (int a_new = a_contract.Min(); a_new < a_contract.Max(); a_new++)
			for (int b_new = b_contract.Min(); b_new < b_contract.Max() ; b_new++)
				for (int a_terminated = a_termination.Min(); a_terminated < a_termination.Max(); a_terminated++)
					for (int b_terminated = b_termination.Min(); b_terminated < b_termination.Max(); b_terminated++)
					{
						double prob = a_contract.vals()[a_new] * a_termination.vals()[a_terminated] * b_contract.vals()[b_new] * b_termination.vals()[b_terminated];
						int max_contract_a = min(new_state[0], a_new);
						int max_contract_b = min(new_state[1], b_new);

						double r = (max_contract_a + max_contract_b) * _income;

						int next_state[2] = { max(min(new_state[0] - max_contract_a + a_terminated,_max_customer),0),
											  max(min(new_state[1] - max_contract_b + b_terminated,_max_customer),0) };

						reward += prob * (r + _discount_factor * _Values[next_state[0]][next_state[1]]);

					}
		
		return reward;

	}
	double get_v(int i, int j) {
		return _Values[i][j];
	}

	void set_v(int i, int j, double value) {
		_Values[i][j] = value;
	}
	~Environment(){}

private:
	const int defaultinit[4] = { 3,3,4,2 };
	int _nS[2];
	int _nA;
	int _max_customer;
	double _penalty;
	double _income;
	double _discount_factor;
	poisson a_contract;
	poisson a_termination;
	poisson b_contract;
	poisson b_termination;
	vector<vector<double>> _Values;

};