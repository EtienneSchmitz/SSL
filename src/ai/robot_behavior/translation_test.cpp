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

#include "translation_test.h"
#include <math/vector2d.h>

namespace rhoban_ssl
{
namespace robot_behavior
{
TranslationTest::TranslationTest(rhoban_geometry::Point point, double reach_radius, int goToCount)
  : RobotBehavior()
  , follower_(Factory::fixedConsignFollower())
  , target_point_(point)
  , reached_(false)
  , reach_radius_(reach_radius)

{
}

void TranslationTest::update(double time, const data::Robot& robot, const data::Ball& ball)
{
  // At First, we update time and update potition from the abstract class robot_behavior.
  // DO NOT REMOVE THAT LINE
  RobotBehavior::updateTimeAndPosition(time, robot, ball);

  annotations_.clear();

  const rhoban_geometry::Point robot_position = robot.getMovement().linearPosition(time);

  if (robot_position.getDist(target_point_) <= reach_radius_)
  {
    switch (goToCount_) {
      case 0:
          this->setPoint(rhoban_geometry::Point(-1, 1));
          goToCount_++;
          break;
      case 1:
          this->setPoint(rhoban_geometry::Point(-2, 0));
          goToCount_++;
          break;
      case 2:
          this->setPoint(rhoban_geometry::Point(-1, 0));
          goToCount_++;
          break;
      case 3:
          this->setPoint(rhoban_geometry::Point(-2, 0));
          goToCount_++;
          break;
      case 4:
          this->setPoint(rhoban_geometry::Point(-1, -1));
          goToCount_++;
          break;
      case 5:
          this->setPoint(rhoban_geometry::Point(-2, 0));
          goToCount_++;
          break;
      case 6:
          this->setPoint(rhoban_geometry::Point(-2, -1));
          goToCount_++;
          break;
      case 7:
          this->setPoint(rhoban_geometry::Point(-2, 0));
          goToCount_++;
          break;
      case 8:
          this->setPoint(rhoban_geometry::Point(-3, -1));
          goToCount_++;
          break;
      case 9:
          this->setPoint(rhoban_geometry::Point(-2, 0));
          goToCount_++;
          break;
      case 10:
          this->setPoint(rhoban_geometry::Point(-3, 0));
          goToCount_++;
          break;
      case 11:
          this->setPoint(rhoban_geometry::Point(-2, 0));
          goToCount_++;
          break;
      case 12:
          this->setPoint(rhoban_geometry::Point(-3, 1));
          goToCount_++;
          break;
      case 13:
          this->setPoint(rhoban_geometry::Point(-2, 0));
          goToCount_++;
          break;
      case 14:
          this->setPoint(rhoban_geometry::Point(-2, 1));
          goToCount_++;
          break;
      case 15:
          this->setPoint(rhoban_geometry::Point(-2, 0));
          reached_ = true;
          break;
    }
  }
  rhoban_geometry::Point position_follower = target_point_;

  Vector2d vect_robot_target = target_point_ - robot_position;
  ContinuousAngle rotation_follower = vector2angle(vect_robot_target);

  follower_->setFollowingPosition(position_follower, rotation_follower);
  follower_->avoidTheBall(false);
  follower_->update(time, robot, ball);
}

void TranslationTest::setPoint(rhoban_geometry::Point point)
{
  target_point_ = point;
  reached_ = false;
}

rhoban_geometry::Point TranslationTest::getPoint() const
{
  return target_point_;
}

void TranslationTest::setReachRadius(double radius)
{
  reach_radius_ = radius;
}

double TranslationTest::getReachRadius() const
{
  return reach_radius_;
}

bool TranslationTest::isReached()
{
  return reached_;
}

Control TranslationTest::control() const
{
  Control ctrl = follower_->control();
  return ctrl;
}

TranslationTest::~TranslationTest()
{
  delete follower_;
}

rhoban_ssl::annotations::Annotations TranslationTest::getAnnotations() const
{
  rhoban_ssl::annotations::Annotations annotations;
  annotations.addAnnotations(annotations_);
  annotations.addAnnotations(follower_->getAnnotations());
  return annotations;
}

}  // namespace robot_behavior
}  // namespace rhoban_ssl
