//
// Created by Antonio Vetro' on 09/04/22.
//
#include <iostream>
#include "MyClass.h"

int MyClass::doWork() {
   std::cout << "Some work done" << std::endl;

}

int main(){

    MyClass mc;
    mc.doWork();
}