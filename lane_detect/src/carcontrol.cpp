#include "carcontrol.h"

CarControl::CarControl()
{
    carPos.x = 120;
    carPos.y = 300;
    steer_publisher = node_obj1.advertise<std_msgs::Float32>("Team1_steerAngle",10);
    speed_publisher = node_obj2.advertise<std_msgs::Float32>("Team1_speed",10);
}

CarControl::~CarControl() {}

float CarControl::errorAngle(const Point &dst)
{
    if (dst.x == carPos.x) return 0;
    if (dst.y == carPos.y) return (dst.x < carPos.x ? -90 : 90);
    double pi = acos(-1.0);
    double dx = dst.x - carPos.x;
    double dy = carPos.y - dst.y; 
    if (dx < 0) return -atan(-dx / dy) * 180 / pi;
    return atan(dx / dy) * 180 / pi;
}

void CarControl::driverCar(const vector<Point> &left, const vector<Point> &right, float velocity)
{
    std_msgs::Float32 angle;
    std_msgs::Float32 speed;

    speed.data = velocity;

    int i = left.size() - 11;
    float error = preError;
    // while (left[i] == DetectLane::null && right[i] == DetectLane::null) {
    //     i--;
    //     if (i < 0) return;
    // }
    // if (left[i] != DetectLane::null && right[i] !=  DetectLane::null)
    // {
    //     error = errorAngle((left[i] + right[i]) / 2);
    // } 
    // else if (left[i] != DetectLane::null)
    // {
    //     error = errorAngle(left[i] + Point(laneWidth / 2, 0));
    // }
    // else
    // {
    //     error = errorAngle(right[i] - Point(laneWidth / 2, 0));
    // }

    while (left[i] == DetectLane::null && right[i] == DetectLane::null) {
        i--;
        if (i < 0) return;
    }
    if (left[i] != DetectLane::null && right[i] !=  DetectLane::null)
    {
        if(sign_signal.sign == 0)
        {
            ROS_INFO("sign signal: %d",sign_signal.sign);
            //error = errorAngle(left[i]);
            error = errorAngle(left[i] + Point(laneWidth / 2, 0));
            speed.data = 30;
        }
        else if(sign_signal.sign == 1)
        {
            ROS_INFO("sign signal: %d",sign_signal.sign);
            //error = errorAngle(right[i]);
            error = errorAngle(right[i] - Point(laneWidth / 2, 0));
            speed.data = 30;      
        }
        else 
            error = errorAngle(right[i] - Point(laneWidth / 2, 0));
    } 
    else if (left[i] != DetectLane::null)
    {
        error = errorAngle(left[i] + Point(laneWidth / 2, 0));
    }
    else
    {
        error = errorAngle(right[i] - Point(laneWidth / 2, 0));
    }    

    angle.data = error;

    steer_publisher.publish(angle);
    speed_publisher.publish(speed);    
} 
