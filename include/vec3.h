#ifndef VEC3_H
#define VEC3_H

#include<stdio.h>
#include<math.h>

class vec3
{
 public:
 vec3(): e{0,0,0}
  {

  }
  vec3(double e0,double e1,double e2) 
  {
    e[0] = e0;
    e[1] = e1;
    e[2] = e2;
  }
  double x() const 
  {
    return e[0];
  }

  double y() const
  {
    return e[1];
  }
  double z() const
  {
    return e[2];
  }

  vec3 operator-() const
  {
    return vec3(-e[0],-e[1],-e[2]);
  }

  double operator[](int i) const
  {
    return e[i];
  }
  //can read of write the value
  double& operator[](int i)
  {
    return e[i];
  }

  vec3& operator+=(const vec3 &v)
    {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
    }
  vec3& operator*=(const vec3 &v)
    {
      e[0] *= v.e[0];
      e[1] *= v.e[1];
      e[2] *= v.e[2];
      return *this;
    }
   vec3& operator*=(const double t)
    {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;
      return *this;
    }
   vec3& operator/=(const double t)
     {
       //TODO:(figure this thing out)
       return *this *=1/t;
     }
   double length()
   {
     return sqrt(length_squared());
   }

   double length_squared()
   {
     return e[0]*e[0]+ e[1]*e[1] + e[2]*e[2];
   }

  inline static vec3 random()
  {
    return vec3(random_double(),random_double(),random_double());
  }

  inline static vec3 random(double min,double max)
  {
    return vec3(random_double(min,max),random_double(min,max),random_double(min,max));
  }

  bool near_zero()
  {
    float s = 1e-8;
    return ((fabs(e[0])<s)&&(fabs(e[1])<s)&&(fabs(e[2])<s));
      
  }
 public:
  double e[3];
};

//type alises




inline vec3 operator+(const vec3 &u,const vec3 &v)
{
  return vec3(u.e[0]+v.e[0],u.e[1]+v.e[1],u.e[2]+v.e[2]);
}

inline vec3 operator*(const vec3 &u,const vec3 &v)
{
  return vec3(u.e[0]*v.e[0],u.e[1]*v.e[1],u.e[2]*v.e[2]);
}

inline vec3 operator-(const vec3 &u,const vec3 &v)
{
  return vec3(u.e[0]-v.e[0],u.e[1]-v.e[1],u.e[2]-v.e[2]);
}

inline vec3 operator*(double t,const vec3 &v)
{
  return vec3(t*v.e[0],t*v.e[1],t*v.e[2]);
}

inline vec3 operator*(const vec3 &v,double t)
{
  return t*v;
}

inline vec3 operator/(vec3 v,double t)
{
  return (1/t)*v;
}

inline double dot(const vec3 &u,const vec3 &v)
{
  return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

inline vec3 cross(const vec3 &u,const vec3 &v)
{
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
	      u.e[2] * v.e[0] - u.e[0] * v.e[2],
	      u.e[0] * v.e[1] - u.e[1] * v.e[0]
	      );
}

inline vec3 unit_vector(vec3 v)
{
  return v / v.length();
}

vec3 random_in_unit_sphere()
{
  while(true)
    {
      vec3 p =vec3::random(1,-1);
      if(p.length_squared()>=1) continue;
      return p;
    }
}

vec3 random_unit_vector()
{
  return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(vec3& normal)
{
  vec3 in_unit_sphere = random_in_unit_sphere();
  if(dot(in_unit_sphere,normal)>0.0)
    {
      return in_unit_sphere;
    }
  else
    {
      return -in_unit_sphere;
    }
}

//reflection
vec3 reflect(vec3 v,vec3 n)
{
  return v - 2*dot(v,n)*n;
}

//refraction

vec3 refract(const vec3& uv,const vec3&n, double eta_over_eta)
{
  float cos_theta = fmin(dot(-uv,n),1.0);
  vec3 r_out_perpendicular = eta_over_eta * (uv + cos_theta * n);
  vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perpendicular.length_squared()))*n;
  return r_out_perpendicular + r_out_parallel;
}
//random points inside unit_disk

vec3 random_in_unit_disk()
{
  while(true)
    {
      vec3 p = vec3(random_double(-1,1),random_double(-1,1),0);
      if(p.length_squared() >= 1)continue;
      return p;
    }
}

#endif 
