#ifndef SPHERE_H
#define SPHERE_H


#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
 public:
  sphere(){}
  sphere(vec3 cent,double rad,std::shared_ptr<material> m)
    {
      center = cent;
      radius = rad;
      mat_ptr = m;
    }

  virtual bool hit(const ray& r,double t_min,double t_max,hit_record& h_rec) const override;
 public:

  vec3 center;
  double radius;
  std::shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r,double t_min,double t_max,hit_record& h_rec) const
{
  vec3 oc = (r.origin()) - center;
  float a = dot(r.direction(),r.direction());
  float b = 2 * dot(r.direction(),oc);
  float c = dot(oc,oc) - radius * radius;

  float discriminant = b*b - 4 * a * c;

  if(discriminant < 0)
    {
      return false;
    }

  float root = (-b - sqrt(discriminant))/2*a;

  if(root<t_min || t_max < root)
    {
      root = (-b + sqrt(discriminant))/2*a;
      if(root <t_min || t_max<root)
	{
	  return false;
	}
    }
  h_rec.t = root;
  h_rec.p = r.at(h_rec.t);
  
  vec3 outward_normal = (h_rec.p - center)/radius;
  h_rec.set_face_normal(r,outward_normal);
  h_rec.mat_ptr = mat_ptr;

  return true;
}
#endif
