#include "chipkick.h"

#include <debug.h>

namespace rhoban_ssl
{
    namespace robot_behavior
    {
        Chipkick::Chipkick() : RobotBehavior(), follower_(Factory::fixedConsignFollower())
        {
        }

        void Chipkick::update(double time, const data::Robot& robot, const data::Ball& ball)
        {
            // At First, we update time and update potition from the abstract class robot_behavior.
            // DO NOT REMOVE THAT LINE
            RobotBehavior::updateTimeAndPosition(time, robot, ball);
            
            annotations_.clear();

            const rhoban_geometry::Point& robot_position = robot.getMovement().linearPosition(Data::get()->time.now());

            rhoban_geometry::Point target_direction_ = ballPosition();

            Vector2d direction = chipkick_direction_ - robot_position;
            ContinuousAngle target_rotation = vector2angle(direction);
            
            ContinuousAngle robot_rotation = robot.getMovement().angularPosition(Data::get()->time.now());
            ContinuousAngle diff_angle = target_rotation - robot_rotation;

            Vector2d ball_target_vector = robot_position - ballPosition();
            double ball_target_distance = ball_target_vector.norm();
            printf("distance: %lf\n", ball_target_distance);

            if(ball_target_distance<0.125){ //0.107214
                chipkick_power = 1;
                ready_to_chipkick = true;
                target_direction_ = robot_position;
                printf("kick actif\n");
            }else{
                target_direction_ = ballPosition();
                ready_to_chipkick = false;
            }

            follower_->setFollowingPosition(target_direction_, target_rotation);

            follower_->avoidTheBall(false);
            follower_->update(time, robot, ball);
        }

        Control Chipkick::control() const
        {
            Control ctrl = follower_->control();
            if (ready_to_chipkick)
            {
                ctrl.kick_power = chipkick_power;
                ctrl.charge = true;
                ctrl.chip_kick = true;
            }
            else{
                ctrl.charge = false;
                ctrl.chip_kick = false;
            }
            return ctrl;
        }

        void Chipkick::setChipkickDirection(rhoban_geometry::Point chipkick_direction)
        {
            chipkick_direction_ = chipkick_direction;
        }

        void Chipkick::setChipkickPower(double chipkick_power)
        {
            chipkick_power = MIN(MAX(0,chipkick_power),MIN(1, chipkick_power));
        }

        Chipkick::~Chipkick()
        {
            delete follower_;
        }

        rhoban_ssl::annotations::Annotations Chipkick::getAnnotations() const
        {
            rhoban_ssl::annotations::Annotations annotations;
            annotations.addAnnotations(this->annotations_);
            annotations.addAnnotations(follower_->getAnnotations());
            return annotations;
        }
    }
}