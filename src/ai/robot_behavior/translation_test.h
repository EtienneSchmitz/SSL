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
#include <vector>

namespace rhoban_ssl
{
namespace robot_behavior
{
class TranslationTest : public RobotBehavior
{
private:
  ConsignFollower* follower_;
  rhoban_ssl::annotations::Annotations annotations_;

  rhoban_geometry::Point target_point_;
  std::vector<rhoban_geometry::Point> target_point_path_;

  bool reached_;
  double reach_radius_;
  int goToCount_, path_size_;

public:
  TranslationTest(rhoban_geometry::Point point = rhoban_geometry::Point(0, 0), double reach_radius = 0.01, int goToCount = 0);

  virtual void update(double time, const data::Robot& robot, const data::Ball& ball);

  virtual Control control() const;

  void setPoint(rhoban_geometry::Point point);

  rhoban_geometry::Point getPoint() const;

  void setPointPath(std::vector<rhoban_geometry::Point>  point_path, int size);

  std::vector<rhoban_geometry::Point> getPointPath() const;

  void setReachRadius(double radius);

  double getReachRadius() const;

  bool isReached();

  virtual rhoban_ssl::annotations::Annotations getAnnotations() const;

  virtual ~TranslationTest();
};

};  // namespace robot_behavior
};  // namespace rhoban_ssl
