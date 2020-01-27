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
            const rhoban_geometry::Point& robot_position = robot.getMovement().linearPosition(time);
            ContinuousAngle robot_rotation = robot.getMovement().angularPosition(time);

            Vector2d ball_target_vector = chipkick_direction_ - ballPosition();
            double ball_target_distance = ball_target_vector.norm();

            Vector2d ball_velocity = ball.getMovement().linearVelocity(time);
            if (ball_target_distance > ZONE_RADIUS && ball_velocity.getX() < 0.15 && ball_velocity.getY() < 0.15)
            {
                Vector2d ball_robot_vector = robot_position - ballPosition();
                ball_target_vector = normalized(ball_target_vector);
                ball_robot_vector = normalized(ball_robot_vector);

                double target_radius_from_ball;
                double scalar_ball_robot = -scalarProduct(ball_robot_vector, ball_target_vector);

                follower_->avoidTheBall(scalar_ball_robot < 0);
                target_radius_from_ball = 1.5;

                if(scalar_ball_robot >= 0){
                    target_radius_from_ball = 1 / (2 * (scalar_ball_robot - 1.2)) + 2;
                }

                robot_position = ballPosition() - ball_target_vector * target_radius_from_ball;
                robot_rotation = detail::vec2angle(ball_target_vector);
            }
            /*
            Vector2d ball_robot_vector = ballPosition() - robot_position;
            double ball_distance_robot = ballPosition().getDist(robot_position);

            rhoban_geometry::Point robot_direction = ballPosition();
            double robot_rotation_ = detail::vec2angle(ball_robot_vector)+0.0;

            //printf("distance: %lf\n", ball_distance_robot);
            */

            if(ball_distance_robot<0.102){ //0.101980
                chipkick_power = 0.3;
                ready_to_chipkick = true;
                robot_direction = robot_position;
                printf('Ball Proche, préparation Tire !');
            }else{
                robot_direction = ballPosition();
                ready_to_chipkick = false;
            }

            follower_->setFollowingPosition(robot_direction, robot_rotation);

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