#import <Cocoa/Cocoa.h>
//#include "../UV/UV.h"


@interface MyView : NSOpenGLView {
	
	
@private
	NSTrackingArea* m_trackingArea;
	
	bool m_wantsInit;

}


- (void)awakeFromNib;


- (void)mouseDown:(NSEvent *)theEvent;
- (void)rightMouseDown:(NSEvent *)theEvent;
- (void)otherMouseDown:(NSEvent *)theEvent;
- (void)mouseUp:(NSEvent *)theEvent;
- (void)rightMouseUp:(NSEvent *)theEvent;
- (void)otherMouseUp:(NSEvent *)theEvent;


- (void) mouseMoved:(NSEvent *)theEvent;
- (void) mouseDragged:(NSEvent *)theEvent;


- (void) drawRect: (NSRect) bounds;


- (void) viewDidMoveToWindow;


- (void) beginLoadMap: (NSString*) a_path;


@end
