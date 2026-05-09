//
// Created by Antonio Vetro' on 22/04/22.
//

#ifndef CPP_SLIDE_04_RECTANGLE_H
#define CPP_SLIDE_04_RECTANGLE_H

#include "iostream"
using namespace std;

class Rectangle {

    private:
        double m_width = 3;
        double m_length = 6;

    public:
        // Constructors
        Rectangle();
        Rectangle(const double w, const double l); //by value, not efficient
        Rectangle(const double &w, const double &l, const double &f); // by reference, it makes more sense

        // getters
        double getW() const;
        double getL() const;
        double getArea() const;
        double getPerimeter() const;

        //setters
        void setW(double &w);
        void setL(double &l);

        void setL(int& l);
        void setW(int& w);

        //operators overloading
        Rectangle operator+(const Rectangle &to_be_added);
        void operator=(const Rectangle &to_be_assigned);
        const Rectangle& operator++(); // prefix
        const Rectangle operator++( int ); // postfix
        bool operator==(const Rectangle &to_be_compared);
        bool operator!=(const Rectangle &to_be_compared);

    /* Overload the << and >> operators */
    friend istream& operator>>(istream& is, Rectangle& rect);

    friend ostream& operator<<(ostream& os,
                               const Rectangle& rect);


};


#endif //CPP_SLIDE_04_RECTANGLE_H
