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

            Vector2d ball_robot_vector = ballPosition() - robot_position;
            double ball_distance_robot = ballPosition().getDist(robot_position);

            rhoban_geometry::Point robot_direction_ = ballPosition();
            double robot_rotation_ = detail::vec2angle(ball_robot_vector)+0.0;

            //printf("distance: %lf\n", ball_distance_robot);

            if(ball_distance_robot<0.102){ //0.101980
                chipkick_power = 1;
                ready_to_chipkick = true;
                robot_direction_ = robot_position;
            }else{
                robot_direction_ = ballPosition();
                ready_to_chipkick = false;
            }

            printf("\n---");
            DEBUG(robot.infraRed());
            DEBUG(robot.driverError());
            DEBUG(robot.isOk());
            printf("\n---\n");

            follower_->setFollowingPosition(robot_direction_, robot_rotation_);

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