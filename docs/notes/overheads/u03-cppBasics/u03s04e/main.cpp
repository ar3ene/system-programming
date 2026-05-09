#include <iostream>

#include "Rectangle.h"

using namespace std;

int main() {

    //default constructor
    Rectangle r1;

    cout << "Rectangle r1 with width " <<
        r1.getW() << " and length " << r1.getL() << endl;

    //multiple constructors
    double a=3, b=6, c=4, d=8;
    Rectangle r2(a,b);
    Rectangle r3(c,d,a);

    cout << "Rectangle r2 with width " <<
         r2.getW() << " and length " << r2.getL() << endl;

    cout << "Rectangle r3 with width " <<
         r3.getW() << " and length " << r3.getL() << endl;

    //setters
    r3.setW(d);
    cout << "Rectangle r3 with width " <<
         r3.getW() << " and length " << r3.getL() << endl;

    int x=3;
    r3.setL(x);

    cout << "Rectangle r3 with width " <<
         r3.getW() << " and length " << r3.getL() << endl;




    // Operator=
    Rectangle rb(10, 12), rc;
    Rectangle ra = rb;
    rc = rb;

    cout << "Rectangle rc with width " <<
         rc.getW() << " and length " << rc.getL() << endl;

    // Operator+
    rb = rc + r1;
    cout << "Rectangle rb (rc + r1) with width " <<
         rb.getW() << " and length " << rb.getL() << endl;

    cout << "Rectangle rc with width " <<
         rc.getW() << " and length " << rc.getL() << endl;


    // Operator==
    Rectangle rd = rb;
    cout << "rd == rb? " << rd.operator==(rb) << endl;
    bool res = (rd == rb);
    cout << "rd == rb? " << res << endl;

    // Operator!=
    cout << "rd != rb? " << rd.operator!=(rb) << endl;

    //operator pre fix increment
    rc++;
    cout << "Rectangle rc with width " <<
         rc.getW() << " and length " << rc.getL() << endl;

    //operator post fix increment (called twice)
    rc.operator++(x);
    ++rc;
    cout << "Rectangle rc with width " <<
         rc.getW() << " and length " << rc.getL() << endl;

    //reuse of existing objects in constructor
    Rectangle r4(rc);
    cout << "Rectangle r4 with width " <<
         r4.getW() << " and length " << r4.getL() << endl;

    // Overload the << and >> operators

    cout << rc << endl;

    Rectangle r5;
    cin >> r5; // get two numbers into m_width and m_length
    cout << r5 << endl;; // print “<m_width> x <m_length>”;

    return 0;
}
