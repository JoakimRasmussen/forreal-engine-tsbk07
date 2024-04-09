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
			vertexArray[(x + z * tex->width)].y = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / currentElevation; 
			vertexArray[(x + z * tex->width)].z = z * quadSize / 1.0;
			// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)] = vec3(0.0, 1.0, 0.0); // Default to up vector, lazy solution
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)].x = ((float)x * 1) / tex->width * quadSize; // repeat the texture 1 time
			texCoordArray[(x + z * tex->width)].y = ((float)z * 1) / tex->height * quadSize; // repeat the texture 1 time
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
	/*
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

			//			1 --- 2 --- 3
			//			|    /|    /|
			//			|  	/ |   / |
			//			|  /  |  /  |
			//			| /   | /   |
			//			|/    |/    |
			//			4 --- c --- 6
			//			|    /|    /|
			//			|  	/ |   / |
			//			|  /  |  /  |
			//			| /   | /   |
			//			|/    |/    |
			//			7 --- 8 --- 9

			// n1 = vertexArray[((x-1) + (z-1) * tex->width)];
			vec3 n2 = vertexArray[(x + (z-1) * tex->width)];
			vec3 n3 = vertexArray[((x+1) + (z-1) * tex->width)];
			vec3 n4 = vertexArray[((x-1) + z * tex->width)];
			vec3 c = vertexArray[(x + z * tex->width)];
			vec3 n6 = vertexArray[((x+1) + (z+1) * tex->width)];
			vec3 n7 = vertexArray[((x-1) + (z+1) * tex->width)];
			vec3 n8 = vertexArray[(x + (z+1) * tex->width)];
			// n9 = vertexArray[((x+1) + (z+1) * tex->width)];

			vec3 upVector = vec3(0.0, 1.0, 0.0);
			vec3 normalSum = vec3(0.0);
			float weightSum = 0.0;

			// Calculate normals for surrounding triangles
			vec3 normals[] = 
			{
				cross(n2 - c, n4 - c),
				cross(n3 - c, n2 - c),
				cross(n6 - c, n3 - c),
				cross(n8 - c, n6 - c),
				cross(n7 - c, n8 - c),
				cross(n4 - c, n7 - c)
			};

			// Calculate the weighted average of the normals
			for (int i = 0; i < 6; i++) 
			{
				vec3 normal = normalize(normals[i]);
				if (sqrt(dot(normal, normal)) > 0) 
				{
					float angleCosine = dot(normal, upVector);
					float weight = abs(angleCosine); // calculate the "steepness" of the triangle, and weight the normal.
					normalSum += normal * weight;
					weightSum += weight;
				}
			}

			if (weightSum > 0) 
			{
				normalArray[(x + z * tex->width)] = normalize(normalSum / weightSum);
			} 
			else 
			{
				normalArray[(x + z * tex->width)] = upVector; // Default to up vector if no valid normals found
			}
		}
		*/

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

    // Retrieve heights for bilinear interpolation
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

/* Möller-Trumbore intersection algorithm, temporary solution with the two triangles.... */
bool Terrain::rayTriangleIntersection(vec3 rayOrigin, vec3 rayDirection, vec3& intersectionPoint, std::vector<vec3>& intersectionVector)
{

	GLint i = 0;
	vec3 closestIntersectionPoint = vec3(0.0f, 0.0f, 0.0f);
	vec3* ipVector = (vec3 *)malloc(sizeof(GLfloat) * 50);
	
	// Loop over all triangles in the terrain model
	for (unsigned int x = 0; x < ttex.width; x++)
	{
		for (unsigned int z = 0; z < ttex.height; z++)
		{
			// First triangle
			vec3 vertex1 = terrainModel->vertexArray[x + z * ttex.width];
			vec3 vertex2 = terrainModel->vertexArray[x + (z+1) * ttex.width];
			vec3 vertex3 = terrainModel->vertexArray[(x+1) + z * ttex.width];

			const float eps = 1e-8;
			vec3 edge1 = vertex2 - vertex1;
			vec3 edge2 = vertex3 - vertex1;
			vec3 h = cross(rayDirection, edge2);
			float det = dot(edge1, h);

			// Check if ray is parallel to the triangle
			if (det > -eps && det < eps) 
				continue;

			float invDet = 1.0f / det;
			vec3 s = rayOrigin - vertex1;
			float u = invDet * dot(s, h); // first barycentric coordinate

			vec3 q = cross(s, edge1);
			float v = invDet * dot(rayDirection, q); // second barycentric coordinate

			// Check if the intersection point is inside the triangle
			if (u >= 0 && v >= 0 && u + v <= 1)
			{
				// Calculate the intersection point
				float t = invDet * dot(edge2, q);
				intersectionPoint = rayOrigin + t * rayDirection;

				ipVector[i] = intersectionPoint;

				// Check if the intersection point is closer than the previous one
				if (i == 0 || abs(Norm(intersectionPoint - rayOrigin)) < abs(Norm(closestIntersectionPoint - rayOrigin)))
				{
					closestIntersectionPoint = intersectionPoint;
				}
				i++;
				continue;
			}

			// Second triangle
			vertex1 = terrainModel->vertexArray[(x+1) + z * ttex.width];
			vertex2 = terrainModel->vertexArray[x + (z+1) * ttex.width];
			vertex3 = terrainModel->vertexArray[(x+1) + (z+1) * ttex.width];

			edge1 = vertex2 - vertex1;
			edge2 = vertex3 - vertex1;
			h = cross(rayDirection, edge2);
			det = dot(edge1, h);

			// Check if ray is parallel to the triangle
			if (det > -eps && det < eps)
				continue;

			invDet = 1.0f / det;
			s = rayOrigin - vertex1;
			u = invDet * dot(s, h); // first barycentric coordinate

			q = cross(s, edge1);
			v = invDet * dot(rayDirection, q); // second barycentric coordinate

			// Check if the intersection point is inside the triangle
			if (u >= 0 && v >= 0 && u + v <= 1)
			{
				// Calculate the intersection point
				float t = invDet * dot(edge2, q);
				intersectionPoint = rayOrigin + t * rayDirection;

				ipVector[i] = intersectionPoint;

				// Check if the intersection point is closer than the previous one
				if (i == 0 || abs(Norm(intersectionPoint - rayOrigin)) < abs(Norm(closestIntersectionPoint - rayOrigin)))
				{
					closestIntersectionPoint = intersectionPoint;
				}
				i++;
				continue;
			}
		}
	}
	if (i > 0)
	{
		// Loop over all intersection points and print them out
		for (int j = 0; j < i; j++)
		{
			// printf("Intersection point in Terrain.cpp: %f, %f, %f\n", ipVector[j].x, ipVector[j].y, ipVector[j].z);
			intersectionVector.push_back(ipVector[j]);
		}
		intersectionPoint = closestIntersectionPoint;
		return true;
	}
	// No intersection found
	printf("No intersection found\n");
	return false;
}


#include "vector"
void Terrain::createSplatMap()
{
	int width = 256; // Example width
	int height = 256; // Example height
	unsigned char pixelDepth = 24; // 24 for RGB, 32 for RGBA
	// Create a vector to hold the image data

	unsigned char* imageData = reinterpret_cast<unsigned char*>(malloc(sizeof(GLuint) * 3 * (width * height * (pixelDepth / 8))));

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = (y * width + x) * (pixelDepth / 8);
			if (x < width / 3) 
			{
				// First third: Red section
				imageData[index] = 255; // Red channel at max
				imageData[index + 1] = 0; // Green channel at min
				imageData[index + 2] = 0; // Blue channel at min
			} 
			else if (x < 2 * width / 3) 
			{
				// Second third: Green section
				imageData[index] = 0; // Red channel at min
				imageData[index + 1] = 255; // Green channel at max
				imageData[index + 2] = 0; // Blue channel at min
			} 
			else 
			{
				// Last third: Blue section
				imageData[index] = 0; // Red channel at min
				imageData[index + 1] = 0; // Green channel at min
				imageData[index + 2] = 255; // Blue channel at max
			}
		}
    }
	// Save the texture
	SaveDataToTGA(const_cast<char*>("splatmap123.tga"), width, height, pixelDepth, imageData);
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

Model* Terrain::setTerrainModel(const char* heightmap) {
	LoadTGATextureData(heightmap, &ttex);
	terrainModel = generateTerrain(&ttex, currentElevation);
	return terrainModel;
}

TextureData* Terrain::getTextureData() {
	return &ttex;
}

Model* Terrain::getTerrainModel() {
	return terrainModel;
}
