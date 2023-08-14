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
#include <adore/apps/platoon_view_provider.h>


namespace adore
{
  namespace if_ROS
  {  
    class UnstructuredPlanNode  : public FactoryCollection, public adore_if_ros_scheduling::Baseapp
    {
      public:
      adore::apps::BasicUnstructuredPlanner* app_;
      ros::Publisher occupancies_publisher_;
      UnstructuredPlanNode(){}
      void init(int argc, char **argv, double rate, std::string nodename)
      {
        Baseapp::init(argc, argv, rate, nodename);
        Baseapp::initSim();
        FactoryCollection::init(getRosNodeHandle());
        app_ = new adore::apps::BasicUnstructuredPlanner();

        // timer callbacks
        std::function<void()> run_fcn(std::bind(&adore::apps::PlotGraphSearch::run,app_));
        Baseapp::addTimerCallback(run_fcn);
        occupancies_publisher_ = getRosNodeHandle()->publish("occupancies",1,&PlotUnstructuredPlannerNode::send,this);
      }
      void send(adore_if_ros_msg::PointArrayConstPtr msg)
      {
        std::vector<int> x;
        std::vector<int> x;

        msg->x = app_->getOccupancies_x();
        msg->y = app_->getOccupancies_y();

      }
    };
  }
}

int main(int argc,char **argv)
{
    adore::if_ROS::UnstructuredPlanNode node;
    node.init(argc, argv, 20.0, "adore_unstructured_plan_node");
    node.run();
    return 0;
}