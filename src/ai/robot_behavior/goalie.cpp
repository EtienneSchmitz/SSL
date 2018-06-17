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

#include "goalie.h"
#include <math/tangents.h>
#include <math/vector2d.h>

namespace RhobanSSL {
namespace Robot_behavior {

rhoban_geometry::Point Goalie::calculate_goal_position(
    const rhoban_geometry::Point & ball_position,
    const Vector2d & poteau_droit,
    const Vector2d & poteau_gauche,
    double goalie_radius
){
        rhoban_geometry::Point defender_position = rhoban_geometry::center_of_cone_incircle(
        ball_position,
        vector2point(poteau_droit), 
        vector2point(poteau_gauche), 
        goalie_radius
    );
    return defender_position;
}


Goalie::Goalie(
    Ai::AiData & ai_data
):
	Goalie::Goalie(
		ai_data,
		Vector2d(-ai_data.field.fieldLength/2.0, ai_data.field.goalWidth/2.0),
		Vector2d(-ai_data.field.fieldLength/2.0, -ai_data.field.goalWidth/2.0),
        rhoban_geometry::Point(-ai_data.field.fieldLength/2.0, 0.0 ) + ai_data.constants.waiting_goal_position,
		ai_data.field.penaltyAreaDepth,
		ai_data.constants.robot_radius,
		ai_data.time, ai_data.dt
	)
{
}


Goalie::Goalie(
    Ai::AiData & ai_data,
    const Vector2d & left_post_position,
    const Vector2d & right_post_position,
    const rhoban_geometry::Point & waiting_goal_position,
    double penalty_rayon,
    double goalie_radius,
    double time, double dt
):
    RobotBehavior(ai_data),
    follower( Factory::fixed_consign_follower(ai_data) )
{
    this->left_post_position = left_post_position;
    this->right_post_position = right_post_position;
    this->waiting_goal_position = waiting_goal_position;
    this->goal_center = (left_post_position + right_post_position)/2.0;
    this->penalty_rayon = penalty_rayon;
    this->goalie_radius = goalie_radius;
}

void Goalie::update(
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

    rhoban_geometry::Point target_position = calculate_goal_position(
        ball_position(), right_post_position, left_post_position,
        goalie_radius
    );

    if( Vector2d(target_position - goal_center).norm() > penalty_rayon ){
        target_position = waiting_goal_position;
    }

    Vector2d target_ball_vector = ball_position() - target_position;

    double target_rotation = detail::vec2angle(target_ball_vector);

    follower->set_following_position(Vector2d(target_position), target_rotation );
    follower->avoid_the_ball(false);
    
    follower->update(time, robot, ball);   
}


Control Goalie::control() const {
    Control ctrl = follower->control();
    ctrl.chipKick = true;
    return follower->control();
}

Goalie::~Goalie(){
    delete follower;
}

RhobanSSLAnnotation::Annotations Goalie::get_annotations() const {
    return follower->get_annotations();
}

}
}
