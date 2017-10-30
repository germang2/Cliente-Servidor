#include <iostream>
#include <unordered_map>

using namespace std;

int main(int argc, char const *argv[])
{
	unordered_map<int, int> m;
	
	for(int i = 0; i < 10; i++)
		m[i] = i;


	cout << "Exist: " << m[3] << endl;
	cout << "Not exist: " << m[31242] << endl;


	return 0;
}