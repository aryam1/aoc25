#include <bits/stdc++.h>

using namespace std;

int main() {

	std::ifstream file("d3key.txt");
    std::string bank;
    long long total = 0;
    while (std::getline(file, bank)) {
    	cout << bank << '\n';
    	vector<int> v;
   	    transform(bank.begin(), bank.end()-1, back_inserter(v), [](char c) { return c - '0'; });
   	    vector<int>::iterator max1, max2;
   	    max1 = max_element(v.begin(),v.end());
   	    max2 = max_element(max1+1,v.end());
   	    cout << *max1 << *max2<< '\n';
   	    
    }
    return 0;
}
