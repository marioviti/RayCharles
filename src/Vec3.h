#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <time.h>
#include <stdlib.h>

class Vec3 {
private:
    float mVals[3];
public:
    Vec3() {}
    Vec3( float x , float y , float z ) {
       mVals[0] = x; mVals[1] = y; mVals[2] = z;
    }
    float & operator [] (unsigned int c) { return mVals[c]; }
    float operator [] (unsigned int c) const { return mVals[c]; }
    void operator = (Vec3 const & other) {
       mVals[0] = other[0] ; mVals[1] = other[1]; mVals[2] = other[2];
    }
    float squareLength() const {
       return mVals[0]*mVals[0] + mVals[1]*mVals[1] + mVals[2]*mVals[2];
    }
    float length() const { return sqrt( squareLength() ); }

    Vec3 opposite() { mVals[0] = -mVals[0]; mVals[1] = -mVals[1]; mVals[2] = -mVals[2]; return *this;}

    Vec3 normalize() { float L = length(); mVals[0] /= L; mVals[1] /= L; mVals[2] /= L; return *this;}

    static float dot( Vec3 const & a , Vec3 const & b ) {
       return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }

    static Vec3 cross( Vec3 const & a , Vec3 const & b ) {
       return Vec3( a[1]*b[2] - a[2]*b[1] ,
                    a[2]*b[0] - a[0]*b[2] ,
                    a[0]*b[1] - a[1]*b[0] );
    }

    static Vec3 componentProduct(Vec3 const & a, Vec3 const & b) {
      return Vec3(a[0]*b[0],a[1]*b[2],a[2]*b[2]);
    }

    static Vec3 random_unit_vector(int seed) {
      float x,y,z;
      srand(seed);
      x = static_cast <float> ( (rand()) / (static_cast <float> (RAND_MAX))) *2 -1;
      y = static_cast <float> ( (rand()) / (static_cast <float> (RAND_MAX))) *2 -1;
      z = static_cast <float> ( (rand()) / (static_cast <float> (RAND_MAX))) *2 -1;
      Vec3 random_unit_vector = Vec3(x,y,z).normalize();
      return random_unit_vector;
    }

    static Vec3 random_in_emisphere(Vec3 const & o, Vec3 const & n, int seed) {
      Vec3 sample = random_unit_vector(seed);
      float theta =  Vec3::dot(sample,n);
      if (theta < 0) sample.opposite();
      return sample;
    }

    void operator += (Vec3 const & other) {
        mVals[0] += other[0];
        mVals[1] += other[1];
        mVals[2] += other[2];
    }
    void operator -= (Vec3 const & other) {
        mVals[0] -= other[0];
        mVals[1] -= other[1];
        mVals[2] -= other[2];
    }
    void operator *= (float s) {
        mVals[0] *= s;
        mVals[1] *= s;
        mVals[2] *= s;
    }
    void operator /= (float s) {
        mVals[0] /= s;
        mVals[1] /= s;
        mVals[2] /= s;
    }
};

static inline Vec3 operator + (Vec3 const & a , Vec3 const & b) {
   return Vec3(a[0]+b[0] , a[1]+b[1] , a[2]+b[2]);
}
static inline Vec3 operator - (Vec3 const & a , Vec3 const & b) {
   return Vec3(a[0]-b[0] , a[1]-b[1] , a[2]-b[2]);
}

// *
static inline Vec3 operator * (float a , Vec3 const & b) {
   return Vec3(a*b[0] , a*b[1] , a*b[2]);
}
static inline Vec3 operator * (Vec3 const & b, float a) {
   return Vec3(a*b[0] , a*b[1] , a*b[2]);
}

// /
static inline Vec3 operator / (Vec3 const &  a , float b) {
   return Vec3(a[0]/b , a[1]/b , a[2]/b);
}

static inline Vec3 operator / (float b, Vec3 const &  a) {
   return Vec3(a[0]/b , a[1]/b , a[2]/b);
}

static inline std::ostream & operator << (std::ostream & s , Vec3 const & p) {
    s << p[0] << " " << p[1] << " " << p[2];
    return s;
}
static inline std::istream & operator >> (std::istream & s , Vec3 & p) {
    s >> p[0] >> p[1] >> p[2];
    return s;
}

#endif
