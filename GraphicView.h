//
//  GraphicView.h
//  Bedrock
//
//  Created by Quinn on 1/14/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface GraphicView : NSOpenGLView

- (void) prepareOpenGL; // Called after being summoned from a NIB/XIB.
- (void) reshape; // Adjust the OpenGL viewport in response to window resize
- (void) drawRect: (NSRect) bounds; // Render the scene using OpenGL API calls

@end