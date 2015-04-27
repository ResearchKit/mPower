// 
//  APHSpatialSpanMemoryGameViewController.m 
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
 
#import "APHSpatialSpanMemoryGameViewController.h"
#import "APHAppDelegate.h"

static  NSString       *kTaskViewControllerTitle      = @"Memory Activity";

static  NSString       *kMemorySpanTitle              = @"Memory Activity";

static  NSString       *kConclusionStepIdentifier     = @"conclusion";

NSString * const kSpatialMemoryScoreSummaryKey = @"spatialMemoryScoreSummaryKey";

static  NSInteger       kInitialSpan                  =  3;
static  NSInteger       kMinimumSpan                  =  2;
static  NSInteger       kMaximumSpan                  =  15;
static  NSTimeInterval  kPlaySpeed                    = 1.0;
static  NSInteger       kMaximumTests                 = 5;
static  NSInteger       kMaxConsecutiveFailures       = 3;
static  NSString       *kCustomTargetPluralName       = nil;
static  BOOL            kRequiresReversal             = NO;

static NSString *const kMomentInDay                             = @"momentInDay";

static NSString *const kMomentInDayFormat                       = @"momentInDayFormat";

static NSString *const kMomentInDayFormatTitle                  = @"We would like to understand how your performance on"
                                                                " this activity could be affected by the timing of your medication.";

static NSString *const kInstruction1                            = @"instruction1";
static NSString *const kMomentInDayFormatItemText               = @"When are you performing this Activity?";
static NSString *const kMomentInDayFormatChoiceJustWokeUp       = @"Immediately before Parkinson medication";
static NSString *const kMomentInDayFormatChoiceTookMyMedicine   = @"Just after Parkinson medication (at your best)";
static NSString *const kMomentInDayFormatChoiceEvening          = @"Another time";
static NSString *const kMomentInDayFormatChoiceNone             = @"I don't take Parkinson medications";

static double kMinimumAmountOfTimeToShowSurvey = 20.0 * 60.0;

@interface APHSpatialSpanMemoryGameViewController ()

@end

@implementation APHSpatialSpanMemoryGameViewController

#pragma  mark  -  Task Creation Methods

+ (ORKOrderedTask *)createTask:(APCScheduledTask *) __unused scheduledTask
{
        ORKOrderedTask  *task = [ORKOrderedTask spatialSpanMemoryTaskWithIdentifier:kMemorySpanTitle
            intendedUseDescription:nil
            initialSpan:kInitialSpan
            minimumSpan:kMinimumSpan
            maximumSpan:kMaximumSpan
            playSpeed:kPlaySpeed
            maxTests:kMaximumTests
            maxConsecutiveFailures:kMaxConsecutiveFailures
            customTargetImage:nil
            customTargetPluralName:kCustomTargetPluralName
            requireReversal:kRequiresReversal
            options:ORKPredefinedTaskOptionNone];
    
    [task.steps[3] setTitle:NSLocalizedString(@"Thank You!", nil)];
    [task.steps[3] setText:NSLocalizedString(@"The results of this activity can be viewed on the dashboard", nil)];

    [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
    
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

        NSMutableArray *spatialSteps = [task.steps mutableCopy];
        
        [spatialSteps insertObject:step
                             atIndex:1];
        task = [[ORKOrderedTask alloc] initWithIdentifier:kTaskViewControllerTitle
                                                    steps:spatialSteps];
    }
    return  task;
}

#pragma  mark  -  Task View Controller Delegate Methods

- (void)taskViewController:(ORKTaskViewController *) __unused taskViewController stepViewControllerWillAppear:(ORKStepViewController *)stepViewController
{
    
    if ([stepViewController.step.identifier isEqualToString:kConclusionStepIdentifier]) {
        [[UIView appearance] setTintColor:[UIColor appTertiaryColor1]];
    }
    [stepViewController.step setTitle:@"Good Job!"];
  
    if ([stepViewController.step.identifier isEqualToString:kInstruction1]) {
        UILabel *label = ((UILabel *)((UIView *)((UIView *)((UIView *) ((UIScrollView *)stepViewController.view.subviews[0]).subviews[0]).subviews[0]).subviews[0]).subviews[2]);
        label.text = NSLocalizedString(@"Some of the flowers will light up one at a time. Tap those flowers in the same order they lit up.\n\nTo begin, tap Next, then watch closely.", @"Instruction text for memory activity in Parkinson");
    }
}

- (void) taskViewController: (ORKTaskViewController *) taskViewController
        didFinishWithReason: (ORKTaskViewControllerFinishReason)reason
                      error: (NSError *) error
{
    [[UIView appearance] setTintColor: [UIColor appPrimaryColor]];

    if (reason == ORKTaskViewControllerFinishReasonFailed && error != nil)
    {
        APCLogError2 (error);
    } else if (reason == ORKTaskViewControllerFinishReasonCompleted) {
        APHAppDelegate *appDelegate = (APHAppDelegate *) [UIApplication sharedApplication].delegate;
        appDelegate.dataSubstrate.currentUser.taskCompletion = [NSDate date];
        [[UIView appearance] setTintColor:[UIColor appPrimaryColor]];
    }
    
    [super taskViewController: taskViewController
          didFinishWithReason: reason
                        error: error];
}

#pragma mark - Results for Dashboard

- (NSString *)createResultSummary
{
    ORKTaskResult  *taskResults = self.result;
    self.createResultSummaryBlock = ^(NSManagedObjectContext * context) {
        ORKSpatialSpanMemoryResult  *memoryResults = nil;
        BOOL  found = NO;
        for (ORKStepResult  *stepResult  in  taskResults.results) {
            if (stepResult.results.count > 0) {
                for (id  object  in  stepResult.results) {
                    if ([object isKindOfClass:[ORKSpatialSpanMemoryResult class]] == YES) {
                        found = YES;
                        memoryResults = object;
                        break;
                    }
                }
                if (found == YES) {
                    break;
                }
            }
        }
        
        // Create the summary dictionary
        NSDictionary *summary = @{kSpatialMemoryScoreSummaryKey: @(memoryResults.score)};
        
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

#pragma  mark  -  View Controller Methods

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationBar.topItem.title = NSLocalizedString(kTaskViewControllerTitle, nil);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end
