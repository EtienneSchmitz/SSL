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
        int nb_bot_;

        bool is_closest_0_;
        bool is_closest_1_;

        std::shared_ptr<robot_behavior::WallStop1> wall_bot_1_;
        std::shared_ptr<robot_behavior::WallStop2> wall_bot_2_;

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
            nb_bot_ = getPlayerIds().size()
            wall_bot_1_ = std::shared_ptr<robot_behavior::WallStop1>(new robot_behavior::WallStop1());
            wall_bot_2_ = std::shared_ptr<robot_behavior::WallStop2>(new robot_behavior::WallStop2());

            behaviors_are_assigned_ = false;
        }

        void WallStrat::stop(double time){
            DEBUG("STOP WALL 1 OU 2");
        }

        void WallStrat::update(double time){
            int nearest_ally_robot_from_ball = GameInformations::getShirtNumberOfClosestRobotToTheBall(ally);
            is_closest_0_ = false;
            is_closest_1_ = false;

            if(nearest_ally_robot_from_ball == playerId(0)){
                is_closest_0_ = true;
            }
            else{
                if(nearest_ally_robot_from_ball == playerId(1) and nb_bot_ > 1){
                    is_closest_1_ = true;
                }
            }
        }

        void WallStrat::assignBehaviorToRobots(std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt){
            //Assignation en fonction du nombre de robot dispo
            if(!behaviors_are_assigned_){
                if(is_closest_0_){
                    assign_behavior(playerId(0), wall_bot_1_);
                }
                
                if(nb_bot_ == 2 and is_closest_1_){
                    assign_behavior(playerId(1), wall_bot_2_);
                }
                
                behaviors_are_assigned_ = true;
            }
        }

        rhoban_ssl::annotations::Annotations WallStrat::getAnnotations() const{
            rhoban_ssl::annotations::Annotations annotations;
            
            return annotations;
        }

    } // namespace strategy
} // namespace rhoban_ssl