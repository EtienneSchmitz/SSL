/*
    This file is part of SSL.

    Copyright 2018 TO COMPLETE

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

#pragma once

#include <robot_behavior/robot_behavior.h>
#include <robot_behavior/factory.h>

namespace rhoban_ssl
{
namespace robot_behavior
{
namespace attacker
{
class Striker : public RobotBehavior
{
private:

  double tempo_;
  rhoban_geometry::Point striking_point_;
  ConsignFollower* follower_;

public:
  Striker(rhoban_geometry::Point striking_point);

  virtual void update(double time, const data::Robot& robot, const data::Ball& ball);

  virtual Control control() const;

  virtual rhoban_ssl::annotations::Annotations getAnnotations() const;
  void declarePointToStrike(rhoban_geometry::Point point);
  virtual ~Striker();
};

};  // namespace attacker
};  // namespace robot_behavior
};  // namespace rhoban_ssl
