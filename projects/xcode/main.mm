#import <Cocoa/Cocoa.h>
#include <mach-o/dyld.h>
#include "Common.h"


int main(int argc, char *argv[])
{
    char path[MAXPATHLEN];
    char link_path[MAXPATHLEN];
    uint32_t sizeOfPath = sizeof(path);
    _NSGetExecutablePath(path, &sizeOfPath);
    realpath(path, link_path);
    
    std::string long_path(link_path);
    size_t pos = long_path.find_last_of("/\\");
    std::string relative_path = long_path.substr(0, pos);
    
    pos = relative_path.find_last_of("/");
    relative_path = relative_path.substr(0, pos);
    
    pos = relative_path.find_last_of("/");
    relative_path = relative_path.substr(0, pos);

    pos = relative_path.find_last_of("/");
    relative_path = relative_path.substr(0, pos);
    
    chdir(relative_path.c_str());
    
    //Common::Init(relative_path);
    Common::Init(".");
    
    while (!Common::Quit())
    {
        Common::DrawScreen();
    }
    
    return 0;
}


void SaveImageFile()
{
	NSSavePanel *tvarNSSavePanelObj	= [NSSavePanel savePanel];
	int tvarInt	= [tvarNSSavePanelObj runModal];
	if(tvarInt == NSOKButton){
		//NSLog(@"doSaveAs we have an OK button");
	} else if(tvarInt == NSCancelButton) {
		//NSLog(@"doSaveAs we have a Cancel button");
		return;
	} else {
		NSLog(@"doSaveAs tvarInt not equal 1 or zero = %3d",tvarInt);
		return;
	} // end if
	
	NSString * tvarDirectory = [tvarNSSavePanelObj directory];
	//NSLog(@"doSaveAs directory = %@",tvarDirectory);
	
	NSString * tvarFilename = [tvarNSSavePanelObj filename];
	//NSLog(@"doSaveAs filename = %@",tvarFilename);
    
	if (tvarFilename)
	{
		const char *bar = [tvarFilename UTF8String];
		std::string s = std::string(bar);
		
		Common::SaveImage(s);
	}
	
}


void LoadMapFile()
{
    NSOpenPanel *tvarNSOpenPanelObj	= [NSOpenPanel openPanel];
    int tvarInt = [tvarNSOpenPanelObj runModal];
	
    if(tvarInt == NSOKButton)
    {
        NSString * tvarFilename = [tvarNSOpenPanelObj filename];

        const char *bar = [tvarFilename UTF8String];
		std::string s = std::string(bar);
        Common::beginLoadMap(s);
    }
}
