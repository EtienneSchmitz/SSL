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

#include "see_ball.h"
#include <math/vector2d.h>

namespace RhobanSSL
{
namespace Robot_behavior
{
Beginner_see_ball::Beginner_see_ball(Ai::AiData& ai_data)
  : RobotBehavior(ai_data), follower(Factory::fixed_consign_follower(ai_data))
{
}

void Beginner_see_ball::update(double time, const Ai::Robot& robot, const Ai::Ball& ball)
{
  // At First, we update time and update potition from the abstract class robot_behavior.
  // DO NOT REMOVE THAT LINE
  RobotBehavior::update_time_and_position(time, robot, ball);

  annotations.clear();

  const rhoban_geometry::Point& robot_position = robot.get_movement().linear_position(ai_data.time);

  Vector2d direction = ball_position() - robot_position;
  ContinuousAngle target_rotation = vector2angle(direction);

  follower->set_following_position(robot_position, target_rotation);

  follower->avoid_the_ball(false);
  follower->update(time, robot, ball);
}

Control Beginner_see_ball::control() const
{
  Control ctrl = follower->control();
  // ctrl.spin = true; // We active the dribler !
  ctrl.kick = false;
  return ctrl;
}

Beginner_see_ball::~Beginner_see_ball()
{
  delete follower;
}

RhobanSSLAnnotation::Annotations Beginner_see_ball::get_annotations() const
{
  RhobanSSLAnnotation::Annotations annotations;
  annotations.addAnnotations(this->annotations);
  annotations.addAnnotations(follower->get_annotations());
  return annotations;
}

}  // namespace Robot_behavior
}  // namespace RhobanSSL
