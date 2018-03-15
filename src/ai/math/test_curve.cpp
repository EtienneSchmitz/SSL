#include <debug.h>
#include "curve.h"

#include <iostream>
#include <Eigen/Dense>
#include <cmath>

bool eq( double u, double v, double err ){
    return std::abs(u-v) <= err;
}

double error_renormalisation(
    double t, const RenormalizedCurve & curve,
    std::function<double (double)> & error_curve,
    double dt
){
    return (
        error_curve(
            curve.inverse_of_arc_length( curve.position_consign(t) )
        ) +
        error_curve(
            curve.inverse_of_arc_length( curve.position_consign(t+dt) )
        )
    )/dt;
};

void test_curves(){
    {

        double length = 2.0;

        // length = \int_0^time_max 1 = time_max
        double time_max = length;
        double dt = time_max/100;
        double dt_micro = dt/100;
        double du = dt_micro/time_max;
        double erreur = dt/10;


        RenormalizedCurve curve(
            [&](double u){ return Eigen::Vector2d(length*u,0.0); },
            du,
            [&](double t){ return time_max/length; },
            dt_micro, dt_micro/100
        );
        for( double u=0.0; u<=1.0; u = u+ dt ){
            assert( curve.original_curve(u) == Eigen::Vector2d(2*u,0));
        }

        assert(
            eq( length, curve.size(),  erreur)
        );

        // As velocity is 1, then length is equal to time to walk on the 
        // complete curve.
        assert( eq(length, curve.max_time(), erreur) );
        // On this example the ration between time en distance is 2.
        assert( eq(length, curve.time(length-erreur), 2*erreur) );


        RenormalizedCurve::TimeCurve time_it = curve.time_iterator();
        for( double d=0.0; d<=length; d = d + dt ){
            assert( eq(d, time_it(d), erreur) );
        }

        RenormalizedCurve::PositionConsign position_it = curve.position_consign_iterator();
        for( double t=0.0; t<2*length; t = t + dt ){
            assert( eq(t, position_it(t), erreur) );
        }

        Curve2d::Length length_it = curve.length_iterator();
        for( double u=0.0; u<1.0; u = u + dt ){
            assert( eq(length*u, length_it(u), erreur) );
        }

        Curve2d::Inverse_of_length inverse_legnth_it = curve.Inverse_of_length_iterator();
        for( double l=0.0; l<=length; l = l + dt ){
            assert( eq(l/length, inverse_legnth_it(l), erreur) );
        }

        std::function<double(double)> error_curve = [&](double u){
            return 2*dt_micro;
        };
        RenormalizedCurve::CurveIterator curve_t_it = curve.curve_iterator();
        RenormalizedCurve::CurveIterator curve_t_dt_it = curve.curve_iterator();
        for( double t=0.0; t<curve.max_time()-dt; t = t + dt ){
            assert(
                eq(
                    1.0,
                    ( curve_t_dt_it(t+dt) - curve_t_it(t) ).norm()/dt,
                    error_renormalisation(t, curve, error_curve, dt) + dt_micro/10.0
                )
            );
        }

        assert(
            eq(
                (
                    curve(curve.max_time()+dt) -
                    curve( curve.max_time() )
                ).norm(),
                0,
                erreur
            ) 
        );
        assert( curve(curve.max_time()+dt) == curve( curve.max_time()+2*dt ) );
        assert( curve(curve.max_time()+3*dt) == curve( curve.max_time()+dt ) );
    }

    {
        // Length = \int_0^1 sqrt( 4*u**2 + 4 ) = 
        //        = 2 * ( 1/2*sqrt(x^2 + 1)*x + 1/2*arcsinh(x) )
        // length = sqrt(2) + arcsinh(1) - 0 - 0 = 2.2955871
        double length = std::sqrt(2) + std::asinh(1);
        // length = \int_0^time_max t+0.1
        // length = [ t^2/2 + 0.1 *t ]_0^time_max
        // 
        double time_max = -0.1 + sqrt( .1*.1 + 2*length );
        double dt = time_max/10;
        double dt_micro = dt/100;
        double du = dt_micro;//dt_micro/time_max;
        double erreur = dt/10;

        RenormalizedCurve curve(
            [](double u){ return Eigen::Vector2d(u*u,2*u); },
            dt_micro,
            [](double t){ return t+.1; },
            dt_micro, dt_micro/100
        );
        assert(
            eq(
                length, curve.size(),
                erreur
            )
        );
        assert(
            eq(
                time_max, curve.get_time_max(),
                erreur
            )
        );
        std::function<double(double)> error_curve = [&](double u){
            return (
                (
                    std::sqrt(u+du) + std::asinh(u+du) - std::sqrt(u) + std::asinh(u)
                ) - (
                    sqrt( std::pow(2*du + du*du, 2) + std::pow(2*du, 2) )
                )
            );
            //return 2*du*std::sqrt( std::pow( u+du/2.0, 2 ) + 1 );
        };
        RenormalizedCurve::CurveIterator curve_t_it = curve.curve_iterator();
        RenormalizedCurve::CurveIterator curve_t_dt_it = curve.curve_iterator();
        for( double t=0.0; t<curve.max_time()-dt; t = t + dt ){
            /* TODO change du to onther valuer */
            /*
            DEBUG("########################");
            DEBUG("size : " << curve.size() );
            DEBUG("length : " << length );
            DEBUG("time_max : " << time_max );
            DEBUG("time_max 1  : " << curve.get_time_max() );
            DEBUG("t+.1 : " << t+.1 );
            DEBUG( ( curve_t_dt_it(t+dt) - curve_t_it(t) ).norm()/dt );
            DEBUG(
                "error_ren : " <<  
                    error_renormalisation(t, curve, error_curve, dt) + du/10.0
            );
            */
            assert(
                eq(
                    t+.1,
                    ( curve_t_dt_it(t+dt) - curve_t_it(t) ).norm()/dt,
                    error_renormalisation(t, curve, error_curve, dt)//+ du/10.0
                )
            );
            //DEBUG("");
        }
        assert( curve(curve.max_time()+dt) == curve( curve.max_time() ) );
        assert( curve(curve.max_time()+3*dt) == curve( curve.max_time() ) );
    }
}

void test_velocityconsign(){
    {
        double erreur = 0.0000001;
        double distance = 4.0;
        double max_acceleration = 3.0;
        double max_velocity = 2.0;
        ContinuousVelocityConsign consign(distance, max_velocity, max_acceleration);
        assert( consign(-0.1) == 0.0 );
        assert( consign(0) == 0.0 );
        assert(
            eq(
                consign(consign.time_of_acceleration()/2.0),
                max_velocity/2.0, erreur
            )
        );
        assert(
            eq(
                consign(consign.time_of_acceleration()),
                max_velocity, erreur
            )
        );
        assert(
            eq(
                consign( consign.time_of_deplacement()
                - consign.time_of_acceleration()),
                max_velocity, erreur
            )
        );
        assert(
            eq(
                consign(
                    consign.time_of_deplacement() -
                    consign.time_of_acceleration()/2.0
                ),
                max_velocity/2.0, erreur
            )
        );
        assert( consign(consign.time_of_deplacement()) == 0.0 );
        assert( consign(consign.time_of_deplacement()+.01) == 0.0 );
        double len = 0.0;
        double step = .00001;
        for( double t = 0.0; t<consign.time_of_deplacement(); t+= step ){
            len += consign(t) * step;
        }
        assert( eq( distance, len, .0001 ) );
    }
}

void test_empty_curves(){
    {
        double dt = 0.01;
        double dt_micro = dt/100;

        Eigen::Vector2d position(3.0,7.0);
        RenormalizedCurve curve(
            [&](double u){ return position; },
            dt_micro,
            [](double t){ return 1.0; },
            dt_micro, dt_micro/100
        );

        assert( 0.0 == curve.size() );
        assert( 0.0 == curve.max_time() );
        assert( position == curve(0.0) );
        assert( position == curve(0.4) );
    }
}

struct Rotation {
    double orientation;

    double operator()(double u) const {
        return  90.0*u + orientation;
    };
};

struct Translation {
    Eigen::Vector2d position;

    Eigen::Vector2d operator()(double u) const {
        return  position + Eigen::Vector2d(u, u*u);
    };

    Translation():position(0.0,0.0){};
};


struct fct_wrapper {
    std::function<double (double u)> rotation;

    fct_wrapper(
        const std::function<double (double u)> & rotation
    ):rotation(rotation){ };

    Eigen::Vector2d operator()(double t){
        return Eigen::Vector2d( rotation(t), 0.0 );
    };
};

void test_use_cases(){
    {
        Rotation rot;
        rot.orientation = 90.0;
        fct_wrapper fct(rot);
        Curve2d curve(fct, 0.0001);
        assert( curve.size() == 90 );
    }

    {
        double dt = 0.01;
        double dt_micro = dt/100;

        RenormalizedCurve curve(
            [](double u){ return Eigen::Vector2d(90.0*u,0.0); },
            dt_micro,
            [](double t){ return 10.0; },
            dt_micro, dt_micro/100
        );
        assert( eq( curve.max_time(), 9, 0.01 ) );
    }
    {
        double dt = 0.01;
        double dt_micro = dt/100;

        Curve2d crv(
            [](double u){ return Eigen::Vector2d(90.0*u,0.0); },
            dt_micro
        );
        assert( crv.size() == 90 );
        double max_acceleration = 300.0;
        double max_velocity = 90.0;
        ContinuousVelocityConsign consign(crv.size(), max_velocity, max_acceleration);

        RenormalizedCurve curve(
            crv,
            consign,
            dt_micro, dt_micro/100
        );
    }
    {
        double dt = 0.001;
        double dt_micro = 0.0001;

        Translation trans;
        Curve2d crv( trans, dt_micro );
        double max_acceleration = 20.0;
        double max_velocity = 1.0;
        ContinuousVelocityConsign consign(crv.size(), max_velocity, max_acceleration);
        RenormalizedCurve curve(
            crv,
            consign,
            dt_micro, dt_micro/100
        );

        RenormalizedCurve::CurveIterator curve_t_it = curve.curve_iterator();
        RenormalizedCurve::CurveIterator curve_t_dt_it = curve.curve_iterator();
        for( double t = 0; t< curve.max_time()-dt; t+= dt ){
            Eigen::Vector2d xt = curve_t_it(t);
            Eigen::Vector2d xdt = curve_t_dt_it(t+dt);
            assert( xdt[0] >= xt[0] );
            assert( xdt[1] >= xt[1] );
        }

    }
}

int main(){
    test_use_cases();
    test_empty_curves();
    test_curves();
    test_velocityconsign();
}