// 
//  APHIntervalTappingTaskViewController.m 
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
 
#import "APHIntervalTappingTaskViewController.h"
#import "APHIntervalTappingRecorderDataKeys.h"
#import <PDScores/PDScores.h>
#import "ConverterForPDScores.h"
#import <AVFoundation/AVFoundation.h>

    //
    //        Step Identifiers
    //
static NSString *const kIntroductionStepIdentifier    = @"instruction";
static NSString *const kInstruction1StepIdentifier    = @"instruction1";
static NSString *const kTapTappingStepIdentifier      = @"tapping";
static NSString *const kConclusionStepIdentifier      = @"conclusion";

static  NSString       *kTaskViewControllerTitle      = @"Tapping Activity";
static  NSString       *kIntervalTappingTitle         = @"Tapping Activity";

static NSString * const kItemKey                    = @"item";
static NSString * const kIdentifierKey              = @"identifier";
static NSString * const kStartDateKey               = @"startDate";
static NSString * const kEndDateKey                 = @"endDate";
static NSString * const kUserInfoKey                = @"userInfo";

//
//    Interval Tapping Dictionary Keys
//
static  NSString  *const  kTappingViewSizeKey                           = @"TappingViewSize";
static  NSString  *const  kButtonRectLeftKey                            = @"ButtonRectLeft";
static  NSString  *const  kButtonRectRightKey                           = @"ButtonRectRight";
static  NSString  *const  kTappingSamplesKey                            = @"TappingSamples";
static  NSString  *const  kTappedButtonIdKey                            = @"TappedButtonId";
static  NSString  *const  kTappedButtonNoneKey                          = @"TappedButtonNone";
static  NSString  *const  kTappedButtonLeftKey                          = @"TappedButtonLeft";
static  NSString  *const  kTappedButtonRightKey                         = @"TappedButtonRight";
static  NSString  *const  kTapTimeStampKey                              = @"TapTimeStamp";
static  NSString  *const  kTapCoordinateKey                             = @"TapCoordinate";
static  NSString  *const  kAPCTappingResultsFileName                    = @"tapping_results.json";

static  NSTimeInterval  kTappingStepCountdownInterval = 20.0;

@interface APHIntervalTappingTaskViewController  ( ) <NSObject>

@property  (nonatomic, assign)  BOOL                 preferStatusBarShouldBeHidden;

@end

@implementation APHIntervalTappingTaskViewController

#pragma  mark  -  Task Creation Methods

+ (ORKOrderedTask *)createTask:(APCScheduledTask *) __unused scheduledTask
{
    ORKOrderedTask  *task = [ORKOrderedTask twoFingerTappingIntervalTaskWithIdentifier:kIntervalTappingTitle
                                                                intendedUseDescription:nil
                                                                              duration:kTappingStepCountdownInterval
                                                                                options:0];
    
    [task.steps[0] setText:NSLocalizedString(@"Speed of finger tapping can reflect severity of motor symptoms in Parkinson disease. "
                                             @"This activity measures your tapping speed. Your medical provider may measure this differently.", nil)];
    [task.steps[0] setDetailText:@""];
    
    [task.steps[3] setTitle:NSLocalizedString(kConclusionStepThankYouTitle, nil)];
    [task.steps[3] setText:NSLocalizedString(kConclusionStepViewDashboard, nil)];
    
    ORKOrderedTask  *replacementTask = [self modifyTaskWithPreSurveyStepIfRequired:task andTitle:(NSString *)kIntervalTappingTitle];

    [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
    
    return  replacementTask;
}


#pragma  mark  -  Results For Dashboard

- (NSString *)createResultSummary
{
    ORKTaskResult  *taskResults = self.result;
    self.createResultSummaryBlock = ^(NSManagedObjectContext * context) {
        ORKTappingIntervalResult  *tapsterResults = nil;
        BOOL  foundIntervalResult = NO;
        for (ORKStepResult  *stepResult  in  taskResults.results) {
            if (stepResult.results.count > 0) {
                for (id  object  in  stepResult.results) {
                    if ([object isKindOfClass:[ORKTappingIntervalResult class]] == YES) {
                        foundIntervalResult = YES;
                        tapsterResults = object;
                        break;
                    }
                }
                if (foundIntervalResult == YES) {
                    break;
                }
            }
        }
        
        NSPredicate *predicate = [NSPredicate predicateWithFormat:@"SELF = 0"];
        NSArray *allSamples = [tapsterResults.samples valueForKey:@"buttonIdentifier"];
        NSArray *tapSamples = [allSamples filteredArrayUsingPredicate:predicate];
        
        NSArray * totalScore = [ConverterForPDScores convertTappings:tapsterResults];
        double scoreSummary = [PDScores scoreFromTappingTest:totalScore];
        
        scoreSummary = isnan(scoreSummary) ? 0 : scoreSummary;
        
        NSUInteger  numberOfSamples = 0;
        NSDictionary  *summary = nil;
        if (tapsterResults == nil) {
            summary = @{ kSummaryNumberOfRecordsKey : @(numberOfSamples), kScoreSummaryOfRecordsKey : @(scoreSummary)};
        } else {
            numberOfSamples = allSamples.count - tapSamples.count;
            summary = @{ kSummaryNumberOfRecordsKey : @(numberOfSamples), kScoreSummaryOfRecordsKey : @(scoreSummary)};
        }
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
    return nil;
}

#pragma  mark  -  Task View Controller Delegate Methods

- (void)taskViewController:(ORKTaskViewController *) __unused taskViewController stepViewControllerWillAppear:(ORKStepViewController *)stepViewController
{
    if ([stepViewController.step.identifier isEqualToString:kInstruction1StepIdentifier] == YES) {
        UILabel *label = ((UILabel *)((UIView *)((UIView *)((UIView *) ((UIScrollView *)stepViewController.view.subviews[0]).subviews[0]).subviews[0]).subviews[0]).subviews[2]);
        label.text = NSLocalizedString(@"Rest your phone on a flat surface. "
                                       @"Then use two fingers on the same hand to alternately tap the buttons that appear. "
                                       @"Keep tapping for 20 seconds and time your taps to be as consistent as possible.\n\n"
                                       @"Tap Next to begin the test.",
                                       @"Instruction text for tapping activity in Parkinson");
    }
    
    if ([stepViewController.step.identifier isEqualToString:kTapTappingStepIdentifier] == YES) {
        self.preferStatusBarShouldBeHidden = YES;
        [[UIApplication sharedApplication] setStatusBarHidden: YES];
    }
    
    if ([stepViewController.step.identifier isEqualToString:kConclusionStepIdentifier] == YES) {
        self.preferStatusBarShouldBeHidden = NO;
        [[UIApplication sharedApplication] setStatusBarHidden: NO];
        [[UIView appearance] setTintColor:[UIColor appTertiaryColor1]];
    }
}

- (void)taskViewController:(ORKTaskViewController *)taskViewController didFinishWithReason:(ORKTaskViewControllerFinishReason)reason error:(nullable NSError *)error
{
    if (reason == ORKTaskViewControllerFinishReasonFailed) {
        if (error != nil) {
            APCLogError2 (error);
        }
    }
    [super taskViewController:taskViewController didFinishWithReason:reason error:error];
}

#pragma  mark  -  View Controller Methods

- (BOOL)prefersStatusBarHidden
{
    return  self.preferStatusBarShouldBeHidden;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationBar.topItem.title = NSLocalizedString(kTaskViewControllerTitle, nil);
    
    self.preferStatusBarShouldBeHidden = NO;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

/*********************************************************************************/
#pragma mark - Add Task-Specific Results — Interval Tapping
/*********************************************************************************/

- (void)addTappingResultsToArchive:(ORKTappingIntervalResult *)result
{
    NSMutableDictionary  *rawTappingResults = [NSMutableDictionary dictionary];
    
    NSString  *tappingViewSize = NSStringFromCGSize(result.stepViewSize);
    rawTappingResults[kTappingViewSizeKey] = tappingViewSize;
    
    rawTappingResults[kStartDateKey] = result.startDate;
    rawTappingResults[kEndDateKey]   = result.endDate;
    
    NSString  *leftButtonRect = NSStringFromCGRect(result.buttonRect1);
    rawTappingResults[kButtonRectLeftKey] = leftButtonRect;
    
    NSString  *rightButtonRect = NSStringFromCGRect(result.buttonRect2);
    rawTappingResults[kButtonRectRightKey] = rightButtonRect;
    
    NSArray  *samples = result.samples;
    NSMutableArray  *sampleResults = [NSMutableArray array];
    for (ORKTappingSample *sample  in  samples) {
        NSMutableDictionary  *aSampleDictionary = [NSMutableDictionary dictionary];
        
        aSampleDictionary[kTapTimeStampKey]     = @(sample.timestamp);
        
        aSampleDictionary[kTapCoordinateKey]   = NSStringFromCGPoint(sample.location);
        
        if (sample.buttonIdentifier == ORKTappingButtonIdentifierNone) {
            aSampleDictionary[kTappedButtonIdKey] = kTappedButtonNoneKey;
        } else if (sample.buttonIdentifier == ORKTappingButtonIdentifierLeft) {
            aSampleDictionary[kTappedButtonIdKey] = kTappedButtonLeftKey;
        } else if (sample.buttonIdentifier == ORKTappingButtonIdentifierRight) {
            aSampleDictionary[kTappedButtonIdKey] = kTappedButtonRightKey;
        }
        [sampleResults addObject:aSampleDictionary];
    }
    rawTappingResults[kTappingSamplesKey] = sampleResults;
    rawTappingResults[kItemKey] = kAPCTappingResultsFileName;
    
    NSDictionary *serializableData = [APCJSONSerializer serializableDictionaryFromSourceDictionary: rawTappingResults];
    
    [self.archive insertIntoArchive:serializableData filename:kAPCTappingResultsFileName];
    
}


@end
