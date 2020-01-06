#include <strategy/strategy.h>
#include <robot_behavior/go_to_xy.h>

namespace rhoban_ssl
{
    namespace strategy
    {
        class SeeAndGoToBallStrat : public Strategy
        {
            private:
                bool behaviors_are_assigned_;

                int NB_ROBOT = 1;

                // Position Balle
                rhoban_geometry::Point position_;

            public:
                SeeAndGoToBallStrat();
                //SeeAndGoToBallStrat(rhoban_geometry::Point position);
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