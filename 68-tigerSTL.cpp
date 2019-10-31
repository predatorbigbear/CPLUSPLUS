/*#include<iostream>
#include<functional>
#include<string>
#include<vector>
#include<algorithm>
#include<iterator>
#include<unordered_set>


using std::unordered_set;
using std::distance;
using std::search;
using std::find;
using std::vector;
using std::string;
using std::reference_wrapper;
using std::cout;


static string User_Name{ "User_Name=" };
static string SelectWord{ "SelectWord=" };






static unordered_set<string>ChineseSet{ User_Name };

void praseBody(const string &source, const vector<string> &vecFindStr, vector<reference_wrapper<string>> &vecDisStr)
{
	if (!source.empty() && !vecFindStr.empty() && !vecDisStr.empty() && vecFindStr.size()== vecDisStr.size())
	{
		vector<string>::const_iterator vecFindStrBegin{ vecFindStr.begin() }, vecFindStrEnd{ vecFindStr.end() };
		vector<reference_wrapper<string>>::iterator vecDisStrBegin{ vecDisStr.begin() }, vecDisStrEnd{ vecDisStr.end() };
		string::const_iterator iterBegin{ source.begin() }, iterEnd{ iterBegin }, iterLast{ source.end() };

		do
		{
			iterBegin = search(iterBegin, iterLast, vecFindStrBegin->begin(), vecFindStrBegin->end());
			if (iterBegin == iterLast)
				break;

			if (distance(vecFindStrBegin, vecFindStrEnd) == 1)
			{
				vecDisStrBegin->get().assign(iterBegin+ vecFindStrBegin->size(), iterLast);
				iterBegin= iterLast;
			}
			else
			{
				iterEnd = find(iterBegin + vecFindStrBegin->size(), iterLast, '&');
				if (iterEnd == iterLast)
					break;

				vecDisStrBegin->get().assign(iterBegin + vecFindStrBegin->size(), iterEnd);
				iterBegin = iterEnd;
			}

			if (ChineseSet.find(*vecFindStrBegin) != ChineseSet.end())      
			{
				//transform(vecDisStrBegin->get());
				
			}

			++vecFindStrBegin;
			++vecDisStrBegin;

		} while (iterBegin != iterLast);
	}
}



int main()
{
	string str{ "User_Name=fwefwefwef&SelectWord=fwef" };

	string User_NameStr;
	string SelectWordStr;
	vector<string>vec{ User_Name, SelectWord };
	vector<reference_wrapper<string>>vecDis{ User_NameStr ,SelectWordStr };
	praseBody(str, vec, vecDis);

	for (auto const &i : vecDis)
		cout << i.get() << " ";
	cout << '\n';


	cout << User_NameStr << " " << SelectWordStr << '\n';

	str.assign(str.begin(), find(str.begin(), str.end(), '&'));
	vec.assign({ User_Name});
	vecDis.assign({ User_NameStr });

	praseBody(str, vec, vecDis);

	for (auto const &i : vecDis)
		cout << i.get() << " ";
	cout << '\n';


	cout << User_NameStr << " "<< '\n';
	
	return 0;
}
*/