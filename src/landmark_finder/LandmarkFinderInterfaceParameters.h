#pragma once

#include <string>
#include <ros/node_handle.h>

namespace stargazer_ros_tool {

struct LandmarkFinderInterfaceParameters {

    static LandmarkFinderInterfaceParameters& getInstance();

    void fromNodeHandle(const ros::NodeHandle&);

    std::string stargazer_config;
    std::string landmark_topic;
    std::string undistorted_image_topic;

    int threshold;
    int tight_filter_size;
    int wide_filter_size;
    double maxRadiusForPixelCluster;
    int minPixelForCluster;
    int maxPixelForCluster;
    double maxRadiusForCluster;
    int minPointsPerLandmark;
    int maxPointsPerLandmark;
    bool debug_mode;

private:
    LandmarkFinderInterfaceParameters();
};

} // namespace stargazer_ros_tool
