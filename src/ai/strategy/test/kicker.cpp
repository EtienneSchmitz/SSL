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

#include "kicker.h"

namespace rhoban_ssl
{
namespace strategy
{
namespace test
{
const std::string Kicker::name = "Test Kicker";

Kicker::Kicker(ai::AiData &ai_data, rhoban_geometry::Point target, double power, double run_up, Vector2d line_imaginary): Strategy(ai_data),
    target_(target), power_(power), run_up_(run_up), line_imaginary_(line_imaginary)
{}

int Kicker::minRobots() const
{
  return 1;
}

int Kicker::maxRobots() const
{
  return 1;
}

GoalieNeed Kicker::needsGoalie() const
{
  return GoalieNeed::NO;
}

void Kicker::start(double time)
{
  DEBUG("START Test Kicker");
  behaviors_are_assigned_ = false;
}

void Kicker::stop(double time)
{
  DEBUG("STOP PREPARE KICKOFF");
}

void Kicker::update(double time)
{
  // TODO : Create the annotations.
}

void Kicker::assignBehaviorToRobots(
    std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt)
{
  if (not(behaviors_are_assigned_))
  {
    assert(getPlayerIds().size() == 1);
    int id = playerId(0);  // we get the first if in get_player_ids()
    assign_behavior(id,
    std::shared_ptr<robot_behavior::RobotBehavior>
                    (new robot_behavior::test::StrikerOnOrder(ai_data_,power_,run_up_, target_, true)));

    behaviors_are_assigned_ = true;
  }
}

std::list<std::pair<rhoban_geometry::Point, ContinuousAngle> >
Kicker::getStartingPositions(int number_of_avalaible_robots)
{
  assert(minRobots() <= number_of_avalaible_robots);
  assert(maxRobots() == -1 or number_of_avalaible_robots <= maxRobots());

  return { std::pair<rhoban_geometry::Point, ContinuousAngle>(ai_data_.relative2absolute(-1.0 / 3.0, 0.0), 0.0) };
}

bool Kicker::getStartingPositionForGoalie(rhoban_geometry::Point& linear_position, ContinuousAngle& angular_position)
{
  return false;
}

Kicker::~Kicker() {}

rhoban_ssl::annotations::Annotations Kicker::getAnnotations() const {
    return annotations_;
}

}  // namespace test
}  // namespace strategy
}  // namespace rhoban_ssl
