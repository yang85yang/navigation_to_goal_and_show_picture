#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include <sstream>
#include <std_msgs/Int32.h>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
int32_t voicenumber = 0;

void VoiceCallBack(const std_msgs::Int32::ConstPtr& goals)
{

    ROS_INFO("received data %d",goals->data);

//    if(voicenumber == goals->data)
//        return;

    MoveBaseClient ac("move_base",true);

    while(!ac.waitForServer(ros::Duration(10.0)))
    {
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    voicenumber = goals->data;

//    float goals_x[5] = {0.0,13.5590,12.45386,6.0,-0.2339};
//    float goals_y[5] = {0.0,14.9178,30.8866,30.8,9.09035};
    float goals_x[2] = {0.0,0.0};
    float goals_y[2] = {-0.6,0.6};
    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose.pose.position.x = goals_x[voicenumber];
    goal.target_pose.pose.position.y = goals_y[voicenumber];
    goal.target_pose.header.frame_id = "/map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.orientation.w = 1.0;
    ROS_INFO("Sending goal...");
    ac.sendGoal(goal);
    ac.waitForResult();
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
//        const char* filename="/home/gqy/Pictures/test1.jpg";
//        IplImage* image=cvLoadImage(filename);
//        cvNamedWindow(filename,CV_WINDOW_NORMAL);
//        cvShowImage(filename,image);
//        cvWaitKey(0);
//        cvReleaseImage(&image) ;
//        cvDestroyAllWindows();
    }
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"simple_navigation_goals_publisher");
    ros::NodeHandle n;
    ros::Publisher cancle_pub=n.advertise<actionlib_msgs::GoalID>("move_base/cancel",1);
    ros::Subscriber voice_sub=n.subscribe<std_msgs::Int32>("/voice_control",10,VoiceCallBack);

    ros::spin();
    return 0;

}
