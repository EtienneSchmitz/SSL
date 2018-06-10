#ifndef __ROBOT_BEHAVIOR__PATROL__H__
#define __ROBOT_BEHAVIOR__PATROL__H__

#include "robot_behavior.h"
#include "factory.h"

namespace RhobanSSL
{
namespace Robot_behavior {

class Patrol : public RobotBehavior  {
    private:
    ConsignFollower* follower;
    int zone;
    bool _see_the_ball;
    std::vector< std::pair<rhoban_geometry::Point, ContinuousAngle> > traject;
    double waiting_time;
    double last_time;
    bool it_s_time_to_change_the_zone;

    public:
    Patrol(Ai::AiData& ai_data);

    void see_the_ball(bool value);

    virtual void update(
        double time,
        const Ai::Robot & robot,
        const Ai::Ball & ball
    );

    static Patrol* two_way_trip( Ai::AiData& ai_data );
    static Patrol* tour_of_the_field( Ai::AiData& ai_data );
    static Patrol* test_translation_for_pid( Ai::AiData& ai_data );	
    static Patrol* test_rotation_for_pid( Ai::AiData& ai_data );	

    void set_traject( const std::vector< std::pair<rhoban_geometry::Point, ContinuousAngle> > & traject );

    virtual Control control() const;
    void set_waiting_time( double time );
    
    RhobanSSLAnnotation::Annotations get_annotations() const;

    virtual ~Patrol();

};

};
}; //Namespace Rhoban

#endif
