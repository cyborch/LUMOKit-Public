//
//  LKActivityStorage.h
//  LumoKit
//
//  Created by Anders Borch on 8/6/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <LUMOKit/LKPeriodActivities.h>

FOUNDATION_EXPORT NSString* const kCFErrorDomainLKActivityStorage;

// Historical sensor activity
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityInactive;              	 // Sensor thinks it is not on a person
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityCar;                  	 // Driving car
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityLieBack;                  // Lie back
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityLieFront;                 // Lie front
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityLieLeft;                  // Lie left
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityLieRight;                 // Lie right
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityNotWorn;                  // Sensor is plugged into USB
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityRun;                      // Run
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitBadForward;            // Sit bad forward
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitBadLeft;               // Sit bad left
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitBadRight;              // Sit bad right
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitBadBackward;           // Sit bad backward
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitGood;                  // Sit good
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandBadForward;          // Stand bad forward
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandBadLeft;             // Stand bad left
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandBadRight;            // Stand bad right
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandBadBackward;         // Stand bad backward
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandGood;                // Stand good
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityWalk;                     // Walk
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandQuiteBadForward;     // Stand quite bad forward
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandReallyBadForward;    // Stand really bad forward
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandQuiteBadBackward;    // Stand quite bad slouch
FOUNDATION_EXPORT NSString* const LKActivityStorageActivityStandReallyBadBackward;   // Stand really bad slouch
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitQuiteBadForward;       // Sit quite bad forward
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitReallyBadForward;      // Sit really bad forward
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitQuiteBadBackward;      // Sit quite bad slouch
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySitReallyBadBackward;     // Sit really bad slouch
FOUNDATION_EXPORT NSString* const LKActivityStorageActivitySubjectiveGoodPosture;    // Sit or stand in calibrated good posture


/*
 Mask of activities which should be aggregated.
 */
typedef NS_OPTIONS(NSUInteger, LKActivityStorageActivityMask) {
	LKActivityStorageActivityMaskInactive               = 0x00000001,       // Sensor thinks it is not on a person
	LKActivityStorageActivityMaskCar                    = 0x00000002,       // Driving car
	LKActivityStorageActivityMaskLieBack                = 0x00000004,       // Lie back
	LKActivityStorageActivityMaskLieFront               = 0x00000008,       // Lie front
	LKActivityStorageActivityMaskLieLeft                = 0x00000010,       // Lie left
	LKActivityStorageActivityMaskLieRight               = 0x00000020,       // Lie right
	LKActivityStorageActivityMaskNotWorn                = 0x00000040,       // Sensor is plugged into USB
	LKActivityStorageActivityMaskRun                    = 0x00000080,       // Run
	LKActivityStorageActivityMaskSitBadForward          = 0x00000100,       // Sit bad forward
	LKActivityStorageActivityMaskSitBadLeft             = 0x00000200,       // Sit bad left
	LKActivityStorageActivityMaskSitBadRight            = 0x00000400,       // Sit bad right
	LKActivityStorageActivityMaskSitBadBackward         = 0x00000800,       // Sit bad backward
	LKActivityStorageActivityMaskSitGood                = 0x00001000,       // Sit good
	LKActivityStorageActivityMaskStandBadForward        = 0x00002000,       // Stand bad forward
	LKActivityStorageActivityMaskStandBadLeft           = 0x00004000,       // Stand bad left
	LKActivityStorageActivityMaskStandBadRight          = 0x00008000,       // Stand bad right
	LKActivityStorageActivityMaskStandBadBackward       = 0x00010000,       // Stand bad backward
	LKActivityStorageActivityMaskStandGood              = 0x00020000,       // Stand good
	LKActivityStorageActivityMaskWalk                   = 0x00040000,       // Walk
	LKActivityStorageActivityMaskWalkSteps              = 0x00080000,       // Walk steps
	LKActivityStorageActivityMaskRunSteps               = 0x00100000,       // Run steps
	LKActivityStorageActivityMaskStandUps               = 0x00200000,       // Stand ups
	LKActivityStorageActivityMaskCalories               = 0x00400000,       // Calories
	LKActivityStorageActivityMaskDistance               = 0x00800000,       // Distance in meters
	LKActivityStorageActivityMaskRunCalories            = 0x01000000,       // Run calories
	LKActivityStorageActivityMaskWalkCalories           = 0x02000000,       // Walk calories
	LKActivityStorageActivityMaskRunDistance            = 0x04000000,       // Run distance in meters
    LKActivityStorageActivityMaskSubjectiveGoodPosture  = 0x08000000,       // Sit or stand in calibrated good posture
    LKActivityStorageActivityMaskAll                    = 0xffffffff        // All of the above
};

/*
 Block invoked when enumerating aggregated activities.
 */
typedef void (^LKActivityStorageBlock)(LKPeriodActivities *activities, NSError *error);

/*
 Block invoked when querying oldest aggregated data
 */
typedef void (^LKActivityStorageOldestQueryBlock)(int oldestLocaltime, NSError *error);

/*
 A protocol implemented by classes which store activity records (ACT messages)
 as received from a Lumo sensor or recovered from the back end.
 Storage is per owner.
 */
@protocol LKActivityStorage <NSObject>

@property (nonatomic,readonly) NSString *owner;

@property (nonatomic,readonly) NSString *lastError;

/*
 Aggregate activities for a given month.
 The LKPeriodActivities object will have defined values for properties matching
 the activities specified in the mask
 */
- (void)aggregatedActivitiesForYear:(int)year
                              month:(int)month
                               mask:(LKActivityStorageActivityMask)mask
                         usingBlock:(LKActivityStorageBlock)block;

/*
 Aggregate activities for a given day.
 The LKPeriodActivities object will have defined values for properties matching
 the activities specified in the mask
 */
- (void)aggregatedActivitiesForYear:(int)year
                              month:(int)month
                                day:(int)day
                               mask:(LKActivityStorageActivityMask)mask
                         usingBlock:(LKActivityStorageBlock)block;

/*
 Aggregate activities for a given hour.
 The LKPeriodActivities object will have defined values for properties matching
 the activities specified in the mask
 */
- (void)aggregatedActivitiesForYear:(int)year
                              month:(int)month
                                day:(int)day
                               hour:(int)hour
                               mask:(LKActivityStorageActivityMask)mask
                         usingBlock:(LKActivityStorageBlock)block;

/*
 Enumerate activities for a given month - one invocation per time interval
 The LKPeriodActviities will have all properties set.
 */
- (void)enumerateActivitiesForYear:(int)year
                             month:(int)month
                        usingBlock:(LKActivityStorageBlock)block;

/*
 Enumerate activities for a given day - one invocation per time interval
 The LKPeriodActviities will have all properties set.
 */
- (void)enumerateActivitiesForYear:(int)year
                             month:(int)month
                               day:(int)day
                        usingBlock:(LKActivityStorageBlock)block;

/*
 Enumerate activities for a given hour - one invocation per time interval
 The LKPeriodActviities will have all properties set.
 */
- (void)enumerateActivitiesForYear:(int)year
                             month:(int)month
                               day:(int)day
                              hour:(int)hour
                        usingBlock:(LKActivityStorageBlock)block;

/*
 Get the timestamp of the oldest locally stored aggregated data.
 'Now' is returned if no data is stored locally.
 */
- (int)localtimeForOldestAggregatedData;

/*
 Get the timestamp of the newest locally stored aggregated data.
 0 is returned if no data is stored locally.
 */
- (int)localtimeForNewestAggregatedData;

@end

