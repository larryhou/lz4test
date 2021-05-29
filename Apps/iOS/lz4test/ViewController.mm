//
//  ViewController.m
//  LZ4
//
//  Created by LARRYHOU on 2021/5/27.
//

#import "ViewController.h"

extern "C" {
#import "lz4test.hpp"
}

@interface ViewController ()

@end

@implementation ViewController

NSTimer* _timer;

- (void)viewDidLoad
{
    [super viewDidLoad];
    [_progressView setProgress:0.0];
    [_elapseLabel setText:@"0.000"];
}


- (IBAction)runTest:(id)sender
{
    NSURL* document = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] firstObject];
    NSURL* sample = [document URLByAppendingPathComponent:@"sample.lz4"];
    
    BOOL exist = [[NSFileManager defaultManager] fileExistsAtPath:sample.path];
    NSLog(@"%@ %d", sample, exist);
    
    NSURL* target = [sample URLByAppendingPathExtension:@"dat"];
    
    lz4t_progress = 0.0f;
    [_progressView setProgress: 0.0f];
    
    [_elapseLabel setText:@"0.000"];
    
    NSDate* __block timestamp = NULL;
    
    int __block success = -1;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        timestamp = [NSDate now];
        success = lz4t_decompress(sample.path.UTF8String, target.path.UTF8String);
        NSLog(@"decompress success = %d", success);
    });
    
    if (_timer) { [_timer invalidate]; }
    _timer = [NSTimer scheduledTimerWithTimeInterval:1.0/30.0 repeats:YES block:^(NSTimer * _Nonnull timer)
    {
        if (success != -1) { [timer invalidate]; }
        
        if (timestamp && timer.valid)
        {
            NSTimeInterval elapse = [timer.fireDate timeIntervalSinceDate:timestamp];
            [self->_elapseLabel setText: [NSString stringWithFormat:@"%.3f", elapse]];
        }
        
        [self->_progressView setProgress:lz4t_progress];
    }];
}

@end
