/********************************************************************************
 * Copyright (C) 2017-2020 German Aerospace Center (DLR). 
 * Eclipse ADORe, Automated Driving Open Research https://eclipse.org/adore
 *
 * This program and the accompanying materials are made available under the 
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0 
 *
 * Contributors: 
 *   Reza Dariani   platoon view provider
 ********************************************************************************/

#include <adore_if_ros_scheduling/baseapp.h>
#include <adore_if_ros/factorycollection.h>

#include <adore_if_ros_msg/PointArray.h>
#include <adore/apps/trajectory_planner_base.h>
#include <adore/fun/basicunstructuredplanner.h>


namespace adore
{
  namespace if_ROS
  {  
    class UnstructuredPlanNode  : public FactoryCollection, public adore_if_ros_scheduling::Baseapp
    {
      public:
      typedef adore::fun::BasicUnstructuredPlanner TUnstructuredPlanner;
      TUnstructuredPlanner* app_;

      ros::Publisher occupancies_publisher_;
      UnstructuredPlanNode(){}
      void init(int argc, char **argv, double rate, std::string nodename)
      {
        Baseapp::init(argc, argv, rate, nodename);
        Baseapp::initSim();
        FactoryCollection::init(getRosNodeHandle());

        app_ = new TUnstructuredPlanner;
        
        bool use_scheduler = false;
        getRosNodeHandle()->getParam("/use_scheduler",use_scheduler);
        /*
        error: no matching function for call to ‘std::function<void()>::function(std::_Bind_helper<false, void (adore::apps::TrajectoryPlannerBase::*)(), adore::fun::BasicUnstructuredPlanner*&>::type)’
        if(use_scheduler)
        {
          //std::function<void()> run_fcn(std::bind(&adore::apps::TrajectoryPlannerBase::planning_request_handler,planner_));
          std::function<void()> run_fcn(std::bind(&adore::apps::TrajectoryPlannerBase::planning_request_handler,app_));
          Baseapp::addTimerCallback(run_fcn);
        }
        else
        {
          planner_->prime();//node is executed event-based on PlanningRequest, prime sets trigger
        }*/
        // timer callbacks
        std::function<void()> run_fcn(std::bind(&UnstructuredPlanNode::publish_occupancy_grid,this));
        Baseapp::addTimerCallback(run_fcn);
        occupancies_publisher_ = getRosNodeHandle()->advertise<adore_if_ros_msg::PointArray>("occupancies",1);
        //occupancies_publisher_ = getRosNodeHandle()->advertise<adore_if_ros_msg::PointArrayConstPtr>("occupancies",1);
        
      }
      void publish_occupancy_grid()
      {
        adore_if_ros_msg::PointArray msg;
        //msg.x = app_->getOccupancies_x();
        msg.x = *app_->getOccupancies_x();
        msg.y = *app_->getOccupancies_y();
        occupancies_publisher_.publish(msg);

      }
    };
  }
}

int main(int argc,char **argv)
{
    adore::if_ROS::UnstructuredPlanNode node;
    node.init(argc, argv, 10.0, "adore_unstructured_plan_node");
    node.run();
    return 0;
}