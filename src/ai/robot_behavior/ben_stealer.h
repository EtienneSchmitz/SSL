/*
    This file is part of SSL.

    Copyright 2019 RomainPC (romainpc.lechat@laposte.net)

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

#include "robot_behavior/factory.h"
#include "robot_behavior/robot_behavior.h"

namespace rhoban_ssl
{
namespace robot_behavior
{
class BenStealer : public RobotBehavior
{
private:
  ConsignFollower* follower_;

  rhoban_ssl::annotations::Annotations annotations_;

  uint robot_id_to_steal_;

  bool in_front_of_;
  bool go_back_;

  const double APPROACH_PERIMETER = 0.4;
  const double ZONE_PRECISION_RADIUS = 0.1;
  const double RESET_RADIUS = 0.6;
  const double FINAL_APPROACH_RADIUS_FIRST_VALUE = 0.1;
  const double FINAL_APPROACH_DECREASE_SPEED = 0.001;

  double final_approach_value_;

public:
  BenStealer(uint robot_id_to_steal = 0);

  void update(double time, const data::Robot& robot, const data::Ball& ball);

  virtual Control control() const;

  rhoban_ssl::annotations::Annotations getAnnotations() const;

  void setRobotIdToSteal(uint id);

  virtual ~BenStealer();
};

}  // namespace robot_behavior
}  // namespace rhoban_ssl
