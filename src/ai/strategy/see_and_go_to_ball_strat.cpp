#include "strategy.h"
#include "see_and_go_to_ball_strat.h"

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
        /*
        Attributs prive:
            bool behaviors_are_assigned_;

            int NB_ROBOT = 1;

            std::shared_ptr<robot_behavior::beginner::GotoBall> bot_ball_;
        */

        SeeAndGoToBallStrat::SeeAndGoToBallStrat() : Strategy(){
        }
        
        SeeAndGoToBallStrat::~SeeAndGoToBallStrat(){
        }
        
        int SeeAndGoToBallStrat::minRobots() const{
            return NB_ROBOT;
        }

        int SeeAndGoToBallStrat::maxRobots() const{
            return NB_ROBOT;
        }

        GoalieNeed SeeAndGoToBallStrat::needsGoalie() const{
            return GoalieNeed::NO;
        }

        const std::string SeeAndGoToBallStrat::name = "Voir et Aller vers la Balle !";

        void SeeAndGoToBallStrat::start(double time){
            DEBUG("START SeeAndGoToBallStrat");
            bot_ball_ = std::shared_ptr<robot_behavior::beginner::GotoBall>(new robot_behavior::beginner::GotoBall());

            behaviors_are_assigned_ = false;
        }

        void SeeAndGoToBallStrat::stop(double time){
            DEBUG("STOP SeeAndGoToBallStrat");
        }

        void SeeAndGoToBallStrat::update(double time){
        }

        void SeeAndGoToBallStrat::assignBehaviorToRobots(std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt){
            assign_behavior(playerId(0), bot_ball_);

            behaviors_are_assigned_ = true;
        }

        //bool SeeAndGoToBallStrat::getStartingPositionForGoalie(rhoban_geometry::Point& linear_position, ContinuousAngle& angular_position);

        /*
        std::list<std::pair<rhoban_geometry::Point, ContinuousAngle> > SeeAndGoToBallStrat::getStartingPositions(int number_of_avalaible_robots){
            assert(minRobots() <= number_of_avalaible_robots);
            assert(maxRobots() == -1 or number_of_avalaible_robots <= maxRobots());

        }
        */

        rhoban_ssl::annotations::Annotations SeeAndGoToBallStrat::getAnnotations() const{
            rhoban_ssl::annotations::Annotations annotations;

            return annotations;
        }

        

    } // namespace strategy
} // namespace rhoban_ssl