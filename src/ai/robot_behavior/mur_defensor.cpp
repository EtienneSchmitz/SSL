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

#include "mur_defensor.h"
#include <math/tangents.h>
#include <math/vector2d.h>

namespace RhobanSSL {
namespace Robot_behavior {


Mur_defensor::Mur_defensor(
    Ai::AiData & ai_data
):
    RobotBehavior(ai_data),
    follower( Factory::fixed_consign_follower(ai_data) )
{
}

void Mur_defensor::update(
    double time,
    const Ai::Robot & robot,
    const Ai::Ball & ball
){
    // At First, we update time and update potition from the abstract class robot_behavior.
    // DO NOT REMOVE THAT LINE
    RobotBehavior::update_time_and_position( time, robot, ball );
    // Now 
    //  this->robot_linear_position
    //  this->robot_angular_position 
    // are all avalaible
    
    //int robot_id = 2;
    //const Robots_table & robot_table = ai_data.robots.at(Vision::Team::Ally);
    //const Ai::Robot & robot = robot_table.at(robot_id);
    
    const rhoban_geometry::Point & robot_position = robot.get_movement().linear_position( ai_data.time );
    
    rhoban_geometry::Point ally_goal_point = ally_goal_center();

    Vector2d ball_goal_vector = ally_goal_point - ball_position();
    Vector2d ball_robot_vector = robot_position - ball_position();

    ball_goal_vector = ball_goal_vector / ball_goal_vector.norm();
    ball_robot_vector = ball_robot_vector / ball_robot_vector.norm();


    double scalar_ball_robot = scalar_product( ball_robot_vector , ball_goal_vector );

    if ( scalar_ball_robot < 0 ) {
        follower->avoid_the_ball(true);
    } else {
        follower->avoid_the_ball(false);
    }

    double distance_defense_line = 1.2;

    double target_rotation = detail::vec2angle(-ball_goal_vector);
    rhoban_geometry::Point target_position;

    
    if (std::abs(target_rotation) > 0.7071) {
        target_position = ally_goal_point - ball_goal_vector * (std::abs(distance_defense_line / std::sin(target_rotation)) + ai_data.constants.robot_radius);
    } else {
        target_position = ally_goal_point - ball_goal_vector * (distance_defense_line / std::cos(target_rotation) + ai_data.constants.robot_radius);
    }

    follower->set_following_position(Vector2d(target_position), target_rotation);
    follower->update(time, robot, ball);
}

Control Mur_defensor::control() const {
    Control ctrl = follower->control();
    // ctrl.spin = true; // We active the dribler !
    ctrl.kick = false; 
    return ctrl; 
}

Mur_defensor::~Mur_defensor(){
    delete follower;
}

RhobanSSLAnnotation::Annotations Mur_defensor::get_annotations() const {
    return follower->get_annotations();
}

}
}