#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera
{
 public:
  camera(vec3 lookfrom,vec3 lookat,vec3 vup,double vfov,double aspect_ratio
	 ,double aperature, double focus_dist
	 )
    {
      double theta = degrees_to_radians(vfov);
      double h = tan(theta/2);
      float viewport_height = 2.0 * h;
      float viewport_width = aspect_ratio * viewport_height;
      float focal_length = 1.0;

      w = unit_vector(lookfrom - lookat);
      u = unit_vector(cross(vup,w));
      v = cross(w,u);
      
      
      origin =lookfrom;
      horizontal = focus_dist* viewport_width * u;
      vertical = focus_dist*viewport_height * v;
      lower_left_corner = origin - horizontal/2 - vertical/2 -focus_dist*w;

      lens_radius = aperature/2;

      
    }

  ray get_ray(double s,double t)
  {
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u*rd.x()+v*rd.y();
    return ray(origin+offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
  }

 public:
  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u,v,w;
  double lens_radius;
};
#endif
