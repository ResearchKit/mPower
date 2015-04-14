// 
//  APHWalkingTaskViewController.m 
//  mPower 
// 
// Copyright (c) 2015, Sage Bionetworks, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this 
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors 
// may be used to endorse or promote products derived from this software without 
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
 
#import "APHWalkingTaskViewController.h"
#import <HealthKit/HealthKit.h>
#import <AVFoundation/AVFoundation.h>
#import "ConverterForPDScores.h"
#import "PDScores.h"
#import "APHAppDelegate.h"

typedef  enum  _WalkingStepOrdinals
{
    WalkingStepOrdinalsInstructionStep = 0,
    WalkingStepOrdinalsCountdownStep,
    WalkingStepOrdinalsWalkOutStep,
    WalkingStepOrdinalsWalkBackStep,
    WalkingStepOrdinalsStandStillStep,
    WalkingStepOrdinalsConclusionStep,
}  WalkingStepOrdinals;

static NSString *const kMomentInDay                             = @"momentInDay";
static NSString *const kMomentInDayFormat                       = @"momentInDayFormat";
static NSString *const kMomentInDayFormatTitle                  = @"We would like to understand how your performance on"
                                                                " this activity could be affected by the timing of your medication.";
static NSString *const kMomentInDayFormatItemText               = @"When are you performing this Activity?";
static NSString *const kMomentInDayFormatChoiceJustWokeUp       = @"Immediately before Parkinson medication";
static NSString *const kMomentInDayFormatChoiceTookMyMedicine   = @"Just after Parkinson medication (at your best)";
static NSString *const kMomentInDayFormatChoiceEvening          = @"Another time";
static NSString *const kMomentInDayFormatChoiceNone             = @"I don't take Parkinson medications";

static double kMinimumAmountOfTimeToShowSurvey = 20.0 * 60.0;

static NSString* const  kFileResultsKey             = @"items";
static NSString* const  kNumberOfStepsTotalOnReturn = @"numberOfSteps";
static NSString* const  kNumberOfStepsTotalOnReturnKey = @"numberOfSteps";
static NSString* const  kPedometerPrefixFileIdentifier = @"pedometer";

static  NSString       *kWalkingActivityTitle     = @"Walking Activity";

static  NSUInteger      kNumberOfStepsPerLeg      = 20;
static  NSTimeInterval  kStandStillDuration       = 30.0;

static  NSString       *kConclusionStepIdentifier = @"conclusion";
static  NSString       *kCountdownStepIdentifier = @"countdown";
static  NSString       *kWalkingOutboundStepIdentifier = @"walking.outbound";
static  NSString       *kWalkingReturnStepIdentifier = @"walking.return";
static  NSString       *kWalkingRestStepIdentifier = @"walking.rest";

NSString  *kScoreForwardGainRecordsKey = @"ScoreForwardGainRecords";
NSString  *kScoreBackwardGainRecordsKey = @"ScoreBackwardGainRecords";
NSString  *kScorePostureRecordsKey = @"ScorePostureRecords";
NSString * const kGaitScoreKey = @"GaitScoreKey";

@interface APHWalkingTaskViewController  ( )

@property  (nonatomic, assign)  WalkingStepOrdinals  walkingStepOrdinal;

@property  (nonatomic, strong)  NSDate              *startCollectionDate;
@property  (nonatomic, strong)  NSDate              *endCollectionDate;

@property  (nonatomic, assign)  NSInteger  __block   collectedNumberOfSteps;

@end

@implementation APHWalkingTaskViewController

#pragma  mark  -  Initialisation

+ (ORKOrderedTask *)createTask:(APCScheduledTask *) __unused scheduledTask
{
    ORKOrderedTask  *task = [ORKOrderedTask shortWalkTaskWithIdentifier:kWalkingActivityTitle
                                                 intendedUseDescription:nil
                                                    numberOfStepsPerLeg:kNumberOfStepsPerLeg
                                                           restDuration:kStandStillDuration
                                                                options:ORKPredefinedTaskOptionNone];
    [task.steps[0] setText:@"This activity measures your gait (walk) and balance, which can be affected by Parkinson disease."];
    [task.steps[0] setDetailText:@"Please do not continue if you cannot safely walk unassisted."];

    [task.steps[6] setTitle:NSLocalizedString(@"Thank You!", nil)];
    [task.steps[6] setText:NSLocalizedString(@"The results of this activity can be viewed on the dashboard", nil)];

    APHAppDelegate *appDelegate = (APHAppDelegate *) [UIApplication sharedApplication].delegate;
    NSDate *lastCompletionDate = appDelegate.dataSubstrate.currentUser.taskCompletion;
    NSTimeInterval numberOfSecondsSinceTaskCompletion = [[NSDate date] timeIntervalSinceDate: lastCompletionDate];
    
    if (numberOfSecondsSinceTaskCompletion > kMinimumAmountOfTimeToShowSurvey || lastCompletionDate == nil) {
        
        NSMutableArray *stepQuestions = [NSMutableArray array];
        
        
        ORKFormStep *step = [[ORKFormStep alloc] initWithIdentifier:kMomentInDay title:nil text:NSLocalizedString(kMomentInDayFormatTitle, nil)];
        
        step.optional = NO;
        
        
        {
            NSArray *choices = @[
                                 NSLocalizedString(kMomentInDayFormatChoiceJustWokeUp,
                                                   kMomentInDayFormatChoiceJustWokeUp),
                                 NSLocalizedString(kMomentInDayFormatChoiceTookMyMedicine,
                                                   kMomentInDayFormatChoiceTookMyMedicine),
                                 NSLocalizedString(kMomentInDayFormatChoiceEvening,
                                                   kMomentInDayFormatChoiceEvening),
                                 NSLocalizedString(kMomentInDayFormatChoiceNone,
                                                   kMomentInDayFormatChoiceNone)
                                 ];
            
            ORKAnswerFormat *format = [ORKTextChoiceAnswerFormat choiceAnswerFormatWithStyle:ORKChoiceAnswerStyleSingleChoice
                                                                                 textChoices:choices];
            
            ORKFormItem *item = [[ORKFormItem alloc] initWithIdentifier:kMomentInDayFormat
                                                                   text:NSLocalizedString(kMomentInDayFormatItemText, kMomentInDayFormatItemText)
                                                           answerFormat:format];
            [stepQuestions addObject:item];
        }
        
        [step setFormItems:stepQuestions];
        
        NSMutableArray *twoFingerSteps = [task.steps mutableCopy];
        
        [twoFingerSteps insertObject:step
                             atIndex:1];
        
        task = [[ORKOrderedTask alloc] initWithIdentifier:kWalkingActivityTitle
                                                    steps:twoFingerSteps];
    }
    
    return  task;
}

#pragma  mark  -  Create Dashboard Summary Results

- (NSString *)createResultSummary
{
    ORKTaskResult  *taskResults = self.result;
    
    APHWalkingTaskViewController* weakSelf = self;
    
    self.createResultSummaryBlock = ^(NSManagedObjectContext * context) {
        BOOL  found = NO;
        NSURL * urlGaitForward = nil;
        NSURL * urlGaitBackward = nil;
        NSURL * urlPosture = nil;
        for (ORKStepResult  *stepResult  in  taskResults.results) {
            if (stepResult.results.count > 0) {
                for (id  object  in  stepResult.results) {
                    if ([object isKindOfClass:[ORKFileResult class]] == YES) {
                        ORKFileResult * fileResult = object;
                        if ([fileResult.fileURL.absoluteString.lastPathComponent hasPrefix: @"accel_walking.outbound"]) {
                            urlGaitForward = fileResult.fileURL;
                        } else if ([fileResult.fileURL.absoluteString.lastPathComponent hasPrefix: @"accel_walking.return"]) {
                            urlGaitBackward = fileResult.fileURL;
                        } else if ([fileResult.fileURL.absoluteString.lastPathComponent hasPrefix: @"accel_walking.rest"]) {
                            urlPosture = fileResult.fileURL;
                        }
                        found = YES;
                        fileResult = object;
                    }
                }
            }
        }
        
        
        NSArray * forwardSteps = [ConverterForPDScores convertPostureOrGain:urlGaitForward];
        NSArray * backwardSteps = [ConverterForPDScores convertPostureOrGain:urlGaitBackward];
        NSArray * posture = [ConverterForPDScores convertPostureOrGain:urlPosture];
        
        double forwardScores = [PDScores scoreFromGaitTest: forwardSteps];
        double backwardScores = [PDScores scoreFromGaitTest: backwardSteps];
        double postureScores = [PDScores scoreFromPostureTest: posture];
        
        forwardScores = isnan(forwardScores) ? 0 : forwardScores;
        backwardScores = isnan(backwardScores) ? 0 : backwardScores;
        postureScores = isnan(postureScores) ? 0 : postureScores;
        
        double avgScores = (forwardScores + backwardScores + postureScores) / 3;
        
        /************/
        
        NSDictionary* walkingResults = nil;
        
        ORKStepResult* stepResult = (ORKStepResult *)[weakSelf.result resultForIdentifier:kWalkingReturnStepIdentifier];
        
        for (ORKFileResult* fileResult in stepResult.results) {
            NSString* fileString = [fileResult.fileURL lastPathComponent];
            NSArray* nameComponents = [fileString componentsSeparatedByString:@"_"];
            
            if ([[nameComponents objectAtIndex:0] isEqualToString:kPedometerPrefixFileIdentifier])
            {
                walkingResults = [weakSelf computeTotalDistanceForDashboardItem:fileResult.fileURL];
                
            }
        }

        /***********/
        
        NSDictionary  *summary = @{
                                   kGaitScoreKey: @(avgScores),
                                   kScoreForwardGainRecordsKey: @(forwardScores),
                                   kScoreBackwardGainRecordsKey: @(backwardScores),
                                   kScorePostureRecordsKey: @(postureScores),
                                   kNumberOfStepsTotalOnReturnKey : walkingResults == nil ? @0 : [walkingResults objectForKey:kNumberOfStepsTotalOnReturn]
                                  };
        
        NSError  *error = nil;
        NSData  *data = [NSJSONSerialization dataWithJSONObject:summary options:0 error:&error];
        NSString  *contentString = nil;
        if (data == nil) {
            APCLogError2 (error);
        } else {
            contentString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
        }
        
        if (contentString.length > 0)
        {
            [APCResult updateResultSummary:contentString forTaskResult:taskResults inContext:context];
        }
    };
    return  nil;
}

#pragma  mark  -  Task View Controller Delegate Methods

- (void)taskViewController:(ORKTaskViewController *) __unused taskViewController stepViewControllerWillAppear:(ORKStepViewController *)stepViewController
{
    if ([stepViewController.step.identifier isEqualToString:kConclusionStepIdentifier]) {
        
        [[UIView appearance] setTintColor:[UIColor appTertiaryColor1]];
    }
    
    if ([stepViewController.step.identifier isEqualToString: kCountdownStepIdentifier]) {
        self.startCollectionDate = [NSDate date];
    }
    if ([stepViewController.step.identifier isEqualToString: kConclusionStepIdentifier]) {
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
        AVSpeechUtterance  *talk = [AVSpeechUtterance
                                    speechUtteranceWithString:NSLocalizedString(@"You have completed the activity.", @"You have completed the activity.")];
        AVSpeechSynthesizer  *synthesiser = [[AVSpeechSynthesizer alloc] init];
        talk.rate = 0.1;
        [synthesiser speakUtterance:talk];
    }
    self.walkingStepOrdinal = self.walkingStepOrdinal + 1;
}

- (void) taskViewController: (ORKTaskViewController *) taskViewController
        didFinishWithReason: (ORKTaskViewControllerFinishReason) result
                      error: (NSError *) error
{
    [[UIView appearance] setTintColor: [UIColor appPrimaryColor]];

    if (result == ORKTaskViewControllerFinishReasonFailed && error != nil)
    {
        APCLogError2 (error);
    } else if (result == ORKTaskViewControllerFinishReasonCompleted) {
        APHAppDelegate *appDelegate = (APHAppDelegate *) [UIApplication sharedApplication].delegate;
        appDelegate.dataSubstrate.currentUser.taskCompletion = [NSDate date];
        [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
    }

    [super taskViewController: taskViewController
          didFinishWithReason: result
                        error: error];
}

#pragma  mark  -  View Controller Methods

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.walkingStepOrdinal = WalkingStepOrdinalsInstructionStep;
    
    self.navigationBar.topItem.title = NSLocalizedString(kWalkingActivityTitle, nil);
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}


#pragma mark - Helper methods

- (NSDictionary *) computeTotalDistanceForDashboardItem:(NSURL *)fileURL{
    
    NSDictionary*   distanceResults     = [self readFileResultsFor:fileURL];
    NSArray*        locations           = [distanceResults objectForKey:kFileResultsKey];
    
    int lastTotalNumberOfSteps = (int) [[[locations lastObject] objectForKey:kNumberOfStepsTotalOnReturn] integerValue];
    
    return @{@"numberOfSteps" : @(lastTotalNumberOfSteps)};
}


- (NSDictionary *) readFileResultsFor:(NSURL *)fileURL {
    
    NSError*        error       = nil;
    NSString*       contents    = [NSString stringWithContentsOfURL:fileURL encoding:NSUTF8StringEncoding error:&error];
    NSDictionary*   results     = nil;
    
    APCLogError2(error);
    
    if (!error) {
        NSError*    error = nil;
        NSData*     data  = [contents dataUsingEncoding:NSUTF8StringEncoding];
        
        results = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
        
        APCLogError2(error);
    }
    
    return results;
}

@end
