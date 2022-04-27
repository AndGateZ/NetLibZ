

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool com(pair<int,string> &p1, pair<int,string> &p2){
	if(p1.first<p2.first) return true;
	if(p1.first==p2.first) return p1.second<p2.second?true:false;
	return false;
}

int main(){
    int n = 0;
    cin>>n;

	string tmp;
	vector<pair<int,string>> hight;
	int high = 0;
	//输入啊输入，这样才对
	while(cin>>high){
		hight.push_back(pair<int,string>{high,""});
		if(cin.get()=='\n') break;
	}

	string name;
	int i = 0;
	while(cin>>name){
		hight[i].second = name;
		i++;
		if(cin.get()=='\n') break;
	}
	
	//sort自定义的两种方式
	//sort(hight.begin(),hight.end(),com);
	sort(hight.begin(),hight.end(),[](pair<int,string> &p1, pair<int,string> &p2){
		if(p1.first<p2.first) return true;
		if(p1.first==p2.first) return p1.second<p2.second?true:false;
		return false;
	});
    
    for(auto &p:hight){
		cout<<p.second<<" ";
	}
    
    return 0;
}

// 4
// 176 170 176 176
// beta tom alpha bamma