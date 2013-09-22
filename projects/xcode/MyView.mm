#import "MyView.h"
#include "Common.h"
#include <string>


void SaveImageFile()
{
	NSLog(@"doSaveAs");	
	NSSavePanel *tvarNSSavePanelObj	= [NSSavePanel savePanel];
	int tvarInt	= [tvarNSSavePanelObj runModal];
	if(tvarInt == NSOKButton){
		NSLog(@"doSaveAs we have an OK button");	
	} else if(tvarInt == NSCancelButton) {
		NSLog(@"doSaveAs we have a Cancel button");
		return;
	} else {
		NSLog(@"doSaveAs tvarInt not equal 1 or zero = %3d",tvarInt);
		return;
	} // end if     
	
	NSString * tvarDirectory = [tvarNSSavePanelObj directory];
	NSLog(@"doSaveAs directory = %@",tvarDirectory);
	
	NSString * tvarFilename = [tvarNSSavePanelObj filename];
	NSLog(@"doSaveAs filename = %@",tvarFilename);

	if (tvarFilename)
	{
		const char *bar = [tvarFilename UTF8String];
		std::string s = std::string(bar);
		
		Common::SaveImage(s);
	}
	
}


void LoadMapFile()
{
	
	
}


@implementation MyView



-(void)awakeFromNib
{
	m_wantsInit = true;
}


- (void)mouseMoved: (NSEvent *)theEvent
{
	NSPoint mouseLoc;
	mouseLoc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	
	Common::OnMouseMove(mouseLoc.x, mouseLoc.y);
	
	// Force display update
	[self setNeedsDisplay:YES];
	
    [super mouseMoved:theEvent];
    //[[self nextResponder] mouseMoved:theEvent];
}


- (void)mouseDragged: (NSEvent *)theEvent
{
	NSPoint mouseLoc;
	mouseLoc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	
	Common::OnMouseMove(mouseLoc.x, mouseLoc.y);
	
	// Force display update
	[self setNeedsDisplay:YES];
	
	[super mouseDragged:theEvent];
}


- (void)mouseDown:(NSEvent *)theEvent
{
	NSPoint mouseLoc;
	mouseLoc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	
	Common::OnLeftMouseDown(mouseLoc.x, mouseLoc.y);
	
	// Force display update
	[self setNeedsDisplay:YES];
	
	[super mouseDown:theEvent];
}


- (void)rightMouseDown:(NSEvent *)theEvent
{
	[super rightMouseDown:theEvent];
}


- (void)otherMouseDown:(NSEvent *)theEvent
{
	[super otherMouseDown:theEvent];
}


- (void)mouseUp:(NSEvent *)theEvent
{
	NSPoint mouseLoc;
	mouseLoc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	
	Common::OnLeftMouseUp(mouseLoc.x, mouseLoc.y);
	
	// Force display update
	[self setNeedsDisplay:YES];
	
	[super mouseUp:theEvent];
}


- (void)rightMouseUp:(NSEvent *)theEvent
{
	[super rightMouseUp:theEvent];
}


- (void)otherMouseUp:(NSEvent *)theEvent
{
	[super otherMouseUp:theEvent];
}


- (NSDragOperation)draggingEntered:(id )sender
{
	if ((NSDragOperationGeneric & [sender draggingSourceOperationMask])
		== NSDragOperationGeneric) {
		
		return NSDragOperationGeneric;
		
	} // end if
	
	// not a drag we can use
	return NSDragOperationNone;
	
} // end draggingEntered


- (BOOL)prepareForDragOperation:(id )sender {
	return YES;
} // end prepareForDragOperation


- (BOOL)performDragOperation:(id )sender {
	NSPasteboard *zPasteboard = [sender draggingPasteboard];
	// define the images  types we accept
	// NSPasteboardTypeTIFF: (used to be NSTIFFPboardType).
	// NSFilenamesPboardType:An array of NSString filenames
	NSArray *zImageTypesAry = [NSArray arrayWithObjects:NSPasteboardTypeTIFF,
							   NSFilenamesPboardType, nil];
	
	NSString *zDesiredType =
	[zPasteboard availableTypeFromArray:zImageTypesAry];
		
    if ([zDesiredType isEqualToString:NSFilenamesPboardType]) {
		// the pasteboard contains a list of file names
		//Take the first one
		NSArray *zFileNamesAry =
		[zPasteboard propertyListForType:@"NSFilenamesPboardType"];
		NSString *zPath = [zFileNamesAry objectAtIndex:0];
		//NSImage *zNewImage = [[NSImage alloc] initWithContentsOfFile:zPath];
		
		if (zPath == nil) {
			NSLog(@"Error: MyNSView performDragOperation zNewImage == nil");
			return NO;
		}// end if
		
		[self beginLoadMap:zPath];
		
		[self setNeedsDisplay:YES];
		return YES;
		
	}// end if
	
	//this cant happen ???
	NSLog(@"Error MyNSView performDragOperation");
	return NO;
	
} // end performDragOperation


- (void)concludeDragOperation:(id )sender {
	[self setNeedsDisplay:YES];
} // end concludeDragOperation


- (void) viewDidMoveToWindow;
{
	if (m_trackingArea == 0)
	{
		m_trackingArea = [[NSTrackingArea alloc] initWithRect:[self frame]
													  options:NSTrackingMouseMoved+NSTrackingActiveInKeyWindow+NSTrackingInVisibleRect
														owner:self
													 userInfo:nil];
		[self addTrackingArea:m_trackingArea];
	}
	
	[self becomeFirstResponder];
}


-(void) drawRect:(NSRect)bounds
{
	if (m_wantsInit)
	{	
		// Set current relative working directory		
		[[NSFileManager defaultManager] changeCurrentDirectoryPath:
		 [[[NSBundle mainBundle] bundlePath]
		  stringByDeletingLastPathComponent]];
		
		Common::Init(".");
		
		// Frame?
		[self registerForDraggedTypes:
		 [NSArray arrayWithObjects:NSFilenamesPboardType,nil]];
		
		// Set window to accept mouse move events
		[[self window] setAcceptsMouseMovedEvents:YES];
		
		m_wantsInit = false;
	}			
	
	
	Common::DrawScreen();
}


- (void) beginLoadMap: (NSString*) a_path
{
	using namespace std;
	
	
	// Create path strings
	const char *bar = [a_path UTF8String];
	std::string fileFullPath(bar);
	
	
	Common::beginLoadMap(fileFullPath);
}


@end
