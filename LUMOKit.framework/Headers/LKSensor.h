//
//  LKSensor.h
//  LumoKit
//
//  Created by Anders Borch on 7/30/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LKSensor;
@class LKSensorVersion;

// Current sensor activity
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityWalk;      // Walking
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityRun;       // Running
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityStand;     // Standing (good or bad)
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivitySitGood;   // Sitting straght
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivitySitBad;    // Sitting bad
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityCar;       // In car
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityLieLeft;   // Lieing on left side
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityLieRight;  // Lieing on right side
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityLieFront;  // Lieing on front
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityLieBack;   // Lieing on back
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityInactive;  // Sensor thinks is isn't being worn
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityNotWorn;   // Sensor knows it isn't being worn - it is charging

FOUNDATION_EXPORT NSString* const LKSensorCurrentActivitySitPrefix;     // Prefix on all sitting postures
FOUNDATION_EXPORT NSString* const LKSensorCurrentActivityLiePrefix;     // Prefix on all lying postures

FOUNDATION_EXPORT NSString* const LKSensorPlatformBack;             // LumoBack type sensor
FOUNDATION_EXPORT NSString* const LKSensorPlatformMod;              // LumoMod (a.k.a. LumoLift) type sensor


// Sensor states
typedef NS_ENUM(NSInteger, LKSensorState) {
    LKSensorStateClosed,    // There is no open connection to the sensor
    LKSensorStateOpening,   // A connection to the sensor is in the process of being opened
    LKSensorStateOpen,      // There is an open connection to the sensor
    LKSensorStateClosing    // The connection to the sensor is closing
};

// Sensor connection result
typedef NS_ENUM(NSInteger, LKSensorConnectionResult) {
    LKSensorConnectFailedTimeout,               // Connection timeout ocurred
    LKSensorConnectFailedInvalidPassword,       // Invalid password sent to sensor
    LKSensorConnectNew,                         // Sensor has no owner - Send a request to own a sensor using activity storage manager
    LKSensorConnectOwned,                       // Login succesful - sensor is owned by user
};

// Sensor buzz strategy
typedef NS_ENUM(NSInteger, LKSensorBuzzStrategy) {
    LKSensorBuzzMore = 1,   // Buzz more often
    LKSensorBuzzLess = 2    // Buzz less often
};

// Sensor buzz strength
typedef NS_ENUM(NSInteger, LKSensorBuzzStrength) {
    LKSensorBuzzStrengthLow = 1,
    LKSensorBuzzStrengthMedium,
    LKSensorBuzzStrengthHigh,
};

// Owner gender
typedef NS_ENUM(NSInteger, LKSensorOwnerGender) {
    LKSensorOwnerMale = 1,
    LKSensorOwnerFemale
};

// Block which is invoked when a connection is completed
typedef void (^LKSensorConnectResultBlock)(LKSensorConnectionResult result);

/*
 Block invoked whenever a request reply is received or a timeout occurs.
 The block is invoked with a nil message and argument if a timeout occurs.
 */
typedef void (^LKSensorRequestBlock)(NSString *message, NSString *argument);

/*
 Block invoked as progress is made collecting walking or sitting calibration data.
 Progress is in the range [0,1].
 */
typedef void(^LKSensorCalibrationProgressBlock)(float progress);

/*
 Block invoked as progress is made importing activity data from the sensor.
 Remaining is the number of remaining pages of data to be received.
 */
typedef void(^LKSensorActivityImportProgressBlock)(int remaining);

/*
 Block invoked with unprocessed messages as they are received.
 */
typedef void(^LKSensorUnprocessedDataBlock)(NSDictionary *message);

/*
 Block invoked when USB power state has been fetched.
 */
typedef void(^LKSensorPowerRequestBlock)(BOOL hasUSBPower);

/*
 LKSensor maintains the information about a discovered sensor.
 After a sensor has been connected then all properties will be updated
 asynchronously as soon as information about the sensor becomes available.
 */
@interface LKSensor : NSObject

// KVO compliant properties which are updated whenever new info is available
@property (nonatomic,readonly) LKSensorState state;
@property (nonatomic,readonly) LKSensorVersion *version;                // Version of the sensor and its base firmware
@property (nonatomic,readonly) int prmRevision;                         // Version of the Posture Recognition Module
@property (nonatomic,readonly) NSString *hardwareID;
@property (nonatomic,readonly) NSString *sensorID;
@property (nonatomic,readonly) float currentCharge;
@property (nonatomic,readonly) BOOL charging;
@property (nonatomic,readonly) NSDate *date;
@property (nonatomic,readonly) BOOL vibrationEnabled;
@property (nonatomic,readonly) double temperature;
@property (nonatomic,readonly) BOOL connected;
@property (nonatomic,readonly) NSString *owner;
@property (nonatomic,readonly) NSNumber *currentWalkSteps;
@property (nonatomic,readonly) NSNumber *currentRunSteps;
@property (nonatomic,readonly) NSNumber *currentStandups;
@property (nonatomic,readonly) NSNumber *currentDistance;
@property (nonatomic,readonly) NSNumber *currentRunDistance;
@property (nonatomic,readonly) NSNumber *currentCalories;
@property (nonatomic,readonly) NSNumber *currentRunCalories;
@property (nonatomic,readonly) NSNumber *currentWalkCalories;
@property (nonatomic,readonly) NSNumber *currentSitTime;
@property (nonatomic,readonly) NSNumber *currentSlouchAngle;            // Forward/backward angle
@property (nonatomic,readonly) NSNumber *currentTiltAngle;              // Left/right angle
@property (nonatomic,readonly) NSString *currentActivity;               // One of the LKSensorActivity* strings
@property (nonatomic,readonly) NSNumber *currentConeAngle;              // Cone angle
@property (nonatomic,strong) NSNumber *currentSitGoodPostureAngle;      // Good posture angle for sitting (default 90)
@property (nonatomic,strong) NSNumber *currentStandGoodPostureAngle;    // Good posture angle for standing (default 90)
@property (nonatomic,assign) BOOL buzzerEnabled;
@property (nonatomic,strong) NSNumber *currentStandingDeskMode;         // YES if standing desk mode is enabled
@property (nonatomic,readonly) NSDate *lastConnectionDate;
@property (nonatomic,readonly) NSString *platform;                      // The type of sensor

@property (nonatomic,copy) LKSensorActivityImportProgressBlock importProgressBlock;
@property (nonatomic,copy) LKSensorUnprocessedDataBlock unprocessedDataBlock;

/*
 Connect anonymously to a sensor, query its owner and disconnect immediately.
 The completion block is invoked as soon as owner has been retrieved.
 */
- (void)queryOwnerWithCompletionBlock:(dispatch_block_t)block;

// Asynchronous connect and login request to sensor
- (void)connectWithOwner:(NSString*)owner
                password:(NSString*)password
         completionBlock:(LKSensorConnectResultBlock)block;

// Re-initialize the PRM plugin
- (void)resetWithBlock:(LKSensorConnectResultBlock)block;

// Factory reset sensor
- (void)factoryResetWithBlock:(LKSensorConnectResultBlock)block;

// Request USB power status
- (void)sendPowerRequest:(LKSensorPowerRequestBlock)block;

// Disconnect from sensor
- (void)disconnect;

// Buzz once now
- (void)buzz;

// Set sensor buzz strategy
- (void)setBuzzStrategy:(LKSensorBuzzStrategy)strategy;

// Set seconds in bad posture before buzz goes off
- (void)setBuzzDelay:(int)delay;

// Set seconds in good posture to recharge buzzer
- (void)setBuzzRecharge:(int)rechargeDuration;

// Set buzz duration in seconds
- (void)setBuzzDuration:(int)duration;

// Set buzz strength
- (void)setBuzzStrength:(LKSensorBuzzStrength)strength;

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

// Start walk calibration
- (void)startWalkReorientationWithBlock:(LKSensorCalibrationProgressBlock)block;

// Start sit calibration
- (void)startSitReorientationWithBlock:(LKSensorCalibrationProgressBlock)block;

// Set owner height in cm
- (void)setHeight:(int)height;

// Set owner weight in kg
- (void)setWeight:(float)weight;

// Set owner gender
- (void)setGender:(LKSensorOwnerGender)gender;

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
    completionBlock:(LKSensorRequestBlock)block;

@end
