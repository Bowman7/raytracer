#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material
{
 public:
  virtual bool scatter(const ray& r_in,const hit_record& h_rec,vec3& attenuation, ray& scattered) const = 0;
};

#endif

//diffuse scatter

class lambertian : public material
{

 public:
  lambertian(vec3 a)
    {
      albedo = a;
    }
  virtual bool scatter(const ray& r_in,const hit_record& h_rec,vec3& attenution, ray& scattered)const override
  {
    vec3 scatter_direction = h_rec.normal + random_unit_vector();
    if(scatter_direction.near_zero())
      {
	scatter_direction = h_rec.normal;
      }
    scattered = ray(h_rec.p,scatter_direction);
    attenution = albedo;
    return true;
  }
 public:
  vec3 albedo;
};

//metal scatter

class metal : public material
{
 public:
  metal(vec3 a,double f)
    {
      albedo = a;
      fuzz = f<1?f:1;
    }
  virtual bool scatter(const ray& r_in,const hit_record& h_rec,vec3& attenution, ray& scattered)const override
  {
    vec3 reflected = reflect(unit_vector(r_in.direction()),h_rec.normal);
     
    scattered = ray(h_rec.p,reflected+ fuzz*random_in_unit_sphere());
    attenution = albedo;
    if(dot(scattered.direction(),h_rec.normal)>0)
      {
	return true;
      }
    else
      {
	return false;
      }
  }
 public:
  vec3 albedo;
  double fuzz;
};

//dielectrics
class dielectric : public material
{
 public:
  dielectric(double index_ref)
    {
      ir = index_ref;
    }
  virtual bool scatter(const ray& r_in,const hit_record& h_rec,vec3& attenution, ray& scattered)const override
  {
    attenution = vec3(1.0,1.0,1.0);
    double ref_ratio = h_rec.front_face?(1/ir):ir;

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction,h_rec.normal),1.0);
    double sin_theta = sqrt(1-cos_theta * cos_theta);

    bool cannot_refract = (ref_ratio * sin_theta)>1.0;
    vec3 direction;

    if(cannot_refract || reflectance(cos_theta,ref_ratio)>random_double())
      {
	direction =reflect(unit_direction,h_rec.normal);
      }
    else
      {
	direction = refract(unit_direction,h_rec.normal,ref_ratio);
      }
    scattered = ray(h_rec.p,direction);
    return true;
  }
  
 public:
  double ir;//relective index

 private:
  static double reflectance(double cos_theta,double ref_ratio)
  {
    double r0 = (1-ref_ratio)/(1+ref_ratio);
    r0 = r0*r0;
    return (r0+(1-r0)*pow((1-cos_theta),5)); 
  }
};
