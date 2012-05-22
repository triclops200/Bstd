#ifndef __SMARTFUNC_HPP__
#define __SMARTFUNC_HPP__
#include <stdarg.h>
#include <map>
#include <cstdio>
#include <string>
#include <sstream>
namespace bstd{
	template<class T, typename ... Args> class smartfunc{
		public:
			T (*funcpt)(smartfunc*, Args...args);
			template<typename argT,typename...ArgsT>
			std::string compile(std::string str,argT arg,ArgsT...args){
				std::ostringstream s;
				s << arg;
				str += ","+s.str();
				return compile(str, args...);
			}
			std::string compile(std::string str){
				return str;
			}
			std::map<std::string,T> m;
			T call(Args... args){
				std::string arg = compile("",args...);
				if(m.find(arg) == m.end()){
					m[arg] = (*funcpt)(this,args...);
				}
				return m[arg];
			}
			T operator () (Args...args){
				return call(args...);
			}
			smartfunc(T (*functionPointer)(smartfunc*,Args...args)){
				funcpt = functionPointer;
			}
	};
}
/*
EXAMPLE USAGE
int fib(smartfunc<int,int>*f,int n){
	if(n<2)
		return n;
	return (*f)(n-1) + (*f)(n-2);
}
int main(){
	smartfunc<int, int> ffib(fib);
	printf("%d\n",ffib(5));
	printf("%d\n",ffib(41));
}
*/
#endif
