//
// Created by Antonio Vetro' on 09/04/22.
//
#include "iostream"
#include "ResultCode.h"

ResultCode::ResultCode(): code(0) {};
ResultCode::ResultCode(int c)  {code = c; };
ResultCode::~ResultCode(){ std::cout << "Resource destroyed" << std::endl;};

int ResultCode::getCode() {return this->code;};
char *ResultCode::getDescription() {return "This is a numeric code";}

int main(int argc, char ** argv){
    ResultCode rc1;
    ResultCode rc2(2);

    std::cout << "Value of rc: " << rc1.getCode()
        << " value of rc2: " << rc2.getCode() << std::endl;
    std::cout << rc1.getDescription()<< std::endl;
}