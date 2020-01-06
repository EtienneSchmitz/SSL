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

            Vector2d ball_robot_vector = normalized(ballPosition() - robot_position);
            double ball_distance_robot = ballPosition().getDist(robot_position);

            rhoban_geometry::Point robot_direction_ = ballPosition();
            double robot_rotation_ = detail::vec2angle(ball_robot_vector)+0.0;

            //printf("distance: %lf\n", ball_distance_robot);

            if(ball_distance_robot<0.102/*0.0000000001*//*0.102*/){ //0.101980
                kick_power_ = 1;
                ready_to_kick_ = true;
                robot_direction_ = robot_position;
            }else{
                robot_direction_ = ballPosition();
                ready_to_kick_ = false;
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

        Control Kicker::control() const
        {
            /*Control ctrl = follower_->control();

            ctrl.kick_power = kick_power_;
            ctrl.charge = true;
            ctrl.kick = true;
            return ctrl;
            */
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