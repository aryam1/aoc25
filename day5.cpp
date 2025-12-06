#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <print>
#include <locale>

int main() {
	// Setting of locale was for printing large numbers with comma seperators
	//std::locale::global(std::locale("en_GB.UTF-8"));
	//std::wcout.imbue(locale());
	std::ifstream file("d5key.txt");
	std::vector<std::pair<long long, long long>> intervals;
	std::vector<std::pair<long long, long long>> merged;
	std::string line;

    int fresh{};
    long long all{};

    // Setup intervals
    while (std::getline(file, line)) {
    	if (line.length() == 0) break;
    	// Find the - between numbers
    	std::string::size_type dash = line.find("-");
    	// Extract numbers either side of dash
    	long long start = std::stoll(line.substr(0,dash));
    	long long end = std::stoll(line.substr(dash+1));
    	
    	intervals.push_back(std::pair{start,end});
    }
    // Sort by start interval
    std::sort(intervals.begin(),intervals.end());
    
	// Merge intervals with a sliding window approach
    long long left = intervals[0].first;
    long long right = intervals[0].second;
    
    for (auto in:intervals) {
    	// Track rolling max, while current interval start is within current merged interval
    	if (in.first <= right) right = std::max(right, in.second);
    	// Once there's a gap, push merged interval to vector and repeat process
    	else {
    		merged.push_back(std::pair{left,right});
    		// Tally width of interval
    		all += (1 + right - left);
    		// Update interval bounds
    		left = in.first;
    		right = in.second;
    	}
    }
    // Sort out left over final interval
    all += (1 + right - left);
    merged.push_back(std::pair{left,right});
    

    // Read in target values and sort
    std::vector<long long> targets;
    while (std::getline(file, line)) targets.push_back(std::stoll(line));
    std::sort(targets.begin(),targets.end());

    // Binary search each target to see if it's in an interval
    // if not increment counter; since targets are sorted
    // start of binary search can keep being pushed forward and reused
    auto ptr = merged.begin();
    for (auto t:targets){
    	// If target outside of interval domain, skip
    	if (t > right || t < merged[0].first) continue;
    	// Binary search to find the interval with start > target
    	ptr = std::upper_bound(ptr,merged.end(), t, [](long long tar, const auto& pr) {return tar < pr.first;});
    	
    	// If target not in preceding interval, it's not in any interval
		if (t > (*(ptr-1)).second) continue;
		fresh++;
    }
    
    std::println("{} Original Intervals, {} Merged Intervals, {} Values", intervals.size(),merged.size(),targets.size());
	std::println("Total Fresh: {}",fresh);
	std::println("All Fresh: {}", all);
    return 0;
}
