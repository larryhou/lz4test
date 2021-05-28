//
//  ViewController.h
//  LZ4
//
//  Created by LARRYHOU on 2021/5/27.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
@property (weak, nonatomic) IBOutlet UILabel *elapseLabel;
@property (weak, nonatomic) IBOutlet UIProgressView *progressView;

- (IBAction)runTest:(id)sender;

@end

