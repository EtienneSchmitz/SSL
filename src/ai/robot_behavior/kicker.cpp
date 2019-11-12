#include "kicker.h"

#include <debug.h>

namespace rhoban_ssl
{
    namespace robot_behavior
    {
        Kicker::Kicker() : RobotBehavior(), follower_(Factory::fixedConsignFollower())
        {
        }

        void Kicker::update(double time, const data::Robot& robot, const data::Ball& ball)
        {
            // At First, we update time and update potition from the abstract class robot_behavior.
            // DO NOT REMOVE THAT LINE
            RobotBehavior::updateTimeAndPosition(time, robot, ball);
            
            annotations_.clear();

            const rhoban_geometry::Point& robot_position = robot.getMovement().linearPosition(Data::get()->time.now());

            rhoban_geometry::Point target_direction_ = ballPosition();

            Vector2d direction = kick_direction_ - robot_position;
            ContinuousAngle target_rotation = vector2angle(direction);
            
            ContinuousAngle robot_rotation = robot.getMovement().angularPosition(Data::get()->time.now());
            ContinuousAngle diff_angle = target_rotation - robot_rotation;

            Vector2d ball_target_vector = robot_position - ballPosition();
            double ball_target_distance = ball_target_vector.norm();

            if(ball_target_distance<0,001){
                ready_to_kick_ = true;
                target_direction_ = robot_position;
                kick_power_ = 1;
            }else{
                ready_to_kick_ = false;
            }
            
            /*if(ball_target_distance<0,001){
                ready_to_kick_ = true;
                kick_power_ = 1;
                target_direction_ = robot_position;

            }else{
                ready_to_kick_ = false;
                target_direction_ = follower_->ballPosition();
            }
            */
            
            follower_->setFollowingPosition(target_direction_, target_rotation);

            follower_->avoidTheBall(false);
            follower_->update(time, robot, ball);
        }

        Control Kicker::control() const
        {
            Control ctrl = follower_->control();
            if(ready_to_kick_){
                ctrl.kick_power = kick_power_;
                ctrl.charge = true;
                ctrl.kick = true;
            }
            else
            {
                ctrl.charge = false;
                ctrl.kick = false;
            }
            return ctrl;
        }

        void Kicker::setKickDirection(rhoban_geometry::Point kick_direction)
        {
            kick_direction = kick_direction;
        }

        void Kicker::setKickPower(double kick_power)
        {
            kick_power_ = MIN(MAX(0,kick_power),MIN(1, kick_power));
        }

        Kicker::~Kicker()
        {
            delete follower_;
        }

        rhoban_ssl::annotations::Annotations Kicker::getAnnotations() const
        {
            rhoban_ssl::annotations::Annotations annotations;
            annotations.addAnnotations(this->annotations_);
            annotations.addAnnotations(follower_->getAnnotations());
            return annotations;
        }
    } // namespace robot_behavior
} // namespace rhoban_ssl