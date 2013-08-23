//
//  LBActivityStorage.h
//  LumoKit
//
//  Created by Anders Borch on 8/6/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <LUMOKit/LBPeriodActivities.h>

FOUNDATION_EXPORT NSString* const kCFErrorDomainLBActivityStorage;

/*
 Mask of activities which should be aggregated.
 */
typedef NS_OPTIONS(NSUInteger, LBActivityStorageActivityMask) {
	LBActivityStorageActivityMaskInactive               = 0x00000001,       // Sensor thinks it is not on a person
	LBActivityStorageActivityMaskCar                    = 0x00000002,       // Driving car
	LBActivityStorageActivityMaskLieBack                = 0x00000004,       // Lie back
	LBActivityStorageActivityMaskLieFront               = 0x00000008,       // Lie front
	LBActivityStorageActivityMaskLieLeft                = 0x00000010,       // Lie left
	LBActivityStorageActivityMaskLieRight               = 0x00000020,       // Lie right
	LBActivityStorageActivityMaskNotWorn                = 0x00000040,       // Sensor is plugged into USB
	LBActivityStorageActivityMaskRun                    = 0x00000080,       // Run
	LBActivityStorageActivityMaskSitBadForward          = 0x00000100,       // Sit bad forward
	LBActivityStorageActivityMaskSitBadLeft             = 0x00000200,       // Sit bad left
	LBActivityStorageActivityMaskSitBadRight            = 0x00000400,       // Sit bad right
	LBActivityStorageActivityMaskSitBadBackward         = 0x00000800,       // Sit bad backward
	LBActivityStorageActivityMaskSitGood                = 0x00001000,       // Sit good
	LBActivityStorageActivityMaskStandBadForward        = 0x00002000,       // Stand bad forward
	LBActivityStorageActivityMaskStandBadLeft           = 0x00004000,       // Stand bad left
	LBActivityStorageActivityMaskStandBadRight          = 0x00008000,       // Stand bad right
	LBActivityStorageActivityMaskStandBadBackward       = 0x00010000,       // Stand bad backward
	LBActivityStorageActivityMaskStandGood              = 0x00020000,       // Stand good
	LBActivityStorageActivityMaskWalk                   = 0x00040000,       // Walk
	LBActivityStorageActivityMaskWalkSteps              = 0x00080000,       // Walk steps
	LBActivityStorageActivityMaskRunSteps               = 0x00100000,       // Run steps
	LBActivityStorageActivityMaskStandUps               = 0x00200000,       // Stand ups
	LBActivityStorageActivityMaskCalories               = 0x00400000,       // Calories
	LBActivityStorageActivityMaskDistance               = 0x00800000,       // Distance in meters
    LBActivityStorageActivityMaskAll                    = 0xffffffff        // All of the above
};

/*
 Block invoked when enumerating aggregated activities.
 */
typedef void (^LBActivityStorageBlock)(LBPeriodActivities *activities, NSError *error);

/*
 Block invoked when querying oldest aggregated data
 */
typedef void (^LBActivityStorageOldestQueryBlock)(int oldestLocaltime, NSError *error);

/*
 A protocol implemented by classes which store activity records (ACT messages)
 as received from a Lumo sensor or recovered from the back end.
 Storage is per owner.
 */
@protocol LBActivityStorage <NSObject>

@property (nonatomic,readonly) NSString *owner;

@property (nonatomic,readonly) NSString *lastError;

/*
 Aggregate activities for a given month.
 The LBPeriodActivities object will have defined values for properties matching
 the activities specified in the mask
 */
- (void)aggregatedActivitiesForYear:(int)year
                              month:(int)month
                               mask:(LBActivityStorageActivityMask)mask
                         usingBlock:(LBActivityStorageBlock)block;

/*
 Aggregate activities for a given day.
 The LBPeriodActivities object will have defined values for properties matching
 the activities specified in the mask
 */
- (void)aggregatedActivitiesForYear:(int)year
                              month:(int)month
                                day:(int)day
                               mask:(LBActivityStorageActivityMask)mask
                         usingBlock:(LBActivityStorageBlock)block;

/*
 Aggregate activities for a given hour.
 The LBPeriodActivities object will have defined values for properties matching
 the activities specified in the mask
 */
- (void)aggregatedActivitiesForYear:(int)year
                              month:(int)month
                                day:(int)day
                               hour:(int)hour
                               mask:(LBActivityStorageActivityMask)mask
                         usingBlock:(LBActivityStorageBlock)block;

/*
 Enumerate activities for a given month - one invocation per time interval
 The LBPeriodActviities will have all properties set.
 */
- (void)enumerateActivitiesForYear:(int)year
                             month:(int)month
                        usingBlock:(LBActivityStorageBlock)block;

/*
 Enumerate activities for a given day - one invocation per time interval
 The LBPeriodActviities will have all properties set.
 */
- (void)enumerateActivitiesForYear:(int)year
                             month:(int)month
                               day:(int)day
                        usingBlock:(LBActivityStorageBlock)block;

/*
 Enumerate activities for a given hour - one invocation per time interval
 The LBPeriodActviities will have all properties set.
 */
- (void)enumerateActivitiesForYear:(int)year
                             month:(int)month
                               day:(int)day
                              hour:(int)hour
                        usingBlock:(LBActivityStorageBlock)block;

- (int)localtimeForOldestAggregatedData;

@end

