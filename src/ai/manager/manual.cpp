  /*
      This file is part of SSL.

      Copyright 2019 Schmitz Etienne (hello@etienne-schmitz.com)

      SSL is free software: you can redistribute it and/or modify
      it under the terms of the GNU Lesser General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      SSL is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU Lesser General Public License for more details.

      You should have received a copy of the GNU Lesser General Public License
      along with SSL.  If not, see <http://www.gnu.org/licenses/>.
  */
  #include <manager/manual.h>

  #include <strategy/from_robot_behavior.h>
  #include <robot_behavior/position_follower.h>
  #include <robot_behavior/kick_to_xy.h>
  #include <robot_behavior/tutorials/beginner/goto_ball.h>
  #include <robot_behavior/tutorials/beginner/go_corner.h>
  #include <robot_behavior/tutorials/beginner/goalie.h>
  #include <robot_behavior/tutorials/beginner/goto_ball.h>
  #include <robot_behavior/tutorials/beginner/robot_have_ball.h>
  #include <robot_behavior/tutorials/beginner/robot_near_ball.h>
  #include <robot_behavior/tutorials/medium/defender.h>
  #include <robot_behavior/tutorials/beginner/see_ball.h>
  #include <robot_behavior/tutorials/beginner/see_robot.h>
  #include <robot_behavior/tutorials/beginner/robot_near_ball.h>
  #include <robot_behavior/tutorials/beginner/robot_have_ball.h>
  #include <robot_behavior/tutorials/beginner/annotations_ball_position.h>
  #include <robot_behavior/tests/kick_measure.h>
  #include <robot_behavior/tutorials/medium/follow_robot.h>
  #include <robot_behavior/go_to_xy.h>
  #include <robot_behavior/translation_test.h>

  #include <strategy/tutorials/caterpillar.h>
  #include <robot_behavior/ben_stealer.h>
  #include <robot_behavior/keeper/clearer.h>
  #include <robot_behavior/keeper/keeper.h>
  #include <robot_behavior/defender/defensive_wall.h>
  #include <robot_behavior/defender/kick_wall.h>
  #include <robot_behavior/defender/obstructor.h>
  #include <strategy/keeper/keeper_strat.h>

  #include <strategy/wall.h>
  #include <strategy/wall_2.h>
  #include <robot_behavior/tests/test_infra.h>
  #include <robot_behavior/tests/test_kicker.h>
  #include <robot_behavior/tests/test_relative_velocity_consign.h>
  #include <robot_behavior/tests/test_follow_path.h>
  #include <robot_behavior/tests/test_field_info.h>

  namespace rhoban_ssl
  {
  namespace manager
  {
  Manual::Manual(std::string name) : Manager(name)
  {
    registerStrategy("Beginner go to ball",
                     std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                         [&](double time, double dt) {
                           robot_behavior::beginner::GotoBall* beginner_goto_ball =
                               new robot_behavior::beginner::GotoBall();
                           return std::shared_ptr<robot_behavior::RobotBehavior>(beginner_goto_ball);
                         },
                         false  // we don't want to define a goal here !
                         )));
    registerStrategy("Beginner - Goalie", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                              [&](double time, double dt) {
                                                robot_behavior::beginner::Goalie* goalie =
                                                    new robot_behavior::beginner::Goalie();
                                                return std::shared_ptr<robot_behavior::RobotBehavior>(goalie);
                                              },
                                              false  // we don't want to define a goal here !
                                              )));
    registerStrategy("Medium - Defender", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                              [&](double time, double dt) {
                                                robot_behavior::medium::Defender* defender =
                                                    new robot_behavior::medium::Defender();
                                                return std::shared_ptr<robot_behavior::RobotBehavior>(defender);
                                              },
                                              false  // we don't want to define a goal here !
                                              )));
    registerStrategy("Beginner Annotations - Ball position",
                     std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                         [&](double time, double dt) {
                           robot_behavior::BeginnerAnnotationsBallPosition* ball_position =
                               new robot_behavior::BeginnerAnnotationsBallPosition();
                           return std::shared_ptr<robot_behavior::RobotBehavior>(ball_position);
                         },
                         false  // we don't want to define a goal here !
                         )));
    registerStrategy("Beginner - See ball", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                [&](double time, double dt) {
                                                  robot_behavior::beginner::SeeBall* see_ball =
                                                      new robot_behavior::beginner::SeeBall();
                                                  return std::shared_ptr<robot_behavior::RobotBehavior>(see_ball);
                                                },
                                                false  // we don't want to define a goal here !
                                                )));
    registerStrategy("Beginner - See Robot 3", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                   [&](double time, double dt) {
                                                     robot_behavior::beginner::SeeRobot* see_robot =
                                                         new robot_behavior::beginner::SeeRobot();
                                                     see_robot->setRobotIdToSee(3);
                                                     return std::shared_ptr<robot_behavior::RobotBehavior>(see_robot);
                                                   },
                                                   false  // we don't want to define a goal here !
                                                   )));
    registerStrategy("Test - KickMeasure", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                               [&](double time, double dt) {
                                                 robot_behavior::test::KickMeasure* kick_m =
                                                     new robot_behavior::test::KickMeasure(1.0);
                                                 return std::shared_ptr<robot_behavior::RobotBehavior>(kick_m);
                                               },
                                               false  // we don't want to define a goal here !
                                               )));
    registerStrategy("Beginner - Robot near ball", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                       [&](double time, double dt) {
                                                         robot_behavior::BeginnerRobotNearBall* near_ball =
                                                             new robot_behavior::BeginnerRobotNearBall();
                                                         return std::shared_ptr<robot_behavior::RobotBehavior>(near_ball);
                                                       },
                                                       false  // we don't want to define a goal here !
                                                       )));
    registerStrategy("Beginner - Robot have ball", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                       [&](double time, double dt) {
                                                         robot_behavior::BeginnerRobotHaveBall* have_ball =
                                                             new robot_behavior::BeginnerRobotHaveBall();
                                                         return std::shared_ptr<robot_behavior::RobotBehavior>(have_ball);
                                                       },
                                                       false  // we don't want to define a goal here !
                                                       )));
    registerStrategy("Kick to a point", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                            [&](double time, double dt) {
                                              robot_behavior::KickToXY* kick = new robot_behavior::KickToXY();
                                              return std::shared_ptr<robot_behavior::RobotBehavior>(kick);
                                            },
                                            false  // we don't want to define a goal here !
                                            )));
    registerStrategy("Medium - Follow robot 0", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                    [&](double time, double dt) {
                                                      robot_behavior::medium::FollowRobot* follower =
                                                          new robot_behavior::medium::FollowRobot(0);
                                                      return std::shared_ptr<robot_behavior::RobotBehavior>(follower);
                                                    },
                                                    false  // we don't want to define a goal here !
                                                    )));
    registerStrategy("Beginner - Go to XY", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                [&](double time, double dt) {
                                                  robot_behavior::GoToXY* go = new robot_behavior::GoToXY();
                                                  go->setPoint(rhoban_geometry::Point(1, 2));
                                                  return std::shared_ptr<robot_behavior::RobotBehavior>(go);
                                                },
                                                false  // we don't want to define a goal here !
                                                )));
    registerStrategy("Beginner - TranslationTest", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                [&](double time, double dt) {
                                                  robot_behavior::TranslationTest* go = new robot_behavior::TranslationTest();
                                                  go->setPoint(rhoban_geometry::Point(-2, 0));
                                                  return std::shared_ptr<robot_behavior::RobotBehavior>(go);
                                                },
                                                false  // we don't want to define a goal here !
                                                )));

    registerStrategy("Tutorial - Caterpillar",
                     std::shared_ptr<strategy::Strategy>(new strategy::Caterpillar(std::vector<rhoban_geometry::Point>{
                         rhoban_geometry::Point(-3, 3), rhoban_geometry::Point(3, 3), rhoban_geometry::Point(-3, -3),
                         rhoban_geometry::Point(3, -3) })));
    registerStrategy("Stealer", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                    [&](double time, double dt) {
                                      robot_behavior::BenStealer* stealer = new robot_behavior::BenStealer();
                                      stealer->setRobotIdToSteal(3);
                                      return std::shared_ptr<robot_behavior::RobotBehavior>(stealer);
                                    },
                                    false  // we don't want to define a goal here !
                                    )));
    registerStrategy("Keeper - Clearer", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                             [&](double time, double dt) {
                                               robot_behavior::keeper::Clearer* clearer =
                                                   new robot_behavior::keeper::Clearer();
                                               return std::shared_ptr<robot_behavior::RobotBehavior>(clearer);
                                             },
                                             false  // we don't want to define a goal here !
                                             )));
    registerStrategy("Keeper", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                   [&](double time, double dt) {
                                     robot_behavior::Keeper* keeper = new robot_behavior::Keeper();
                                     return std::shared_ptr<robot_behavior::RobotBehavior>(keeper);
                                   },
                                   false  // we don't want to define a goal here !
                                   )));
    registerStrategy("Defensive Wall", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                           [&](double time, double dt) {
                                             robot_behavior::DefensiveWall* defW = new robot_behavior::DefensiveWall();
                                             return std::shared_ptr<robot_behavior::RobotBehavior>(defW);
                                           },
                                           false  // we don't want to define a goal here !
                                           )));
    registerStrategy("Kick Wall", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                      [&](double time, double dt) {
                                        robot_behavior::KickWall* kickW = new robot_behavior::KickWall();
                                        return std::shared_ptr<robot_behavior::RobotBehavior>(kickW);
                                      },
                                      false  // we don't want to define a goal here !
                                      )));

    registerStrategy("Wall1", std::shared_ptr<strategy::Strategy>(new strategy::Wall()));
    registerStrategy("Wall2", std::shared_ptr<strategy::Strategy>(new strategy::Wall_2()));
    registerStrategy("Obstructor", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                       [&](double time, double dt) {
                                         robot_behavior::Obstructor* obstructor = new robot_behavior::Obstructor();
                                         obstructor->declareRobotToObstruct(3);
                                         return std::shared_ptr<robot_behavior::RobotBehavior>(obstructor);
                                       },
                                       false  // we don't want to define a goal here !
                                       )));
    registerStrategy("Keeper Strat (need goalie)", std::shared_ptr<strategy::Strategy>(new strategy::KeeperStrat()));
    registerStrategy("Test - IR", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                      [&](double time, double dt) {
                                        robot_behavior::tests::TestInfra* test_ir =
                                            new robot_behavior::tests::TestInfra();
                                        return std::shared_ptr<robot_behavior::RobotBehavior>(test_ir);
                                      },
                                      false  // we don't want to define a goal here !
                                      )));
    registerStrategy("Test - Kicker", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                          [&](double time, double dt) {
                                            robot_behavior::tests::TestKicker* test_kick =
                                                new robot_behavior::tests::TestKicker();
                                            return std::shared_ptr<robot_behavior::RobotBehavior>(test_kick);
                                          },
                                          false  // we don't want to define a goal here !
                                          )));
    registerStrategy("Test - Relative velocity consign",
                     std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                         [&](double time, double dt) {
                           robot_behavior::tests::TestRelativeVelocityConsign* test_rvc =
                               new robot_behavior::tests::TestRelativeVelocityConsign();
                           test_rvc->setAngularVelocity(1);  // tourbilol
                           test_rvc->setLinearVelocity(Vector2d(0.8, 0));
                           return std::shared_ptr<robot_behavior::RobotBehavior>(test_rvc);
                         },
                         false  // we don't want to define a goal here !
                         )));
    registerStrategy("Test - Velocity consign", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                    [&](double time, double dt) {
                                                      robot_behavior::tests::TestRelativeVelocityConsign* test_vc =
                                                          new robot_behavior::tests::TestRelativeVelocityConsign();
                                                      test_vc->setAngularVelocity(1);
                                                      test_vc->setLinearVelocity(Vector2d(0.8, 0));
                                                      return std::shared_ptr<robot_behavior::RobotBehavior>(test_vc);
                                                    },
                                                    false  // we don't want to define a goal here !
                                                    )));
    registerStrategy(
        "Test - Follow Path",
        std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
            [&](double time, double dt) {
              robot_behavior::tests::TestFollowPath* test_fp = new robot_behavior::tests::TestFollowPath(

                  /* std::vector<rhoban_geometry::Point>{
                       // path under all 4 cams
                       rhoban_geometry::Point(-3, 2.75), rhoban_geometry::Point(-3, -2.75),
                       rhoban_geometry::Point(3, -2.75), rhoban_geometry::Point(3, 2.75) }*/
                  /*std::vector<rhoban_geometry::Point>{ // path under left cams
                      rhoban_geometry::Point(-3,
                     2.75), rhoban_geometry::Point(-3, -2.75) }*/
                  /* std::vector<rhoban_geometry::Point>{ // path under right cams
                                                        rhoban_geometry::Point(3, 2.75),
                                                        rhoban_geometry::Point(3, -2.75) });*/
                  /*std::vector<rhoban_geometry::Point>{ // path under bottom cams
                                                        rhoban_geometry::Point(-3, -2.75),
                                                        rhoban_geometry::Point(3, -2.75) });*/
                  /*std::vector<rhoban_geometry::Point>{ // path under top cams
                                                       rhoban_geometry::Point(-3, 2.75),
                                                       rhoban_geometry::Point(3, 2.75) });*/
                  /*std::vector<rhoban_geometry::Point>{ // path trouh the center *2
                                                       rhoban_geometry::Point(1, 1), rhoban_geometry::Point(-1, -1),
                                                       rhoban_geometry::Point(-1, 1), rhoban_geometry::Point(1, -1) });*/
                  std::vector<rhoban_geometry::Point>{ // path under left bottom cam
                                                       rhoban_geometry::Point(-3, -2.75),
                                                       rhoban_geometry::Point(-3.5, -2.75) });

              return std::shared_ptr<robot_behavior::RobotBehavior>(test_fp);
            },
            false  // we don't want to define a goal here !
            )));

    registerStrategy("Test - Info Field", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                              [&](double time, double dt) {
                                                robot_behavior::tests::TestFieldInfo* field_info =
                                                    new robot_behavior::tests::TestFieldInfo();
                                                return std::shared_ptr<robot_behavior::RobotBehavior>(field_info);
                                              },
                                              false  // we don't want to define a goal here !
                                              )));
  }

  void Manual::update()
  {
    updateCurrentStrategies();
  }

  Json::Value Manual::getProperties()
  {
    Json::Value properties;
    properties_factory.addSetValue("name_test", "");

    properties = properties_factory.getJson();

    properties_factory.clear();
    return properties;
  }

  void Manual::setProperties(Json::Value json)
  {
    DEBUG(json);
  }

  Manual::~Manual()
  {
  }
  }  // namespace manager
  }  // namespace rhoban_ssl
