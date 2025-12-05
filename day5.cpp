#include <bits/stdc++.h>
#include <locale>

using namespace std;

int main() {
	locale::global(locale("en_GB.UTF-8"));
	wcout.imbue(locale());
	ifstream file("d5key.txt");
	vector<pair<long long, long long>> intervals;
	vector<pair<long long, long long>> merged;
	string line;
    regex seg(R"((\d+)-(\d+),?)");
    int fresh{};
    long long all{};

    // setup intervals
    while (getline(file, line)) {
    	if (line.length() == 0) break;
    	string::size_type dash = line.find("-");
    	long long start = stoll(line.substr(0,dash));
    	long long end = stoll(line.substr(dash+1));
    	intervals.push_back(pair(start,end));
    }
    sort(intervals.begin(),intervals.end());
    
	// merge intervals
    long long left = intervals[0].first;
    long long right = intervals[0].second;
    
    for (auto in:intervals) {
    	if (in.first <= right) right = max(right, in.second);
    	else {
    		merged.push_back(pair(left,right));
    		all += (1 + right - left);
    		left = in.first;
    		right = in.second;
    	}
    }
    all += (1 + right - left);
    merged.push_back(pair(left,right));
    

    // read in values
    vector<long long> targets;
    while (getline(file, line)) targets.push_back(stoll(line));

    // for each target, binary search to see if it's in an interval
    // if not increment counter
    for (auto t:targets){
    	if (t > right || t < merged[0].first) continue;
    	auto ptr = --upper_bound(merged.begin(),merged.end(), 
    						  t, [](long long tar, auto pr) {
    						      return tar < pr.first; });
		if (t > (*ptr).second) continue;
		fresh++;
    }


    println("{} Original Intervals, {} Merged Intervals, {} Values", intervals.size(),merged.size(),targets.size());
	println("Total Fresh: {}",fresh);
	println("All Fresh: {}", all);
    return 0;
}
