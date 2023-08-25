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
*   Thomas Lobig
********************************************************************************/

#include <adore/apps/plot_occupancy_grid.h>
#include <plotlablib/figurestubfactory.h>
#include <adore_if_ros_scheduling/baseapp.h>
#include <adore_if_ros/factorycollection.h>
#include <string>
#include <adore_if_ros_msg/PointArray.h>

namespace adore
{
  namespace if_ROS
  {  
    class PlotOccupancyGridNode : public FactoryCollection, public adore_if_ros_scheduling::Baseapp
    {
      public:
      adore::apps::PlotOccupanyGrid* app_;
      ros::Subscriber occupancies_subscriber_; 
      PlotOccupancyGridNode(){}
      void init(int argc, char **argv, double rate, std::string nodename)
      {
        Baseapp::init(argc, argv, rate, nodename);
        Baseapp::initSim();
        FactoryCollection::init(getRosNodeHandle());
        DLR_TS::PlotLab::FigureStubFactory fig_factory;
        auto figure = fig_factory.createFigureStub(2);
        figure->show();
        int simulationID = 0;
        getParam("simulationID",simulationID);
        

        std::stringstream ss;
        ss<<"v"<<simulationID<<"/unstructured/";
        app_ = new adore::apps::PlotOccupanyGrid();
        /*
        app_ = new adore::apps::PlotOccupanyGrid(figure,
                                            ss.str());
        */
        std::function<void()> run_fcn(std::bind(&adore::apps::PlotOccupanyGrid::run,app_));
        Baseapp::addTimerCallback(run_fcn);
        occupancies_subscriber_ = getRosNodeHandle()->subscribe("occupancies",1,&PlotOccupancyGridNode::receive,this); 
      }

      void receive(adore_if_ros_msg::PointArrayConstPtr msg)
      {
        //TO DO: Plotting der occupenciey mit den Koordinaten msg->x & msg->y
        app_->occupancies_x.push_back(msg->x);
        app_->occupancies_y.push_back(msg->y);

      }

    };
  }
}

int main(int argc,char **argv)
{
    adore::if_ROS::PlotOccupancyGridNode appnode;    
    appnode.init(argc, argv, 10, "plot_occupancy_grid_node");
    appnode.run();
    return 0;
}
