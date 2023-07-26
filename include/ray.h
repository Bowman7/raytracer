#ifndef RAY_H
#define RAY_H

#include"vec3.h"

//ray equation P(t)=A+tb where P is a 3D point A is origin and b is direction and t is mag
class ray
{
 public:
  ray()
    {

    }
  ray(const vec3& origin,const vec3& direction)
    {
      orig = origin;
      dir  = direction;
    }

  vec3 origin() const
  {
    return orig;
  }
  vec3 direction() const
  {
    return dir;
  }
  vec3 at(double t) const
  {
    return orig+t*dir;
  }

 public:
  vec3 orig;
  vec3 dir;
  
};
#endif
