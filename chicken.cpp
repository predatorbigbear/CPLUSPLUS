//今有雉兔同笼，上有三十五头，下有九十四足，问雉兔各几何

#include<vector>
#include<numeric>
#include<algorithm>
#include<iostream>



using std::for_each;
using std::vector;
using std::iota;
using std::partition;
using std::cout;

int main()
{
	vector<int>vec(35);
	iota(vec.begin(), vec.end(), 1);
	auto iter = partition(vec.begin(), vec.end(), [](int const num)
		{
			return (num * 2 + (35 - num) * 4) == 94;
		});
	for_each(vec.begin(), iter, [](auto const num)
		{
			cout << "chicken :" << num << "  rabbit :" << 35 - num << '\n';
		});

	cout << 12 * 4 + 23*2 << '\n';

	return 0;
}

