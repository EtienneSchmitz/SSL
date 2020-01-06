#include <strategy/strategy.h>
#include <robot_behavior/tutorials/beginner/goto_ball.h>

/*
Strategie ayant pour but:
    -Regarder la balle
    -Se diriger vers la balle
    ->goto_ball
*/

namespace rhoban_ssl
{
    namespace strategy
    {
        class SeeAndGoToBallStrat : public Strategy
        {
            private:
                bool behaviors_are_assigned_;

                int NB_ROBOT = 1;

                std::shared_ptr<robot_behavior::beginner::GotoBall> bot_ball_;

            public:
                SeeAndGoToBallStrat();
                virtual ~SeeAndGoToBallStrat();
                
                virtual int minRobots() const;
                virtual int maxRobots() const;
                virtual GoalieNeed needsGoalie() const;

                static const std::string name;

                virtual void start(double time);
                virtual void stop(double time);

                virtual void update(double time);

                virtual void assignBehaviorToRobots(std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt);

                virtual bool getStartingPositionForGoalie(rhoban_geometry::Point& linear_position, ContinuousAngle& angular_position);

                void setPosition(rhoban_geometry::Point position);

                virtual rhoban_ssl::annotations::Annotations getAnnotations() const;

            };
        };
    }; // namespace strategy
}; // namespace rhoban_ssl