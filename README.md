LUMOKit-Public
==============

![LUMOback Logo](http://www.lumoback.com/wp-content/uploads/2013/05/logo_color-150x100.png)

## Overview

**LUMOKit** is an Objective-C framework for iOS. This framework lets you develop iOS applications that communicate with a LUMOback sensor in real time. The sensor communicates the user's current posture and activity once a second.

This document demonstrates the procedures to get started with LUMOKit and to create a basic iOS application using the framework.

### Compatible Devices for App and Setup
The SDK currently supports the following devices and operating systems: 

- iPhone 4s+ with iOS 6.1+
- iPad 3 with iOS 6.1+
- iPad Mini with iOS 6.1+
- iPod Touch 5th Generation with iOS 6.1+

### The LUMOback Sensor
You will need the [LUMOback](http://www.lumoback.com) Sensor. 

![LUMOback Image](http://cdn.shopify.com/s/files/1/0214/4274/products/shopify_fold_640_large.jpg?768)

#### What is LUMOback?
**The Smart Posture & Movement Sensor.**  
The LUMOback<sup>TM</sup> sensor gently vibrates when you slouch and the app tracks daily movements to inspire proper posture and increased mobility.

#### Need a Sensor?  
[Purchase one here](http://www.lumobackstore.com/products/lumoback-posture-sensor). 

## LUMOKit Tutorial
This tutorial provides an overview of the basic functionalities of the SDK and the steps to build a simple iOS application integrated with LUMOKit framework. Please refer to the repository on Github for the complete source code of the tutorials and the framework.

#### Prerequisites

You will need Xcode for iOS application development. You can download Xcode from [here](https://developer.apple.com/xcode/).

#### Project Creation and Settings

1) Open Xcode application.  
2) On the main Xcode menu, click on File > New > Project to create a new Xcode project.  
3) Choose "Empty Application".  
4) Next, choose an appropriate location on your local machine to save the project. Click on "Create" to create your project with the above settings.  
5) Next, create a local copy of the LUMOback-Public repository available on GitHub using the command: 

```
git clone git@github.com:LUMOback/LUMOKit-Public.git 
```

6) Locate `LUMOKit.framework` from the cloned copy of the repository on your local machine and drag it into your XCode project.

7) Select the Summary tab in project settings. In the section for "Linked Frameworks and Libraries" click the "+" button and add the following frameworks and libraries:  
- CoreBluetooth.framework
- Security.framework
- SystemConfiguration.framework
- libsqlite3.dylib
- libz.dylib

8) In project settings navigate to Targets > Build Settings > (All|Combined). Scroll to "Linking" > "Other Linker Flags". Set the Linker flags for the Debug and Release modes to `-ObjC`. 

9) In the Info.plist add a `UIBackgroundModes` key with an array value where the first item is `"bluetooth-central"`.

10) On the main Xcode menu, navigate to Product > Scheme > Edit Scheme. Change "Destination" to iOS device in order to build the application for an iPhone device. Please note that the SDK __DOES NOT__ connect to a sensor from an iPhone/iPad Simulator. 

### LUMOKit Integration Steps

1) __HEADER IMPORTS__

In order to start using the framework, add this line of code to your source file:

```objective-c
#import <LUMOKit/LUMOKit.h>
```

2) __REGISTERING AN APPLICATION ID, ACCESSING SENSOR DATA__

To register your application with us and request an Application ID, please contact  **support@lumoback.com**. 

Once you have a registered Application ID, you can pass it to any of the manager classes in the framework and use those manager to discover nearby sensors, connect to a sensor and access activity data.

```objective-c
LBSensorManager *sensorManager = [[LBSensorManager alloc] initWithAppId: @"<YOUR APP ID HERE>"];
LBActivityStorageManager *storageManager = [[LBActivityStorageManager alloc] initWithOwner: @"<SENSOR OWNER EMAIL ADDRESS HERE>"
                                                                                     appId: @"<YOUR APP ID HERE>"];
```

3) __DISCOVERING AND CONNECTING TO A SENSOR__

In order to get an updated list of sensors observe the sensors array on a `LBSensorManager` instance. Firstly, we want to declare a few instance variables:

```objective-c
@interface SampleClass : NSObject {
    LBSensorManager *_sensorManager;             // Discovery and management of sensors
    LBSensor *_connectedSensor;                  // The currently connected sensor
    LBActivityStorageManager *_storageManager;   // Optional: Accessing historical activity storage
}
@end
```

Next, we want start the implementation of our class with a method to scan for nearby sensors and observe the list of sensors as new sensors are discovered:

```objective-c
@implementation SampleClass
// Start scanning for sensors and observe changes in the sensors array
- (void)startScanningForSensors
{
    _sensorManager = [[LBSensorManager alloc] initWithAppId: @"<YOUR APP ID HERE>"];
    _storageManager = [[LBActivityStorageManager alloc] initWithOwner: @"<SENSOR OWNER EMAIL ADDRESS HERE>"
                                                                appId: @"<YOUR APP ID HERE>"];
    [_sensorManager addObserver: self
                     forKeyPath: @"sensors"
                        options: NSKeyValueObservingOptionNew
                        context: NULL];
    [_sensorManager startScan];
}
```

The observer is called every time a new sensor is discovered or disappears or the order of nearby sensors (ordered by vicinity based on signal strength) changes:

```objective-c
- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context
{
    if ([keyPath isEqualToString: @"sensors"]) [self sensorsUpdated];
}
```

Now, try to connect to the closest sensor if a connection attempt hasn't already been made or if the last connection attempt failed.

```objective-c
- (void)sensorsUpdated
{
    static BOOL connecting = NO;
    NSLog(@"sensors changed!");
    if (!connecting) {
        /* 
         The sensors array contains a list of discovered sensors, 
         sorted by signal strength with the closest sensor first
         */
        if (_sensorManager.sensors.count) {
            connecting = YES;
            __weak SampleClass *weak = self;
            LBSensor *sensor = _sensorManager.sensors[0];
            [sensor connectWithOwner: @"<SENSOR OWNER EMAIL ADDRESS HERE>"
                            password: @"<PASSWORD HERE>"
                     completionBlock: ^(LBSensorConnectionResult result) {
                         if (result == LBSensorConnectOwned) {
                             _connectedSensor = sensor;
                             // Start observing activity changes - see below
                             [weak observeActivities];
                         } else if (result == LBSensorConnectNew) {
                             _connectedSensor = sensor;
                             // Register as an owner of a sensor here - see below
                             [weak registerOwner];
                         } else {
                             NSLog(@"failed connect: %d", result);
                             connecting = NO;
                         }
                     }];

        }
    }
}
```

4) __AUTHORIZATION OF OWNERSHIP__

To "own" a sensor means that a sensor is associated with one and only one account. To avoid any conflicts in ownership all owned sensors are registered in a cloud service. The sensor manager has a method to request authorization of ownership from the cloud service.

```objective-c
- (void)registerOwner
{
    [_sensorManager authorizeOwnSensor: _connectedSensor
                          withPassword: @"<PASSWORD HERE>"
                       completionBlock: ^(LBSensorManagerOwnResult result) {
                           switch (result) {
                               case LBSensorManagerOwnOK:
                                   NSLog(@"own authorized");
                                   break;
                               case LBSensorManagerOwnNetworkUnavailable:
                                   NSLog(@"network error - please try again later");
                                   break;
                               case LBSensorManagerOwnInvalidPassword:
                                   NSLog(@"invalid password");
                                   break;
                               case LBSensorManagerOwnInvalidOwnerName:
                                   NSLog(@"invalid owner name");
                                   break;
                               case LBSensorManagerOwnOwnedByOther:
                                   NSLog(@"somebody else already owns this sensor");
                                   break;
                           }
                       }];
}
```

5) __NOTIFICATION OF CHANGES IN CURRENT ACTIVITY__

Next, we would like to get notified of activity changes as they occur. To do that we need to add a new KVO call and update the `observeValueForKeyPath:` method:

```objective-c
- (void)observeActivities
{
    [_connectedSensor addObserver: self
                       forKeyPath: @"currentActivity"
                          options: NSKeyValueObservingOptionNew
                          context: NULL];
}

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context
{
    if ([keyPath isEqualToString: @"sensors"]) [self sensorsUpdated];
    if ([keyPath isEqualToString: @"currentActivity"]) [self activityUpdated];
}

- (void)activityUpdated
{
    // Log changes from walking to running
    if ([_connectedSensor.currentActivity isEqualToString: LBSensorActivityWalk]) {
        NSLog(@"now walking");
    }
    if ([_connectedSensor.currentActivity isEqualToString: LBSensorActivityRun]) {
        NSLog(@"now running");
    }
}
@end
```

6) __OPTIONAL: ACCESSING HISTORICAL ACTIVITY DATA__

Optionally, we can access information about how much time we spent on any given activity. To to that we need to update the `observeActivities` method to record activity data as it is received and implement a query for activities over time:

```objective-c
- (void)observeActivities
{
    [_connectedSensor addObserver: self
                       forKeyPath: @"currentActivity"
                          options: NSKeyValueObservingOptionNew
                          context: NULL];
    // Tell the storage manager to start storing activities received from a sensor
    [_storageManager startStoringActivitiesFromSensor: _connectedSensor];
}

- (void)requestWalkTimeForToday
{
    NSCalendar *calendar = [NSCalendar currentCalendar];
    NSDateComponents *components = [calendar components: NSYearCalendarUnit | NSMonthCalendarUnit |  NSDayCalendarUnit
                                               fromDate: [NSDate date]];
    
    [_storageManager.activityStorage aggregatedActivitiesForYear: components.year
                                                           month: components.month
                                                             day: components.day
                                                            mask: LBActivityStorageActivityMaskWalk
                                                      usingBlock: ^(LBPeriodActivities *activities, NSError *error) {
                                                          float walkTime = activities.walk * activities.duration;
                                                          NSLog(@"walked for %f hours today", walkTime / 3600.0f);
                                                      }];
}
```

7) __CAVEATS__

The LUMOKit uses the LB* and ZO* class prefixes internally. These prefixes should be reserved for LUMOKit use to avoid class name collisions.

