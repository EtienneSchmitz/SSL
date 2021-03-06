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

#include "slow_striker.h"
#include <math/tangents.h>
#include <math/vector2d.h>

namespace rhoban_ssl
{
namespace robot_behavior
{
SlowStriker::SlowStriker()
  : RobotBehavior(), robot_to_pass_id_(-1), robot_to_pass_team_(Ally), follower_(Factory::fixedConsignFollower())
{
  tempo_ = 0.0;
}

void SlowStriker::update(double time, const data::Robot& robot, const data::Ball& ball)
{
  // At First, we update time and update potition from the abstract class robot_behavior.
  // DO NOT REMOVE THAT LINE
  RobotBehavior::updateTimeAndPosition(time, robot, ball);
  // Now
  //  this->robot_linear_position
  //  this->robot_angular_position
  // are all avalaible

  const rhoban_geometry::Point& robot_position = robot.getMovement().linearPosition(time);

  if (robot_to_pass_id_ != -1)
  {  // if point_to_pass wasn't declare and robot_to_pass_id was.
    const data::Robot& robot_to_pass = getRobot(robot_to_pass_id_, robot_to_pass_team_);
    striking_point_ = robot_to_pass.getMovement().linearPosition(time);
  }

  Vector2d ball_striking_vector = striking_point_ - ballPosition();
  Vector2d ball_robot_vector = robot_position - ballPosition();
  double dist_ball_robot = ball_robot_vector.norm();

  ball_striking_vector = ball_striking_vector / ball_striking_vector.norm();
  ball_robot_vector = ball_robot_vector / ball_robot_vector.norm();

  double target_radius_from_ball;
  double scalar_ball_robot = -scalarProduct(ball_robot_vector, ball_striking_vector);
  if (tempo_ == 0.0)
  {
    target_radius_from_ball = 0.3;
  }

  if (scalar_ball_robot < 0)
  {
    follower_->avoidTheBall(true);
  }
  else
  {
    follower_->avoidTheBall(false);

    if (dist_ball_robot < target_radius_from_ball)
    {
      follower_->avoidOpponent(false);
    }
  }
  // TODO Add hysteresis
  if (dist_ball_robot > target_radius_from_ball)
  {
    follower_->avoidOpponent(true);
  }

  rhoban_geometry::Point target_position = ballPosition() - ball_striking_vector * (target_radius_from_ball);
  double target_rotation = detail::vec2angle(ball_striking_vector);

  double position_margin = 0.05;
  double waiting_time = 3.0;

  if ((Vector2d(target_position - robot_position).norm() < position_margin) && (tempo_ == 0.0))
  {
    tempo_ = time;
  }

  // if( Vector2d(target_position - robot_position).norm() > position_margin ) {
  //     tempo = 0.0;
  // }

  if ((Vector2d(target_position - robot_position).norm() < position_margin) && (tempo_ != 0.0))
  {
    if (std::abs(time - tempo_) >= waiting_time)
    {
      target_radius_from_ball = -0.5;
    }
  }
  follower_->setFollowingPosition(target_position, target_rotation);
  follower_->update(time, robot, ball);
}

Control SlowStriker::control() const
{
  Control ctrl = follower_->control();
  if (robot_to_pass_id_ != -1)
  {  // if point_to_pass wasn't declare and robot_to_pass_id was.
    ctrl.kick_power = 0.5;
  }
  else
  {
    ctrl.kick_power = 1;
  }
  ctrl.charge = true;
  ctrl.kick = true;
  return ctrl;
}

void SlowStriker::declarePointToStrike(rhoban_geometry::Point point)
{
  striking_point_ = point;
}

void SlowStriker::declareRobotToPass(int robot_id, Team team)
{
  robot_to_pass_id_ = robot_id;
  robot_to_pass_team_ = team;
}

SlowStriker::~SlowStriker()
{
  delete follower_;
}

rhoban_ssl::annotations::Annotations SlowStriker::getAnnotations() const
{
  return follower_->getAnnotations();
}

}  // namespace robot_behavior
}  // namespace rhoban_ssl
