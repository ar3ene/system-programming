//
// Created by Antonio Vetro' on 22/04/22.
//

#include "Rectangle.h"

// Constructors

Rectangle::Rectangle() = default;
/*
Rectangle::Rectangle(): m_width(2), m_length(1) {
   // this->m_width = 2;
   // this->m_length = 1;

}*/

Rectangle::Rectangle(const double w, const double l) {
    this->m_width = w;
    this->m_length = l;
}

Rectangle::Rectangle(const double &w, const double &l, const double &f) {
    this->m_width = w;
    this->m_length =l;
}


// Getters
double Rectangle::getArea() const {
    return this->m_length * this->m_width;
}

double Rectangle::getPerimeter() const {
    //this->m_width = 4;
    return (this->m_width + this->m_length)*2;
}

double Rectangle::getL() const {
    return this->m_length;
}

double Rectangle::getW() const {
    return this->m_width;
}

//setters
void Rectangle::setW(double& w) {
    this->m_width = w;
}

void Rectangle::setL(double& l) {
    this->m_length  = l;
}

void Rectangle::setW(int &w) {
    this->m_width = static_cast<double>(w);
}

void Rectangle::setL(int &l){
    this->m_length = static_cast<double>(l);
}

// operators

void Rectangle::operator=(const Rectangle &to_be_assigned) {
    this->m_width = to_be_assigned.m_width;
    this->m_length = to_be_assigned.m_length;
}

Rectangle Rectangle::operator+(const Rectangle &to_be_added) {
    Rectangle output;
    output.m_length = this->m_length +to_be_added.m_length;
    output.m_width = this->m_width + to_be_added.m_width;
    return output;
}

bool Rectangle::operator==(const Rectangle &to_be_compared) {
    return ( (this->m_width == to_be_compared.m_width) &&
            (this->m_length == to_be_compared.m_length) );
}

bool Rectangle::operator!=(const Rectangle &to_be_compared) {
    // notice the re-usage of operator== and use of pointer to this object
    return !(*this == to_be_compared);
}
//operator pre fix increment
const Rectangle& Rectangle::operator++() {
    m_width++;
    m_length++;
    return *this;
}

//operator post fix increment
const Rectangle Rectangle::operator++( int ) {
    Rectangle R(*this);
    ++(*this);
    return R;
}

/* Overload the << and >> operators */
ostream &operator<<(ostream &os, const Rectangle &rect) {
    os << rect.m_width << "x" << rect.m_length << endl;
    return os;
}

istream &operator>>(istream &is, Rectangle &rect) {
    is >> rect.m_width >> rect.m_length;
    return is;
}
