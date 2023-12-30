#include<bits/stdc++.h>
#include"spider.h"
using namespace std;

void test_async_fetch(){
	auto st=get_time();
	vector<Task<string>> v;
	for(int i=1;i<=3;i++) v.push_back(async_fetch("https://zh.cppreference.com/w/cpp/symbol_index/chrono"));
	for(auto& i:v) i.get();
	auto ed=get_time();
	cerr<<"async_fetch:"<<(ed-st)/1.0s<<endl;
}

void test_fetch(){
	auto st=get_time();
	vector<string> v;
	for(int i=1;i<=3;i++) v.push_back(fetch("https://zh.cppreference.com/w/cpp/symbol_index/chrono"));
	auto ed=get_time();
	cerr<<"fetch:"<<(ed-st)/1.0s<<endl;
}

int main(){
	cerr<<fetch("https://zh.cppreference.com/w/cpp/symbol_index/chrono")<<endl;
	test_async_fetch();
	test_fetch();
	return 0;
}
