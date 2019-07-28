/* fotoboxmac.mm
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "fotobox.h"

//Objective-C SDK
#import <AppKit/NSView.h>
#import <AppKit/NSWindow.h>


void FotoBox::closeFullscreenWindowOnMac()
{
    //WORKAROUND QTBUG-36714: closing a full screen QMainWindow leaves the screen black on macOS if there are multiple instances of QMainWindow created
    auto *view = reinterpret_cast<NSView *>(winId());
    if (view == nil) {
        return;
    }

    auto *window = view.window;
    if (window == nil) {
        return;
    }

    //close the window again, using the native MacOS API
    [window close];
}
