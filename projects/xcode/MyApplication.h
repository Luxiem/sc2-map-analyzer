//
//  MyApplication.h
//  UV Test
//
//  Created by Joshua Maloney on 23/08/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/*
@interface AppWrapper : NSApplication
{
	SEL _process_func;
	//App* _app;
}
- (void) process;
@end

@implementation AppWrapper

- (id) init
{
	NSLog(@"App init");
	
	self = [super init];
	
	[self setDelegate: self];
	
	NSString* main_nib_name = [[[NSBundle mainBundle] infoDictionary]
							   objectForKey:@"NSMainNibFile"];
	
	bool nib_loaded = main_nib_name && [NSBundle loadNibNamed:
										main_nib_name owner: self];
	
	_process_func = nib_loaded ? @selector(eventLoop) :
	@selector(initError);
	
	[NSApp finishLaunching];
	
	return self;
}

- (void) dealloc
{
	[super dealloc];
}

- (void) initError
{
	NSLog(@"Error in application initialization");
}

- (void) eventLoop
{
	[NSEvent startPeriodicEventsAfterDelay: 0.0 withPeriod: 0.1];
	
	NSDate* distantFuture = [NSDate distantFuture];
	
	for (;;)
	{
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		
		NSEvent* event = [NSApp nextEventMatchingMask: NSAnyEventMask
											untilDate: distantFuture
											   inMode: NSDefaultRunLoopMode
											  dequeue: YES];
		
		NSLog(@"%@", event);
		
		if (event)
			[NSApp sendEvent: event];
		
		[pool release];
	}
}

- (void) process
{
	if (_process_func)
		[self performSelector: _process_func];
}

- (void) applicationWillFinishLaunching: (NSNotification*) notification
{
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
	NSLog(@"App Interface: init");
	
	//_app = [[App create] retain];
}

- (NSApplicationTerminateReply) applicationShouldTerminate:
(NSApplication*) sender
{
	return NSTerminateNow;
}

- (void) applicationWillTerminate: (NSNotification*) notification
{
	//if (_app)
	//{
	//	[_app release];
	//	_app = nil;
	//}
}

@end

*/
