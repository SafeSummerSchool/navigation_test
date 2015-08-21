#!/usr/bin/env python
# license removed for brevity
# ROS STUFF 
import rospy
import numpy as np
from std_msgs.msg import String
from sensor_msgs.msg import Imu
from geometry_msgs.msg import Vector3
from geometry_msgs.msg import Quaternion
from std_msgs.msg import Header
from std_msgs.msg import Float64MultiArray

# PLOTTING STUFF
from math import pi, sqrt, atan2
import matplotlib.pyplot as plt
from pylab import ion
#from imu_box3d import imu_visualize


#test = Float64MultiArray()
class KalmanFilterClass:
    def __init__(self):
        self.states = []
        rospy.init_node('kalman_filter', anonymous=True)
        #self.listningAngularData()
        self.pubImu = rospy.Publisher("/measHoughKalman",  Float64MultiArray, queue_size=10)
        # Plotting parameters

        self.showPlot = rospy.get_param('/kalman_filter_pkg/showMeasurements',False)
        self.show3DLiveView = False
        self.show3DLiveViewInterval = 3
        
        self.unwrapLast = 0.0;
        self.unwrapCurrent = 0.0;
        # Time variables
        self.ts_gyro_now = rospy.Time.now()
        self.ts_gyro_prev = rospy.Time.now()

        self.ts_hough_now = rospy.Time.now()
        self.ts_hough_prev = rospy.Time.now()

        # approx. bias values determined by averaging over static measurements
        self.bias_gyro_x = 0.0753 # [rad/measurement]
        self.bias_gyro_y = 0.0417 # [rad/measurement]
        self.bias_gyro_z = 0.0008 # [rad/measurement]cd


        self.bias_gyro_x = 0.0 # [rad/measurement]
        self.bias_gyro_y = 0.0 # [rad/measurement]
        self.bias_gyro_z = 0.0 # [rad/measurement]

        ##### Insert initialize code below ###################

        # variances
        self.gyroVar = 0.005
        #self.yawHoughVar = 0.01
        self.yawHoughVar = 0.01

        self.predVar = 1 # Don't known what the start value should be (is only a problem if a hough is received before a imu msg)

        # Kalman filter start guess
        self.estAngle = -pi/4.0      # PC: Why this random value? 
        self.estVar = 3.14           # PC: The uncertainty is set high, because we are unsure.
        #estVar = 0.0000000001

        # Kalman filter housekeeping variables
        self.gyroVarAcc = 0.0

        self.predAngle = 0.0
        ######################################################

        ## Variables for plotting ##
        self.plotDataGyro = []
        self.plotDataAcc = []
        self.plotDataKalman = []

        ## Initialize your variables here ##
        self.gyro_x_rel = 0.0
        self.gyro_y_rel = 0.0
        self.gyro_z_rel = 0.0

        self.yawHough = 0.0;
        # initialize variables
        self.countIMU = 0
        self.countHough = 0

        self.kalman_estimate = 0.0
        
        # initialize 3D liveview
        if self.show3DLiveView == True:
            self.imuview = imu_visualize()
            self.imuview.set_axis (0, 0, 0)
            self.imuview.update()

    def systemUpdate(self):
        print "test"

    def predictionUpdate(self):
        print "test"

    def callbackImu(self,data):
        msgImu = data
        
        
        # Getting and updating time information for the IMU
        ts_gyro_recv = msgImu.header.stamp
        if self.countIMU == 1: 
            self.ts_gyro_now = ts_gyro_recv # only the first time
        self.ts_gyro_prev = self.ts_gyro_now
        self.ts_gyro_now = ts_gyro_recv
        test = self.ts_gyro_now-self.ts_gyro_prev
        Ttmp = long(test.to_nsec())
        T = float(Ttmp)/1000000000.0  # Time between timestamps
        #rospy.loginfo(rospy.get_caller_id() + " Topic received")
        #rospy.loginfo(msgImu)
        



        #gyro_x = int(csv[5]) * 1/14.375 * pi/180.0;
        #gyro_y = int(csv[6]) * 1/14.375 * pi/180.0;
        #gyro_z = int(csv[7]) * 1/14.375 * pi/180.0;
        #rospy.loginfo("IMU: Time between messages: %f", T)
        gyro_x = msgImu.angular_velocity.x;
        gyro_y = msgImu.angular_velocity.y;
        gyro_z = msgImu.angular_velocity.z;
        #rospy.loginfo("IMU: gyro_x: %f, gyro_y: %f, gyro_z: %f", gyro_x, gyro_y, gyro_z)


        ### Kalman prediction #######################################
        # subtract defined static bias for each gyro        
        gyro_x -= self.bias_gyro_x
        gyro_y -= self.bias_gyro_y
        gyro_z -= self.bias_gyro_z
        #rospy.loginfo("IMU after bias: gyro_x: %f, gyro_y: %f, gyro_z: %f", gyro_x, gyro_y, gyro_z)


        
        
        # integrate gyro velocities to releative angles
        #rospy.loginfo("IMU gyro_relative (before): gyro_x_rel: %f, gyro_y_rel: %f, gyro_z_rel: %f", self.gyro_x_rel, self.gyro_y_rel, self.gyro_z_rel)
        #rospy.loginfo("IMU : gyro_z * T: %f, gyro_y * T: %f, gyro_z * T: %f", gyro_x * T, gyro_y * T, gyro_z * T)
        self.gyro_x_rel += gyro_x * T
        self.gyro_y_rel += gyro_y * T
        self.gyro_z_rel += gyro_z * T
        #rospy.loginfo("IMU gyro_relative (after): gyro_x_rel: %f, gyro_y_rel: %f, gyro_z_rel: %f", self.gyro_x_rel, self.gyro_y_rel, self.gyro_z_rel)
    
        # Kalman prediction/Motion/System step (we have new data in each iteration). 
        #rospy.loginfo("IMU: count %d Time between messages: %f, measurements, gyro_x: %f, gyro_y: %f, gyro_z: %f",self.countIMU, T,gyro_x,gyro_y,gyro_z)
        #rospy.loginfo("IMU (before): estAngle %f, estVariance: %f", self.estAngle, self.estVar)
        self.predAngle = self.estAngle + gyro_z * T # Predict angle (Used as a controll input)
        self.gyroVarAcc = self.gyroVarAcc + self.gyroVar 
        self.predVar = self.estVar + self.gyroVarAcc * T # Determine the increased variance.
        self.estAngle = self.predAngle
        self.estVar = self.predVar
        #rospy.loginfo("IMU (After): estAngle %f, estVariance: %f", self.estAngle, self.estVar)
        
        self.countIMU = self.countIMU+1

    def callbackHough(self, data):
        msgHough = data

        
        # Getting and updating time information for the IMU
        ts_hough_recv = rospy.Time.now()
        if self.countHough == 1: 
            self.ts_hough_now = ts_hough_recv # only the first time
        self.ts_hough_prev = self.ts_hough_now
        self.ts_hough_now = ts_hough_recv
        T = float((self.ts_hough_now-self.ts_hough_prev).to_nsec())/1000000000.0;
        

        self.yawHough = pi*msgHough.data[0]/180
        #self.yawHoughVar = msgHough.data[3]

        #rospy.loginfo(rospy.get_caller_id() + " Topic received")
        #rospy.loginfo(msgHough);

        ## Kalman Measurement/Correction #############################
        # calculate pitch (x-axis) and roll (y-axis) angles
        #pitch = atan2(acc_y, sqrt(acc_x**2 + acc_z**2))
        
        #roll = -1.0*atan2(acc_x, acc_z)
        
        #self.pitch = self.()
        #rospy.loginfo("Hou: Time between messages: %f, measurements: yaw: %f", T,self.yawHough)
        #rospy.loginfo("Hou (before): estAngle %f, estVariance: %f", self.estAngle, self.estVar)
        # Kalman Measurement/Correction step (we have new data in each iteration)
        K = self.predVar/(self.predVar+self.yawHoughVar)
        corrAngle = self.predAngle + K*(self.yawHough - self.predAngle) # The angle is estimated with improved accuracy.
        corrVar = self.predVar*(1 - K) # The variance is reduced (improved accuracy)
        self.estAngle = corrAngle
        self.estVar = corrVar
        self.gyroVarAcc = 0
        #rospy.loginfo("Hou (after): estAngle %f, estVariance: %f", self.estAngle, self.estVar)
        # define which value to plot as the Kalman filter estimate
        self.kalman_estimate = self.estAngle

        msgHough.data = [180*self.kalman_estimate/pi, msgHough.data[1], msgHough.data[2], msgHough.data[3]]
        self.pubImu.publish(msgHough)
        #print("Hough angle %f, kalman angle %f, HoughVariance %f", self.yawHough, self.kalman_estimate, self.yawHoughVar)
        #rospy.loginfo("Hough: %f Time between messages: %f",self.kalman_estimate, T)
        #self.showAngularDirection()
        self.storeMeasurements()
        ######################################################

    def SettingUpRos(self):
        # In ROS, nodes are uniquely named. If two nodes with the same
        # node are launched, the previous one is kicked off. The
        # anonymous=True flag means that rospy will choose a unique
        # name for our 'gettingImuData' node so that multiple listeners can
        # run simultaneously.
        
        rospy.Subscriber("/fmInformation/imu", Imu, self.callbackImu)
        rospy.Subscriber("/measHough", Float64MultiArray, self.callbackHough)

        

        # spin() simply keeps python from exiting until this node is stopped
        rospy.spin()

    # def UnwrapFunction(self):
    #     if self.countHough == 1:
    #         self.unwrapCurrent = self.yawHough
    #     self.unwrapLast = self.unwrapCurrent
    #     self.unwrapCurrent = self.yawHough

    #     output = np.unwrap([self.unwrapLast, self.unwrapCurrent],pi/2);
    #     print [self.unwrapLast, self.unwrapCurrent]
    #     print output
    #     return output[1]

    # def showAngularDirection(self):
    #     # if 3D liveview is enabled
    #     if self.show3DLiveView == True and self.countIMU % self.show3DLiveViewInterval == 0:
    #         # determine what variables to liveview
    #         roll_view = 0.0
    #         yaw_view = self.kalman_estimate
    #         pitch_view = 0.0

    #         self.imuview.set_axis (-pitch_view, -yaw_view, roll_view)
    #         self.imuview.update()

    def storeMeasurements(self):
        
        #rospy.loginfo("IMU: count %d, countdown %d",self.countIMU,self.countIMU%100)
        if self.showPlot == True:
            self.plotDataGyro.append(self.gyro_z_rel*180.0/pi)
            self.plotDataAcc.append(self.yawHough*180.0/pi)
            self.plotDataKalman.append(self.kalman_estimate*180.0/pi)
            # # show the plot

            if self.countIMU > 100:
                self.countIMU= 0
                ion()
                plt.figure(1)
                plt.title(' Gyro integrated (relative) angle')
                plt.plot(self.plotDataGyro)
                plt.savefig('imu_exercise_gyro.png')

                plt.figure(2)
                plt.title('Hough angle  (blue) & Kalman est. angle (red) angles')
                plt.plot(self.plotDataAcc,'blue')
                plt.plot(self.plotDataKalman,'red')
                plt.savefig('imu_exercise_acc_kalman.png')
                plt.draw()

        
            
        


if __name__ == '__main__':
    try:
        #listningAngularData()
        #rospy.loginfo("test")
        kalmanFilter = KalmanFilterClass()
        kalmanFilter.SettingUpRos()
        #kalmanFilter.systemUpdate()
    except rospy.ROSInterruptException:
        rospy.loginfo("Error")
        pass
