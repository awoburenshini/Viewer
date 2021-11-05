#include<iostream>
#include <memory>


using namespace std;

shared_ptr<int> test(int& a){
    return make_shared<int>(a);
}


int main(){

    std::cout<< "hello"<<std::endl; 
    return 0;
}