//
//  LBSensor.h
//  LumoKit
//
//  Created by Anders Borch on 7/30/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LBSensor;
@class LBSensorVersion;

// Sensor states
typedef NS_ENUM(NSInteger, LBSensorState) {
    LBSensorStateClosed,    // There is no open connection to the sensor
    LBSensorStateOpening,   // A connection to the sensor is in the process of being opened
    LBSensorStateOpen,      // There is an open connection to the sensor
    LBSensorStateClosing    // The connection to the sensor is closing
};

// Sensor connection result
typedef NS_ENUM(NSInteger, LBSensorConnectionResult) {
    LBSensorConnectFailedTimeout,               // Connection timeout ocurred
    LBSensorConnectFailedInvalidPassword,       // Invalid password sent to sensor
    LBSensorConnectNew,                         // Sensor has no owner - Send a request to own a sensor using activity storage manager
    LBSensorConnectOwned,                       // Login succesful - sensor is owned by user
};

// Sensor buzz strategy
typedef NS_ENUM(NSInteger, LBSensorBuzzStrategy) {
    LBSensorBuzzMore = 1,   // Buzz more often
    LBSensorBuzzLess = 2    // Buzz less often
};

// Sensor buzz strength
typedef NS_ENUM(NSInteger, LBSensorBuzzStrength) {
    LBSensorBuzzStrengthLow = 1,
    LBSensorBuzzStrengthMedium,
    LBSensorBuzzStrengthHigh,
};

// Current sensor activity
FOUNDATION_EXPORT NSString* const LBSensorActivityInactive;             	// Sensor thinks it is not on a person
FOUNDATION_EXPORT NSString* const LBSensorActivityCar;                  	// Driving car
FOUNDATION_EXPORT NSString* const LBSensorActivityLieBack;                  // Lie back
FOUNDATION_EXPORT NSString* const LBSensorActivityLieFront;                 // Lie front
FOUNDATION_EXPORT NSString* const LBSensorActivityLieLeft;                  // Lie left
FOUNDATION_EXPORT NSString* const LBSensorActivityLieRight;                 // Lie right
FOUNDATION_EXPORT NSString* const LBSensorActivityNotWorn;                  // Sensor is plugged into USB
FOUNDATION_EXPORT NSString* const LBSensorActivityRun;                      // Run
FOUNDATION_EXPORT NSString* const LBSensorActivitySitBadForward;            // Sit bad forward
FOUNDATION_EXPORT NSString* const LBSensorActivitySitBadLeft;               // Sit bad left
FOUNDATION_EXPORT NSString* const LBSensorActivitySitBadRight;              // Sit bad right
FOUNDATION_EXPORT NSString* const LBSensorActivitySitBadBackward;           // Sit bad backward
FOUNDATION_EXPORT NSString* const LBSensorActivitySitGood;                  // Sit good
FOUNDATION_EXPORT NSString* const LBSensorActivityStandBadForward;          // Stand bad forward
FOUNDATION_EXPORT NSString* const LBSensorActivityStandBadLeft;             // Stand bad left
FOUNDATION_EXPORT NSString* const LBSensorActivityStandBadRight;            // Stand bad right
FOUNDATION_EXPORT NSString* const LBSensorActivityStandBadBackward;         // Stand bad backward
FOUNDATION_EXPORT NSString* const LBSensorActivityStandGood;                // Stand good
FOUNDATION_EXPORT NSString* const LBSensorActivityWalk;                     // Walk
FOUNDATION_EXPORT NSString* const LBSensorActivityStandQuiteBadForward;     // Stand quite bad forward
FOUNDATION_EXPORT NSString* const LBSensorActivityStandReallyBadForward;    // Stand really bad forward
FOUNDATION_EXPORT NSString* const LBSensorActivityStandQuiteBadBackward;    // Stand quite bad slouch
FOUNDATION_EXPORT NSString* const LBSensorActivityStandReallyBadBackward;   // Stand really bad slouch
FOUNDATION_EXPORT NSString* const LBSensorActivitySitQuiteBadForward;       // Sit quite bad forward
FOUNDATION_EXPORT NSString* const LBSensorActivitySitReallyBadForward;      // Sit really bad forward
FOUNDATION_EXPORT NSString* const LBSensorActivitySitQuiteBadBackward;      // Sit quite bad slouch
FOUNDATION_EXPORT NSString* const LBSensorActivitySitReallyBadBackward;     // Sit really bad slouch

// Block which is invoked when a connection is completed
typedef void (^LBSensorConnectResultBlock)(LBSensorConnectionResult result);

/*
 Block invoked whenever a request reply is received or a timeout occurs.
 The block is invoked with a nil message and argument if a timeout occurs.
 */
typedef void (^LBSensorRequestBlock)(NSString *message, NSString *argument);

/*
 Block invoked as progress is made collecting walking or sitting calibration data.
 Progress is in the range [0,1].
 */
typedef void(^LBSensorCalibrationProgressBlock)(float progress);

/*
 Block invoked as progress is made importing activity data from the sensor.
 Remaining is the number of remaining pages of data to be received.
 */
typedef void(^LBSensorActivityImportProgressBlock)(int remaining);

/*
 LBSensor maintains the information about a discovered sensor.
 After a sensor has been connected then all properties will be updated
 asynchronously as soon as information about the sensor becomes available.
 */
@interface LBSensor : NSObject

// KVO compliant properties which are updated whenever new info is available
@property (nonatomic,readonly) LBSensorState state;
@property (nonatomic,readonly) LBSensorVersion *version;        // Version of the sensor and its base firmware
@property (nonatomic,readonly) int prmRevision;                 // Version of the Posture Recognition Module
@property (nonatomic,readonly) NSString *hardwareID;
@property (nonatomic,readonly) NSString *sensorID;
@property (nonatomic,readonly) float currentCharge;
@property (nonatomic,readonly) BOOL charging;
@property (nonatomic,readonly) NSDate *date;
@property (nonatomic,readonly) BOOL vibrationEnabled;
@property (nonatomic,readonly) double temperature;
@property (nonatomic,readonly) NSString *owner;
@property (nonatomic,readonly) NSNumber *currentWalkSteps;
@property (nonatomic,readonly) NSNumber *currentRunSteps;
@property (nonatomic,readonly) NSNumber *currentStandups;
@property (nonatomic,readonly) NSNumber *currentDistance;
@property (nonatomic,readonly) NSNumber *currentCalories;
@property (nonatomic,readonly) NSNumber *currentSitTime;
@property (nonatomic,readonly) NSNumber *currentSlouchAngle;    // Forward/backward angle
@property (nonatomic,readonly) NSNumber *currentTiltAngle;      // Left/right angle
@property (nonatomic,readonly) NSString *currentActivity;       // One of the LBSensorActivity* strings

@property (nonatomic,copy) LBSensorActivityImportProgressBlock importProgressBlock;

// Asynchronous connect and login request to sensor
- (void)connectWithOwner:(NSString*)owner
                password:(NSString*)password
         completionBlock:(LBSensorConnectResultBlock)block;

// Re-initialize the PRM plugin
- (void)resetWithBlock:(LBSensorConnectResultBlock)block;

// Disconnect from sensor
- (void)disconnect;

// Buzz once now
- (void)buzz;

// Turn buzzing on/off
- (void)setBuzzerEnabled:(BOOL)enabled;

// Set sensor buzz strategy
- (void)setBuzzStrategy:(LBSensorBuzzStrategy)strategy;

// Set seconds in bad posture before buzz goes off
- (void)setBuzzDelay:(int)delay;

// Set seconds in good posture to recharge buzzer
- (void)setBuzzRecharge:(int)rechargeDuration;

// Set buzz duration in seconds
- (void)setBuzzDuration:(int)duration;

// Set buzz strength
- (void)setBuzzStrength:(LBSensorBuzzStrength)strength;

// Set the good posture angle for sitting (default 90)
- (void)setSitGoodPostureAngle:(int)angle;

// Turn sensor logging on/off
- (void)setLogging:(BOOL)logging;

// Turn the PRM plugin on/off, to avoid wrong statistics while sensor in suitcase or similiar.
- (void)setSensorEnabled:(BOOL)enabled;

// Tell sensor to start/stop sending raw accelerometer data at 25Hz
- (void)setRawMode:(BOOL)raw;

// Tell sensor (not) to use the orientation matrix.
- (void)setRotationEnabled:(BOOL)enabled;

// Set tolerance sit bad backward
- (void)setSitBackwardTolerance:(int)tolerance;

// Set tolerance sit bad forward
- (void)setSitForwardTolerance:(int)tolerance;

// Set tolerance sit bad left/right
- (void)setSitSideTolerance:(int)tolerance;

// Set tolerance stand bad backward
- (void)setStandBackwardTolerance:(int)tolerance;

// Set tolerance stand bad forward
- (void)setStandForwardTolerance:(int)tolerance;

// Set tolerance stand bad left/right
- (void)setStandSideTolerance:(int)tolerance;

// Turn buzzing on/off for bad standing
- (void)setStandBuzzerEnabled:(BOOL)buzz;

// Set the good posture angle for standing (default 90)
- (void)setStandGoodPostureAngle:(int)angle;

// Start walk calibration
- (void)startWalkReorientationWithBlock:(LBSensorCalibrationProgressBlock)block;

// Start sit calibration
- (void)startSitReorientationWithBlock:(LBSensorCalibrationProgressBlock)block;

/*
 Send arbitrary command with a list of arguments
 Arguments is an array strings.
 */
- (void)sendCommand:(NSString*)command withArguments:(NSArray*)arguments;

/*
 Send arbitrary request with a list of arguments
 Arguments is an array strings.
 The completion block is invoked when one of the expected replies is received or when a timeout occurs.
 If a timeout or other failure occurs then the completion block is invoked with nil parameters.
 */
- (void)sendRequest:(NSString*)request
      withArguments:(NSArray*)arguments
    expectedReplies:(NSArray*)replies
            timeout:(NSTimeInterval)timeout
    completionBlock:(LBSensorRequestBlock)block;

@end
