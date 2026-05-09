//
// Created by Antonio Vetro' on 09/04/22.
//

#ifndef CPP_SLIDE_02_RESULTCODE_H
#define CPP_SLIDE_02_RESULTCODE_H

class ResultCode{

public:
    ResultCode();
    ResultCode(int c);
    ~ResultCode();
    int getCode();
    char* getDescription();

private:
    int code;
};

#endif //CPP_SLIDE_02_RESULTCODE_H
