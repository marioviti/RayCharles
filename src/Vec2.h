#ifndef VEC2_H
#define VEC2_H

class Vec2 {
private:
    float mVals[2];
public:
    Vec2() {}
    Vec2( float x , float y  ) {
       mVals[0] = x; mVals[1] = y;
    }
    float & operator [] (unsigned int c) { return mVals[c]; }
    float operator [] (unsigned int c) const { return mVals[c]; }
};

#endif
