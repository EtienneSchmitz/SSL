#include "strategy.h"

#include <robot_behavior/defender/defensive_wall.h>

#include <robot_behavior/tutorials/beginner/goto_ball.h>

/*
Strategie Wall
    -Mono   (wall_stop)
    -Duo    (wall_stop_2)
*/

namespace rhoban_ssl
{
    namespace strategy
    {
        class WallStrat : public Strategy
        {
            private:
                int MIN_ROBOT_ = 1;
                int MAX_ROBOT_ = 2;
                int nb_bot_ = 0;

                bool is_closest_0_;
                bool is_closest_1_;

                std::shared_ptr<robot_behavior::DefensiveWall> wall_bot_1_;
                std::shared_ptr<robot_behavior::DefensiveWall> wall_bot_2_;

                bool behaviors_are_assigned_;

            public:
                WallStrat();

                virtual ~WallStrat();

                virtual int minRobots() const;
                virtual int maxRobots() const;
                virtual GoalieNeed needsGoalie() const;

                static const std::string name;

                virtual void start(double time);
                virtual void stop(double time);

                virtual void update(double time);
                void set_near_bot();

                virtual void assignBehaviorToRobots(std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt);

                virtual rhoban_ssl::annotations::Annotations getAnnotations() const;

        };
    }; // namespace strategy
}; // namespace rhoban_ssl