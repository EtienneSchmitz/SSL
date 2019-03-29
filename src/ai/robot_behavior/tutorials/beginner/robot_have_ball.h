/*
    This file is part of SSL.

    Copyright 2019 Schmitz Etienne (hello@etienne-schmitz.com)

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

// REVIEW AB : Use absolute path
#include "../../robot_behavior.h"
#include "../../factory.h"

namespace RhobanSSL
{
namespace Robot_behavior {

// REVIEW AB : Add a namespace beginner

// REVIEW AB : Rename the class to Annotations_determine_which_robot_have_the_ball
class Begginer_robot_have_ball : public RobotBehavior  {
    private:
    RhobanSSLAnnotation::Annotations annotations;


  virtual void update(double time, const Ai::Robot& robot, const Ai::Ball& ball);

  virtual Control control() const;

  virtual RhobanSSLAnnotation::Annotations get_annotations() const;
  virtual ~Begginer_robot_have_ball();
};

};  // namespace Robot_behavior
};  // namespace RhobanSSL
