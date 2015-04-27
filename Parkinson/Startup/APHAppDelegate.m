// 
//  APHAppDelegate.m 
//  mPower 
// 
// Copyright (c) 2015, Sage Bionetworks. All rights reserved. 
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1.  Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2.  Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// 3.  Neither the name of the copyright holder(s) nor the names of any contributors 
// may be used to endorse or promote products derived from this software without 
// specific prior written permission. No license is granted to the trademarks of 
// the copyright holders even if such marks are included in this software. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
 
@import APCAppCore;
#import "APHAppDelegate.h"
#import "APHProfileExtender.h"


static NSString *const kWalkingActivitySurveyIdentifier             = @"4-APHTimedWalking-80F09109-265A-49C6-9C5D-765E49AAF5D9";
static NSString *const kVoiceActivitySurveyIdentifier               = @"3-APHPhonation-C614A231-A7B7-4173-BDC8-098309354292";
static NSString *const kTappingActivitySurveyIdentifier             = @"2-APHIntervalTapping-7259AC18-D711-47A6-ADBD-6CFCECDED1DF";
static NSString *const kMemoryActivitySurveyIdentifier              = @"7-APHSpatialSpanMemory-4A04F3D0-AC05-11E4-AB27-0800200C9A66";
static NSString *const kPDSurveyIdentifier                          = @"6-PDQ8-20EF83D2-E461-4C20-9024-F43FCAAAF4C8";
static NSString *const kWeeklySurveyIdentifier                      = @"5-MDSUPDRS-20EF82D1-E461-4C20-9024-F43FCAAAF4C8";
static NSString *const kMyThoughtsSurveyIdentifier                  = @"8-MyThoughts-12ffde40-1551-4b48-aae2-8fef38d61b61";

/*********************************************************************************/
#pragma mark - Initializations Options
/*********************************************************************************/
static NSString *const kStudyIdentifier             = @"studyname";
static NSString *const kAppPrefix                   = @"studyname";
static NSString* const  kConsentPropertiesFileName  = @"APHConsentSection";

static NSString *const kVideoShownKey = @"VideoShown";


static NSString *const kJsonScheduleStringKey           = @"scheduleString";
static NSString *const kJsonTasksKey                    = @"tasks";
static NSString *const kJsonScheduleTaskIDKey           = @"taskID";
static NSString *const kJsonSchedulesKey                = @"schedules";


static NSString *const kPDQ8TaskIdentifier              = @"6-PDQ8-20EF83D2-E461-4C20-9024-F43FCAAAF4C8";
static NSInteger const kPDQ8TaskOffset                  = 2;
static NSString *const kMDSUPDRS                        = @"5-MDSUPDRS-20EF82D1-E461-4C20-9024-F43FCAAAF4C8";
static NSInteger const kMDSUPDRSOffset                  = 1;

static NSInteger const kMonthOfDayObject                = 2;






@interface APHAppDelegate ()
@property (nonatomic, strong) APHProfileExtender* profileExtender;

@end

@implementation APHAppDelegate

- (void) setUpInitializationOptions
{
    NSDictionary *permissionsDescriptions = @{
                                              @(kSignUpPermissionsTypeLocation) : NSLocalizedString(@"Using your GPS enables the app to accurately determine distances travelled. Your actual location will never be shared.", @""),
                                              @(kSignUpPermissionsTypeCoremotion) : NSLocalizedString(@"Using the motion co-processor allows the app to determine your activity, helping the study better understand how activity level may influence disease.", @""),
                                              @(kSignUpPermissionsTypeMicrophone) : NSLocalizedString(@"Access to microphone is required for your Voice Recording Activity.", @""),
                                              @(kSignUpPermissionsTypeLocalNotifications) : NSLocalizedString(@"Allowing notifications enables the app to show you reminders.", @""),
                                              @(kSignUpPermissionsTypeHealthKit) : NSLocalizedString(@"On the next screen, you will be prompted to grant mPower access to read and write some of your general and health information, such as height, weight and steps taken so you don't have to enter it again.", @""),
                                                  };
    
    NSMutableDictionary * dictionary = [super defaultInitializationOptions];
    dictionary = [self updateOptionsFor5OrOlder:dictionary];
    [dictionary addEntriesFromDictionary:@{
                                           kStudyIdentifierKey                  : kStudyIdentifier,
                                           kAppPrefixKey                        : kAppPrefix,
                                           kBridgeEnvironmentKey                : @(SBBEnvironmentProd),
                                           kHKReadPermissionsKey                : @[
                                                   HKQuantityTypeIdentifierBodyMass,
                                                   HKQuantityTypeIdentifierHeight,
                                                   HKQuantityTypeIdentifierStepCount
                                                   ],
                                           kHKWritePermissionsKey                : @[
                                                   ],
                                           kAppServicesListRequiredKey           : @[
                                                   @(kSignUpPermissionsTypeLocation),
                                                   @(kSignUpPermissionsTypeCoremotion),
                                                   @(kSignUpPermissionsTypeMicrophone),
                                                   @(kSignUpPermissionsTypeLocalNotifications)
                                                   ],
                                           kAppServicesDescriptionsKey : permissionsDescriptions,
                                           kAppProfileElementsListKey            : @[
                                                   @(kAPCUserInfoItemTypeEmail),
                                                   @(kAPCUserInfoItemTypeDateOfBirth),
                                                   @(kAPCUserInfoItemTypeBiologicalSex),
                                                   @(kAPCUserInfoItemTypeHeight),
                                                   @(kAPCUserInfoItemTypeWeight),
                                                   @(kAPCUserInfoItemTypeWakeUpTime),
                                                   @(kAPCUserInfoItemTypeSleepTime),
                                                   ],
                                           kShareMessageKey : NSLocalizedString(@"Please take a look at Parkinson mPower, a research study app about Parkinson Disease.  Download it for iPhone at http://apple.co/1FO7Bsi", nil)
                                           }];
    
    self.initializationOptions = dictionary;
    
    self.profileExtender = [[APHProfileExtender alloc] init];
}

-(void)setUpTasksReminder{
    
    APCTaskReminder *walkingActivityReminder = [[APCTaskReminder alloc]initWithTaskID:kWalkingActivitySurveyIdentifier reminderBody:NSLocalizedString(@"Walking Activity", nil)];
    APCTaskReminder *voiceActivityReminder = [[APCTaskReminder alloc]initWithTaskID:kVoiceActivitySurveyIdentifier reminderBody:NSLocalizedString(@"Voice Activity", nil)];
    APCTaskReminder *tappingActivityReminder = [[APCTaskReminder alloc]initWithTaskID:kTappingActivitySurveyIdentifier reminderBody:NSLocalizedString(@"Tapping Activity", nil)];
    APCTaskReminder *memoryActivityReminder = [[APCTaskReminder alloc]initWithTaskID:kMemoryActivitySurveyIdentifier reminderBody:NSLocalizedString(@"Memory Activity", nil)];
    APCTaskReminder *pdSurveyReminder = [[APCTaskReminder alloc]initWithTaskID:kPDSurveyIdentifier reminderBody:NSLocalizedString(@"PD Survey", nil)];
    APCTaskReminder *weeklySurveyReminder = [[APCTaskReminder alloc]initWithTaskID:kWeeklySurveyIdentifier reminderBody:NSLocalizedString(@"Weekly Survey", nil)];
    APCTaskReminder *myThoughtsSurveyReminder = [[APCTaskReminder alloc]initWithTaskID:kMyThoughtsSurveyIdentifier reminderBody:NSLocalizedString(@"My Thoughts", nil)];
    
    [self.tasksReminder manageTaskReminder:walkingActivityReminder];
    [self.tasksReminder manageTaskReminder:voiceActivityReminder];
    [self.tasksReminder manageTaskReminder:tappingActivityReminder];
    [self.tasksReminder manageTaskReminder:memoryActivityReminder];
    [self.tasksReminder manageTaskReminder:pdSurveyReminder];
    [self.tasksReminder manageTaskReminder:weeklySurveyReminder];
    [self.tasksReminder manageTaskReminder:myThoughtsSurveyReminder];
    
}

- (void) setUpAppAppearance
{
    [APCAppearanceInfo setAppearanceDictionary:@{
                                                 kPrimaryAppColorKey : [UIColor colorWithRed:255 / 255.0f green:0.0 blue:56 / 255.0f alpha:1.000],
                                                 @"2-APHIntervalTapping-7259AC18-D711-47A6-ADBD-6CFCECDED1DF" : [UIColor appTertiaryPurpleColor],
                                                 @"7-APHSpatialSpanMemory-4A04F3D0-AC05-11E4-AB27-0800200C9A66" : [UIColor appTertiaryRedColor],
                                                 @"3-APHPhonation-C614A231-A7B7-4173-BDC8-098309354292" : [UIColor appTertiaryBlueColor],
                                                 @"4-APHTimedWalking-80F09109-265A-49C6-9C5D-765E49AAF5D9" : [UIColor appTertiaryYellowColor],
                                                 @"1-EnrollmentSurvey-20EF83D2-E461-4C20-9024-F43FCAAAF4C3": [UIColor lightGrayColor],
                                                 @"6-PDQ8-20EF83D2-E461-4C20-9024-F43FCAAAF4C8": [UIColor lightGrayColor],
                                                 @"5-MDSUPDRS-20EF82D1-E461-4C20-9024-F43FCAAAF4C8": [UIColor lightGrayColor],
                                                 @"8-MyThoughts-12ffde40-1551-4b48-aae2-8fef38d61b61": [UIColor lightGrayColor],
                                                 @"9-Feedback-394348ce-ca4f-4abe-b97e-fedbfd7ffb8e": [UIColor lightGrayColor],
                                                 @"a-APHMedicationTracker-20EF8ED2-E461-4C20-9024-F43FCAAAF4C3": [UIColor colorWithRed:0.933
                                                                                                                               green:0.267
                                                                                                                                blue:0.380
                                                                                                                               alpha:1.000]
                                                 }];
    [[UINavigationBar appearance] setTintColor:[UIColor appPrimaryColor]];
    [[UINavigationBar appearance] setTitleTextAttributes: @{
                                                            NSForegroundColorAttributeName : [UIColor appSecondaryColor2],
                                                            NSFontAttributeName : [UIFont appNavBarTitleFont]
                                                            }];
    [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
    
    //  Enable server bypass
    self.dataSubstrate.parameters.bypassServer = YES;
}

- (id <APCProfileViewControllerDelegate>) profileExtenderDelegate {
    
    return self.profileExtender;
}

- (void) showOnBoarding
{
    [super showOnBoarding];
    
    [self showStudyOverview];
}

- (void) showStudyOverview
{
    APCStudyOverviewViewController *studyController = [[UIStoryboard storyboardWithName:@"APCOnboarding" bundle:[NSBundle appleCoreBundle]] instantiateViewControllerWithIdentifier:@"StudyOverviewVC"];
    [self setUpRootViewController:studyController];
}

- (BOOL) isVideoShown
{
    return [[NSUserDefaults standardUserDefaults] boolForKey:kVideoShownKey];
}

- (NSMutableDictionary *) updateOptionsFor5OrOlder:(NSMutableDictionary *)initializationOptions {
    if (![APCDeviceHardware isiPhone5SOrNewer]) {
        [initializationOptions setValue:@"APHTasksAndSchedules_NoM7" forKey:kTasksAndSchedulesJSONFileNameKey];
    }
    return initializationOptions;
}

- (NSArray *)allSetTextBlocks
{
    NSArray *allSetBlockOfText = nil;
    
    NSString *activitiesAdditionalText = NSLocalizedString(@"Please perform the activites each day when you are at your lowest before you take your Parkinson medications, after your medications take effect, and then a third time during the day.",
                                                 @"Please perform the activites each day when you are at your lowest before you take your Parkinson medications, after your medications take effect, and then a third time during the day.");
    allSetBlockOfText = @[@{kAllSetActivitiesTextAdditional: activitiesAdditionalText}];
    
    return allSetBlockOfText;
}

/*********************************************************************************/
#pragma mark - Datasubstrate Delegate Methods
/*********************************************************************************/
- (void) setUpCollectors
{
    //
    // Set up location tracker
    //
    APCCoreLocationTracker * locationTracker = [[APCCoreLocationTracker alloc] initWithIdentifier: @"locationTracker"
                                                                           deferredUpdatesTimeout: 60.0 * 60.0
                                                                            andHomeLocationStatus: APCPassiveLocationTrackingHomeLocationUnavailable];
    
    if (locationTracker != nil)
    {
        [self.passiveDataCollector addTracker: locationTracker];
    }
}

/*********************************************************************************/
#pragma mark - APCOnboardingDelegate Methods
/*********************************************************************************/

- (APCScene *)inclusionCriteriaSceneForOnboarding:(APCOnboarding *) __unused onboarding
{
    APCScene *scene = [APCScene new];
    scene.name = @"APHInclusionCriteriaViewController";
    scene.storyboardName = @"APHOnboarding";
    scene.bundle = [NSBundle mainBundle];
    
    return scene;
}


/*********************************************************************************/
#pragma mark - Consent
/*********************************************************************************/

- (ORKTaskViewController *)consentViewController
{
    APCConsentTask*         task = [[APCConsentTask alloc] initWithIdentifier:@"Consent"
                                                           propertiesFileName:kConsentPropertiesFileName];
    ORKTaskViewController*  consentVC = [[ORKTaskViewController alloc] initWithTask:task
                                                                        taskRunUUID:[NSUUID UUID]];
    
    return consentVC;
}

- (NSDictionary *) tasksAndSchedulesWillBeLoaded {
    
    NSString                    *resource = [[NSBundle mainBundle] pathForResource:self.initializationOptions[kTasksAndSchedulesJSONFileNameKey]
                                                                            ofType:@"json"];
    
    NSData                      *jsonData = [NSData dataWithContentsOfFile:resource];
    NSError                     *error;
    NSDictionary                *dictionary = [NSJSONSerialization JSONObjectWithData:jsonData
                                                                              options:NSJSONReadingMutableContainers
                                                                                error:&error];
    if (dictionary == nil) {
        APCLogError2 (error);
    }
    
    NSArray                     *schedules = [dictionary objectForKey:kJsonSchedulesKey];
    NSMutableDictionary         *newDictionary = [dictionary mutableCopy];
    NSMutableArray              *newSchedulesArray = [NSMutableArray new];
    
    for (NSDictionary *schedule in schedules) {
        
        NSString *taskIdentifier = [schedule objectForKey:kJsonScheduleTaskIDKey];
        
        if ( [taskIdentifier isEqualToString: kPDQ8TaskIdentifier] || [taskIdentifier isEqualToString: kMDSUPDRS])
        {
            NSDate              *date = [NSDate date];
            NSDateComponents    *dateComponent = [[NSDateComponents alloc] init];
            

            
            NSDate              *newDate = [[NSCalendar currentCalendar] dateByAddingComponents:dateComponent
                                                                                         toDate:date
                                                                                        options:0];
            
            NSCalendar          *cal = [NSCalendar currentCalendar];
            
            NSDateComponents    *components = [cal components:(NSCalendarUnitDay | NSCalendarUnitMonth)
                                                     fromDate:newDate];
            
            if ( [taskIdentifier isEqualToString: kPDQ8TaskIdentifier])
            {
                [components setDay:kPDQ8TaskOffset];
                
                newDate = [[NSCalendar currentCalendar] dateByAddingComponents:components
                                                                        toDate:[NSDate date]
                                                                       options:0];
                
                components = [cal components:(NSCalendarUnitDay | NSCalendarUnitMonth)
                                    fromDate:newDate];
            } else if ([taskIdentifier isEqualToString: kMDSUPDRS]) {
                [components setDay:kMDSUPDRSOffset];
                
                newDate = [[NSCalendar currentCalendar] dateByAddingComponents:components
                                                                        toDate:[NSDate date]
                                                                       options:0];
                
                components = [cal components:(NSCalendarUnitDay | NSCalendarUnitMonth)
                                    fromDate:newDate];
            }
            
            NSString            *scheduleString = [schedule objectForKey:kJsonScheduleStringKey];
            NSMutableArray      *scheduleObjects = [[scheduleString componentsSeparatedByString:@" "] mutableCopy];
            
            
            [scheduleObjects replaceObjectAtIndex:kMonthOfDayObject withObject:@([components day])];

            
            NSString            *newScheduleString = [scheduleObjects componentsJoinedByString:@" "];
            
            [schedule setValue:newScheduleString
                        forKey:kJsonScheduleStringKey];
            
            [newSchedulesArray addObject:schedule];
        }
        else {
            [newSchedulesArray addObject:schedule];
        }
    }
    
    [newDictionary setValue:[dictionary objectForKey:kJsonTasksKey]
                     forKey:kJsonTasksKey];
    
    [newDictionary setValue:newSchedulesArray
                     forKey:kJsonSchedulesKey];
    
    return newDictionary;
}


@end
