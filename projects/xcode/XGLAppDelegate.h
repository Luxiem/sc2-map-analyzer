//
//  XGLAppDelegate.h
//  XGL
//
//  Created by Joshua Maloney on 23/08/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#import <Cocoa/Cocoa.h>


@interface XGLAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}


@property (assign) IBOutlet NSWindow *window;


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*) inSender;


@end
