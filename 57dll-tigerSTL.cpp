/*#include<iostream>
#include<windows.h>
#include<functional>
#include<algorithm>
#include<string>
#include<vector>
#include<map>

using std::map;
using std::vector;
using std::string;
using std::transform;
using std::placeholders::_1;
using std::bind;
using std::not_equal_to;
using std::all_of;
using std::cout;
typedef int(__stdcall*fun)(void);

using fun1 = int(__stdcall*)(void);

int main()
{

	typedef int(__stdcall*fun)(void);
	HMODULE h1 = ::LoadLibraryA("C:\\Users\\predatorBigbear1\\Videos\\DLL.dll");
	if (h1)
		cout << "yes\n";
	fun f = (fun)GetProcAddress(h1, "?fnDLL@@YAHXZ");
	cout << f() << '\n';


	using fun1 = int(__stdcall*)(void);
	HMODULE h2{};
	vector<string>vec{ "?fnDLL@@YAHXZ" };
	map<string, fun1>m1;
	if (all_of(&(h2 = ::LoadLibraryA("C:\\Users\\predatorBigbear1\\Videos\\DLL.dll")), &h2 + 1, bind(not_equal_to<>(), _1, nullptr)))
	{
		for_each(vec.begin(), vec.end(), [&m1,&h2](auto const &elem)
		{
			m1.insert(make_pair(elem, reinterpret_cast<fun1>(GetProcAddress(h2, elem.data()))));
		});

		

		cout << m1["?fnDLL@@YAHXZ"]() << '\n';
	}

	return 0;
}
*/