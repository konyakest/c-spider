#ifndef SPIDER_H
#define SPIDER_H

#include<future>
#include<utility>
#include<mutex>
#include<random>
#include<string>
#include<fstream>
#include<chrono>

//Task
template<typename T>using Task=std::future<T>;

//make_task
template<typename F>
auto make_task(F&& f){return std::async(std::forward(f));}
template<typename F,typename ...Args>
auto make_task(F&& f,Args&& ...args){return std::async(std::forward<F>(f),std::forward<Args>(args)...);}

//global_mutex
std::mutex global_mutex;

//global_rng
std::mt19937_64 global_rng(std::random_device{}());

//safe_run
template<typename F>
auto safe_run(F&& func){
	std::lock_guard<std::mutex> lock(global_mutex);
	return func();
}
template<typename F,typename ...Args>
auto safe_run(F&& func,Args&& ...args){
	std::lock_guard<std::mutex> lock(global_mutex);
	return func(std::forward<Args...>(args...));
}

//get_random
unsigned long long get_random(){return safe_run([](){return global_rng();});}

//fetch
std::string fetch(std::string url,std::string arguments=""){
	std::string commad=
#ifdef _WIN32
		"powershell Curl"
#elif defined(__APPLE__) || defined(__unix__) || defined(__linux__)
		"curl -s"
#else
	#error "Sorry, your system type is unsupported."
#endif
	;

	std::string filename=std::to_string(get_random())+".html";
	
#ifdef _WIN32
	std::system((commad+" "+url+" -o "+filename+"_tmp"+" "+arguments).c_str());
	std::system(("type "+filename+"_tmp"+">"+filename).c_str());
#else
	std::system((commad+" "+url+" -o "+filename+" "+arguments).c_str());
#endif
	std::string res,tmp;
	{
		std::ifstream is(filename,std::ios::binary);
		while(getline(is,tmp)) res+=tmp;
	}
#ifdef _WIN32
	std::system(("del "+filename+" "+filename+"_tmp").c_str());
#else
	std::system(("rm "+filename).c_str());
#endif
	return res;
}

//async_fetch

Task<std::string> async_fetch(std::string url,std::string arguments=""){
	return make_task(fetch,url,arguments);
}

//get_time

auto get_time(){return std::chrono::system_clock().now().time_since_epoch();}

#endif
