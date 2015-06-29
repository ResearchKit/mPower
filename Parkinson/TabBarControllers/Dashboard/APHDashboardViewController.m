// 
//  APHDashboardViewController.m 
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
 
/* Controllers */
#import "APHDashboardViewController.h"
#import "APHDashboardEditViewController.h"
#import "APHIntervalTappingRecorderDataKeys.h"
#import "APHSpatialSpanMemoryGameViewController.h"
#import "APHWalkingTaskViewController.h"

static NSString * const kAPCBasicTableViewCellIdentifier       = @"APCBasicTableViewCell";
static NSString * const kAPCRightDetailTableViewCellIdentifier = @"APCRightDetailTableViewCell";

@interface APHDashboardViewController ()<UIViewControllerTransitioningDelegate, APCCorrelationsSelectorDelegate>

@property (nonatomic, strong) NSMutableArray *rowItemsOrder;

@property (nonatomic, strong) APCScoring *tapScoring;
@property (nonatomic, strong) APCScoring *gaitScoring;
@property (nonatomic, strong) APCScoring *stepScoring;
@property (nonatomic, strong) APCScoring *memoryScoring;
@property (nonatomic, strong) APCScoring *phonationScoring;

@end

@implementation APHDashboardViewController

#pragma mark - Init

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder]) {
        
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        _rowItemsOrder = [NSMutableArray arrayWithArray:[defaults objectForKey:kAPCDashboardRowItemsOrder]];
        
        if (!_rowItemsOrder.count) {
            _rowItemsOrder = [[NSMutableArray alloc] initWithArray:@[
                                                                     @(kAPHDashboardItemTypeCorrelation),
                                                                     @(kAPHDashboardItemTypeSteps),
                                                                     @(kAPHDashboardItemTypeIntervalTapping),
                                                                     @(kAPHDashboardItemTypeSpatialMemory),@(kAPHDashboardItemTypePhonation),]];
            
            if ([APCDeviceHardware isiPhone5SOrNewer]) {
                [_rowItemsOrder addObject:@(kAPHDashboardItemTypeGait)];
            }
            [defaults setObject:[NSArray arrayWithArray:_rowItemsOrder] forKey:kAPCDashboardRowItemsOrder];
            [defaults synchronize];
            
        }
        
        self.title = NSLocalizedString(@"Dashboard", @"Dashboard");
    }
    
    return self;
}

#pragma mark - LifeCycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(prepareCorrelatedScoring) name:APCScheduleUpdatedNotification object:nil];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    [self prepareScoringObjects];
    [self prepareData];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    self.rowItemsOrder = [NSMutableArray arrayWithArray:[defaults objectForKey:kAPCDashboardRowItemsOrder]];
    
    [self prepareScoringObjects];
    [self prepareData];
}

- (void)updateVisibleRowsInTableView:(NSNotification *) __unused notification
{
    [self prepareData];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - Data

- (void)prepareScoringObjects
{
    self.tapScoring = [[APCScoring alloc] initWithTask:@"2-APHIntervalTapping-7259AC18-D711-47A6-ADBD-6CFCECDED1DF"
                                          numberOfDays:-kNumberOfDaysToDisplay
                                              valueKey:kSummaryNumberOfRecordsKey];
    self.tapScoring.caption = NSLocalizedString(@"Tapping", @"");
    
    self.gaitScoring = [[APCScoring alloc] initWithTask:@"4-APHTimedWalking-80F09109-265A-49C6-9C5D-765E49AAF5D9"
                                           numberOfDays:-kNumberOfDaysToDisplay
                                               valueKey:kGaitScoreKey];
    self.gaitScoring.caption = NSLocalizedString(@"Gait", @"");
    
    self.memoryScoring = [[APCScoring alloc] initWithTask:@"7-APHSpatialSpanMemory-4A04F3D0-AC05-11E4-AB27-0800200C9A66"
                                             numberOfDays:-kNumberOfDaysToDisplay
                                                 valueKey:kSpatialMemoryScoreSummaryKey
                                               latestOnly:NO];
    self.memoryScoring.caption = NSLocalizedString(@"Memory", @"");
    
    self.phonationScoring = [[APCScoring alloc] initWithTask:@"3-APHPhonation-C614A231-A7B7-4173-BDC8-098309354292"
                                                numberOfDays:-kNumberOfDaysToDisplay
                                                    valueKey:kScoreSummaryOfRecordsKey];
    self.phonationScoring.caption = NSLocalizedString(@"Voice", @"");
    
    HKQuantityType *hkQuantity = [HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierStepCount];
    self.stepScoring = [[APCScoring alloc] initWithHealthKitQuantityType:hkQuantity
                                                                    unit:[HKUnit countUnit]
                                                            numberOfDays:-kNumberOfDaysToDisplay];
    self.stepScoring.caption = NSLocalizedString(@"Steps", @"Steps");
    
    if (!self.correlatedScoring) {
        [self prepareCorrelatedScoring];
    }
}

- (void)prepareCorrelatedScoring
{
    self.correlatedScoring = [[APCScoring alloc] initWithTask:@"4-APHTimedWalking-80F09109-265A-49C6-9C5D-765E49AAF5D9"
                                                 numberOfDays:-kNumberOfDaysToDisplay
                                                     valueKey:kGaitScoreKey];
    
    HKQuantityType *hkQuantity = [HKQuantityType quantityTypeForIdentifier:HKQuantityTypeIdentifierStepCount];
    [self.correlatedScoring correlateWithScoringObject:[[APCScoring alloc] initWithHealthKitQuantityType:hkQuantity
                                                                                                    unit:[HKUnit countUnit]
                                                                                            numberOfDays:-kNumberOfDaysToDisplay]];
    
    self.correlatedScoring.caption = NSLocalizedString(@"Data Correlations", nil);
    
    //default series
    self.correlatedScoring.series1Name = self.gaitScoring.caption;
    self.correlatedScoring.series2Name = self.stepScoring.caption;
}

- (void)prepareData
{
    [self.items removeAllObjects];
    
    {
        NSMutableArray *rowItems = [NSMutableArray new];
        
        NSUInteger allScheduledTasks = ((APCAppDelegate *)[UIApplication sharedApplication].delegate).dataSubstrate.countOfAllScheduledTasksForToday;
        NSUInteger completedScheduledTasks = ((APCAppDelegate *)[UIApplication sharedApplication].delegate).dataSubstrate.countOfCompletedScheduledTasksForToday;
        
        {
            APCTableViewDashboardProgressItem *item = [APCTableViewDashboardProgressItem new];
            item.identifier = kAPCDashboardProgressTableViewCellIdentifier;
            item.editable = NO;
            item.progress = (CGFloat)completedScheduledTasks/allScheduledTasks;
            item.caption = NSLocalizedString(@"Activity Completion", @"Activity Completion");
            
            item.info = NSLocalizedString(@"This graph shows the percent of Today's activities that you completed. You can complete more of your tasks in the Activities tab.", @"Dashboard tooltip item info text for Activity Completion in Parkinson");
            
            APCTableViewRow *row = [APCTableViewRow new];
            row.item = item;
            row.itemType = kAPCTableViewDashboardItemTypeProgress;
            [rowItems addObject:row];
        }
        
        for (NSNumber *typeNumber in self.rowItemsOrder) {
            
            APHDashboardItemType rowType = typeNumber.integerValue;
            
            switch (rowType) {
                    
                case kAPHDashboardItemTypeCorrelation:{
                    
                    APCTableViewDashboardGraphItem *item = [APCTableViewDashboardGraphItem new];
                    item.caption = NSLocalizedString(@"Data Correlations", @"");
                    item.graphData = self.correlatedScoring;
                    item.graphType = kAPCDashboardGraphTypeLine;
                    item.identifier = kAPCDashboardGraphTableViewCellIdentifier;
                    item.editable = YES;
                    item.tintColor = [UIColor appTertiaryYellowColor];
                    
                    NSString *info = [NSString stringWithFormat:@"This chart plots the index of your %@ against the index of your %@. For more comparisons, click the series name.", self.correlatedScoring.series1Name, self.correlatedScoring.series2Name];
                    item.info = NSLocalizedString(info, nil);
                    item.detailText = @"";
                    item.legend = [APCTableViewDashboardGraphItem legendForSeries1:self.correlatedScoring.series1Name series2:self.correlatedScoring.series2Name];
                    APCTableViewRow *row = [APCTableViewRow new];
                    row.item = item;
                    row.itemType = rowType;
                    [rowItems addObject:row];
                    
                }
                    break;
                    
                case kAPHDashboardItemTypeIntervalTapping:
                {
                    APCTableViewDashboardGraphItem *item = [APCTableViewDashboardGraphItem new];
                    item.caption = self.tapScoring.caption;
                    item.taskId = @"2-APHIntervalTapping-7259AC18-D711-47A6-ADBD-6CFCECDED1DF";
                    item.graphData = self.tapScoring;
                    item.graphType = kAPCDashboardGraphTypeDiscrete;
                    
                    double avgValue = [[self.tapScoring averageDataPoint] doubleValue];
                    
                    if (avgValue > 0) {
                        NSString  *detail = [NSString stringWithFormat:@"Min: %0.0f  Max: %0.0f",
                                             [[self.tapScoring minimumDataPoint] doubleValue], [[self.tapScoring maximumDataPoint] doubleValue]];
                        item.detailText = NSLocalizedString(detail, @"");
                    }
                    
                    item.identifier =  kAPCDashboardGraphTableViewCellIdentifier;
                    item.editable = YES;
                    item.tintColor = [UIColor colorForTaskId:item.taskId];
                    
                    item.info = NSLocalizedString(@"This plot shows your finger tapping speed each day as measured by the Tapping Interval Activity. The length and position of each vertical bar represents the range in the number of taps you made in 20 seconds for a given day. Any differences in length or position over time reflect variations and trends in your tapping speed, which may reflect variations and trends in your symptoms.", @"Dashboard tooltip item info text for Tapping in Parkinson");
                    
                    APCTableViewRow *row = [APCTableViewRow new];
                    row.item = item;
                    row.itemType = rowType;
                    [rowItems addObject:row];
                    
                }
                    break;
                case kAPHDashboardItemTypeGait:
                {
                    APCTableViewDashboardGraphItem *item = [APCTableViewDashboardGraphItem new];
                    item.caption = self.gaitScoring.caption;
                    item.taskId = @"4-APHTimedWalking-80F09109-265A-49C6-9C5D-765E49AAF5D9";
                    item.graphData = self.gaitScoring;
                    item.graphType = kAPCDashboardGraphTypeDiscrete;
                    
                    double avgValue = [[self.gaitScoring averageDataPoint] doubleValue];
                    
                    if (avgValue > 0) {
                        NSString  *detail = [NSString stringWithFormat:@"Min: %0.0f  Max: %0.0f",
                                             [[self.gaitScoring minimumDataPoint] doubleValue], [[self.gaitScoring maximumDataPoint] doubleValue]];
                        item.detailText = NSLocalizedString(detail, @"");
                    }
                    
                    item.identifier = kAPCDashboardGraphTableViewCellIdentifier;
                    item.editable = YES;
                    item.tintColor = [UIColor colorForTaskId:item.taskId];
                    
                    item.info = NSLocalizedString(@"This plot combines several accelerometer-based measures for the Walking Activity. The length and position of each vertical bar represents the range of measures for a given day. Any differences in length or position over time reflect variations and trends in your Walking measure, which may reflect variations and trends in your symptoms.", @"Dashboard tooltip item info text for Gait in Parkinson");
                    
                    APCTableViewRow *row = [APCTableViewRow new];
                    row.item = item;
                    row.itemType = rowType;
                    [rowItems addObject:row];
                }
                    break;
                case kAPHDashboardItemTypeSpatialMemory:
                {
                    APCTableViewDashboardGraphItem *item = [APCTableViewDashboardGraphItem new];
                    item.caption = self.memoryScoring.caption;
                    item.taskId = @"7-APHSpatialSpanMemory-4A04F3D0-AC05-11E4-AB27-0800200C9A66";
                    item.graphData = self.memoryScoring;
                    item.graphType = kAPCDashboardGraphTypeDiscrete;
                    
                    double avgValue = [[self.memoryScoring averageDataPoint] doubleValue];
                    
                    if (avgValue > 0) {
                        NSString  *detail = [NSString stringWithFormat:@"Min: %0.0f  Max: %0.0f",
                                             [[self.memoryScoring minimumDataPoint] doubleValue], [[self.memoryScoring maximumDataPoint] doubleValue]];
                        item.detailText = NSLocalizedString(detail, @"");
                    }
                    
                    item.identifier = kAPCDashboardGraphTableViewCellIdentifier;
                    item.editable = YES;
                    item.tintColor = [UIColor colorForTaskId:item.taskId];
                    
                    item.info = NSLocalizedString(@"This plot shows the score you received each day for the Memory Game. The length and position of each vertical bar represents the range of scores for a given day. Any differences in length or position over time reflect variations and trends in your score, which may reflect variations and trends in your symptoms.", @"Dashboard tooltip item info text for Memory in Parkinson");
                    
                    APCTableViewRow *row = [APCTableViewRow new];
                    row.item = item;
                    row.itemType = rowType;
                    [rowItems addObject:row];
                }
                    break;
                case kAPHDashboardItemTypePhonation:
                {
                    APCTableViewDashboardGraphItem *item = [APCTableViewDashboardGraphItem new];
                    item.caption = self.phonationScoring.caption;
                    item.taskId = @"3-APHPhonation-C614A231-A7B7-4173-BDC8-098309354292";
                    item.graphData = self.phonationScoring;
                    item.graphType = kAPCDashboardGraphTypeDiscrete;
                    
                    double avgValue = [[self.phonationScoring averageDataPoint] doubleValue];
                    
                    if (avgValue > 0) {
                        NSString  *detail = [NSString stringWithFormat:@"Min: %0.0f  Max: %0.0f",
                                             [[self.phonationScoring minimumDataPoint] doubleValue], [[self.phonationScoring maximumDataPoint] doubleValue]];
                        item.detailText = NSLocalizedString(detail, @"");
                    }
                    
                    item.identifier = kAPCDashboardGraphTableViewCellIdentifier;
                    item.editable = YES;
                    item.tintColor = [UIColor colorForTaskId:item.taskId];
                    
                    item.info = NSLocalizedString(@"This plot combines several microphone-based measures as a single score for the Voice Activity. The length and position of each vertical bar represents the range of measures for a given day. Any differences in length or position over time reflect variations and trends in your Voice measure, which may reflect variations and trends in your symptoms.", @"Dashboard tooltip item info text for Voice in Parkinson");
                    
                    APCTableViewRow *row = [APCTableViewRow new];
                    row.item = item;
                    row.itemType = rowType;
                    [rowItems addObject:row];
                }
                    break;
                    
                case kAPHDashboardItemTypeSteps:
                {
                    APCTableViewDashboardGraphItem  *item = [APCTableViewDashboardGraphItem new];
                    item.caption = self.stepScoring.caption;
                    item.graphData = self.stepScoring;
                    
                    double avgValue = [[self.stepScoring averageDataPoint] doubleValue];
                    
                    if (avgValue > 0) {
                        NSString  *detail = [NSString stringWithFormat:@"Average: %0.0f", avgValue];
                        item.detailText = NSLocalizedString(detail, @"Average: {value} steps");
                    }
                    
                    item.identifier = kAPCDashboardGraphTableViewCellIdentifier;
                    item.editable = YES;
                    item.tintColor = [UIColor appTertiaryGreenColor];
                    
                    item.info = NSLocalizedString(@"This graph shows how many steps you took each day, according to your phone's motion sensors. Remember that for this number to be accurate, you should have the phone on you as frequently as possible.", @"Dashboard tooltip item info text for Steps in Parkinson");
                    
                    APCTableViewRow *row = [APCTableViewRow new];
                    row.item = item;
                    row.itemType = rowType;
                    [rowItems addObject:row];
                }
                    break;
                default:
                    break;
            }
            
        }
        
        APCTableViewSection *section = [APCTableViewSection new];
        section.rows = [NSArray arrayWithArray:rowItems];
        section.sectionTitle = NSLocalizedString(@"Recent Activity", @"");
        [self.items addObject:section];
    }
    [self.tableView reloadData];
}

#pragma mark - CorrelationsSelector Delegate
    
- (void)dashboardTableViewCellDidTapLegendTitle:(APCDashboardTableViewCell *)__unused cell
{
    APCCorrelationsSelectorViewController *correlationSelector = [[APCCorrelationsSelectorViewController alloc]
                                                                  initWithScoringObjects:[NSArray arrayWithObjects:self.tapScoring,
                                                                                          self.gaitScoring, self.stepScoring,
                                                                                          self.memoryScoring, self.phonationScoring, nil]];
    correlationSelector.delegate = self;
    [self.navigationController pushViewController:correlationSelector animated:YES];
}

- (void)viewController:(APCCorrelationsSelectorViewController *)__unused viewController didChangeCorrelatedScoringDataSource:(APCScoring *)scoring
{
    self.correlatedScoring = scoring;
    [self prepareData];
}

@end
