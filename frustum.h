//
//  frustum.h
//  Bedrock
//
//  Created by Quinn on 3/22/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

namespace frustumUtil {
	/// extracts the view frustum planes from the current matrices
	void extractFrustum();
	
	/// checks if a point is in the view frustum
	bool isPointInFrustum(float x, float y, float z);
	/// checks if a bounding cube is in the view frustum
	bool isCubeInFrustum(float x, float y, float z, float size);
	/// checks if a bounding sphere is in the frustum
	bool isSphereInFrustum(float x, float y, float z, float radius);
}