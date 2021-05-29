//
//  ViewController.swift
//  lz4test-swift
//
//  Created by LARRYHOU on 2021/5/29.
//

import UIKit

class ViewController: UIViewController
{
    @IBOutlet weak var progressView: UIProgressView!
    @IBOutlet weak var elapseLabel: UILabel!
    
    var _timer: Timer?
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        
        elapseLabel.text = "0.000";
        progressView.progress = 0.0;
    }

    @IBAction func runTest(_ sender: Any)
    {
        guard let document = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first else { return }
        
        let sample = document.appendingPathComponent("sample.lz4");
        let exists = FileManager.default.fileExists(atPath: sample.path);
        print(sample.path, exists)
        
        let target = sample.appendingPathExtension("dat");
        
        lz4t_progress = 0.0;
        progressView.progress = 0.0;
        elapseLabel.text = "0.000";
        
        var timestamp: Date? = nil;
        
        var success = -1;
        DispatchQueue.global(qos: .userInitiated).async
        {
            timestamp = Date()
            success = lz4t_decompress(sample.path, target.path) ? 1 : 0
            print("decompress success = \(success)")
        }
        
        _timer?.invalidate();
        _timer = Timer.scheduledTimer(withTimeInterval: 1.0/30.0, repeats: true)
        { timer in
            
            if (success != -1) { timer.invalidate() }
            if let timestamp = timestamp, timer.isValid
            {
                let elapse = timer.fireDate.timeIntervalSince(timestamp)
                self.elapseLabel.text = .init(format: "%.3f", elapse);
            }
            
            self.progressView.progress = Float(lz4t_progress);
        }
    }
}

