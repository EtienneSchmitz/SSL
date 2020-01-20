#include "strategy.h"

#include "wall_strat_1_2.h"

#include <robot_behavior/wall_stop.h>
#include <robot_behavior/wall_stop_2.h>

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
        /*
        int MIN_ROBOT_ = 1;
        int MAX_ROBOT_ = 2;
        int nb_bot_;

        bool is_closest_0_;
        bool is_closest_1_;

        std::shared_ptr<robot_behavior::WallStop1> wall_bot_1_;
        std::shared_ptr<robot_behavior::WallStop2> wall_bot_2_;
        std::shared_ptr<robot_behavior::beginner::GotoBall> go_ball_;


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
            wall_bot_1_ = std::shared_ptr<robot_behavior::WallStop1>(new robot_behavior::WallStop1());
            wall_bot_2_ = std::shared_ptr<robot_behavior::WallStop2>(new robot_behavior::WallStop2());

            behaviors_are_assigned_ = false;
        }

        void WallStrat::stop(double time){
            DEBUG("STOP WALL 1 OU 2");
        }

        void WallStrat::set_near_bot(){
            nb_bot_ = getPlayerIds().size();
            fprintf(stdout,"Taille tableau Update %i\n",nb_bot_);
            int nearest_ally_robot_from_ball = GameInformations::getShirtNumberOfClosestRobotToTheBall(Ally);
            is_closest_0_ = false;
            is_closest_1_ = false;

            if(nearest_ally_robot_from_ball != -1) {
                if(nearest_ally_robot_from_ball == playerId(0)){
                    is_closest_0_ = true;
                }
                else{
                    if(nb_bot_ > 1 && nearest_ally_robot_from_ball == playerId(1)){
                        is_closest_1_ = true;
                    }
                }
            }
        }

        void WallStrat::update(double time){
            WallStrat::set_near_bot();

            if(is_closest_0_){
                assign_behavior(playerId(0), wall_bot_1_);
            }
            
            if(getPlayerIds().size() == 2 and is_closest_1_){
                assign_behavior(playerId(1), wall_bot_2_);
            }

        }

        void WallStrat::assignBehaviorToRobots(std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt){
            WallStrat::set_near_bot();
            
            if(!behaviors_are_assigned_){
                //Assigner les robots à des behavior par "défaut"
                //Vérifier avant le comportement complet des Behavior
                if(is_closest_0_){
                    assign_behavior(playerId(0), wall_bot_1_);
                }
                else{
                    assign_behavior(playerId(0), go_ball_);
                }
                if(getPlayerIds().size() == 2 and is_closest_1_){
                    assign_behavior(playerId(1), wall_bot_2_);
                }else if (getPlayerIds().size == 2))
                {
                    assign_behavior(playerId(1), go_ball_);
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