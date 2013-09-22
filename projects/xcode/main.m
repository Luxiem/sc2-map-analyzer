//
//  main.m
//  XGL
//
//  Created by Joshua Maloney on 23/08/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#import <Cocoa/Cocoa.h>
#import "MyApplication.h"


int main(int argc, char *argv[])
{
    return NSApplicationMain(argc,  (const char **) argv);
}

/*

int main(int argc, const char *argv[])
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool allocWithZone: NULL]
							   init];
	
	NSString* principal_class = [[[NSBundle mainBundle] infoDictionary]
								 objectForKey:@"NSPrincipalClass"];
	
	AppWrapper* app_wrapper = (AppWrapper*)
	[NSClassFromString(principal_class) sharedApplication];
	
	if (app_wrapper)
	{
		if ([app_wrapper respondsToSelector: @selector(process)])
			[app_wrapper process];
		
		[app_wrapper release];
	}
	
	[pool release];
	
	exit(0);
	
	return 0;
}
*/