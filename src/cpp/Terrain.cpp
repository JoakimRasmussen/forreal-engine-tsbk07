#include "../h/Terrain.h"

Terrain::Terrain() {}

Model* Terrain::generateTerrain(TextureData* tex, float currentElevation) {
    // Generate the terrain mesh
    int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	unsigned int x, z;
	
	vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = (GLuint *) malloc(sizeof(GLuint) * triangleCount*3);
	
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)].x = x * quadSize / 1.0;
			vertexArray[(x + z * tex->width)].y = (tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / currentElevation); 
			vertexArray[(x + z * tex->width)].z = z * quadSize / 1.0;
			// Normal vectors. You need to calculate these.
			// normalArray[(x + z * tex->width)] = vec3(0.0, 1.0, 0.0); // Default to up vector, lazy solution
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)].x = ((float)x * 1) / tex->width * 1; // repeat the texture 1 time
			texCoordArray[(x + z * tex->width)].y = ((float)z * 1) / tex->height * 1; // repeat the texture 1 time
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = (x+1) + z * tex->width;
			// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = (x+1) + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = (x+1) + (z+1) * tex->width;
		}
	// Calculate normals
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// skip if on edge
			if (x == 0 || x == tex->width - 1 || z == 0 || z == tex->height - 1)
			{
				normalArray[(x + z * tex->width)] = vec3(0.0, 1.0, 0.0); // Default to up vector, lazy solution
				continue;
			}

			// Calculate the normal for one triangle, low poly style
			vec3 v1 = vertexArray[(x + z * tex->width)];
			vec3 v2 = vertexArray[(x + (z+1) * tex->width)];
			vec3 v3 = vertexArray[(x+1) + z * tex->width];
			vec3 normal = normalize(cross(v2 - v1, v3 - v1));
			normalArray[(x + z * tex->width)] = normal;

		}

	terrainModel = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return terrainModel;
}

float Terrain::getHeightAtPoint(float x, float z) const {
    // Height map starts at (0,0) and spans positively (normalized to 1.0 for the width and height of the terrain)
    float normalizedX = x / quadSize;
    float normalizedZ = z / quadSize;

    // Get the integer part of the coordinates (bottom-left corner of the cell)
    GLuint ix = floor(normalizedX);
    GLuint iz = floor(normalizedZ);

    // Ensure we're within bounds of the heightmap
    if (ix < 0 || ix >= ttex.width - 1 || iz < 0 || iz >= ttex.height - 1) {
        return 0; // Out of bounds, return a default height (e.g., 0)
    }

    // Calculate the fractional part for interpolation (how far (x, z) is from the bottom-left corner of the cell)
    float fractionalX = normalizedX - ix;
    float fractionalZ = normalizedZ - iz; 

    // Retrieve heights for bilinear interpolationv
    float h00 = terrainModel->vertexArray[ix + iz *ttex.width].y; // Bottom-left corner
    float h10 = terrainModel->vertexArray[(ix + 1) + iz *ttex.width].y; // Bottom-right corner
    float h01 = terrainModel->vertexArray[ix + (iz + 1) *ttex.width].y; // Top-left corner
    float h11 = terrainModel->vertexArray[(ix + 1) + (iz + 1) *ttex.width].y; // Top-right corner

    // Bilinear interpolation (Bilinear texture mapping)
    float height = (1 - fractionalX) * (1 - fractionalZ) * h00 +
                   fractionalX * (1 - fractionalZ) * h10 +
                   (1 - fractionalX) * fractionalZ * h01 +
                   fractionalX * fractionalZ * h11;

    return height;
}

vec3 Terrain::getNormalAtPoint(float x, float z) const
{
	// Height map starts at (0,0) and spans positively (normalized to 1.0 for the width and height of the terrain)
	float normalizedX = x / quadSize;
	float normalizedZ = z / quadSize;

	// Get the integer part of the coordinates (bottom-left corner of the cell)
	GLuint ix = floor(normalizedX);
	GLuint iz = floor(normalizedZ);

	// Ensure we're within bounds of the heightmap
	if (ix < 0 || ix >= ttex.width - 1 || iz < 0 || iz >= ttex.height - 1) {
		return vec3(0.0, 1.0, 0.0); // Out of bounds, return a default normal (e.g., up vector)
	}

	// Calculate the fractional part for interpolation (how far (x, z) is from the bottom-left corner of the cell)
	float fractionalX = normalizedX - ix;
	float fractionalZ = normalizedZ - iz;

	// Retrieve normals for bilinear interpolation
	vec3 n00 = terrainModel->normalArray[ix + iz * ttex.width]; // Bottom-left corner
	vec3 n10 = terrainModel->normalArray[(ix + 1) + iz * ttex.width]; // Bottom-right corner
	vec3 n01 = terrainModel->normalArray[ix + (iz + 1) * ttex.width]; // Top-left corner
	vec3 n11 = terrainModel->normalArray[(ix + 1) + (iz + 1) * ttex.width]; // Top-right corner

	// Bilinear interpolation (Bilinear texture mapping)
	vec3 normal = (1 - fractionalX) * (1 - fractionalZ) * n00 +
		fractionalX * (1 - fractionalZ) * n10 +
		(1 - fractionalX) * fractionalZ * n01 +
		fractionalX * fractionalZ * n11;

	return normal;
}


void Terrain::editTerrainAtIntersectionPoint(vec3 intersectionPoint)
{
	unsigned int x = intersectionPoint.x / quadSize;
	unsigned int z = intersectionPoint.z / quadSize;

	// Iterate over the surrounding area with a radius of 1
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			unsigned int newX = x + i;
			unsigned int newZ = z + j;

			// Check if the new coordinates are within the terrain bounds
			if (newX >= 0 && newX < ttex.width && newZ >= 0 && newZ < ttex.height) {
				ttex.imageData[(newX + newZ * ttex.width) * (ttex.bpp / 8)] += heightIncrement;
			}
		}
	}

	terrainModel = generateTerrain(&ttex, currentElevation);
}

void Terrain::smoothTerrainAtIntersectionPoint(vec3 intersectionPoint)
{
	unsigned int x = intersectionPoint.x / quadSize;
	unsigned int z = intersectionPoint.z / quadSize;
	GLfloat mean = 0.0;

	// Iterate over the surrounding area with a radius of 1
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			unsigned int newX = x + i;
			unsigned int newZ = z + j;

			// Check if the new coordinates are within the terrain bounds
			if (newX >= 0 && newX < ttex.width && newZ >= 0 && newZ < ttex.height) {
				mean += ttex.imageData[(newX + newZ * ttex.width) * (ttex.bpp / 8)];
			}
		}
	}

	mean = mean/25.0;
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			unsigned int newX = x + i;
			unsigned int newZ = z + j;

			// Check if the new coordinates are within the terrain bounds
			if (newX >= 0 && newX < ttex.width && newZ >= 0 && newZ < ttex.height) {
				ttex.imageData[(newX + newZ * ttex.width) * (ttex.bpp / 8)] = mean;
			}
		}
	}

	terrainModel = generateTerrain(&ttex, currentElevation);

}

// /* Möller-Trumbore intersection algorithm, temporary solution with the two triangles.... */
// bool Terrain::rayTriangleIntersection(vec3 rayOrigin, vec3 rayDirection, vec3& intersectionPoint, std::vector<vec3>& intersectionVector)
// {

// 	GLint i = 0;
// 	vec3 closestIntersectionPoint = vec3(0.0f, 0.0f, 0.0f);
// 	vec3* ipVector = (vec3 *)malloc(sizeof(GLfloat) * 50);
	
// 	// Loop over all triangles in the terrain model
// 	for (unsigned int x = 0; x < ttex.width; x++)
// 	{
// 		for (unsigned int z = 0; z < ttex.height; z++)
// 		{
// 			// First triangle
// 			vec3 vertex1 = terrainModel->vertexArray[x + z * ttex.width];
// 			vec3 vertex2 = terrainModel->vertexArray[x + (z+1) * ttex.width];
// 			vec3 vertex3 = terrainModel->vertexArray[(x+1) + z * ttex.width];

// 			const float eps = 1e-8;
// 			vec3 edge1 = vertex2 - vertex1;
// 			vec3 edge2 = vertex3 - vertex1;
// 			vec3 h = cross(rayDirection, edge2);
// 			float det = dot(edge1, h);

// 			// Check if ray is parallel to the triangle
// 			if (det > -eps && det < eps) 
// 				continue;

// 			float invDet = 1.0f / det;
// 			vec3 s = rayOrigin - vertex1;
// 			float u = invDet * dot(s, h); // first barycentric coordinate

// 			vec3 q = cross(s, edge1);
// 			float v = invDet * dot(rayDirection, q); // second barycentric coordinate

// 			// Check if the intersection point is inside the triangle
// 			if (u >= 0 && v >= 0 && u + v <= 1)
// 			{
// 				// Calculate the intersection point
// 				float t = invDet * dot(edge2, q);
// 				intersectionPoint = rayOrigin + t * rayDirection;

// 				ipVector[i] = intersectionPoint;

// 				// Check if the intersection point is closer than the previous one
// 				if (i == 0 || abs(Norm(intersectionPoint - rayOrigin)) < abs(Norm(closestIntersectionPoint - rayOrigin)))
// 				{
// 					closestIntersectionPoint = intersectionPoint;
// 				}
// 				i++;
// 				continue;
// 			}

// 			// Second triangle
// 			vertex1 = terrainModel->vertexArray[(x+1) + z * ttex.width];
// 			vertex2 = terrainModel->vertexArray[x + (z+1) * ttex.width];
// 			vertex3 = terrainModel->vertexArray[(x+1) + (z+1) * ttex.width];

// 			edge1 = vertex2 - vertex1;
// 			edge2 = vertex3 - vertex1;
// 			h = cross(rayDirection, edge2);
// 			det = dot(edge1, h);

// 			// Check if ray is parallel to the triangle
// 			if (det > -eps && det < eps)
// 				continue;

// 			invDet = 1.0f / det;
// 			s = rayOrigin - vertex1;
// 			u = invDet * dot(s, h); // first barycentric coordinate

// 			q = cross(s, edge1);
// 			v = invDet * dot(rayDirection, q); // second barycentric coordinate

// 			// Check if the intersection point is inside the triangle
// 			if (u >= 0 && v >= 0 && u + v <= 1)
// 			{
// 				// Calculate the intersection point
// 				float t = invDet * dot(edge2, q);
// 				intersectionPoint = rayOrigin + t * rayDirection;

// 				ipVector[i] = intersectionPoint;

// 				// Check if the intersection point is closer than the previous one
// 				if (i == 0 || abs(Norm(intersectionPoint - rayOrigin)) < abs(Norm(closestIntersectionPoint - rayOrigin)))
// 				{
// 					closestIntersectionPoint = intersectionPoint;
// 				}
// 				i++;
// 				continue;
// 			}
// 		}
// 	}
// 	if (i > 0)
// 	{
// 		// Loop over all intersection points and print them out
// 		for (int j = 0; j < i; j++)
// 		{
// 			// printf("Intersection point in Terrain.cpp: %f, %f, %f\n", ipVector[j].x, ipVector[j].y, ipVector[j].z);
// 			intersectionVector.push_back(ipVector[j]);
// 		}
// 		intersectionPoint = closestIntersectionPoint;
// 		return true;
// 	}
// 	// No intersection found
// 	// printf("No intersection found\n");
// 	return false;
// }

#include <vector>
#include <limits>

bool Terrain::rayTriangleIntersection(vec3 rayOrigin, vec3 rayDirection, vec3& intersectionPoint, std::vector<vec3>& intersectionVector)
{
    vec3 closestIntersectionPoint = vec3(std::numeric_limits<float>::max());
    float closestDistance = std::numeric_limits<float>::max();
    bool hasIntersection = false;

    auto checkIntersection = [&](const vec3& v1, const vec3& v2, const vec3& v3) {
        const float eps = 1e-8;
        vec3 edge1 = v2 - v1;
        vec3 edge2 = v3 - v1;
        vec3 h = cross(rayDirection, edge2);
        float det = dot(edge1, h);

        if (std::abs(det) < eps)
            return false; // Ray is parallel to the triangle

        float invDet = 1.0f / det;
        vec3 s = rayOrigin - v1;
        float u = invDet * dot(s, h); // First barycentric coordinate
        if (u < 0.0f || u > 1.0f) return false;

        vec3 q = cross(s, edge1);
        float v = invDet * dot(rayDirection, q); // Second barycentric coordinate
        if (v < 0.0f || u + v > 1.0f) return false;

        float t = invDet * dot(edge2, q); // Compute t to find intersection point
        if (t > eps) { // Intersection is in positive ray direction
            vec3 ip = rayOrigin + t * rayDirection;
            float dist = dot(ip - rayOrigin, ip - rayOrigin);
            if (dist < closestDistance && dist < 5000.0f) {
                closestDistance = dist;
                closestIntersectionPoint = ip;
                hasIntersection = true;
            }
        }
        return true;
    };

    // Loop over all triangles in the terrain model
    for (unsigned int x = 0; x < ttex.width; x++) {
        for (unsigned int z = 0; z < ttex.height; z++) {
            vec3 vertex1 = terrainModel->vertexArray[x + z * ttex.width];
            vec3 vertex2 = terrainModel->vertexArray[x + (z + 1) * ttex.width];
            vec3 vertex3 = terrainModel->vertexArray[(x + 1) + z * ttex.width];
            checkIntersection(vertex1, vertex2, vertex3);

            vertex1 = terrainModel->vertexArray[(x + 1) + z * ttex.width];
            vertex2 = terrainModel->vertexArray[x + (z + 1) * ttex.width];
            vertex3 = terrainModel->vertexArray[(x + 1) + (z + 1) * ttex.width];
            checkIntersection(vertex1, vertex2, vertex3);
        }
    }

    if (hasIntersection) {
        intersectionPoint = closestIntersectionPoint;
        intersectionVector.push_back(closestIntersectionPoint); // Store or process the intersection point
        return true;
    }

    return false; // No intersection found
}

void Terrain::updateTerrain()
{
	if (currentElevation - previousElevation != 0.0)
	{
		terrainModel = generateTerrain(&ttex, currentElevation);
		previousElevation = currentElevation;
	}
	if (quadSize - previousQuadSize != 0.0)
	{
		terrainModel = generateTerrain(&ttex, currentElevation);
		previousQuadSize = quadSize;
	}
}

Model* Terrain::setTerrainModel(const char* heightmap) 
{
	LoadTGATextureData(heightmap, &ttex);
	terrainModel = generateTerrain(&ttex, currentElevation);
	return terrainModel;
}

TextureData* Terrain::getTextureData() 
{
	return &ttex;
}

Model* Terrain::getTerrainModel() 
{

	return terrainModel;
}

void Terrain::editTerrainTextureAtIntersectionPoint(vec3 intersectionPoint, GLubyte colorPixel[4], int radius)
{
	// I do not think pixelX and pixelZ is the correct labeling...
	GLfloat pixelX = intersectionPoint.x;
	GLfloat pixelZ = intersectionPoint.z;

	// Make sure to bind the correct texture (map texture found in GameMode)
	glActiveTexture(GL_TEXTURE3);

	// Update the texture with the new pixel data in a circle around the intersection point
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			if (i * i + j * j <= radius * radius) {
				glTexSubImage2D(GL_TEXTURE_2D, 0, pixelX + i, pixelZ + j, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, colorPixel);
			}
		}
	}
}

