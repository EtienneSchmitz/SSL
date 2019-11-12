#pragma once

#include <robot_behavior/robot_behavior.h>
#include <robot_behavior/factory.h>

namespace rhoban_ssl
{
    namespace robot_behavior
    {
        class Chipkick : public RobotBehavior
        {
            private:
                ConsignFollower * follower_;
                rhoban_ssl::annotations::Annotations annotations_;
                bool ready_to_chipkick = false;
                double chipkick_power = 1;
                rhoban_geometry::Point chipkick_direction_ = Data::get()->field.goalCenter(Opponent);

            public:
                Chipkick();

                virtual void update(double time, const data::Robot& robot, const data::Ball& ball);

                virtual Control control() const;

                virtual rhoban_ssl::annotations::Annotations getAnnotations() const;

                void setChipkickDirection(rhoban_geometry::Point chipkick_direction);

                void setChipkickAngle();

                void setChipkickPower(double chipkick_power);

                virtual ~Chipkick();

        };
    };  // namespace robot_behavior
};  // namespace rhoban_ssl