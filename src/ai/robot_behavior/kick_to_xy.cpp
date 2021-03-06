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

#include "kick_to_xy.h"
#include <math/vector2d.h>

namespace rhoban_ssl
{
namespace robot_behavior
{
KickToXY::KickToXY(rhoban_geometry::Point target_point) : RobotBehavior(), follower_(Factory::fixedConsignFollower())

{
  target_point_ = target_point;
}

void KickToXY::update(double time, const data::Robot& robot, const data::Ball& ball)
{
  // At First, we update time and update position from the abstract class robot_behavior.
  // DO NOT REMOVE THAT LINE
  RobotBehavior::updateTimeAndPosition(time, robot, ball);

  annotations_.clear();

  rhoban_geometry::Point robot_position = robot.getMovement().linearPosition(time);
  ContinuousAngle robot_rotation = robot.getMovement().angularPosition(time);

  Vector2d ball_target_vector = target_point_ - ballPosition();
  double ball_target_distance = ball_target_vector.norm();

  Vector2d ball_velocity = ball.getMovement().linearVelocity(time);
  if (ball_target_distance > ZONE_RADIUS && ball_velocity.getX() < 0.15 && ball_velocity.getY() < 0.15)  // if ball not
                                                                                                         // yet arrived
                                                                                                         // in the zone
  {
    Vector2d ball_robot_vector = robot_position - ballPosition();
    ball_target_vector = normalized(ball_target_vector);
    ball_robot_vector = normalized(ball_robot_vector);

    double target_radius_from_ball;
    double scalar_ball_robot = -scalarProduct(ball_robot_vector, ball_target_vector);

    if (scalar_ball_robot < 0)  // if the robot is behind the ball
    {
      follower_->avoidTheBall(true);
      target_radius_from_ball = 1.5;
    }
    else  // if the robot need to backward to go behind the ball
    {
      follower_->avoidTheBall(false);
      target_radius_from_ball = 1 / (2 * (scalar_ball_robot - 1.2)) + 2;
    }
    robot_position = ballPosition() - ball_target_vector * target_radius_from_ball;
    robot_rotation = detail::vec2angle(ball_target_vector);
  }
  // set kick power:
  kick_power_ = Control::getNeededPower(ball_target_distance, robot.id);

  follower_->setFollowingPosition(robot_position, robot_rotation);
  follower_->avoidTheBall(false);
  follower_->update(time, robot, ball);
}

Control KickToXY::control() const
{
  Control ctrl = follower_->control();
  ctrl.kick_power = kick_power_;
  ctrl.kick = true;
  ctrl.charge = true;
  return ctrl;
}

KickToXY::~KickToXY()
{
  delete follower_;
}

rhoban_ssl::annotations::Annotations KickToXY::getAnnotations() const
{
  rhoban_ssl::annotations::Annotations annotations;
  annotations.addAnnotations(annotations_);
  annotations.addAnnotations(follower_->getAnnotations());
  return annotations;
}

}  // namespace robot_behavior
}  // namespace rhoban_ssl