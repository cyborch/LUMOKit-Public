//
//  LBSensorManager.m
//  LumoKit
//
//  Created by Anders Borch on 7/30/13.
//  Copyright (c) 2013 LUMO BodyTech. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <LUMOKit/LBSensor.h>

// Sensor manager states
typedef NS_ENUM(NSInteger, LBSensorManagerState) {
    LBSensorManagerStateUnknown,            // The current state of the sensor manager is unknown; an update is imminent.
    LBSensorManagerStateResetting,          // The connection with the system service was momentarily lost; an update is imminent.
    LBSensorManagerStateUnsupported,        // The platform does not support LumoKit.
    LBSensorManagerStateUnauthorized,       // The app is not authorized to use Bluetooth low energy.
    LBSensorManagerStatePoweredOff,         // Sensor manager is currently powered off.
    LBSensorManagerStatePoweredOn           // Sensor manager is currently powered on and available to use.
};

// Sensor manager asynchronous request result
typedef NS_ENUM(NSInteger, LBSensorManagerRequestResult) {
    LBSensorManagerRequestFailedTimeout,
    LBSensorManagerRequestFailedInvalidPassword,
    LBSensorManagerRequestFailedInvalidOwner,
    LBSensorManagerRequestSuccess,
};

/*
 Sensor manager error codes

 These error codes are returned as part of userInfo in exceptions thrown by the sensor manager.
 The LBLBSensorManagerErrorCodeKey key is used to reference error codes in the userInfo dictionary.
 */
typedef NS_ENUM(NSInteger, LBSensorManagerError) {
    LBSensorManagerErrorInvalidAppId = -1,             // Invalid app ID
    LBSensorManagerErrorMissingEnumerationBlock = -2,  // Nil block passed to enumeration method
    LBSensorManagerErrorInvalidSensorUUID = -3         // Unique identifier missing for sensor - connect to sensor first
};

// Possible error codes returned by authorizeOwnSensor:completionBlock:
typedef NS_ENUM(NSInteger, LBSensorManagerOwnResult) {
    LBSensorManagerOwnOK                   = 0,        // Successful own
    LBSensorManagerOwnNetworkUnavailable   = -1001,    // Network error occured
    LBSensorManagerOwnInvalidPassword      = -1002,    // Invalid password
    LBSensorManagerOwnInvalidOwnerName     = -1003,    // Owner name could not be percent escaped
    LBSensorManagerOwnOwnedByOther         = -1004     // Other owner already owns this sensor id
};

// Type of block invoked on completion of authorizeOwnSensor:completionBlock:
typedef void (^LBSensorManagerOwnCompletionBlock)(LBSensorManagerOwnResult result);

// Error code key used to index error codes in userInfo dictionaries sent with exceptions
FOUNDATION_EXPORT NSString* const LBSensorManagerErrorCodeKey;

// Block invoked when state is updated
typedef void (^LBSensorManagerStateUpdateBlock)(LBSensorManagerState state);

// Block invoked if a known sensor is discovered and automatically connected to
typedef void (^LBSensorManagerAutoconnectBlock)(LBSensor *sensor);

// Block invoked on completion of asynchronous request
typedef void (^LBSensorManagerCompletionBlock)(LBSensorManagerRequestResult result);

// Sensor manager
@interface LBSensorManager : NSObject

@property (nonatomic,readonly) LBSensorManagerState state;
@property (nonatomic,readonly) NSArray *sensors;

/*
 Assign a block to this property to enable autoconnect.
 This block will get invoked if a known sensor is discovered and
 successfully connected to.
 */
@property (nonatomic, copy) LBSensorManagerAutoconnectBlock sensorAutoconnectBlock;

/*
 Initialize with an app ID.
 The app ID is supplied by LUMO BodyTech.
 */
- (id)initWithAppId:(NSString*)appId;

// Scanning for sensors
- (void)startScan;
- (void)stopScan;

/*
 Accessing stored owner passwords
 
 Storing last known good passwords allows the sensor manager to automatically
 connect to a sensor when it is discovered if a sensorAutoconnectBlock is specified.
 */
- (NSString*)lastKnownGoodPasswordForOwner:(NSString*)owner;
- (void)storeKnownGoodPassword:(NSString*)password forOwner:(NSString*)owner;

// Request a password reset from cloud service
- (void)resetPasswordForSensor:(LBSensor*)sensor withCompletionBlock:(LBSensorManagerCompletionBlock)block;

// Send a request to own a sensor to the cloud service
- (void)authorizeOwnSensor:(LBSensor*)sensor
              withPassword:(NSString*)password
           completionBlock:(LBSensorManagerOwnCompletionBlock)block;



@end
