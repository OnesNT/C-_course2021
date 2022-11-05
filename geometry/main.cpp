#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>

const double EPS = 1e-5;

bool equal(double a, double b) {
    return std::fabs(a - b) < EPS;
}

struct vector {
    double x , y;

     vector(const double& z , const double& t) {
        x = z;
        y = t;
    }
    vector() {
        x = 0.0;
        y = 0.0;
    }
    vector(const vector& other) {
        x = other.x;
        y = other.y;
    }
    ~vector() = default;

    vector& operator+=(const vector& other){
        x += other.x;
        y += other.y;
        return (*this);
    }
    vector& operator-=(const vector& other){
        x -= other.x;
        y -= other.y;
        return (*this);
    }
    vector& operator*=(const double& other){
        x *= other;
        y *= other;
        return (*this);
    }
    vector& operator/=(const double& other){
        x /= other;
        y /= other;
        return *this;
    }
    vector operator+() const{
        return *this;
    }
    vector operator-() const {
        vector temp;
        temp.x = -x;
        temp.y = -y;
        return temp;
    }
    double length() const {
        return sqrt(pow(x,2) + pow(y,2));
    }
};
bool operator==(const vector& first , const vector& second){
    return ( equal(first.x , second.x) && equal(first.y , second.y));
}
bool operator!=(const vector& first , const vector& second){
    return (!equal(first.x , second.x) || !equal(first.y , second.y));
}
vector operator+(const vector& first , const vector& second){
    return vector(first)+=second;
}
vector operator-(const vector& first , const vector& second){
    return vector(first)-=second;
}
vector operator*(const vector& first , const double& num){
    return vector(first)*=num;
}
vector operator*(const double& num , const vector& first){
    return (first*num);
}
vector operator/(const vector& first , const double& num){
    return vector(first)/=num;
}
vector operator/(const double& num , const vector& first){
    return (first/num);
}

double dot_product (const vector& lhs , const vector& rhs){
    return (lhs.x*rhs.x + lhs.y*rhs.y);
}
double cross_product (const vector& lhs , const vector& rhs){
    return (lhs.x*rhs.y - lhs.y*rhs.x);
}
bool collinear (const vector& lhs , const vector& rhs){
    return ( equal (cross_product ( lhs, rhs ) , 0.0) );
}

class shape {
public:
    virtual vector center() const = 0;
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool operator== (const shape& another) const = 0;
    virtual bool operator!= (const shape& another) const = 0;
    virtual bool congruent_to (const shape& another) const = 0;
    virtual void rotate(double angle) = 0;
    virtual void scale(double  coefficient) = 0;
    virtual void translate(vector transform) = 0;
};

class polygon : public shape{
public:
    std::vector<vector> vertex;
    polygon() {
        vertex.push_back(vector(0.0,0.0));
    }

    explicit polygon(const std::vector<vector>& vertex_more) : vertex(vertex_more) {
        if (sign() < 0) {
            vertex.clear();
            for ( size_t i = 0 ; i < vertex_more.size() ; i++)
                vertex.push_back(vertex_more[vertex_more.size()-1-i]);
        }
    }

    double sign(){
        double temp = 0.0;
        for ( size_t i = 0 ; i < vertex.size() ; i++)
            temp += ((vertex[i].x*vertex[(i+1)%(vertex.size())].y - vertex[i].y*vertex[(i+1)%(vertex.size())].x));
        return temp;
    }

    vector center() const override {
        double center_x, center_y;
        double temp_x = 0.0, temp_y = 0.0;
        double A = 0.0;

        for ( size_t i = 0 ; i < vertex.size() ; i++ ){
            temp_x += ((vertex[i].x + vertex[(i+1)%vertex.size()].x) * (vertex[i].x*vertex[(i+1)%vertex.size()].y - vertex[(i+1)%vertex.size()].x*vertex[i].y));
            temp_y += ((vertex[i].y + vertex[(i+1)%vertex.size()].y) * (vertex[i].x*vertex[(i+1)%vertex.size()].y - vertex[(i+1)%vertex.size()].x*vertex[i].y));
            A      += (0.5 * (vertex[i].x*vertex[(i+1)%vertex.size()].y - vertex[(i+1)%vertex.size()].x*vertex[i].y));
        }

        center_x = 1 / (6.0*A) * temp_x;
        center_y = 1 / (6.0*A) * temp_y;
        vector temp(center_x , center_y);
        return temp;
    }

    double perimeter() const override {
        double temp = 0.0;

        for ( size_t i = 0 ; i < vertex.size() ; i++)
            temp += sqrt(
                    pow((vertex[(i+1)%(vertex.size())].x-vertex[i].x),2)
                    +  pow((vertex[(i+1)%(vertex.size())].y-vertex[i].y),2) );

        return temp;
    }

    double area () const override {
        double temp = 0.0;
        for ( size_t i = 0 ; i < vertex.size() ; i++)
            temp += (0.5*(vertex[i].x*vertex[(i+1)%(vertex.size())].y - vertex[i].y*vertex[(i+1)%(vertex.size())].x));
        return fabs(temp);
    }

    bool operator==(const shape& another) const {

        const polygon* other = dynamic_cast<const polygon*>(&another);
        if (other == nullptr)
            return false;
        size_t t = 0;

        for ( size_t i = 0 ; i < vertex.size() ; i++ ) {
            if (vertex[0] == (*other).vertex[i]) {
                t = i;
                break;
            }
        }

        for (size_t i = 0; i < vertex.size() ; i++ ) {
            if (vertex[i] != (*other).vertex[t%(vertex.size())])
                return false;
            ++t;
        }

        return true;
    }

    bool operator!=(const shape& another) const override{
        const polygon* other = dynamic_cast<const polygon*>(&another);
        if (other == nullptr)
            return true;
        return !(*this==(*other));
    }

    bool congruent_to(const shape& another) const override{

        const polygon* other = dynamic_cast<const polygon*>(&another);
        ///***************************************
        if (other != nullptr) {
            std::vector<vector> long_(vertex.size()), long_t(vertex.size());

            size_t  t = 0,
                    n = vertex.size();

            for ( size_t i = 0 ; i < vertex.size() ; i++) {
                long_[i] = vector(vertex[(i+1)%n].x - vertex[i].x,
                                  vertex[(i+1)%n].y - vertex[i].y);

                long_t[i] = vector((*other).vertex[(i+1)%n].x - (*other).vertex[i].x,
                                   (*other).vertex[(i+1)%n].y - (*other).vertex[i].y);
            }

            for ( size_t i = 0 ; i < n ; i++ ) {
                if ( equal(long_[0].length(), long_t[i].length()) ) {
                    for (size_t j = 0; j < n; j++) {
                        if (!equal(long_[j].length(), long_t[(j + i) % n].length()) ||
                            !equal(long_[(j + 1) % n].length(), long_t[(j + 1 + i) % n].length()) ||
                            !equal(std::fabs(cross_product(long_[j], long_[(j + 1) % n])),
                                   std::fabs(cross_product(long_t[(j + i) % n], long_t[(j + 1 + i) % n])))) {
                            break;

                        }
                        if (j == n - 1)
                            return true;
                    }

                    for (size_t j = 0; j < n; j++) {
                        if (!equal(long_[j].length(), long_t[(i - j + n ) % n].length()) ||
                            !equal(long_[(j + 1) % n].length(), long_t[(i - j - 1 + n) % n].length())  ||
                            !equal(std::fabs(cross_product(long_[j], long_[(j + 1) % n])),
                                   std::fabs(cross_product(long_t[(i - j + n) % n], long_t[(i - j - 1 + n) % n])))) {

                            break;
                        }
                        if (j == n - 1)
                            return true;
                    }
                }
            }
            return false;
        } else return false;

    }

    void rotate(double angle) override {
        const vector vector_center= center();
        std::vector<vector> tempt(vertex.size());
        vector tempt1, tempt2;

        for (size_t i = 0; i < vertex.size(); i++) {
            tempt[i] = vertex[i] - vector_center;
            tempt1 = tempt[i];

            tempt2.x = tempt1.x*cos(angle) - tempt1.y*sin(angle);
            tempt2.y = tempt1.x*sin(angle) + tempt1.y*cos(angle);
            tempt[i] = tempt2;
            tempt[i] += vector_center;

            vertex[i] = tempt[i];
        }
    }

    void scale(double  coefficient) override{
        const vector vector_center = center();
        std::vector<vector> radius(vertex.size());

        for ( size_t i = 0 ; i < vertex.size() ; i++ ) {

            radius[i] = vertex[i]-vector_center;
            radius[i] *= coefficient;

            radius[i] += vector_center;
            vertex[i] = radius[i];
        }
    }

    void translate(vector transform) override{
        for ( size_t i = 0 ; i < vertex.size() ; i++ ){
            vertex[i] += transform;
        }
    }

    size_t vertices_cout() const {
        return vertex.size();
    }
    const std::vector<vector>& get_vertices() const {
        return vertex;
    }
};

class circle : public shape {
public:
    double radius_;
    vector center_;

    explicit circle(const vector& other_center , const double& other_radius) {
        radius_ = other_radius;
        center_ = other_center;
    }

    vector center() const override {
        return center_;
    }
    double perimeter() const override {
        return (2*M_PI*radius_);
    }
    double area() const override {
        return (M_PI*pow(radius_,2));
    }
    bool operator==(const shape& another) const override {
        const circle* other = dynamic_cast<const circle*>(&another);
        if (equal(radius_, (*other).radius_) && center_ == (*other).center_)
            return true;
        return false;
    }
    bool operator!=(const shape& another) const override{
        const circle* other = dynamic_cast<const circle*>(&another);
        if (other == nullptr)
            return true;
        return !((*this)==(*other));
    }
    bool congruent_to(const shape& another) const override{
        const circle* other = dynamic_cast<const circle*>(&another);
        if (other == nullptr)
            return false;
        return equal(radius_, (*other).radius_);

    }

    void rotate(double angle){};
    void scale(double coefficient) override{
        radius_ *= fabs(coefficient);
    }
    void translate(vector transform) override{
        center_ += transform;
    }
    const double radius() const {
        return radius_;
    }
};

class rectangle : public polygon{
public:
    double height_, width_;
    vector center_;

    explicit rectangle(const vector& other_center_,
                       const double& other_height,
                       const double& other_width) :
    polygon(std::vector<vector>{
        vector(other_center_.x + (other_width / 2), other_center_.y + (other_height / 2)),
        vector(other_center_.x - (other_width / 2), other_center_.y + (other_height / 2)),
        vector(other_center_.x - (other_width / 2), other_center_.y - (other_height / 2)),
        vector(other_center_.x + (other_width / 2), other_center_.y - (other_height / 2))
    })
        {
        center_ = other_center_;
        height_ = other_height;
        width_ = other_width;
    }

    vector center() const override {
        return center_;
    }
    double perimeter() const override {
        return (2*(height_+width_));
    }
    double area() const override {
        return height_*width_;
    }
    bool congruent_to(const shape& another) const override{
        const rectangle& other = dynamic_cast<const rectangle&>(another);
        return (equal(height_,other.height_) && equal(width_,other.width_));
    }
    void scale(double coefficient) override {
        vector radius;
        for ( size_t i = 0 ; i < vertex.size() ; i++ ) {
            radius.x = vertex[i].x - center().x;
            radius.y = vertex[i].y - center().y;

            radius*= fabs(coefficient);

            vertex[i].x = radius.x + center().x;
            vertex[i].y = radius.y + center().y;
        }
        width_ *= fabs(coefficient);
        height_ *= fabs(coefficient);
    }

    double height() const{
        return height_;
    }
    double width() const{
        return width_;
    }
};

class square : public rectangle {
public:
    double side_;
    vector center_;

    explicit square(const vector& other_center_ , const double& other_side) :
        rectangle(other_center_, other_side, other_side) {
            side_ = other_side;
            center_ = other_center_;
    }

    double side() const {
        return side_;
    }
    circle circumscribed_circle() const {
        circle temp(center_,side_*sqrt(2));
        return temp;
    }
    circle inscribed_circle() const {
        circle temp(center_,side_);
        return temp;
    }
    bool congruent_to(const shape& another) const {
        const square& other = dynamic_cast<const square&>(another);
        return (equal(side_, other.side_));
    }
};

class triangle : public polygon {
public:
    vector v1,v2,v3;

    explicit triangle(const vector& other_v1 , const vector& other_v2, const vector& other_v3) :
    polygon (std::vector<vector> {other_v1,
                                 other_v2,
                                 other_v3}
    ) {
        v1 = other_v1;
        v2 = other_v2;
        v3 = other_v3;
    }

    circle circumscribed_circle() const {
        double a,b;
        a = ((((pow(v2.length(),2)-pow(v1.length(),2))*(v3.y-v1.y)-(pow(v3.length(),2)-pow(v1.length(),2))*(v2.y-v1.y)))
                / ((2*(v2.x-v1.x)*(v3.y-v1.y))-(2*(v3.x-v1.x)*(v2.y-v1.y))));
        b = ((pow(v2.length(),2)-pow(v1.length(),2))-2*a*(v2.x-v1.x))/(2*(v2.y-v1.y));

        double radius = vector((a-v1.x),(b-v1.y)).length();
        return circle(vector(a,b),radius);
    }

    circle inscribed_circle() const {
        double tile = -(vector(v2.x-v1.x,v2.y-v1.y).length())/(vector(v3.x-v1.x,v3.y-v1.y)).length();
        vector D((v3.x*tile - v2.x)/(tile-1.0),(v3.y*tile - v2.y)/(tile-1.0));
        tile = -(D-v2).length()/(v1-v2).length();
        vector J((v1.x*tile-D.x)/(tile-1.0) , (v1.y*tile-D.y)/(tile-1.0));
        double radius;
        radius = std::fabs((v2.y-v1.y)*J.x + (v1.x-v2.x)*J.y - (v2.y-v1.y)*v1.x - (v1.x-v2.x)*v1.y) / (v2-v1).length();
        return circle(J,radius);
    }
};
int main() {
    return 0;
}
