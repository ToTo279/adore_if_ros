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
 *   Reza Dariani- initial API and implementation
 ********************************************************************************/

#include <adore_if_ros_scheduling/baseapp.h>
#include <adore/apps/graph_search.h>
#include <adore_if_ros/factorycollection.h> 

namespace adore
{
    namespace if_ROS
    {
        class GraphSearchNode : public FactoryCollection,public adore_if_ros_scheduling::Baseapp
        {
        public:
            adore::apps::GraphSearch *gs_;
            GraphSearchNode() {}
            void init(int argc, char **argv, double rate, std::string nodename)
            {
                adore_if_ros_scheduling::Baseapp::init(argc, argv, rate, nodename);
                Baseapp::initSim();
                gs_ = new adore::apps::GraphSearch();

                // timer callbacks
                std::function<void()> run_fcn(std::bind(&adore::apps::TrajectoryPlannerBase::planning_request_handler, gs_));
                adore_if_ros_scheduling::Baseapp::addTimerCallback(run_fcn);
            }
        };
    } // namespace if_ROS
} // namespace adore

int main(int argc, char **argv)
{
    auto node = new adore::if_ROS::GraphSearchNode();
    node->init(argc, argv, 10.0, "GraphSearchNode");   
    node->run();
    return 0;
}