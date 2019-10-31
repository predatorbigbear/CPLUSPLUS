/*#include<string>
#include<algorithm>
#include<iostream>

using std::equal;
using std::string;
using std::distance;
using std::remove_if;
using std::cout;


//  erase-remove_if   ÈçºÎÉ¾³ý×Ó´®


void removeSubstring(string &str, const string &subString)
{
	if (!str.empty() && !subString.empty() && str.size() >= subString.size())
	{
		int index{};   
		bool shouldRemove{};
		auto iter = str.begin();
		bool result{};
		str.erase(remove_if(str.begin(), str.end(), [index, shouldRemove, &subString, iter, &str, result](auto elem)mutable
		{
			result = false;
			if (shouldRemove && ++index != subString.size())
				result = true;
			else
			{
				index = 1;
				shouldRemove = false;
				if (distance(iter, str.end()) >= subString.size() && equal(iter, iter + subString.size(), subString.begin(), subString.end()))
					result = shouldRemove = true;
			}
			++iter;
			return result;
		}), str.end());
	}
}


int main()
{
	string str{ "abcfefabc5465abcfewfewabcfgegwerabcfewfgweabcgergre" };
	removeSubstring(str, string("abc"));
	cout << str << '\n';
	return 0;
}
*/