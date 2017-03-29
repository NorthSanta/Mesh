#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>

bool show_test_window = false;
float radius;
float reset ;
const int numCols = 14;
const int numRows = 18;

glm::vec3 p1;
glm::vec3 p2;
glm::vec3 vec;


float posX = 0;
float posY = 0;
float posZ = 0;

float posX0 = 0;
float posY0 = 0;
float posZ0 = 0;

float incliZ = 0.1;

float data[numCols * numRows * 3];

float vecx;
float vecy;
float vecz;

float modul;

float v1;
float v2;
float ke;
float kd;
float llargada;

float mass = 1;
struct Particle
{

	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 antPos;
	glm::vec3 Forces;

};

Particle* totalParts;
Particle* AntParts;

namespace Sphere {
	extern void setupSphere(glm::vec3 pos = glm::vec3(0.f, 1.f, 0.f), float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}

namespace ClothMesh {
	extern void setupClothMesh();
	extern void cleanupClothMesh();
	extern void updateClothMesh(float* array_data);
	extern void drawClothMesh();
}

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	//TODO
	ke = 1;//duresa de la molla
	kd = 2;//restriccio del moviment
	llargada = 0.5f;
	reset = 20;
	radius = ((float)rand() / RAND_MAX)*1.5+0.5;
	//printf("%f", radius);
	glm::vec3 vector = glm::vec3(((float)rand() / RAND_MAX) * 5 - 5, ((float)rand() / RAND_MAX) * 10, ((float)rand() / RAND_MAX) * 5 - 5);
	if (vector.y >= 10 - radius) {
		vector.y -= radius;
	}
	else if (vector.y <= 0 + radius) {
		vector.y += radius;
	}
	if (vector.x >= 5 - radius) {
		vector.x -= radius;
	}
	else if (vector.x <= -5 + radius) {
		vector.x += radius;
	}
	if (vector.z >= 5 - radius) {
		vector.z -= radius;
	}
	else if (vector.z <= -5 + radius) {
		vector.z += radius;
	}

	Sphere::updateSphere(vector,radius);
	/*for (int i = 0; i < numCols*numRows; i++)
	{
		printf("%d\n", 3*i +0);
		printf("%d\n", 3*i + 1);
		printf("%d\n", 3*i + 2);
		data[ 3+i + 0] = -4.5*i*0.5;
		data[ 3*i + 1] = -4.5*i*0.5;
		data[ 3*i + 2] = -4.5*i*0.5;
	}*/
	totalParts = new Particle[14 * 18];
	AntParts = new Particle[14 * 18];
	for (int i = 0; i < (numRows); ++i) {
		
		for (int j = 0; j < (numCols); ++j) {
			

			totalParts[i*numCols + j].pos = glm::vec3(j*0.5 - 5, 8, i*0.5 - 5 );
			//printf("CurPos: %f\n", totalParts[i*numCols + j].pos.x);
			totalParts[i*numCols + j].antPos = glm::vec3(totalParts[i*numCols + j].pos.x, totalParts[i*numCols + j].pos.y, totalParts[i*numCols + j].pos.z);
			//printf("AntPos: %f\n", totalParts[i*numCols + j].antPos.x);

			totalParts[i*numCols + j].Forces.x = 0;
			totalParts[i*numCols + j].Forces.y = -9.8;
			totalParts[i*numCols + j].Forces.z = 0;

			totalParts[i*numCols + j].velocity.x = 0;
			totalParts[i*numCols + j].velocity.y = 2;
			totalParts[i*numCols + j].velocity.z = 0;

			data[3 * (i*numCols + j) + 0] = totalParts[i*numCols + j].pos.x;
			data[3 * (i*numCols + j) + 1] = totalParts[i*numCols + j].pos.y;
			data[3 * (i*numCols + j) + 2] = totalParts[i*numCols + j].pos.z;
			
			
		}
	}
	
	posX0 = data[0];
	posY0 = data[1];
	posZ0 = data[2];

	posX = data[39];
	posY = data[40];
	posZ = data[41];
	
	ClothMesh::updateClothMesh(data);
	
}

void PhysicsUpdate(float dt) {
	
	reset -= dt;
	if (reset <= 0) {
		PhysicsInit();
	}
	//TODO
	for (int i = 0; i < (numRows); ++i) {
		//printf("%d", i);
		for (int j = 0; j < (numCols); ++j) {
			glm::vec3 temp = totalParts[i*numCols + j].pos;
			glm::vec3 structuralForce;
			
			totalParts[i*numCols + j].pos.x = totalParts[i*numCols + j].pos.x + (totalParts[i*numCols + j].pos.x - totalParts[i*numCols + j].antPos.x) + (totalParts[i*numCols + j].Forces.x / mass)*(dt*dt);

			totalParts[i*numCols + j].pos.y = totalParts[i*numCols + j].pos.y + (totalParts[i*numCols + j].pos.y - totalParts[i*numCols + j].antPos.y) + (totalParts[i*numCols + j].Forces.y / mass)*(dt*dt);

			totalParts[i*numCols + j].pos.z = totalParts[i*numCols + j].pos.z + (totalParts[i*numCols + j].pos.z - totalParts[i*numCols + j].antPos.z) + (totalParts[i*numCols + j].Forces.z / mass)*(dt*dt);

			totalParts[i*numCols + j].velocity = (totalParts[i*numCols + j].pos - totalParts[i*numCols + j].antPos) / dt;
			//printf("%f", totalParts[i*numCols + j].velocity.z);
			totalParts[0].pos = glm::vec3(posX0, posY0, posZ0);
			totalParts[13].pos = glm::vec3(posX, posY, posZ);
			totalParts[i*numCols + j].antPos = temp;

			if (j== 0 && i < 17  ) {

				p1 = glm::vec3(totalParts[i*numCols + j].pos.x, totalParts[i*numCols + j].pos.y, totalParts[i*numCols + j].pos.z);

				p2 = glm::vec3(totalParts[(i+1)*numCols + j].pos.x, totalParts[(i+1)*numCols + j].pos.y, totalParts[(i+1)*numCols + j].pos.z);
				
				
				vec = p1 - p2;
				
				modul = glm::length(vec);//modul del vector
			
				float var = -(ke*(modul - llargada) + kd * glm::dot((totalParts[i*numCols + j].velocity - totalParts[(i + 1)*numCols + j].velocity), (vec / modul)));
				structuralForce = var*(vec / modul);
				printf("%f", structuralForce.y);
				
				totalParts[i*numCols + j].Forces += structuralForce;
				totalParts[(i + 1)*numCols + j].Forces += -structuralForce;
					
			}
			/*if (j == 0 && i == 1) {
				
				structuralForce = -structuralForce;
				totalParts[i*numCols + j].Forces = structuralForce;
			}*/
			
			

			data[3 * (i*numCols + j) + 0] = totalParts[i*numCols + j].pos.x;
			data[3 * (i*numCols + j) + 1] = totalParts[i*numCols + j].pos.y;
			data[3 * (i*numCols + j) + 2] = totalParts[i*numCols + j].pos.z;
			
			if (data[3 * (i*numCols + j) + 2] <= -5) {
				data[3 * (i*numCols + j) + 2] = -5;
			}
		}
	}
	
	
	
	
	
	
	
	
	
	/*data[0] = posX0;
	data[1] = posY0;
	data[2] = posZ0;

	data[39] = posX;
	data[40] = posY;
	data[41] = posZ;*/
	ClothMesh::updateClothMesh(data);
	
	
	
}
void PhysicsCleanup() {
	//TODO
	ClothMesh::cleanupClothMesh();
}