/*
    This file is part of SSL.

    Copyright 2018 Boussicault Adrien (adrien.boussicault@u-bordeaux.fr)

    SSL is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SSL.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MOVEMENT_WITH_NO_PREDICTION_H
#define MOVEMENT_WITH_NO_PREDICTION_H

#include <physic/movement.h>

namespace RhobanSSL
{
class Movement_with_no_prediction : public Movement
{
private:
  MovementSample samples;

public:
  virtual Movement* clone() const;

  virtual double last_time() const;

  virtual void set_sample(const MovementSample& samples);
  virtual const MovementSample& get_sample() const;

  virtual rhoban_geometry::Point linear_position(double time) const;
  virtual ContinuousAngle angular_position(double time) const;

  virtual Vector2d linear_velocity(double time) const;
  virtual ContinuousAngle angular_velocity(double time) const;

  virtual Vector2d linear_acceleration(double time) const;
  virtual ContinuousAngle angular_acceleration(double time) const;

  virtual void print(std::ostream& stream) const;

  virtual ~Movement_with_no_prediction();
};

}  // namespace RhobanSSL

#endif
