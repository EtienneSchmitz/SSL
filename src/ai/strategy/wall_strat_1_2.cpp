#include "strategy.h"

#include "wall_strat_1_2.h"

#include <robot_behavior/wall_stop.h>
#include <robot_behavior/wall_stop_2.h>

/*
Strategie Wall
    -Mono   (wall_stop)
    -Duo    (wall_stop_2)
*/

namespace rhoban_ssl
{
    namespace strategy
    {
        /*
        int MIN_ROBOT_ = 1;
        int MAX_ROBOT_ = 2;

        bool behaviors_are_assigned_;
        */

        WallStrat::WallStrat(){

        }

        WallStrat::~WallStrat(){

        }

        int WallStrat::minRobots() const{
            return MIN_ROBOT_;
        }

        int WallStrat::maxRobots() const{
            return MAX_ROBOT_;
        }

        GoalieNeed WallStrat::needsGoalie() const{
            return GoalieNeed::NO;
        }

        const std::string WallStrat::name = "Wall 1 OU 2";

        void WallStrat::start(double time){
            DEBUG("START WALL 1 OU 2");



            behaviors_are_assigned_ = false;
        }

        void WallStrat::stop(double time){
            DEBUG("STOP WALL 1 OU 2");
        }

        void WallStrat::update(double time){

        }

        void WallStrat::assignBehaviorToRobots(std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt);

        rhoban_ssl::annotations::Annotations WallStrat::getAnnotations() const;

    } // namespace strategy
} // namespace rhoban_ssl