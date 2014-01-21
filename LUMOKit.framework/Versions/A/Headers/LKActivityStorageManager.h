//
//  LKActivityStorageManager.h
//  LumoKit
//
//  Created by Anders Borch on 8/8/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <LUMOKit/LKActivityStorage.h>

@class LKSensor;

/*
 Activity storage manager error codes
 
 These error codes are returned as part of userInfo in exceptions thrown by the user manager.
 The LKActivityStorageManagerErrorCodeKey key is used to reference error codes in the userInfo dictionary.
 */
typedef NS_ENUM(NSInteger, LKActivityStorageManagerError) {
    LKActivityStorageManagerErrorInvalidPassword    = -1,       // Invalid password
    LKActivityStorageManagerErrorInvalidAppId       = -2,       // Invalid App ID
    LKActivityStorageManagerErrorInvalidOwner       = -3        // Malformed owner or owner does not match sensor owner
};

typedef NS_ENUM(NSInteger, LKActivityStorageManagerResult) {
    LKActivityStorageManagerSuccess,                            // Asynchronous task completed
    LKActivityStorageManagerFailed                  = -101,     // General server error
    LKActivityStorageManagerNetworkUnavailable      = -102      // A required network resource is unavailable
};

/*
 Block invoked whenever a day of data has been recovered from the cloud activity storage
 The date argument is the date of the recovered data.
 */
typedef void (^LKActivityStorageManagerDataRecevoryUpdateBlock)(NSDate *date);

/*
 Block invoked when an asynchronous task is completed.
 */
typedef void(^LKActivityStorageManagerCompletionBlock)(LKActivityStorageManagerResult result);


// Error code key used to index error codes in userInfo dictionaries sent with exceptions
FOUNDATION_EXPORT NSString* const LKActivityStorageManagerErrorCodeKey;

@interface LKActivityStorageManager : NSObject

@property (nonatomic,readonly) id<LKActivityStorage> activityStorage;

/*
 Create an instance of a activity storage manager with an owner and an app id.
 Throw an exception in case instatiating the storage manager fails.

 The app ID is supplied by LUMO BodyTech.
 
 The version number is a string comprised of three period-separated integers.
 The first integer represents major revisions to the application, such as
 revisions that implement new features or major changes. The second integer
 denotes revisions that implement less prominent features. The third integer
 represents maintenance releases.
 */
- (id)initWithOwner:(NSString*)owner appId:(NSString*)appId version:(NSString*)version;

// Request a password reset from cloud service
- (void)resetPasswordWithCompletionBlock:(LKActivityStorageManagerCompletionBlock)block;

/*
 Associate a sensor with an activity storage and start storing incomming activities
 Activities 
 An exception will be thrown if the sensor owner does not match the activity storage owner.
 */
- (void)startStoringActivitiesFromSensor:(LKSensor*)sensor;

/*
 Start background recovering activities from cloud for currrent owner with given password
 Data is recovered one day at a time going backwards from the first day of locally stored data.
 */
- (void)startRecoveringActivitiesWithPassword:(NSString*)password
                              withUpdateBlock:(LKActivityStorageManagerDataRecevoryUpdateBlock)block;

/*
 Recover a single day of data from cloud for currrent owner with given password.
 */
- (void)recoverActivitiesWithPassword:(NSString*)password
                                 year:(int)year
                                month:(int)month
                                  day:(int)day
                      withUpdateBlock:(LKActivityStorageManagerDataRecevoryUpdateBlock)block;

@end
