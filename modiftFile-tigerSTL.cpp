/*#include<filesystem>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<functional>


using std::placeholders::_1;
using std::bind;
using std::equal_to;
using std::logical_or;
using std::getline;
using std::ifstream;
using std::stringstream;
using std::error_code;
using std::cout;
using std::cin;
using std::string;
using std::transform;
//namespace fs= std::experimental::filesystem;

namespace fs = std::filesystem;

using fs::recursive_directory_iterator;
using fs::path;
using fs::is_directory;
using fs::is_symlink;
using fs::exists;
using fs::create_directories;
using fs::copy_file;
using fs::remove_all;
using std::ofstream;
using std::find_if;


void replaceString(string &str,const string &findStr, const string &replaceStr)   //  /home/testDir1/1.txt     \       /
{

	if (!str.empty() && !findStr.empty() && !replaceStr.empty())
	{
		auto strLeft = find_if(str.begin(), str.end(), bind(logical_or<>(), bind(equal_to<>(), _1, '\\'), bind(equal_to<>(), _1,'/')));
		auto findStrLeft = find_if(findStr.begin(), findStr.end(), bind(logical_or<>(), bind(equal_to<>(), _1, '\\'), bind(equal_to<>(),_1, '/')));
		
		string realFindStr{ findStr };

		if (strLeft != str.end() && findStrLeft != findStr.end())
		{
			transform(realFindStr.begin(), realFindStr.end(), realFindStr.begin(),[&findStrLeft,&strLeft](auto elem)
			{
				return elem == *findStrLeft ? *strLeft : elem;
			});
		}
		
		auto iter = str.begin();
		int len{};
		while (iter != str.end())
		{
			iter = search(iter, str.end(), realFindStr.begin(), realFindStr.end());
			if (iter != str.end())
			{
				len = distance(str.begin(), iter);
				str.replace(iter, iter + realFindStr.size(), replaceStr.data());
				iter = str.begin() + len + replaceStr.size();
			}
		}
	}
}


//遍历文件夹 然后将文件夹中的特定文件类型进行搜索  之后将里面的想要替换的字符串进行替换   然后所有文件会被拷贝到新的文件家里面


int main()
{
	string pathStr, destinationStr;
	error_code err;
	path thisPath;

	string fileExtension;
	string localhost, ip;
	int fileNum{}, directoryNum{};
	while (1)
	{
		cout << "Please input a original path输入源文件夹路径:";
		cin >> pathStr;
		thisPath = pathStr;
		if (!(exists(thisPath, err)) || err.value())
		{
			if (err)
			{
				cout << err.value() << ":" << err.message() << '\n';
			}
			else
				cout << pathStr << " is not a exist path\n";
			continue;
		}
		if (!is_directory(thisPath, err) || err.value())
		{
			if (err)
			{
				cout << err.value() << ":" << err.message() << '\n';
			}
			else
				cout << pathStr << " is not a directory\n";
			continue;
		}
		cout << "Please input destination path输入处理文件夹路径:";
		cin >> destinationStr;
		if ((exists(destinationStr, err)) || err.value())
		{
			remove_all(path(destinationStr),err);      //rm -rf 
			if (err)
			{
				cout << err.value() << " " << err.message() << '\n';
			}
		}
		cout << "输入修改原字段:";
		cin>>localhost;
		cout << "输入修改后字段:";
		cin >> ip;
		cout << "输入文件后缀带.,比如.cpp:";
		cin >> fileExtension;
		recursive_directory_iterator iterBegin(thisPath), iterEnd;
		fileNum = directoryNum = 0;
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if (is_directory(iterBegin->status()) || is_symlink(iterBegin->status()))
				continue;
			string temp1{ iterBegin->path().string() }, temp2{temp1};
			replaceString(temp2, pathStr, destinationStr);
			path p1{ temp2 };
			
			create_directories(p1.parent_path());
			if (iterBegin->path().extension().string() == fileExtension)
			{
				stringstream istr,ostr;
				istr << ifstream(iterBegin->path().string(), std::ios::binary).rdbuf();
				string str;
				while (!istr.eof())
				{
					getline(istr, str);
					if (!str.empty())
					{
						if (str.back() == '\r')
							str.erase(str.end() - 1, str.end());
						replaceString(str, localhost, ip);
						str.append("\r\n");
						ostr << str;
					}
				}
				{
					ofstream(temp2, std::ios::trunc) << "";
				}
				ofstream(temp2, std::ios::app|std::ios::binary) << ostr.str();
			}
			else
			{
				copy_file(path(temp1), path(temp2));

			}		
		}
		cout << "Finished\n";           //  chmod 777
		cout << "yes\n";
	}

	return 0;
}
*/
