#include "Import.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <string>

#include "renderr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny/tiny_obj_loader.h"

std::string* import::readFile(const char * filePath)
{
	std::string* fileData = new std::string("");
	std::string line;
	std::ifstream fileStream;

	fileStream.open(filePath);
	if (fileStream.is_open())
	{
		while (std::getline(fileStream, line))
		{
			fileData->append(line + "\n");
		}
		fileStream.close();
	}
	else
	{
		printf("file not found");
	}

	return fileData;
}

shader import::createShaderFromFile(std::string fileName)
{
	std::string vertLoc = "shaders\\" + fileName + ".vert";
	std::string fragLoc = "shaders\\" + fileName + ".frag";
	const char *vert = readFile(vertLoc.c_str())->c_str();
	const char *frag = readFile(fragLoc.c_str())->c_str();
	return makeShader(vert, frag);
}

geometry import::createGeometryFromOBJ(const char * filePath, int shapeNum)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> mats;
	std::string err;

	bool ret = tinyobj::LoadObj(&attributes, &shapes, &mats, &err, filePath);

	if(!err.empty())
	{
		std::cout << err << std::endl;
	}
	if (!ret)
	{
		return geometry();
	}

	//std::vector<float> vertex_idx;
	//std::vector<float> normal_idx;
	//std::vextor<float> texcoord_idx;
	//for(int i =0; i < shapes[0].mesh.indices.size; i++)
	//{
	//		vertex_idx.push_back(shapes[0].nesh.indices[i].vertex_index);
	//		normal_idx.push_back(shapes[0].mesh.indices[i].normal_index);
	//	texcoord_idx.push_back(shapes[0].mesh.indices[i].texcoord_index);
	//}
	printf("# of vertices  = %d\n", (int)(attributes.vertices.size()) / 3);
	printf("# of normals   = %d\n", (int)(attributes.normals.size()) / 3);
	printf("# of texcoords = %d\n", (int)(attributes.texcoords.size()) / 2);
	printf("# of materials = %d\n", (int)mats.size());
	printf("# of shapes    = %d\n", (int)shapes.size());

	std::vector<vertex> verticies;
	std::vector<unsigned int> indicies;

	size_t indxCount = 0;
	size_t index_offset = 0;
	for (size_t f = 0; f < shapes[shapeNum].mesh.num_face_vertices.size(); f++) {
		int faceVerticies = shapes[shapeNum].mesh.num_face_vertices[f];
		//if (fv != 3) {
		//	return geometry();
		//}
		// Loop over vertices in the face.
		for (size_t v = 0; v < faceVerticies; v++) {
			// access to vertex
			tinyobj::index_t idx = shapes[shapeNum].mesh.indices[index_offset + v];
			tinyobj::real_t vx = attributes.vertices[3 * idx.vertex_index + 0];
			tinyobj::real_t vy = attributes.vertices[3 * idx.vertex_index + 1];
			tinyobj::real_t vz = attributes.vertices[3 * idx.vertex_index + 2];

			tinyobj::real_t nx = attributes.normals[3 * idx.normal_index + 0];
			tinyobj::real_t ny = attributes.normals[3 * idx.normal_index + 1];
			tinyobj::real_t nz = attributes.normals[3 * idx.normal_index + 2];

			tinyobj::real_t tx = attributes.texcoords[2 * idx.texcoord_index + 0];
			tinyobj::real_t ty = attributes.texcoords[2 * idx.texcoord_index + 1];
			// Optional: vertex colors
			//tinyobj::real_t red = attributes.colors[3*idx.vertex_index+0];
			//tinyobj::real_t green = attributes.colors[3*idx.vertex_index+1];
			//tinyobj::real_t blue = attributes.colors[3*idx.vertex_index+2];
			verticies.push_back(vertex{ { vx, vy, vz, 1 },{ 1,1,1,1 },{ nx,ny,nz, 1 } ,{ tx, ty } });
			//verticies.push_back(newVertex);
			indicies.push_back(indxCount++);
		}
		index_offset += faceVerticies;
	}

	geometry newGeo = makeGeometry(verticies.data(), verticies.size(), indicies.data(), shapes[0].mesh.indices.size());
	return newGeo;
}

geometry import::loadGeometry(const char * filePath)
{
	using namespace tinyobj;

	// contains all data for all vertex attributes loaded from the file
	attrib_t vertexAttributes;

	// enumeration of all shapes in obj file
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error;

	bool success = LoadObj(&vertexAttributes, &shapes, &materials, &error, filePath);

	if (!error.empty())
	{
		fprintf(stderr, "%s", error.c_str());
	}
	if (!success || shapes.size() < 1)
	{
		return {};
	}

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;

	// get mesh data
	size_t offset = 0;
	for (size_t i = 0; i < shapes[0].mesh.num_face_vertices.size(); ++i)
	{
		// number of vertices for current face
		int faceVertices = shapes[0].mesh.num_face_vertices[i];

		// iterate over vertices used to form current face
		for (size_t j = 0; j < faceVertices; ++j)
		{
			tinyobj::index_t idx = shapes[0].mesh.indices[offset + j];

			tinyobj::real_t vx = vertexAttributes.vertices[3 * idx.vertex_index + 0];
			tinyobj::real_t vy = vertexAttributes.vertices[3 * idx.vertex_index + 1];
			tinyobj::real_t vz = vertexAttributes.vertices[3 * idx.vertex_index + 2];

			tinyobj::real_t nx = vertexAttributes.normals[3 * idx.normal_index + 0];
			tinyobj::real_t ny = vertexAttributes.normals[3 * idx.normal_index + 1];
			tinyobj::real_t nz = vertexAttributes.normals[3 * idx.normal_index + 2];

			tinyobj::real_t tx = vertexAttributes.texcoords[2 * idx.texcoord_index + 0];
			tinyobj::real_t ty = vertexAttributes.texcoords[2 * idx.texcoord_index + 1];

			vertices.push_back(vertex{ { vx, vy, vz, 1 },{ (3 * i + j) % 3,(3 * i + j) % 2,(3 * i + j) % 1,1 },{ nx,ny,nz, 1 } ,{ tx, ty } });
			indices.push_back(3 * i + j);
		}
		offset += faceVertices;
	}

	return makeGeometry(&vertices[0], vertices.size(), &indices[0], shapes[0].mesh.indices.size());
}

texture import::createTexture2D(const char * filePath)
{
	int x, y, format;
	unsigned char *pixels = nullptr;

	stbi_set_flip_vertically_on_load(true);
	pixels = stbi_load(filePath, &x, &y, &format, STBI_default);

	if (pixels == NULL) { return texture(); }

	texture newTex = makeTexture(x, y, format, pixels);

	stbi_image_free(pixels);
	std::cout << "Loaded Texture" << std::endl;
	return newTex;
}