/*#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<functional>
#include<map>

using std::pair;
using std::string;
using std::vector;
using std::find_if;
using std::bind;
using std::logical_or;
using std::equal_to;
using std::search;
using std::copy;
using std::cout;


static string UrlDecode(const string &source)
{
	if (!source.empty())
	{
		string result{ source };
		static vector<pair<string, char>>vec{ { "%21",'!'},{"%2A",'*'}, {"%22",'\"'}, {"%27", '\''},{"%28",'('}, {"%29",')'},{ "%3B",';'},{"%3A",':'},{"%40",'@'}, {"%26",'&' },
		{ "%3D",'='}, {"%2B",'+'},{ "%24",'$'},{"%2C",','},{"%2F",'/'},{"%3F",'?'},{"%25",'%'},{"%23",'#'},{"%5B",'['},{"%5D",']' },{"%20",' '},{"%60",'`'} };

		auto iterBegin = result.begin(), iterEnd = result.end();
		vector<pair<string, char>>::iterator iter;

		while (iterBegin != iterEnd)
		{
			iterBegin = find_if(iterBegin, iterEnd, bind(logical_or<>(), bind(equal_to<>(), std::placeholders::_1, '%'), bind(equal_to<>(), std::placeholders::_1, '+')));
			if (iterBegin != iterEnd)
			{
				if (*iterBegin == '%')
				{
					for (iter = vec.begin(); iter != vec.end(); ++iter)
					{
						if (search(iterBegin, iterEnd, iter->first.begin(), iter->first.end()) == iterBegin)
							break;
					}
					if (iter != vec.end())
					{
						*iterBegin = iter->second;
						copy(iterBegin + 3, iterEnd, iterBegin + 1);
						iterEnd -= 2;
					}
				}
				else
					*iterBegin = ' ';
				++iterBegin;
			}
		}
		result.erase(iterEnd, result.end());
		return result;
	}
	return source;
}



int main()
{
	string source{ "fwefwef%2C%%%2Pgergre%23hnty" };
	cout << UrlDecode(source) << '\n';


	return 0;
}
*/