#include "../include/rtweekend.h"

#include "../include/hittable_list.h"
#include "../include/sphere.h"
#include "../include/material.h"
#include "../include/camera.h"
#include<ctime>


//hittal list random_world

hittable_list random_scene()
{
  hittable_list world;

  std::shared_ptr<lambertian>ground_material = std::make_shared<lambertian>(vec3(0.5,0.5,0.5));
  world.add(std::make_shared<sphere>(vec3(0,-1000,0),1000,ground_material));

  for(int a =-11;a<11;a++)
    {
      for(int b= -11;b<11;b++)
	{
	  double choose_mat = random_double();
	  vec3 center(a+0.9*random_double(),0.2,b+0.9*random_double());

	  if((center - vec3(4,0.2,0)).length()>0.9)
	    {
	      std::shared_ptr<material> sphere_material;

	      if(choose_mat<0.8)
		{
		  //diffuse
		  vec3 albedo = vec3::random() * vec3::random();
		  sphere_material = std::make_shared<lambertian>(albedo);
		  world.add(std::make_shared<sphere>(center,0.2,sphere_material));
		}
	      else if(choose_mat < 0.95)
		{
		  //metal
		  vec3 albedo =vec3::random(0.5,1);
		  double fuzz = random_double(0,0.5);
		  sphere_material = std::make_shared<metal>(albedo,fuzz);
		  world.add(std::make_shared<sphere>(center,0.2,sphere_material));
		  
		}
	      else
		{
		  //glass
		  sphere_material = std::make_shared<dielectric>(1.5);
		  world.add(std::make_shared<sphere>(center,0.2,sphere_material));
		}
	    }
	}
    }
  std::shared_ptr<dielectric> material1 = std::make_shared<dielectric>(1.5);
  world.add(std::make_shared<sphere>(vec3(0,1,0),1.0,material1));

  std::shared_ptr<lambertian> material2 = std::make_shared<lambertian>(vec3(0.4,0.2,0.1));
  world.add(std::make_shared<sphere>(vec3(-4,1,0),1.0,material2));

  std::shared_ptr<metal> material3 = std::make_shared<metal>(vec3(0.7,0.6,0.5),0.0);
  world.add(std::make_shared<sphere>(vec3(4,1,0),1.0,material3));

  return world;
  
}


double hit_sphere(vec3 center,float radius,ray **r)
{
  ray *ra = *r;
  vec3 oc = (ra->origin()) - center;
  float a = dot(ra->direction(),ra->direction());
  float b = 2 * dot(ra->direction(),oc);
  float c = dot(oc,oc) - (radius * radius);

  float discriminant = b*b - 4*a*c;

  if(discriminant < 0)
    {
      return -1;
    }
  else
    {
      return (-b - sqrt(discriminant))/2 * a ;
    }
  
    
}
vec3 ray_color(const ray& r,const hittable_list& world,int max_depth)
{
  hit_record h_rec;

  if(max_depth <=0)
    {
      return vec3(0,0,0);
    }
  if(world.hit(r,0.001,infinity,h_rec))
    {
      /*
      vec3 target = h_rec.p+ h_rec.normal + random_in_hemisphere(h_rec.normal);
      ray rr(h_rec.p, target - h_rec.p);
      return 0.5 * ray_color(&rr,world,max_depth-1);
      */
      ray scattered;
      vec3 attenutaion;
      if(h_rec.mat_ptr->scatter(r,h_rec,attenutaion,scattered))
	{
	  return attenutaion * ray_color(scattered,world,max_depth-1);
	}
      else
	{
	  return vec3(0,0,1);
	}
    }
  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5 * (unit_direction.y() + 1.0);

  return (1.0 - t)* vec3(1.0,1.0,1.0) + t * vec3(0.2,0.4,1.0);
}
int main()
{

  //setting time

  clock_t begin = clock();
  //setting up the camera

  //image
  const float aspect_ratio = 3.0/2.0;
  const int   image_width = 400;
  const int   image_height = static_cast<int>(image_width/aspect_ratio);
  const int   samples_per_pixel = 500;
  const int   max_depth = 50;


  //world
  //hittable_list world;

  hittable_list world = random_scene();
  
  /*
  world.add(std::make_shared<sphere>(vec3(0,-100.5,-1),100));
  world.add(std::make_shared<sphere>(vec3(0,0.1,-1),0.5));
  
  */
  auto material_left   = std::make_shared<metal>(vec3(0.8, 0.6, 0.2),0.0);
  auto material_right  = std::make_shared<dielectric>(1.5);
  auto material_ground = std::make_shared<lambertian>(vec3(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<lambertian>(vec3(0.1,0.2,0.5));
  
 
  
  world.add(std::make_shared<sphere>(vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(std::make_shared<sphere>(vec3( 0.0,    0.0, -1.0),   0.5, material_center));
  world.add(std::make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.5, material_left));
  world.add(std::make_shared<sphere>(vec3( 1.0,    0.0, -1.0),   0.5, material_right));
  world.add(std::make_shared<sphere>(vec3( 1.0,    0.0, -1.0),  -0.45, material_right));

  /*
  auto material_left = std::make_shared<lambertian>(vec3(0,0,1));
  auto material_right = std::make_shared<lambertian>(vec3(1,0,0));

  world.add(std::make_shared<sphere>(vec3(-R,0,-1),R,material_left));
  world.add(std::make_shared<sphere>(vec3(R,0,-1),R,material_right));
  */
  
  //camera
  vec3 lookfrom(13,3,2);
  vec3 lookat(0,0,0);
  vec3 vup(0,1,0);
  double dist_to_focus = 10;
  double aperature = 0.1;

  camera cam(vec3(-2,2,1),vec3(0,0,-1),vec3(0,1,0),20,aspect_ratio,aperature,dist_to_focus);

 
  //In ray tracing, the lower left corner is often used as a reference point or origin for ge   nerating rays that traverse the scene and interact with objects. This corner typically rep   resents the position of the viewer or camera in the virtual scene.
  

  
  
  FILE *fp = fopen("../img/image.ppm","wb");
  (void)fprintf(fp,"P6\n%d %d\n255\n",image_width,image_height);
  static unsigned char color[3];
  for(int i = image_height;i>=0;--i)
    {
      for(int j=0;j<image_width;++j)
	{
	  /*double r = (double)j/image_width -1;
	  double g = (double)i/image_height -1;
	  double b = 0.25;
	  */
	  vec3 pixel_color(0,0,0);

	  for(int s =0;s<samples_per_pixel;++s)
	    {

	      double u = (double)(j+random_double())/(image_width  - 1);
	      double v = (double)(i+random_double())/(image_height - 1);
	      
	      ray r = cam.get_ray(u,v);

	      pixel_color += ray_color(r,world,max_depth);
	    }

	  double r = pixel_color.x();
	  double g = pixel_color.y();
	  double b = pixel_color.z();
	  
	  double scale = 1.0/samples_per_pixel;
	  r = sqrt(scale * r);
	  g = sqrt(scale * g);
	  b = sqrt(scale * b);
	  
	      
	  
	  int ir = (int)(255.999 * clamp(r,0.0,0.999));
	  int ig = (int)(255.999 * clamp(g,0.0,0.999));
	  int ib = (int)(255.999 * clamp(b,0.0,0.999));
	      
	  color[0] = ir;
	  color[1] = ig;
	  color[2] = ib;

	  (void)fwrite(color,1,3,fp);
	  clock_t end = clock();
	  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	  printf(" time : %f\n",time_spent);
	}
    }
  (void)fclose(fp);

  return 0;
}
