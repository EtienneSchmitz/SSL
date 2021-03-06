/*
    This file is part of SSL.

    Copyright 2018 Bezamat Jérémy (jeremy.bezamat@gmail.com)

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

#include "defensive.h"

namespace rhoban_ssl
{
namespace strategy
{
Defensive::Defensive()
  : Strategy()
  , degageur_(std::shared_ptr<robot_behavior::keeper::Clearer>(new robot_behavior::keeper::Clearer()))
  , obstructeur_(std::shared_ptr<robot_behavior::Obstructor>(new robot_behavior::Obstructor()))
{
}

Defensive::~Defensive()
{
}

/*
 * We define the minimal number of robot in the field.
 * The goalkeeper is not counted.
 */
int Defensive::minRobots() const
{
  return 1;
}

/*
 * We define the maximal number of robot in the field.
 * The goalkeeper is not counted.
 */
int Defensive::maxRobots() const
{
  return 1;
}

GoalieNeed Defensive::needsGoalie() const
{
  return GoalieNeed::NO;
}

const std::string Defensive::name = "defensive";

void Defensive::start(double time)
{
  DEBUG("START PREPARE KICKOFF");
  behaviors_are_assigned_ = false;
}
void Defensive::stop(double time)
{
  DEBUG("STOP PREPARE KICKOFF");
}

void Defensive::update(double time)
{
}

void Defensive::assignBehaviorToRobots(
    std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt)
{
  // we assign now all the other behavior
  assert(getPlayerIds().size() == 1);
  int robotID = playerId(0);  // we get the first if in get_player_ids()

  int nearest_ballID = getShirtNumberOfClosestRobotToTheBall(Ally);

  int id_to_obstruct = shirtNumberOfThreatMax(Opponent);
  obstructeur_->declareRobotToObstruct(id_to_obstruct, Opponent);
  if (nearest_ballID == robotID)
  {
    assign_behavior(robotID, degageur_);
  }
  else
  {
    assign_behavior(robotID, obstructeur_);
  }

  behaviors_are_assigned_ = true;
}

// We declare here the starting positions that are used to :
//   - place the robot during STOP referee state
//   - to compute the robot order of get_player_ids(),
//     we minimize the distance between
//     the startings points and all the robot position, just
//     before the start() or during the STOP referee state.
std::list<std::pair<rhoban_geometry::Point, ContinuousAngle> >
Defensive::getStartingPositions(int number_of_avalaible_robots)
{
  assert(minRobots() <= number_of_avalaible_robots);
  assert(maxRobots() == -1 or number_of_avalaible_robots <= maxRobots());

  return { std::pair<rhoban_geometry::Point, ContinuousAngle>(Data::get()->field.goalCenter(Ally), 0.0) };
}

//
// This function return false if you don't want to
// give a staring position. So the manager will chose
// a default position for you.
//
bool Defensive::getStartingPositionForGoalie(rhoban_geometry::Point& linear_position, ContinuousAngle& angular_position)
{
  linear_position = Data::get()->field.goalCenter(Ally);
  angular_position = ContinuousAngle(0.0);
  return true;
}

rhoban_ssl::annotations::Annotations Defensive::getAnnotations() const
{
  rhoban_ssl::annotations::Annotations annotations;

  // for (auto it = this->getPlayerIds().begin(); it != this->getPlayerIds().end(); it++)
  // {
  //   const rhoban_geometry::Point& robot_position = getRobot(*it).getMovement().linearPosition(time());
  //   // annotations.addText("Behaviour: " + this->name, robot_position.getX() + 0.15, robot_position.getY(), "white");
  //   annotations.addText("Strategy: " + this->name, robot_position.getX() + 0.15, robot_position.getY() + 0.30,
  //   "white");
  // }
  return annotations;
}

}  // namespace strategy
}  // namespace rhoban_ssl
