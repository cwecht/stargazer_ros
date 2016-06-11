//
// Created by bandera on 10.06.16.
//

#pragma once
#include <ceres/rotation.h>
#include <geometry_msgs/Pose.h>
#include <tf/tf.h>
#include "stargazer/StargazerImgTypes.h"
#include "stargazer/StargazerTypes.h"
#include "stargazer_ros_tool/Landmarks.h"

namespace stargazer_ros_tool {

inline stargazer::Landmark convert2Landmark(const stargazer_ros_tool::Landmark& lm_in) {

    stargazer::Landmark lm_out(lm_in.id);
    lm_out.points.clear();
    lm_out.points.reserve(lm_in.corner_points.size() + lm_in.id_points.size());

    for (auto& el : lm_in.corner_points) {
        stargazer::Point pt = {(double)el.u, (double)el.v, 0};
        lm_out.points.push_back(pt);
    }
    for (auto& el : lm_in.id_points) {
        stargazer::Point pt = {(double)el.u, (double)el.v, 0};
        lm_out.points.push_back(pt);
    }

    return lm_out;
};

inline stargazer::ImgLandmark convert2ImgLandmark(const stargazer_ros_tool::Landmark& lm_in) {

    stargazer::ImgLandmark lm_out;
    lm_out.nID = lm_in.id;

    lm_out.voCorners.reserve(lm_in.corner_points.size());
    for (auto& el : lm_in.corner_points) {
        cv::Point pt;
        pt.x = el.u;
        pt.y = el.v;
        lm_out.voCorners.push_back(pt);
    }

    lm_out.voIDPoints.reserve(lm_in.id_points.size());
    for (auto& el : lm_in.id_points) {
        cv::Point pt;
        pt.x = el.u;
        pt.y = el.v;
        lm_out.voIDPoints.push_back(pt);
    }

    return lm_out;
};

inline std::vector<stargazer::Landmark> convert2Landmarks(const stargazer_ros_tool::Landmarks& lms_in) {
    std::vector<stargazer::Landmark> lms_out;
    lms_out.reserve(lms_in.landmarks.size());

    for (auto& lm_in : lms_in.landmarks) {
        lms_out.push_back(convert2Landmark(lm_in));
    }

    return lms_out;
}

inline std::vector<stargazer::ImgLandmark> convert2ImgLandmarks(const stargazer_ros_tool::Landmarks& lms_in) {
    std::vector<stargazer::ImgLandmark> lms_out;
    lms_out.reserve(lms_in.landmarks.size());

    for (auto& lm_in : lms_in.landmarks) {
        lms_out.push_back(convert2ImgLandmark(lm_in));
    }

    return lms_out;
}

inline stargazer_ros_tool::Landmarks convert2LandmarkMsg(const std::vector<stargazer::ImgLandmark>& lm_in,
                                                         std_msgs::Header header = {}) {

    stargazer_ros_tool::Landmarks landmarksMessage;
    landmarksMessage.landmarks.reserve(lm_in.size());
    landmarksMessage.header = header;

    for (auto& lm : lm_in) {
        stargazer_ros_tool::Landmark landmark;
        landmark.header = header;
        landmark.id = lm.nID;

        for (auto& pt : lm.voCorners) {
            stargazer_ros_tool::LandmarkPoint lmpt;
            lmpt.u = static_cast<stargazer_ros_tool::LandmarkPoint::_u_type>(pt.x);
            lmpt.v = static_cast<stargazer_ros_tool::LandmarkPoint::_v_type>(pt.y);
            landmark.corner_points.push_back(lmpt);
        }

        for (auto& pt : lm.voIDPoints) {
            stargazer_ros_tool::LandmarkPoint lmpt;
            lmpt.u = static_cast<stargazer_ros_tool::LandmarkPoint::_u_type>(pt.x);
            lmpt.v = static_cast<stargazer_ros_tool::LandmarkPoint::_v_type>(pt.y);
            landmark.id_points.push_back(lmpt);
        }

        landmarksMessage.landmarks.push_back(landmark);
    }

    return landmarksMessage;
}

inline void pose2tf(const stargazer::pose_t pose_in, tf::StampedTransform& transform) {
    using namespace stargazer;
    transform.setOrigin(tf::Vector3(pose_in[(int)POSE::X], pose_in[(int)POSE::Y], pose_in[(int)POSE::Z]));
    double quaternion[4];
    double angleAxis[3];
    angleAxis[0] = pose_in[(int)POSE::Rx];
    angleAxis[1] = pose_in[(int)POSE::Ry];
    angleAxis[2] = pose_in[(int)POSE::Rz];
    ceres::AngleAxisToQuaternion(&angleAxis[0], &quaternion[0]);
    tf::Quaternion q;
    q.setW(quaternion[0]);
    q.setX(quaternion[1]);
    q.setY(quaternion[2]);
    q.setZ(quaternion[3]);
    transform.setRotation(q);
    return;
}

inline geometry_msgs::Pose pose2gmPose(const stargazer::pose_t& pose_in) {
    using namespace stargazer;
    geometry_msgs::Pose pose_out;
    pose_out.position.x = pose_in[(int)POSE::X];
    pose_out.position.y = pose_in[(int)POSE::Y];
    pose_out.position.z = pose_in[(int)POSE::Z];
    double quaternion[4];
    double angleAxis[3];
    angleAxis[0] = pose_in[(int)POSE::Rx];
    angleAxis[1] = pose_in[(int)POSE::Ry];
    angleAxis[2] = pose_in[(int)POSE::Rz];
    ceres::AngleAxisToQuaternion(&angleAxis[0], &quaternion[0]);
    pose_out.orientation.w = quaternion[0];
    pose_out.orientation.x = quaternion[1];
    pose_out.orientation.y = quaternion[2];
    pose_out.orientation.z = quaternion[3];
    return pose_out;
}

inline stargazer::pose_t gmPose2pose(const geometry_msgs::Pose& pose_in) {
    using namespace stargazer;
    stargazer::pose_t pose_out;
    pose_out[(int)POSE::X] = pose_in.position.x;
    pose_out[(int)POSE::Y] = pose_in.position.y;
    pose_out[(int)POSE::Z] = pose_in.position.z;
    double quaternion[4];
    double angleAxis[3];
    quaternion[0] = pose_in.orientation.w;
    quaternion[1] = pose_in.orientation.x;
    quaternion[2] = pose_in.orientation.y;
    quaternion[3] = pose_in.orientation.z;
    ceres::QuaternionToAngleAxis(&quaternion[0], &angleAxis[0]);
    pose_out[(int)POSE::Rx] = angleAxis[0];
    pose_out[(int)POSE::Ry] = angleAxis[1];
    pose_out[(int)POSE::Rz] = angleAxis[2];
    return pose_out;
}

} // namespace stargazer_ros_tool
