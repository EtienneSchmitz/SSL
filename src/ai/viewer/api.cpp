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

#include "api.h"
#include <rhoban_utils/timing/time_stamp.h>
#include <iostream>
#include "field_packet.pb.h"

namespace rhoban_ssl
{
namespace viewer
{
Api Api::api_singleton_;

Api::Api()
{
}

Api& Api::getApi()
{
  return Api::api_singleton_;
}

void Api::addPacket(AIPacket packet)
{
  packets_.push(packet);
}
std::queue<AIPacket>& Api::getQueue()
{
  return packets_;
}

void Api::updateField()
{
  AIPacket packet;
  FieldPacket field_packet;
  rhoban_ssl::vision::Field field = GlobalDataSingleThread::singleton_.vision_data_.field_;

  // Prepare packet
  packet.mutable_field()->set_fieldlength(field.fieldLength);
  packet.mutable_field()->set_fieldwidth(field.fieldWidth);
  packet.mutable_field()->set_boundarywidth(field.boundaryWidth);
  packet.mutable_field()->set_goaldepth(field.goalDepth);
  packet.mutable_field()->set_goalwidth(field.goalWidth);
  packet.mutable_field()->set_penaltyareadepth(field.penaltyAreaDepth);
  packet.mutable_field()->set_penaltyareawidth(field.penaltyAreaWidth);

  addPacket(packet);
  packet.release_field();
}

void Api::updateLocationPacket(ai::AiData& ai_data)
{
  AIPacket packet;
  rhoban_geometry::Point ball_position = ai_data.ball.getMovement().linearPosition(ai_data.time);
  packet.mutable_location()->mutable_ball()->set_x(ball_position.getX());
  packet.mutable_location()->mutable_ball()->set_y(ball_position.getY());

  // Robot Location
   rhoban_ssl::vision::Robot robots[2][ai::Config::NB_OF_ROBOTS_BY_TEAM] = GlobalDataSingleThread::singleton_.vision_data_.robots_;
   for (int team = 0; team < 2; team++)
   {
     for (int rid = 0; rid < ai::Config::NB_OF_ROBOTS_BY_TEAM; rid++) {
         RobotLocation* location_robot  = packet.mutable_location()->add_robot();

         rhoban_geometry::Point robot_position = robots[team][rid].movement.linearPosition();
         double robot_direction = robots[team][rid].movement.angularPosition().value();

         location_robot->set_x(robot_position.getX());
         location_robot->set_y(robot_position.getY());
         location_robot->set_isally(!team);
         location_robot->set_ispresent(robots[team][rid].isOk());
         location_robot->set_robot_id(robots[team][rid].id);
         location_robot->set_dir(robot_direction);
    }
    }

  addPacket(packet);
  packet.release_location();
}
}  // namespace viewer
}  // namespace rhoban_ssl
