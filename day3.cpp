#include <bits/stdc++.h>

using namespace std;

int main() {

	std::ifstream file("d3key.txt");
    std::string bank;
    long long total = 0;
    while (std::getline(file, bank)) {
    	vector<int> v;
   	    for (char c : bank) {
   	        if (isdigit((unsigned char)c)) v.push_back(c - '0');
   	    }
   	    // Logic now is given some N and string len X, buffer is
   	    // X -(N-1), so each digit has the following buffer of numbers
   	    // to max from, leaving enough digits for remaining N, 
   	    // this buffer is reduced by how far into its buffer region
   	    // a digit is selected from, as all following numbers 
   	    // lose this same amount of buffer 
   	    vector<int>::iterator prev, curr;
   	    const int batts = 12;
   	    int buffer = v.size() - (batts-1);
   	    prev = v.begin();
   	    array<int,batts> val{};
   	    //cout << bank << '\n';
   	    for (int i = 0; i < batts; ++i){
   	    	curr = max_element(prev,prev+buffer);
   	    	//cout <<string((int)(curr-prev),' ')<<"^";
   	    	val[i] = *curr;
   	    	buffer -= (curr-prev);
   	    	prev = curr+1;
   	    }
   	    string valstr;
   	    for (int x : val) valstr += '0' + x;
   	    total += stoll(valstr);
   	    //print("Val: {}\n",valstr);
    }
    print("Joltage Total: {}\n", total);
    return 0;
}
