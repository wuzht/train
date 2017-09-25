#ifndef RESCHDEDULINGSTSTEM_HPP
#define RESCHDEDULINGSTSTEM_HPP value

#include <iostream>
#include <stack>
#include <sstream>
#include <string>
#define MAX_CARRIAGE_NUMBER 12
using namespace std;

class ReschedulingSystem
{
public:
	ReschedulingSystem();
	~ReschedulingSystem();
	
	bool isNumValid(const string test_str) const; // is the number_carriages input valid
	bool isRailValid(const string test_str) const; // is the rail input valid

	void read(); //read the information from user
	void rescheduling(); //excute the system
	void print_current();//print the concrete current state
	void print(); //print the result of rescheduling

private:
    stack<int> rail_in;
    stack<int> rail_out;
    stack<int> *buffer;
    int number_buffers; // the number of buffers using
    int number_carriages; //the total number of carriages
    int step; 
    int count; // the current carriage that should leave
    int from;
    int to;
};

ReschedulingSystem::ReschedulingSystem() {
	buffer = NULL;
	number_buffers = 0;
	number_carriages = 0;
	step = 0;
	count = 1;
	from = 0;
	to = 0;
}

ReschedulingSystem::~ReschedulingSystem() {
	if (buffer != NULL) {
		delete []buffer;
		buffer = NULL;
	}
}

bool ReschedulingSystem::isNumValid(const string test_str) const {
	if (test_str.size() <= 0)
		return false;
	for (auto it : test_str)
		if (!isdigit(it))
			return false;
	stringstream ss(test_str);
	int num = 0;
	ss >> num;
	if (num <= 0 || num > MAX_CARRIAGE_NUMBER) {
		cout << "[carriages number Error] The number of carriages should be more than 0 and less than " 
			 << MAX_CARRIAGE_NUMBER << endl;
		return false;
	}
	return true;
}

bool ReschedulingSystem::isRailValid(const string test_str) const {
	if (test_str.size() <= 0)
		return false;
	//is the str valid(digit or space is valid)
	for (auto it : test_str)
		if (!isdigit(it) && !isspace(it))
			return false;
	//is the numbers are 1 ~ number_carriages
	bool test[number_carriages+1] = {false};
	test[0] = true;
	stringstream ss(test_str);
	int temp_carriage = 0;
	int temp_carriage_num = 0;
	while (ss >> temp_carriage) {
		temp_carriage_num++;
		if (temp_carriage > number_carriages)
			return false;
		test[temp_carriage] = true;
	}
	if (temp_carriage_num != number_carriages) {
		return false;
	}
	for (int i = 1; i <= number_carriages; ++i)
		if (test[i] == false)
			return false;
	return true;
}


void ReschedulingSystem::read() {
	//------input carriages number------
	cout << "[carriages number] How many train carriages do you need to reschedule: " << endl
		 << "[carriages number] ";
	//test
	string carriagesNum_forTesting;
	cin.clear();
	cin.sync();
	getline(cin, carriagesNum_forTesting);
	while (!isNumValid(carriagesNum_forTesting)) {
		cout << "[carriages number Error] Invalid input! Please try again!" << endl;
		cout << "[carriages number] How many train carriages do you need to reschedule: " << endl
		     << "[carriages number] ";
		cin.clear();
		cin.sync();
		getline(cin, carriagesNum_forTesting);
	}
	//get number_carriages
	stringstream num_ss(carriagesNum_forTesting);
	num_ss >> number_carriages;
	
	buffer = new stack<int> [number_carriages - 1];

	//------input the rail------
	cout << "[carriages rail] Please input the serial number of train carriages." << endl
		 << "--> " ;
	//test
	string rail_forTesting;
	cin.clear();
	cin.sync();
	getline(cin, rail_forTesting);
	while (!isRailValid(rail_forTesting)) {
		cout << "[carriages rail Error] Invalid input! Please try again!" << endl;
		cout << "[carriages rail] Please input the serial number of train carriages." << endl
		 	 << "--> " ;
		cin.clear();
		cin.sync();
		getline(cin, rail_forTesting);
	}

	//put them in rail_in
	stringstream rail_ss(rail_forTesting);
	int number = 0;
	while (rail_ss >> number) {
		rail_in.push(number);
	}
}

void ReschedulingSystem::rescheduling() {
	bool isInBuffers = false;
	print_current();
	while (rail_out.size() != number_carriages) {
		isInBuffers = false;

		//is the current number on the top of the rail
		if (rail_in.size() && rail_in.top() == count) {

			if(!(from == 0 && to == -1)){
				if(to != 0)
					print_current();
				from = 0;
				to = -1;
				step++;
			}

			rail_out.push(rail_in.top());
			rail_in.pop();

			++count;
		} else {
			//is the current number in one of the tops of buffers
			for (int i = 0; i < number_buffers; ++i) {
				if (buffer[i].size() && buffer[i].top() == count) {

					if(!(from == i + 1 && to == -1)){
						from = i + 1;
						to = -1;
						print_current();
						step++;
					}

					rail_out.push(buffer[i].top());
					buffer[i].pop();

					++count;

					isInBuffers = true;
					break;
				}
			}

			//put the top of the rail to the buffers
			if (!isInBuffers) {
				bool openNewBuffer = true; //do it need to open a new buffer rail
				for (int i = 0; i < number_buffers; ++i) {
					if (rail_in.top() && buffer[i].size() 
						&& rail_in.top() < buffer[i].top()) {

						if(!(from == 0 && to == (i + 1))){
							print_current();
							step++;
							from = 0;
							to = i + 1;
							
						}

						buffer[i].push(rail_in.top());
						rail_in.pop();

						openNewBuffer = false;
						break;
					}

					if (buffer[i].size() == 0) {
						if(!(from == 0 && to == (i + 1))){
							print_current();
							from = 0;
							to = i + 1;
							step++;
						}


						buffer[i].push(rail_in.top());
						rail_in.pop();

						openNewBuffer = false;
						break;						
					}
				}	

				//open a new buffer rail
				if (openNewBuffer) {
					if(!(from == 0 && to == number_buffers + 1)){
						if(to != 0){
							print_current();
						}
						from = 0;
						to = number_buffers + 1;
						step++;
					}
					buffer[number_buffers].push(rail_in.top());
					rail_in.pop();

					++number_buffers;
				}
			}
		}
	}
	print_current();
}

void ReschedulingSystem::print() {
	cout << endl
		 << "[result] " << number_buffers << " buffers is used" << endl
		 << "[result] " << step << " steps totally" << endl;


}
void ReschedulingSystem::print_current(){
	cout << "step" << step << endl;
	if(rail_in.empty()){
		cout << "rail_in : none";
	}
	else{
		cout << "rail_in : ";
		stack<int> tem1(rail_in);
		stack<int> tem;
		while(!tem1.empty()){	
			tem.push(tem1.top());
			tem1.pop();
		}
		while(!tem.empty()){
			cout << tem.top() << ' ';
			tem.pop();
		}
	}
	cout << endl;
	for(int i = 0; i < number_buffers; i++){
		cout << "stack" << i + 1 << " : ";
		if(buffer[i].empty()){
			cout << "none";
		}
		else{
			stack<int> tem1(buffer[i]);
			stack<int> tem;
			while(!tem1.empty()){	
			tem.push(tem1.top());
			tem1.pop();
			}
			while(!tem.empty()){
				cout << tem.top() << ' ';
				tem.pop();
			}
		}
		cout << endl;
	}
	cout << "rail_out : ";
	if(rail_out.empty()){
		cout << "none";
	}
	else{
		stack<int> tem1(rail_out);
		stack<int> tem;
		while(!tem1.empty()){	
			tem.push(tem1.top());
			tem1.pop();
		}
		while(!tem.empty()){
			cout << tem.top() << ' ';
			tem.pop();
		}
	}
	cout << endl;
	cout << "-----------------------------" << endl;
}

#endif