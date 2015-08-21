#!/usr/bin/env python

import rospy
import time
from collections import namedtuple
from std_msgs.msg import Float64MultiArray
from std_msgs.msg import UInt16
from sound_play.libsoundplay import SoundClient

distanceBoundary1 = 1
distanceBoundary2 = 3
distanceBoundary3 = 5
timeBetweenEvaluation = 0.2 # Time between evaulations in seconds
timeBetweenVoice = 3.0 # The  robot must repeat state every X seconds
time2ForgetHumans = 5.0 # Forget humans after X seconds
Human = namedtuple('Human', ['distance', 'angle','timeSinceDetection'])

pubHumanDangerLevel = rospy.Publisher("/HumanDangerLevel", UInt16 , queue_size=10)

soundhandle = SoundClient()
previousHumanDangerState = 0
timeSinceVoice = timeBetweenVoice
human = []

def EvaluateHumanAwareness(data):
    global human
    global pubHumanDangerLevel
    global previousHumanDangerState
    global soundhandle
    global timeBetweenEvaluation
    global timeBetweenVoice
    global timeSinceVoice
    msgHumanDangerLevel = UInt16;
    minDistanceToHuman = []
    currentHumanDangerState = 4
    #print "timer interrupt"
    # Calculate stuff if humans detected.
    if len(human)>0:
        humanNew = []
        # Remove pedestrins detected for more than 
        for n in range(len(human)):
            #print human[n]
            if(human[n].timeSinceDetection>0):
                humanNew.append(Human(human[n].distance, human[n].angle,human[n].timeSinceDetection-timeBetweenEvaluation))

        # Find minimum distnace
        distance = []
        for n in range(len(humanNew)):
            distance.append(humanNew[n].distance)

        if len(distance)>0:
            minDistanceToHuman = min(distance)
            #print minDistanceToHuman
        human = humanNew
    
    # Determine state
    if len(human)==0:
        currentHumanDangerState = 0
    elif(minDistanceToHuman<distanceBoundary2) and (minDistanceToHuman>=distanceBoundary1):
        currentHumanDangerState = 3
    elif (minDistanceToHuman<distanceBoundary3) and (minDistanceToHuman>=distanceBoundary2):
        currentHumanDangerState = 2
    elif minDistanceToHuman>=distanceBoundary3:
        currentHumanDangerState = 1
    else:
        currentHumanDangerState = 4

    
    # Execute cognitive 
    if currentHumanDangerState == 0:
        msgHumanDangerLevel.data = 0
        voice = ""        
        print "No human detected."
    elif currentHumanDangerState == 1:
        msgHumanDangerLevel.data = 1
        voice = ""
        print "HumanDanger1: Human detected far away do nothing..."
    elif currentHumanDangerState == 2:
        msgHumanDangerLevel.data = 2
        voice = "Approaching! Move away!"
        print "HumanDanger2: Slow down and send warning."        
    elif currentHumanDangerState == 3:     
        msgHumanDangerLevel.data = 3
        voice = "Stopping! Move away!"
        print "HumanDanger3: Stop and send warning."
    else:
        msgHumanDangerLevel.data = 4
        voice = "Emergency! Emergency! Emergency!"
        print "HumanDanger4: Emergency."        
        
    if(voice != "" ):
        timeSinceVoice = timeSinceVoice-timeBetweenEvaluation
        if (currentHumanDangerState != previousHumanDangerState) or (timeSinceVoice<0):        
            print "Speak out warning!"
            soundhandle.stopAll()
            soundhandle.say(voice)
            timeSinceVoice = timeBetweenVoice
        
    pubHumanDangerLevel.publish(msgHumanDangerLevel)
    previousHumanDangerState = currentHumanDangerState

def callbackPedDetected(data):
    #rospy.loginfo("DetectionReceived")
    distance = []
    angles = []

    nHumansDetected = len(data.data)/2
    for n in range(nHumansDetected):
        distance = data.data[n*2];
        if distance < 0:
            distance = 10000
        human.append(Human(distance,data.data[n*2+1],time2ForgetHumans))

    #minDistanceToHuman = []
    #if len(distance)>0:
    #    minDistanceToHuman = min(distance)
    # print angles
    # print distance
    # print minDistanceToHuman





# main
def main():
    #global soundhandle
    rospy.init_node('human_awareness', anonymous=True)
    
    rospy.Subscriber("/ped/DistanceAngle", Float64MultiArray, callbackPedDetected)
    rospy.Timer(rospy.Duration(timeBetweenEvaluation), EvaluateHumanAwareness)
    rospy.spin()


if __name__ == '__main__':
    main()
