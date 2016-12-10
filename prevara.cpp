#include <iostream>
#include <set>
using namespace std;

double sweep = 0;
struct Point;
struct Duz;
struct duzStatusComp;
char get_line_intersection(double p0_x, double p0_y, double p1_x, double p1_y,
    double p2_x, double p2_y, double p3_x, double p3_y, double *i_x, double *i_y)
{
    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision
}





struct Point {
    Point(){}
    Point(double x, double y):x(x),y(y){}
    double x, y;

    bool operator < (const Point& other) const{
        if(y < other.y) return true;
        if(y > other.y) return false;
        if(x < other.x) return true;
        return false;
    }
};


struct Duz {
    Duz(){

    }

    Duz(Point a, Point b):A(a),B(b){}
    Duz(double a, double b, double c, double d){
        A = Point(a,b);
        B = Point(c,d);
    }

    Point A, B;

    bool operator < (const Duz& other) const{
        if(A < other.A) return true;
        if(other.A < A) return false;
        if(B < other.B) return true;
        return false;
    }
};

char intersection(Duz a, Duz b, Point* P){
    return get_line_intersection(a.A.x, a.A.y, a.B.x, a.B.y, b.A.x, b.A.y, b.B.x, b.B.y, &P->x, &P->y);
}

struct duzStatusComp {
    bool operator() (const Duz& first, const Duz& second) const{
        Point P, Q;
        intersection(first, Duz(0, sweep, 800, sweep), &P);
        intersection(second, Duz(0, sweep, 800, sweep), &Q);

        if (P.x < Q.x) return true;
        if (Q.x < P.x) return false;
//OBRADITI SLUCAJ KADA SE DUZ POKLAPA SA SWEEP-LINE-om
        intersection(first, Duz(0, sweep+0.1, 800, sweep+0.1), &P);
        intersection(second, Duz(0, sweep+0.1, 800, sweep+0.1), &Q);

        if (P.x < Q.x) return true;
        if (Q.x < P.x) return false;
        return false;
    }
};

int main()
{
    Duz a,b,c,d,e;
    std::set<Duz, duzStatusComp> status;

    sweep = 100;
    status.insert(a = Duz(150,100,225,250));
    status.insert(b = Duz(300,100,100,300));
    status.insert(c = Duz(400,10,400,400));
    status.insert(d = Duz(10,10,10,400));

    /*
    sweep = 120;
    status.insert(c = Duz(120,120,400,230));
    sweep = 150;
    status.insert(d = Duz(15,150,200,200));

    sweep = 160;
    status.insert(e = Duz(150,160,20,290));
    */
    sweep = 200;
    /*
     * status.insert(Duz(200.001,50,200.001,300));
    status.insert(Duz(199.999,50,199.999,300));
    */
    auto q = status.lower_bound(Duz(199.998,50,199.998,300));
    auto p = status.upper_bound(Duz(200.001,50,200.001,300));

    for(auto it = q; it != p; it++){
        cout << it->A.x << endl;
    }

    status.erase(q,p);

    status.insert(a = Duz(150,100,225,250));
    status.insert(b = Duz(300,100,100,300));

    return 0;
}



