#pragma once

#include <robot_behavior/robot_behavior.h>
#include <robot_behavior/factory.h>

namespace rhoban_ssl
{
    namespace robot_behavior
    {
        class Kicker : public RobotBehavior
        {
            private:
                ConsignFollower * follower_;
                rhoban_ssl::annotations::Annotations annotations_;
                bool ready_to_kick_ = false;
                double kick_power_ = 1;
                rhoban_geometry::Point kick_direction_ = Data::get()->field.goalCenter(Opponent);
                

            public:
                Kicker();

                virtual void update(double time, const data::Robot& robot, const data::Ball& ball);

                virtual Control control() const;

                virtual rhoban_ssl::annotations::Annotations getAnnotations() const;

                void setKickDirection(rhoban_geometry::Point kick_direction);

                void setKickPower(double kick_power);

                virtual ~Kicker();

        };
    };  // namespace robot_behavior
};  // namespace rhoban_ssl