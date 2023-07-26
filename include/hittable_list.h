#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include"hittable.h"

#include<memory>
#include<vector>


class hittable_list : public hittable
{
 public:
  hittable_list()
    {

    };
  hittable_list(std::shared_ptr<hittable> object)
  {
    add(object);
  }
  void add(std::shared_ptr<hittable> object)
  {
    objects.push_back(object);
  }

  void clear()
  {
    objects.clear();
  }

  virtual bool hit(const ray& r,double t_min,double t_max,hit_record& h_rec) const override;

 public:
  std::vector<std::shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r,double t_min,double t_max,hit_record& h_rec)const
{
  hit_record temp_rec;
  bool hit_anything = false;
  double closest_sofar = t_max;


  for(const std::shared_ptr<hittable>&object : objects)
    {
      if(object->hit(r,t_min,t_max,temp_rec))
	{
	  hit_anything = true;
	  closest_sofar = temp_rec.t;
	  h_rec = temp_rec;
	  
	  
	}
    }

  return hit_anything;
}
#endif
