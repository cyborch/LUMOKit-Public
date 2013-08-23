//
//  LBPeriodActivities.h
//  LumoKit
//
//  Created by Anders Borch on 8/8/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LBPeriodActivities : NSObject

@property (nonatomic, readonly) NSString *sensorId;         // ID of sensor which recorded the activities for the period
@property (nonatomic, readonly) NSTimeInterval time;        // GMT timestamp for period start time
@property (nonatomic, readonly) NSTimeInterval localTime;   // Timestamp shifted according to local time for period start time
@property (nonatomic, readonly) NSTimeInterval duration;    // Duration of time period in seconds

// Proportion of each activity for a given time period in the range [0,1]
@property (nonatomic, readonly) double walk;
@property (nonatomic, readonly) double run;
@property (nonatomic, readonly) double sitGood;
@property (nonatomic, readonly) double sitBadLeft;
@property (nonatomic, readonly) double sitBadRight;
@property (nonatomic, readonly) double sitBadBackward;
@property (nonatomic, readonly) double sitBadForward;
@property (nonatomic, readonly) double standGood;
@property (nonatomic, readonly) double standBadLeft;
@property (nonatomic, readonly) double standBadRight;
@property (nonatomic, readonly) double standBadBackward;
@property (nonatomic, readonly) double standBadForward;
@property (nonatomic, readonly) double lieLeft;
@property (nonatomic, readonly) double lieRight;
@property (nonatomic, readonly) double lieFront;
@property (nonatomic, readonly) double lieBack;
@property (nonatomic, readonly) double car;
@property (nonatomic, readonly) double notWorn;
@property (nonatomic, readonly) double inactive;

@property (nonatomic, readonly) int walkSteps;              // Number of steps taken
@property (nonatomic, readonly) int runSteps;               // Number of running steps taken
@property (nonatomic, readonly) int standups;               // Number of stand ups performed
@property (nonatomic, readonly) int calories;               // Number of calories burned
@property (nonatomic, readonly) int distance;               // Distance in meters walked or ran

@end
